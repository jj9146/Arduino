#include <USBDetectorLib.h>
#include <Adafruit_NeoPixel.h>

//  Use pin 12 to detect a USB Power Connection (via 10k resistor) so we can
//  reduce the number of LEDs used at runtime to keep it from frying the Nano,
//  and to eliminate attempts to send serial data when there's no one listening
USBDetectorLib usbDetectorLib(12, 250000); 

bool _cancel = false;
bool _usingUSBPower;

// Parameter 1 = max number of pixels in strip (MAX_LED_COUNT)
// Parameter 2 = pin number (PIN)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
#define PIN 6 // Set the pin used by the LED array
#define MAX_LED_COUNT 44
#define MIN_LED_COUNT 3
uint32_t LED_COUNT; //Used to set the actual number of LEDs we'll use
uint32_t _pixelColors[MAX_LED_COUNT]; //Set the maximum number of LEDs that will be used (even if you won't use all of them all of the time)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int InterruptButton = 2; // Allow the mic sensor to cut in when its threshold is reached
int analogMicPin = A0; // Allow the mic sensor to cut in when its threshold is reached
int FadeModePin = 7; // Use only fade/blend type patterns w/no sound interupts
int SoundModePin = 8; // Sound-mode patterns using sound interupts
int RandomModePin = 9; // Randomly use all patterns w/no sound interupts
int _displayMode; // Set by the mode pins above
byte _ColNum1; // Contains the "color wheel color" (0 - 255, set by Wheel and others)
byte _ColNum2; // Not currently used - the exact opposite color of _ColNum1
long _lastHalt;  // For tracking how long it's been since the Halt/Interupt fired
const int FADE_MODE = 0;
const int SOUND_MODE = 1;
const int RANDOM_MODE = 2;
const int RANDOMIZING_PIN = A1;


void setup() 
{
    // Find out if we have a USB connection
    _usingUSBPower = usbDetectorLib.ConnectAndValidateUSBPower(); 
    if(_usingUSBPower) { LED_COUNT = MIN_LED_COUNT; }
    else { LED_COUNT = MAX_LED_COUNT; }
    
    //Setup the sound sensor interrupt
    pinMode(InterruptButton, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(InterruptButton), Halt, HIGH);
    _cancel = false; // Set by the interrupt so the other actions can respond, too

    //Setup the analog mic pin
    pinMode(analogMicPin, INPUT);
    //Setup the "mode" pins
    pinMode(FadeModePin, INPUT_PULLUP);
    pinMode(SoundModePin, INPUT_PULLUP);
    pinMode(RandomModePin, INPUT_PULLUP);
    pinMode(RANDOMIZING_PIN, INPUT); // This will return random values based on magnetic fields at the time

    //Setup the strip
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    usbDetectorLib.println("USB Power Detected.  Messages will be sent to COM port.  " + String(LED_COUNT) + " pixels will be used.");
}

void loop() 
{
    unsigned long RandomRead = analogRead(RANDOMIZING_PIN); 
    RandomRead *= micros();
    randomSeed(RandomRead); // This should be truly random, based on reading an unset pin and
                            // using the number of microseconds it took to perform that read
    usbDetectorLib.print("Random Seed: ");
    usbDetectorLib.println(RandomRead); 

    while(0 == 0)
    {
        int randEffect;
        //Reset the cancel flags
        if(_cancel) {_cancel = false;}
        //Check the mode
        if(digitalRead(FadeModePin) == LOW) { _displayMode = FADE_MODE;}
        if(digitalRead(SoundModePin) == LOW) { _displayMode = SOUND_MODE;}
        if(digitalRead(RandomModePin) == LOW) { _displayMode = RANDOM_MODE;}
        
        /*****************************/
        /**** FADE MODE (Default) ****/
        /*****************************/
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
        
        /********************/
        /**** SOUND MODE ****/
        /********************/
        if(_displayMode == SOUND_MODE)
        {   
            SoundAlgorithm1();
            
        } // END: if(_displayMode == SOUND_MODE)
        
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

void Halt()
{
    if(_displayMode == SOUND_MODE)
    {
        //Let anything on the outside know we've been interrupted
        _cancel = true;
        _lastHalt = millis(); // Note when it happened, too.
        
        //The colors will be slowly advancing across the color wheel,
        //but an interupt will cause it to advance several hops at once
        _ColNum1 += random(1,5);
        if(_ColNum1 > 255) { _ColNum1 = random(1,5); }
        
        // If I ever need a second color that's the opposite of the first,
        // I can do that here
        /*
        if(_ColNum1 < 128)
        {
            _ColNum2 = _ColNum1 + 128;
        }
        else
        {
            _ColNum2 = 128 - (255 - _ColNum1);
        }
        */
    }
}
    // This acts like a normal "Delay" call, but allows the Interupt to intercede
void BreakDelay(long DelayTime)
{
    //DelayTime = DelayTime * 10;
    double StartTime = millis();
    double EndTime = StartTime + DelayTime;
    while (millis() < EndTime)
        //Allows us to stop things on the fly
        if(_cancel == true)
        {
            _cancel = false;
            break;
        }
    loop;
}

    // This algorithm starts by "bundling" analog readings into an array with 100 elements.  It then determines the lowest
    // and higest reading within that group, and starts gathering the next group of 100 readings.  During that time, each 
    // iteration of the loop "adds" a new LED to the strip, forcing the last to drop off.  The new LED's color slowly cycles
    // from Red to Green to Blue, unless the threshold interupt fires, in which case it jumps forward significantly.  As each
    // new element is added, its "intensity" is scaled back based on its reading relative to the high and low in the last
    // group of 100 readings.
    
void SoundAlgorithm1()
{
    // Note what mode we're in (SOUND)
    usbDetectorLib.println("Sound Mode"); 
    // Use this to turn off debugging statements without removing them
    //usbDetectorLib.HaltPrintToCOM(true); // *** Use this to instantly disable all COM messages without having to remove them from the code ***
    
    byte Red = 0; // Holds the red level
    byte Green = 0; // Holds the green level
    byte Blue = 0; // Holds the blue level
    byte NewRed = 0; // Holds the adjusted red level
    byte NewGreen = 0; // Holds the adjusted green level
    byte NewBlue = 0; // Holds the adjusted blue level
    
    int Intensity = 0; // The analog pin's "intensity", from 0 to 100
    float IntensityMultiplier; // Intensity / 100 (from 0.0000 to 1.000)
    int analogPinReading; // The reading off of the sound sensor's analog pin
    int AnalogBuffer[125]; // Holds the last 200 (or however many set by SampleSize) analog pin readings
    int BufferCounter = 0; // A counter used to track and fill the AnalogBuffer array
    int WholeColor = 0; // Needed for return value - not used
    long T1 = millis(); // Used when timing for debugging
    long T2 = 0; // Used when timing for debugging
    int Min = 1200; // The minimum value in the AnalogBuffer when last filled (set high to force the lowest to be grabbed)
    int Max = 0; // The maximum value in the AnalogBuffer when last filled (set low to force the higest to be grabbed)
    int Span; // Difference between Min and Max
    int SampleSize = 40; // The number of analog readings collected before they're analyzed into new "Min" and "Max" values
    float Crawl = 0;  // Used to slowly rotate the colors forward on the color wheel when Halt isn't firing
    float CrawlRate = 1; // Sets the rate at which the colors move without Halt firing

    // Start all pixels as random color
    for(int i = 0; i < LED_COUNT; i++)
    {
        _pixelColors[i] = strip.Color(random(255), random(255), random(255));
    }

    _ColNum1 = random(10, 256); // The color number returned by the Red-to-Green-to-Blue-back-to-Red "ColorWheel" function (0 to 255)

    // MAIN SOUND LOOP
    while (_displayMode == SOUND_MODE) // Breaks only if _displayMode changes
    {
        // Read the analog sound value
        analogPinReading = analogRead(analogMicPin); //read the analog sound pin
        usbDetectorLib.println("analogPinReading = " + String(analogPinReading)); 

        // See if our array is full yet
        if(BufferCounter < (SampleSize + 1))
        {

            // It's not full yet - keep adding to it
            AnalogBuffer[BufferCounter] = analogPinReading;
            BufferCounter++;

            // See if we have enough of a sample
            if(BufferCounter > SampleSize)
            {
                // Analyze it and see what the Min/Max range is
                Min = 1200;
                Max = 0;
                for(byte b = 0; b < (SampleSize + 1); b++)
                {
                    if(AnalogBuffer[b] > Max) { Max = AnalogBuffer[b]; }
                    if(AnalogBuffer[b] < Min) { Min = AnalogBuffer[b]; }
                }
                Span = Max - Min; // Not currently used, but see note further down
                usbDetectorLib.println("SPAN: " + String(Span)); 
                
                // Reset the counter
                BufferCounter = 0;
            }
        }

        // Make sure we don't go down this road until we've got a Mix/Max value to use
        if(Max > 0)
        {
            // The longer it's been since "Halt" fired, the dimmer it should be...
            long TimeDiff = millis() - _lastHalt;
            if(TimeDiff > 15000) { TimeDiff = 15000; }
            usbDetectorLib.println("TimeDiff = " + String(TimeDiff)); 
            float TimeFade = ( float(TimeDiff) / float(15000) );
            usbDetectorLib.println("TimeFade = " + String(TimeFade)); 
            analogPinReading = analogPinReading - (float(20) * TimeFade);
            usbDetectorLib.println("New analogPinReading = " + String(analogPinReading)); 

            // Turn the last analogPin reading into a percentage using the current 
            // Min and Max values, telling us the intensity of this reading
            Intensity = map(analogPinReading, Min, Max, 0, 100);
            // Deal with spikes - clip it between 0 and 100 if it gets really loud at some point
            if(Intensity > 100) { Intensity = 100; }
            if(Intensity < 0) { Intensity = 0; }
            IntensityMultiplier = (float)Intensity / 100.000; // Now in a percentage so we can multiply
    
            //NOTE: Another option might be use the "Span" (difference between Min and Max, which is 0 or 1 when "calm", and as high as
            //      60 or 70 when really active) to kick out all of the new pixels with the same color at an intensity based on it.  Or,
            //      the Span could drive how fast they pixel changing happens by inversely controlling the delay
            
            //Get our current color wheel color, divided into Red, Green, and Blue components
            WholeColor = WheelWithComponents(_ColNum1, Red, Green, Blue);
            // NOTE: The _ColNum1 value is also adjusted by the "Halt" interrupt
             
            // Adjust the intensity of the color by scaling its Red, Green, and Blue components
            // using our intensity percentage (0.000 - 1.000)
            NewRed = Red * (float)IntensityMultiplier;
            NewGreen = Green * (float)IntensityMultiplier;
            NewBlue = Blue * (float)IntensityMultiplier;
            
            // _pixelColors[i] is a module-level array of colors that corresponds to the pixels
            // in their array.  It allows us to adjust all of their colors in a time-insensitive
            // manner, which when done can be applied to the *actual* pixel's colors all at once.
            for(byte b = (LED_COUNT - 1); b > 0; b--) 
            {
                // Starting at the top, shift each pixel up to the one above it.  No need to
                // roll the highest one down to the bottom, since we're introducing the newest
                // color into the bottom pixel and allowing it to rotate up and around
                _pixelColors[b] = _pixelColors[b - 1];
            }

            // With all others shifted up, change the color of the "new" (first) pixel
            _pixelColors[0] = strip.Color(NewRed, NewGreen, NewBlue);

            //Now assign the actual pixels the shifted and new values in the _pixelColors array
            for(byte b = 0; b < LED_COUNT; b++) 
            {
                strip.setPixelColor(b, _pixelColors[b]);
            }

            // When the volume is low or the music is in a quiet period, slowly rotate the colors so
            // there's even movement when Halt isn't firing
            Crawl += CrawlRate;
            if(Crawl > 10)
            {
                Crawl = 0; // Reset the "crawl" counter
                _ColNum1++;
                if(_ColNum1 > 255) { _ColNum1 = 0;}
            }

            BreakDelay(25); // Delay to make it easier to see what's happening

            //Display our changes
            strip.show();
            
            //See if they changed modes...
            if(digitalRead(FadeModePin) == LOW) { _displayMode = FADE_MODE;}
            if(digitalRead(SoundModePin) == LOW) { _displayMode = SOUND_MODE;}
            if(digitalRead(RandomModePin) == LOW) { _displayMode = RANDOM_MODE;}

        } // END: if(Max > 0)
        
    } // END: while (_displayMode == SOUND_MODE)
    usbDetectorLib.println("Someone must have changed the mode to something other than Sound..."); 
}

    // OTHER SOUND ALGORITHM IDEAS
void Temp()
{
    // Using the existing one, have the intensity control the color,
    // (low = blue, middle = green, high = red), and have the interupt
    // reverse direction
}

    // OTHER SOUND ALGORITHM IDEAS
void Waterfall()
{
    // Have them start in the center of the top side and emmit in ever
    // changing colors at the same slow rate, then have them speed up
    // as they "wash down the sides", collecting at the bottom and 
    // draining out
}

    // OTHER SOUND ALGORITHM IDEAS
void LetEmRace()
{
    // Fire off a new color each loop, using the relative intensity
    // as its velocity.  When two land on the same LED, blend their 
    // colors

    // Maybe have a version where they blend and stay combined, mopping
    // up others as they go.  When the threshold interupt fires, flip
    // their direction
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// Colors are returned ByRef in their Red, Green, and Blue components
uint32_t WheelWithComponents(byte WheelPos, byte & Red, byte & Green, byte & Blue) 
{
    if(WheelPos < 85) 
    {
        Red = WheelPos * 3;
        Green = 255 - WheelPos * 3;
        Blue = 0;
    } 
    else if(WheelPos < 170) 
    {
        WheelPos -= 85;
        Red = 255 - WheelPos * 3;
        Green = 0;
        Blue = WheelPos * 3;
    } 
    else 
    {
        WheelPos -= 170;
        Red = 0;
        Green =  WheelPos * 3;
        Blue = 255 - WheelPos * 3;
    }
    return strip.Color(Red, Green, Blue);
}

