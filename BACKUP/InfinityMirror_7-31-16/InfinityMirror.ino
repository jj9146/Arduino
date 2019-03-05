#include <USBDetectorLib.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6 // Set the pin used by the LED array
#define MAX_LED_COUNT 44
#define MIN_LED_COUNT 5
uint32_t LED_COUNT; //Used to set the actual number of LEDs we'll use
uint32_t _pixelColors[MAX_LED_COUNT]; //Set the maximum number of LEDs that will be used (even if you won't use all of them all of the time)

//  Use pin 12 to detect a USB Power Connection (via 10k resistor) so we can
//  reduce the number of LEDs used at runtime to keep it from frying the Nano,
//  and to eliminate attempts to send serial data when there's no one listening
USBDetectorLib usbDetectorLib(12, 250000); 

bool _cancel = false;
bool _Halting = false;
bool _usingUSBPower;

// Parameter 1 = max number of pixels in strip (MAX_LED_COUNT)
// Parameter 2 = pin number (PIN)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int InterruptButton = 2; // Allow the mic sensor to cut in when its threshold is reached
int analogMicPin = A0; // Allow the mic sensor to cut in when its threshold is reached
int FadeModePin = 7; // Use only fade/blend type patterns w/no sound interupts
int SoundModePin = 8; // Sound-mode patterns using sound interupts
int RandomModePin = 9; // Randomly use all patterns w/no sound interupts
int _displayMode; // Set by the mode pins above
const int FADE_MODE = 0;
const int SOUND_MODE = 1;
const int RANDOM_MODE = 2;
void setup() 
{
    // Find out if we have a USB connection
    _usingUSBPower = usbDetectorLib.ConnectAndValidateUSBPower(); 
    if(_usingUSBPower) {LED_COUNT = MIN_LED_COUNT;}
    else {LED_COUNT = MAX_LED_COUNT;}
    //Setup the sound sensor interrupt
    pinMode(InterruptButton, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(InterruptButton), Halt, HIGH);
    _cancel = false; // Used by the interupt to halt the current pattern
    //Setup the analog mic pin
    pinMode(analogMicPin, INPUT);
    //Setup the "mode" pins
    pinMode(FadeModePin, INPUT_PULLUP);
    pinMode(SoundModePin, INPUT_PULLUP);
    pinMode(RandomModePin, INPUT_PULLUP);
    //Setup the strip
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    usbDetectorLib.println("USB Power Detected - Messages will be sent to COM port");
}

void loop() 
{
    randomSeed(millis());
    while(0 == 0)
    {
        int randEffect;
        //Reset the cancel/halting flags
        if(_cancel) {_cancel = false;}
        if(_Halting) {_Halting = false;}
        //Check the mode
        if(digitalRead(FadeModePin) == LOW) { _displayMode = FADE_MODE;}
        if(digitalRead(SoundModePin) == LOW) { _displayMode = SOUND_MODE;}
        if(digitalRead(RandomModePin) == LOW) { _displayMode = RANDOM_MODE;}
        
        /**** FADE MODE (Default) ****/
        if(_displayMode == FADE_MODE)
        {
            int randEffect = random(0,2);
            if(randEffect == 0)
            {
                usbDetectorLib.println("Fade Mode - Calling Rainbow"); 
                rainbow(random(5,50));
            }
            if(randEffect == 1)
            {
                usbDetectorLib.println("Fade Mode - Calling RainbowCycle"); 
                rainbowCycle(random(5,50));
            }
        } // END OF FADE MODE
        
        /**** SOUND MODE ****/
        if(_displayMode == SOUND_MODE)
        {
            int randEffect = random(0,4);
            if(randEffect == 0)
            {
                usbDetectorLib.println("Sound Mode - Calling RandomRotate");
                RandomRotate();
            }
            if(randEffect == 1)
            {
                int randColor = random(0,4);
                int randDelay = random(0,500);
                if(randColor == 0)
                {
                    usbDetectorLib.println("Sound Mode - Calling ColorWipe RED");
                    colorWipe(strip.Color(255, 0, 0), randDelay); // Red
                }
                if(randColor == 1)
                {
                    usbDetectorLib.println("Sound Mode - Calling ColorWipe GREEN");
                    colorWipe(strip.Color(0, 255, 0), randDelay); // Green
                }
                if(randColor == 2)
                {
                    usbDetectorLib.println("Sound Mode - Calling ColorWipe BLUE"); 
                    colorWipe(strip.Color(0, 0, 255), randDelay); // Blue
                }
                if(randColor == 3)
                {
                    usbDetectorLib.println("Sound Mode - Calling ColorWipe Random"); 
                    int R = random(255);
                    int G = random(255);
                    int B = random(255);
                    colorWipe(strip.Color(R, G, B), randDelay); // Random color
                }
            }
            if(randEffect == 2)
            {
                usbDetectorLib.println("Sound Mode - Calling Rainbow"); 
                rainbow(random(5,50));
            }
            if(randEffect == 3)
            {
                usbDetectorLib.println("Sound Mode - Calling RainbowCycle"); 
                rainbowCycle(random(5,50));
            }
        } // END OF SOUND MODE
        
        /**** RANDOM MODE ****/
        if(_displayMode == RANDOM_MODE)
        {
            int randEffect = random(0,4);
            if(randEffect == 0)
            {
                usbDetectorLib.println("Random Mode - Calling RandomRotate");
                RandomRotate();
            }
            if(randEffect == 1)
            {
                int randColor = random(0,4);
                int randDelay = random(0,500);
                if(randColor == 0)
                {
                    usbDetectorLib.println("Random Mode - Calling ColorWipe RED");
                    colorWipe(strip.Color(255, 0, 0), randDelay); // Red
                }
                if(randColor == 1)
                {
                    usbDetectorLib.println("Random Mode - Calling ColorWipe GREEN");
                    colorWipe(strip.Color(0, 255, 0), randDelay); // Green
                }
                if(randColor == 2)
                {
                    usbDetectorLib.println("Random Mode - Calling ColorWipe BLUE"); 
                    colorWipe(strip.Color(0, 0, 255), randDelay); // Blue
                }
                if(randColor == 3)
                {
                    usbDetectorLib.println("Random Mode - Calling ColorWipe Random"); 
                    int R = random(255);
                    int G = random(255);
                    int B = random(255);
                    colorWipe(strip.Color(R, G, B), randDelay); // Random color
                }
            }
            if(randEffect == 2)
            {
                usbDetectorLib.println("Random Mode - Calling Rainbow"); 
                rainbow(random(5,50));
            }
            if(randEffect == 3)
            {
                usbDetectorLib.println("Random Mode - Calling RainbowCycle"); 
                rainbowCycle(random(5,50));
            }
        } // END OF RANDOM MODE

    } // END OF MAIN WHILE LOOP
}

void SideBounce()
{
    //Pick a couple of random colors
    
    //convert the existing colors to one solid color

    //starting in the middle of each side, grow the new color until it reaches
    //the end of the side, then shrink it back again to the original color

    //repeat as desired

    //consider making shades or fading, too
    
}

void RandomRotate()
{
    randomSeed(millis());
    int randDirection = random(0,2);
    int randColorType = random(0,3);
    int randLoops = random(2,11);
    int randDelay = random(20,400);
    String strDirection = String(randDirection);
    String strColorType = String(randColorType);
    String strLoops = String(randLoops);
    String strDelay = String(randDelay);
    usbDetectorLib.println("RandomRotate with " + strDirection + " direction, " + strColorType + " color type, " + strLoops + " loops, and " + strDelay + " delay"); 
    if(randColorType == 0) { AssignRandomColors(); }
    if(randColorType == 1) { AssignRandomPrimaryThreeColors(); }
    if(randColorType == 2) { AssignRandomPrimarySixColors(); }
    RotatePixels(randDirection, randDelay, randLoops);
}

void BreakDelay(long DelayTime)
{
    //DelayTime = DelayTime * 10;
    double StartTime = millis();
    double EndTime = StartTime + DelayTime;
    while (millis() < EndTime)
        //Allows us to stop things on the fly
        if(_cancel == true) {break;}
    loop;
}

// Original Halt - *** THIS IS THE METHOD CALLED WHEN THE INTERUPT STARTS GOING HIGH ***
void Halt()
{
    if(_displayMode == SOUND_MODE)
    {
        int intBtn = digitalRead(2);
        //String ButtonState = String(intBtn);
        usbDetectorLib.print("INTERRUPT - Halting = ");
        usbDetectorLib.print(_Halting);
        usbDetectorLib.print(", _cancel = ");
        usbDetectorLib.print(_cancel);
        usbDetectorLib.print(", Button State = ");
        usbDetectorLib.println(intBtn);
        if(intBtn == 1)
        {
            // The interupt is getting called, but the mic's state is "1",
            // and we only trigger off of "0", so bail.
            return;
        }
        if(_Halting == true)
        {
            // The interupt has already been called to trigger a cancel, 
            // so we can ignore this and bail.
            return;
        }
        // If we get here, then we're not in the middle of halting,
        // and the mic's state is "0" so we know we should start cancelling,
        // if we're not already
        _Halting = true;
        if(_cancel == false)
        {
            // We're not already cancelling, so start it now, and delay
            // afterwards to hopefully eliminate "mic bounce" (like button bounce)
            _cancel = true;
            delay(250);
            // Now we're really done, so reset the Halting variable
            _Halting = false;
        }
    }
}

// Rotate the pixels through one full loop
void RotatePixels(int rotateDirection, long wait, int numLoops)
{
    for(uint16_t x=0; x<numLoops; x++) //Set the number of loops we'll make
    {
        uint32_t endValue; //Used to store the value at the end of the strip
        for(uint16_t a = 0; a < strip.numPixels(); a++) //Repeat the shifting for one full loop
        {
            if(rotateDirection == 0)  //Move the colors one position down
            {
                endValue = _pixelColors[0];  //Save the first so it can become the last
                for(uint16_t i = 0; i < strip.numPixels() - 1; i++) 
                {
                    if(_cancel) return;
                    _pixelColors[i] = _pixelColors[i + 1]; //Shift the pixel above down to the current
                }
                _pixelColors[strip.numPixels() - 1] = endValue;  //Now shift the pixel that had been at the start to the very end
                //Now assign them to the actual pixels
                for(uint16_t i = 0; i < strip.numPixels(); i++) 
                {
                    if(_cancel) return;
                    strip.setPixelColor(i, _pixelColors[i]);
                }
                //Display our changes
                strip.show();
                //Pause momentarily
                BreakDelay(wait);
            }
            else  //Move the colors one position up
            {
                endValue = _pixelColors[strip.numPixels() - 1];  //Save the last so it can become the first
                for(uint16_t i = strip.numPixels() - 1; i > 0; i--) 
                {
                    if(_cancel) return;
                    _pixelColors[i] = _pixelColors[i - 1]; //Shift the pixel below up to the current
                }
                _pixelColors[0] = endValue;  //Now shift the pixel that had been at the end to the first
                //Now assign them to the actual pixels
                for(uint16_t i = 0; i < strip.numPixels(); i++) 
                {
                    if(_cancel) return;
                    strip.setPixelColor(i, _pixelColors[i]);
                }
                //Display our changes
                strip.show();
                //Pause momentarily
                BreakDelay(wait);
            }
        }
    }
}

// Assign purely random colors to all of the pixels
void AssignRandomColors()
{
  randomSeed(millis());
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    _pixelColors[i] = strip.Color(random(255), random(255), random(255));
  }
}

// Assign random colors to all of the pixels, but make
// each heavily biased towards one of the three primary colors
void AssignRandomPrimaryThreeColors()
{
    randomSeed(millis());
    for(uint16_t i = 0; i < strip.numPixels(); i++) 
    {
        int primaryColor = random(1,4);
        if(primaryColor == 1) //RED  
        {
            _pixelColors[i] = strip.Color(random(210,255), random(0,100), random(0,100));
        }
        if(primaryColor == 2) //GREEN  
        {
            _pixelColors[i] = strip.Color(random(0,100), random(210,255), random(0,100));
        }
        if(primaryColor == 3) //BLUE  
        {
            _pixelColors[i] = strip.Color(random(0,100), random(0,100), random(210,255));
        }
    }
}

// Assign random colors to all of the pixels, but make
// each heavily biased towards one of 6 major colors
void AssignRandomPrimarySixColors()
{
    randomSeed(millis());
    for(uint16_t i = 0; i < strip.numPixels(); i++) 
    {
        int primaryColor = random(1,7);
        if(primaryColor == 1) //RED  
        {
            _pixelColors[i] = strip.Color(random(210, 255), random(0, 100), random(0, 100));
        }
        if(primaryColor == 2) //GREEN  
        {
            _pixelColors[i] = strip.Color(random(0,100), random(210,255), random(0,100));
        }
        if(primaryColor == 3) //BLUE  
        {
            _pixelColors[i] = strip.Color(random(0,100), random(0,100), random(210,255));
        }
        if(primaryColor == 4) //RED+GREEN  
        {
            _pixelColors[i] = strip.Color(random(210, 255), random(210, 255), random(0, 100));
        }
        if(primaryColor == 5) //RED+BLUE  
        {
            _pixelColors[i] = strip.Color(random(210, 255), random(0, 100), random(210, 255));
        }
        if(primaryColor == 6) //GREEN+BLUE  
        {
            _pixelColors[i] = strip.Color(random(0, 100), random(210, 255), random(210, 255));
        }
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, long wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      if(_cancel) break;
      strip.setPixelColor(i, c);
      strip.show();
      if(_cancel) break;
      BreakDelay(wait);
  }
}

void rainbow(long wait) 
{
  uint16_t i, j;
  for(j=0; j<256; j++) 
  {
    if(_cancel) break;
    for(i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel((i+j) & 255));
      if(_cancel) break;
    }
    strip.show();
    if(_cancel) return;
    BreakDelay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(long wait) 
{
  uint16_t i, j;

  for(j=0; j<256*5; j++) 
  { // 5 cycles of all colors on wheel
    if(_cancel) break;
    for(i=0; i< strip.numPixels(); i++) 
    {
      if(_cancel) break;
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    if(_cancel) return;
    BreakDelay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) 
  {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else 
  {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

