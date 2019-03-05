/*** THIS IS ALL FOR SETTING UP THE NEO-PIXELS ***/
    #include <Adafruit_NeoPixel.h>
    #define PIN_NEO_Data 2 // Set the PWM pin used by the LED array - uses inline 330 Ω resistor
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
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED_COUNT, PIN_NEO_Data, NEO_GRB + NEO_KHZ800);

/*** NOW DEFINE ALL OF THE OTHER PINS ***/
    /* OUTPUT/CONTROL PINS: */
    PIN_Relay_Power_LED1 = 23 //Green-to-Green - uses inline 330 Ω resistor
    PIN_Relay_Power_LED2 = 25 //Light Blue-to-Blue - uses inline 330 Ω resistor
    PIN_Relay_Power_Toggle1 = 27 //Yellow-to-Yellow
    PIN_Relay_Power_Toggle2 = 29 //Orange w/White Stripe-to-Brown
    PIN_DP3T_Up = 31 //Red w/Black stripe-to-Purple
    PIN_DP3T_Down = 33 //Red w/White stripe-to-White
    PIN_Set_Button = 35 //Red-to-Red
    PIN_Up_Button = 37 //Brown w/White stripe-to-Yellow
    PIN_Down_Button = 39 //Brown-to-Brown
    PIN_Ambient_Air_Signal = A0 //Black w/White stripe-to-Gray
    PIN_LCD_4_RS = 41 //Black-to-Black
    PIN_LCD_6_Enable = 42 //Pink-to-Green
    PIN_LCD_11_D4 = 43 //White-to-White
    PIN_LCD_12_D5 = 44
    PIN_LCD_12_D5 = 44
    PIN_LCD_12_D5 = 44

void setup() {
    // Setup the USB COM port
    Serial.begin(115200);
    /*** CONFIGURE ALL OF THE PINS ***/
            

    
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);


    
    /*** NOW SETUP THE NEO-PIXELS ***/
    LED_COUNT = MAX_LED_COUNT;
    //Setup the strip
    strip.begin();
    //Start Blue
    _pixelColors[0] = strip.Color(25, 25, 255);
    strip.setPixelColor(0, _pixelColors[0]);
    strip.setPixelColor(1, _pixelColors[0]);
    strip.setPixelColor(2, _pixelColors[0]);
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    // put your main code here, to run repeatedly:
    println("blah");
    delay(500);

}

/*** NEO-PIXEL FUNCTIONS TO BE USED LATER ON! ***/
void SetRandomColor(long LastRotationTime)
{
    // *** NEED TO DO! *** - This is currently assigning random colors - need something better

    byte Red = random(255); // Holds the red level
    byte Green = random(255); // Holds the green level
    byte Blue = random(255); // Holds the blue level
    for(byte b = 0; b < LED_COUNT; b++) 
    {
        _pixelColors[b] = strip.Color(Red, Green, Blue);
        strip.setPixelColor(b, _pixelColors[b]);
        usbDetectorLib.print("Pixel ");
        usbDetectorLib.print(b);
        usbDetectorLib.print("'s color: ");
        usbDetectorLib.println(_pixelColors[b]);
    }
    strip.show();
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

