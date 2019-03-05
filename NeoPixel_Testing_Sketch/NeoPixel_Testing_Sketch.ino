/*
  This is an app for testing Neopixel LEDs
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
  #define PIN 6 //NeoPixel Data Pin
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
  
  double RCol = 0;
  double GCol = 0;
  double BCol = 0;
  int RInc = 1;
  int GInc = 3;
  int BInc = 9;

  bool DigitalMic;
  int AnalogMic;

  // *** GENERAL CONFIGURATION ***
  bool Reporting = true;

// *********************************************************************
// The setup function runs once when you press reset or power the board
// *********************************************************************
void setup() 
{
  // *** GENERAL STUFF ***
  // If we're in "Reporting" mode, open the COM port
  if(Reporting) { Serial.begin(250000); }

  //*** NeoPixel Setup ***
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(2, INPUT);
  pinMode(A7, INPUT);  
}

// ********************************************************
// The main loop function runs over and over again forever
// ********************************************************
void loop() 
{
  //DigitalMic = digitalRead(2);
  AnalogMic = analogRead(A7);
  Serial.println(AnalogMic);      
/*
  if(DigitalMic == true)
  {
    for(int i = 0; i < strip.numPixels(); i++)
    {
      //RCol = RCol + RInc;
      //GCol = GCol + GInc;
      //BCol = BCol + BInc;
      
      //PixelCol = strip.Color(RCol, GCol, BCol);
      PixelCol = strip.Color(200, 20, 100);
      strip.setPixelColor(i, PixelCol);
      strip.show();
      //delay(50);
    }
    delay(0);
  }
  else
  {
    for(int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, 0);
      strip.show();
    }    
  }

  if(Reporting)
  {
      //PrintCurrentData();
      Serial.print(DigitalMic);
      Serial.print(" : ");
      Serial.println(AnalogMic);      
  }
*/
  // start_time = millis();

  // rainbow(20);
  // rainbowCycle(20);
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
  
//**********************************
//**** DATA REPORTING FUNCTIONS ****
//**********************************
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

/*
class PixelColor
{
  private:
    int _Red;
    int _Green;
    int _Blue;

  public:
    int PixelColor::Red()
    {
      return _Red;
    }

}

class NeoPixel 
{
  private:
    PixelColor _Color;
    int _PixelNum
 
  public:
    NeoPixel(int PixelNum);
    void SetPixelColor(int R, int G, int B);
    void SetPixelNum();
};

NeoPixel::NeoPixel(int PixelNum)
{
  this.
}
*/

/*
void Button::turnOff()
{
 this->down = 0;
 return;
}
*/
