#include <Adafruit_NeoPixel.h>

#define PIN 4 // Set the pin used by the LED array
#define MAX_LED_COUNT 3
#define MIN_LED_COUNT 3
int LED_COUNT; //Used to set the actual number of LEDs we'll use
uint32_t _pixelColors[MAX_LED_COUNT]; //Set the maximum number of LEDs that will be used (even if you won't use all of them all of the time)

// Parameter 1 = max number of pixels in strip (MAX_LED_COUNT)
// Parameter 2 = pin number (PIN)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int ModeButton = 1; // The Mode-switching Button
int HallSwitch = 2; // Allow the Hall Switch to cut in when tripped
bool _isLow = false; // Used by main loop to know if a LOW is the first LOW or a continuation of a previous LOW
long _lastTime = 0; // The last time the switch went LOW
int _mode = 0; // Determines which "mode" the spinner is in
uint16_t _ColorPoint = 0;

void setup() 
{
    LED_COUNT = MAX_LED_COUNT;

    //Setup the Hall Switch
    pinMode(HallSwitch, INPUT_PULLUP);

    //Setup the Mode Button
    pinMode(ModeButton, INPUT_PULLUP);

    //Setup the strip
    strip.begin();
    //Start Blue
    _pixelColors[0] = strip.Color(25, 25, 255);
    strip.setPixelColor(0, _pixelColors[0]);
    strip.setPixelColor(1, _pixelColors[0]);
    strip.setPixelColor(2, _pixelColors[0]);
    
    strip.show(); // Initialize all pixels to 'off'

    _lastTime = millis();
}

void SetColor(long LastRotationTime)
{
    // *** NEED TO DO! *** - This is currently assigning random colors - need to make it based on the rotation speed

    byte Red = random(255); // Holds the red level
    byte Green = random(255); // Holds the green level
    byte Blue = random(255); // Holds the blue level
    for(byte b = 0; b < LED_COUNT; b++) 
    {
        _pixelColors[b] = strip.Color(Red, Green, Blue);
        strip.setPixelColor(b, _pixelColors[b]);
    }
    strip.show();
}

void loop() 
{
    // See if the button has been clicked, and which mode we should be in
    if(digitalRead(ModeButton) == LOW) 
    {
        // They're switching modes...
        if(_mode == 0)
        {
            _mode = 1; // Switch to rotation-based colors
        }
        else
        {
            _mode = 0; // Switch to the rainbow cycle
        }
        // Now stop everything for 1/2 second so the button can reset
        delay(500);
    }

    // MODE = 0 - RAINBOW 
    if(_mode == 0)
    {
        Rainbow();
        delay(25);
        _ColorPoint += 1;
        if(_ColorPoint == 256)
            _ColorPoint = 0;
    }

    // MODE = 1 - ROTATION-BASED COLORS 
    if(_mode == 1)
    {
        // Get the current time
        long Now = millis();
        
        // Get the state of the Hall Switch
        bool HallState = digitalRead(HallSwitch);
        
        // Determine which state we're in 
        if(HallState == LOW)
        {
            //See if this is the first time we've detected a LOW
            if(_isLow == false)
            {
                // The sensor just went LOW - see how long it's been since the last time
                long RotationTime =  Now - _lastTime;
                
                // Set the state and _lastTime
                _isLow = true;
                _lastTime = Now;
    
                // Adjust the color accordingly
                // *** NEED TO DO! ***
                SetColor(RotationTime);
            }
            else
            {
                // The sensor was alread LOW last time - no need to do anything else
            }
        }
        else
        {
            // The sensor is HIGH - reset the state
            _isLow = false;
        }
    }
 }

// Makes the pixels rotate through all of the colors
void Rainbow() 
{
    uint16_t i;
    for(i = 0; i < LED_COUNT; i++) 
    {
      strip.setPixelColor(i, Wheel((i + _ColorPoint) & 255));
    }
    strip.show();
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

