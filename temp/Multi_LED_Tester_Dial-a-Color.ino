/*
  This is an app for testing multiple types of RGB LEDs
*/
// * DATA TYPE MAXIMUMS (Uses some C/C++ data types) *
// uint8_t = 255
// uint16_t = 65,536
// uint32_t = 4,294,967,296
// uint64_t = 18,446,744,073,709,551,616

  // *******************************************
  // ***** CONFIGURE THE NeoPixel RGB LEDS *****
  // *******************************************
  #include <Adafruit_NeoPixel.h>
  #define PIN 9 //NeoPixel Data Pin
  #define LED_COUNT 5
  // Parameter 1 = number of pixels in strip
  // Parameter 2 = pin number (most are valid)
  // Parameter 3 = pixel type flags, add together as needed:
  //   NEO_RGB     Pixels are wired for RGB bitstream
  //   NEO_GRB     Pixels are wired for GRB bitstream
  //   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
  //   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

  // ** Used to call NoDelayColorWipe NeoPixel Function **
  unsigned long start_time;
  uint16_t PixelNum;
  uint32_t PixelCol;
  uint32_t NoDelayColorWipeCallNumber;
  
  // *******************************************
  // ***** CONFIGURE THE STANDARD RGB LEDS *****
  // *******************************************
  //Define the pins
  int R_Pin_Out = 3;
  int G_Pin_Out = 5;
  int B_Pin_Out = 6;
  int R_Pin_In = 0;
  int G_Pin_In = 1;
  int B_Pin_In = 2;
  int R_Read = 0;
  int G_Read = 0;
  int B_Read = 0;
  double RCol = 0;
  double GCol = 0;
  double BCol = 0;

  // *** GENERAL CONFIGURATION ***
  bool Reporting = true;
  bool FirstPass = true;

// *********************************************************************
// The setup function runs once when you press reset or power the board
// *********************************************************************
void setup() 
{
  // *** GENERAL STUFF ***
  // If we're in "Reporting" mode, open the COM port
  if(Reporting) { Serial.begin(57600); }

  //*** Standard RGB LED Setup ***
  //Setup the pins
  pinMode(R_Pin_Out, OUTPUT);
  pinMode(G_Pin_Out, OUTPUT);
  pinMode(B_Pin_Out, OUTPUT);
  pinMode(R_Pin_In, INPUT);
  pinMode(G_Pin_In, INPUT);
  pinMode(B_Pin_In, INPUT);

  //*** NeoPixel Setup ***
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  NoDelayColorWipeCallNumber = 1;
}

// ********************************************************
// The main loop function runs over and over again forever
// ********************************************************
void loop() 
{
  //**********************
  //**** REGULAR LEDS ****
  //**********************
  //Read the Red, Green, and Blue values from the three potentiometers, scaling them between 0 and 254
  R_Read = analogRead(R_Pin_In);
  RCol = map(R_Read, 0, 1023, 0, 254);
  G_Read = analogRead(G_Pin_In);
  GCol = map(G_Read, 0, 1023, 0, 254);
  B_Read = analogRead(B_Pin_In);
  BCol = map(B_Read, 0, 1023, 0, 254);

  for(int i = 0; i < strip.numPixels(); i++)
  {
    PixelCol = strip.Color(RCol, GCol, BCol);
    strip.setPixelColor(i, PixelCol);
    strip.show();
  }
/*
  analogWrite(R_Pin_Out, int(RCol));   // Set the Red LED level
  analogWrite(G_Pin_Out, int(GCol));   // Set the Green LED level
  analogWrite(B_Pin_Out, int(BCol));   // Set the Blue LED level
*/
  if(Reporting)
  {
      PrintCurrentData();
  }
/*
  //************************
  //**** NEOPIXELS LEDS ****
  //************************
  // Some example procedures showing how to display the pixels:

  if(FirstPass == true)
  {
    start_time = millis();
    PixelNum = 0;
    Serial.print("Switch on ");
    Serial.println(NoDelayColorWipeCallNumber);
    switch (NoDelayColorWipeCallNumber) 
    {
      case 1:
        PixelCol = strip.Color(255, 0, 0); //Red
        break;
      case 2:
        PixelCol = strip.Color(255, 165, 0); //Orange
        break;
      case 3:
        PixelCol = strip.Color(255, 255, 0); //Yellow
        break;
      case 4:
        PixelCol = strip.Color(0, 255, 0); //Green
        break;
      case 5:
        PixelCol = strip.Color(0, 0, 255); //Blue
        break;
      case 6:
        PixelCol = strip.Color(75, 0, 130); //Indigo
        break;
      case 7:
        PixelCol = strip.Color(238, 130, 238); //Violet
        break;
      default:
        NoDelayColorWipeCallNumber = 1; //Restart the cycle
        PixelCol = strip.Color(255, 130, 130); //Pinkish
    }
    FirstPass = false;
  }

  //Call the function to update the pixels, and check to see if it's done each time
  if(NoDelayColorWipe(PixelCol, 500, start_time, PixelNum) == true)
  {
    //It's done with the Color Wipe we told it to do, so let's move to our next color wipe
    FirstPass = true;
    NoDelayColorWipeCallNumber++;
  }

  */
  // rainbow(20);
  // rainbowCycle(20);
}

  //********************************
  //**** REGULAR LEDS FUNCTIONS ****
  //********************************
  void PrintCurrentData()
  {
        if(Reporting)
        {
          Serial.print("Current Values: ");
          // Print the Red Component
          PrintSpacedComponent(RCol);
          Serial.print(" : ");
          PrintSpacedComponent(GCol);
          Serial.print(" : ");
          PrintSpacedComponent(BCol);
          Serial.println("");
        }
  }

  void PrintSpacedComponent(int value)
  {      
        if(value < 10)
        {
          Serial.print(value);
          Serial.print("  ");
        }
        else if(value < 100)
        {
          Serial.print(value);
          Serial.print(" ");
        }
        else
        {
          Serial.print(value);
        }
  }

  //**********************************
  //**** NEOPIXELS LEDS FUNCTIONS ****
  //**********************************

  // Fill the dots one after the other with a color
  bool NoDelayColorWipe(uint32_t color, uint16_t delay_ms, unsigned long & start_time, uint16_t & PixelNum) 
  {
    if(Reporting) { PrintNoDelayColorWipeParams(color, delay_ms, start_time, PixelNum); }
    if((millis() - start_time) >= delay_ms)
    {
      //Timer's up - time to move to the next pixel
      PixelNum++;
      start_time = millis();
      //If we've moved beyond the end, return the stop code
      if(PixelNum > strip.numPixels())
      {
        return true; //Tell the caller it's time to stop
      }
    }
    // Actually tell the strip what to do
    strip.setPixelColor(PixelNum, color);
    strip.show();
    return false; //Don't tell the caller it's time to stop
  }

void PrintNoDelayColorWipeParams(uint32_t color, uint16_t delay_ms, unsigned long start_time, uint16_t PixelNum) 
{
          Serial.print("NoDelayColorWipeParams: ");
          // Print the color
          PrintSpacedComponent(color);
          Serial.print(" : ");
          PrintSpacedComponent(delay_ms);
          Serial.print(" : ");
          Serial.print(start_time);
          Serial.print(" : ");
          Serial.println(PixelNum);
}

  /*
  // Fill the dots one after the other with a color
  void colorWipe(uint32_t c, uint8_t wait) 
  {
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
  }
  */
  
  /*
  void rainbow(uint8_t wait) 
  {
    uint16_t i, j;
    for(j=0; j<256; j++) 
    {
      for(i=0; i<strip.numPixels(); i++) 
      {
        strip.setPixelColor(i, Wheel((i+j) & 255));
      }
      strip.show();
      delay(wait);
    }
  }
  */

  /*
  // Slightly different, this makes the rainbow equally distributed throughout
  void rainbowCycle(uint8_t wait) 
  {
    uint16_t i, j;
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
      for(i=0; i< strip.numPixels(); i++) 
      {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
      delay(wait);
    }
  }
  */

  /*
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
  */
  



