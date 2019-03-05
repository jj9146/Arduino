// ****************************************************************************************************************************
// ***********************   THIS IS THE DEFINITION/DECLARATION SECTION, HIT ONLY ONCE PRIOR TO SETUP   ***********************
// ****************************************************************************************************************************

/***************************************   DEFINE ALL OF THE INPUT/OUTPUT PINS   *********************************************/
  int PIN_Relay_Power_LED1 = 23; //Green-to-Green  - uses inline 330 Ω resistor
  int PIN_Relay_Power_LED2 = 25; //Light Blue-to-Blue  - uses inline 330 Ω resistor
  int PIN_Relay_Power_Toggle1 = 27; //Yellow-to-Yellow
  int PIN_Relay_Power_Toggle2 = 29; //Orange w/White Stripe-to-Brown
  int PIN_Digital_LEDS = 2; //Digital LEDs signal pin
  int PIN_DP3T_Up = 31; //Red w/Black stripe-to-Purple
  int PIN_DP3T_Down = 33; //Red w/White stripe-to-White
  int PIN_Set_Button = 35; //Red-to-Red
  int PIN_Up_Button = 37; //Brown w/White stripe-to-Yellow
  int PIN_Down_Button = 39; //Brown-to-Brown
  int PIN_Ambient_Air_Signal = A0; //Black w/White stripe-to-Gray
  int PIN_LCD_4_RS = 41; //Black-to-Black
  int PIN_LCD_6_Enable = 42; //Pink-to-Green
  int PIN_LCD_11_D4 = 43; //White-to-White
  int PIN_LCD_12_D5 = 44; //Gray-to-Gray
  int PIN_LCD_13_D6 = 45; //Purple w/White stripe-to-Orange
  int PIN_LCD_14_D7 = 46; //Purple-to-Purple
  int PIN_Temp_Humid_Signal = 47; //Orange-to-Gray
  int PIN_Lower_Relay = 48; //Brown-to-Green
  int PIN_Upper_Relay = 49; //Brown-to-White
  int PIN_Limit_Switch = 50; //Black
  int PIN_Motor_Controller_12_Speed = 3; //PWM - Purple
  int PIN_Motor_Controller_10_Direction = 51; // Green
  int PIN_Motor_Controller_11_Direction = 52; // Blue

/***************************************   NEO-PIXEL VARIABLES AND CONSTANTS   **********************************************/
  #include <Adafruit_NeoPixel.h>
  #define PIN_NEO_Data PIN_Digital_LEDS // Set the PWM pin used by the LED array - uses inline 330 Ω resistor
  #define MAX_LED_COUNT 14
  #define MIN_LED_COUNT 14
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
    
/***************************************   LCD VARIABLES AND CONSTANTS   ******************************************************/
   /* The circuit:
   * LCD RS pin to digital pin 12 (PIN_LCD_4_RS on mine)
   * LCD Enable pin to digital pin 11 (PIN_LCD_6_Enable on mine)
   * LCD D4 pin to digital pin 5 (PIN_LCD_11_D4 on mine)
   * LCD D5 pin to digital pin 4 (PIN_LCD_11_D5 on mine)
   * LCD D6 pin to digital pin 3 (PIN_LCD_11_D6 on mine)
   * LCD D7 pin to digital pin 2 (PIN_LCD_11_D7 on mine)
   * LCD R/W pin to ground
   * LCD VSS pin to ground
   * LCD VCC pin to 5V
   * 10K resistor:
   * ends to +5V and ground
   * wiper to LCD VO pin (pin 3) */
  // include the library code:
  #include <LiquidCrystal.h>
  // initialize the library with the numbers of the interface pins
  //LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // original
  LiquidCrystal lcd(PIN_LCD_4_RS, PIN_LCD_6_Enable, PIN_LCD_11_D4, PIN_LCD_12_D5, PIN_LCD_13_D6, PIN_LCD_14_D7); // mine

/***************************************   SETUP THE DHT11 TEMP/HUMIDITY SENSOR   *********************************************/
  #include <DHT.h>
  #define DHTPIN PIN_Temp_Humid_Signal
  #define DHTTYPE DHT22
  DHT dht(DHTPIN, DHTTYPE);

/***************************************   SETUP THE DS18B20 DIGITAL TEMP SENSOR   ********************************************/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS PIN_Ambient_Air_Signal //ds18b20 module attach to pin PIN_Ambient_Air_Signal
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/***************************************   NOW DEFINE VARIABLES AND CONSTANTS FOR THE MAIN PROGRAM   ***************************/
  uint8_t lastToggle1 = LOW; //Tracks state of first toggle for LED1
  uint8_t lastToggle2 = LOW; //Tracks state of second toggle for LED2
  // These define the locations of the LEDs for each illuminated word in the upper panel
  uint8_t LED_PRINTER_START = 12;
  uint8_t LED_PRINTER_END = 13;
  uint16_t LED_PRINTER_COL = 0;
  uint8_t LED_LIGHTS_START = 10;
  uint8_t LED_LIGHTS_END = 11;
  uint16_t LED_LIGHTS_COL = 50;
  uint8_t LED_PRUSA_START = 4;
  uint8_t LED_PRUSA_END = 9;
  uint16_t LED_PRUSA_COL = 100;
  uint8_t LED_TEMP_START = 2;
  uint8_t LED_TEMP_END = 3;
  uint16_t LED_TEMP_COL = 150;
  uint8_t LED_MANUAL_START = 0;
  uint8_t LED_MANUAL_END = 1;
  uint16_t LED_MANUAL_COL = 200;
  unsigned long CheckTempInterval = 5000; // (value in miliseconds) Checking the digital temp takes .782 seconds, so we don't want to check it very often
  unsigned long StartTempTime = 0;
  unsigned long EndTempTime = 0;
  float lastDS18B20Temp = 0;
  unsigned long LastLEDCycleMillis = 0;
  uint8_t lastUpReading = LOW; //Tracks state of "UP" pins of the DP3T

  // Setup the COM port
  bool AlreadyConnected = false;
  long COM_Speed = 115200;
  bool PrintToCOM = true; // Set this to false to disable all messages to the COM port
        

// ******************************************************************************************************************
// ***********************   THIS IS THE MAIN SETUP FUNCTION, CALLED ONLY ONCE DURING STARTUP ***********************
// ******************************************************************************************************************
void setup() 
{
  /*** CONFIGURE ALL OF THE PINS ***/
  pinMode(PIN_Relay_Power_LED1, OUTPUT);
  pinMode(PIN_Relay_Power_LED2, OUTPUT);
  pinMode(PIN_Relay_Power_Toggle1, INPUT);
  pinMode(PIN_Relay_Power_Toggle2, INPUT);
  pinMode(PIN_DP3T_Up, INPUT);
  pinMode(PIN_DP3T_Down, INPUT);
  pinMode(PIN_Set_Button, INPUT);
  pinMode(PIN_Up_Button, INPUT);
  pinMode(PIN_Down_Button, INPUT);
  pinMode(PIN_Ambient_Air_Signal, INPUT);
  pinMode(PIN_LCD_4_RS, OUTPUT);
  pinMode(PIN_LCD_6_Enable, OUTPUT);
  pinMode(PIN_LCD_11_D4, OUTPUT);
  pinMode(PIN_LCD_12_D5, OUTPUT);
  pinMode(PIN_LCD_13_D6, OUTPUT);
  pinMode(PIN_LCD_14_D7, OUTPUT);
  pinMode(PIN_Temp_Humid_Signal, INPUT);
  pinMode(PIN_Lower_Relay, OUTPUT);
  pinMode(PIN_Upper_Relay, OUTPUT);
  pinMode(PIN_Limit_Switch, INPUT);
  pinMode(PIN_Motor_Controller_12_Speed, OUTPUT);
  pinMode(PIN_Motor_Controller_10_Direction, OUTPUT);
  pinMode(PIN_Motor_Controller_11_Direction, OUTPUT);
  
  /*** NOW SETUP THE NEO-PIXELS ***/
  LED_COUNT = MAX_LED_COUNT;
  //Setup the strip
  strip.begin();
  //Start Blue
  _pixelColors[0] = strip.Color(25, 25, 255);
  for(int i=0; i < MAX_LED_COUNT; i++)
  {
    strip.setPixelColor(i, _pixelColors[0]);
  }
  strip.show();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("It's Alive!");

  //Setup the temp/humidity sensor
  dht.begin();

  //Setup the digital temp sensor
  sensors.begin(); // initialize the bus
  
  PrintLn("Completed Setup");
}

// ******************************************************************************************************************
// ***********************     THIS IS THE MAIN LOOP OF THE PROGRAM, LOOPING FOREVER    *****************************
// ******************************************************************************************************************
void loop()
{
	// Check the toggle switch states and control their LEDs
	String ret = TakeCareOfToggleLEDs();
	if (ret != "OK") PrintLn("ERROR while checking toggle switches and LEDs: " + ret);

	// Now cycle all of the digital LEDs, giving each word a different color
	unsigned long LEDDelay = 5;
	CycleAllLEDs(LEDDelay); // Cycle the LEDs every (x) milliseconds

	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float Humidity = 0;
	float Temp = 0;
	GetHumidityAndTemp(Humidity, Temp);
	// Display the values on the second line
	lcd.setCursor(0, 1);
	lcd.print("H: ");
	lcd.setCursor(2, 1);
	lcd.print(Humidity);
	lcd.setCursor(6, 1);
	lcd.print("% T:");
	lcd.print(Temp);

	// If the timer has flipped, request the temp from the DS18B20 Sensor(s) (we're just using one for now) (takes 3/4 second per check!)
	float Temp2 = 0;
	unsigned long Ret = CheckDS18B20TempSensor(Temp2);
	if (Temp2 != 0)
	{
		lastDS18B20Temp = Temp2;
	}
	lcd.setCursor(0, 0);
	lcd.print(lastDS18B20Temp); //Uses 5 characters

	// Check the state of the motor control buttons, and move it as needed.
	// This includes checking the limit switch, too
	bool MoveUp = false;
	bool MoveDown = false;
	bool HitTopLimit = false;
	CheckMotorControls(MoveUp, MoveDown, HitTopLimit);
	if (!MoveUp && !MoveDown && !HitTopLimit)
	{
		lcd.setCursor(10, 0);
		lcd.print("[NADA]");
	}
	else
	{
		if (MoveUp)
		{
			lcd.setCursor(10, 0);
			lcd.print("  [UP]");
		}
		else
		{

		}
		char MsgLine1[16];
		char MsgLine2[16];
	}
}



// *******************************************************************************************************************
// *****************     SUB-FUNCTIONS/ROUTINES CALLED BY THE MAIN SETUP AND LOOP ROUTINES ABOVE     *****************
// *******************************************************************************************************************

void CheckMotorControls(bool & MoveUp, bool & MoveDown, bool & HitTopLimit)
{
	// First, check the "UP" button
	uint8_t UpReading = digitalRead(PIN_DP3T_Up);
	if (UpReading == HIGH)
	{
		MoveUp = true;
		if (lastUpReading != HIGH)
		{
			lastUpReading = HIGH;
			PrintLn("The UP motor control has been activated");
		}
	}
	else
	{
		MoveUp = false;
		if (lastUpReading == HIGH)
		{
			lastUpReading = LOW;
			PrintLn("The UP motor control has been DE-activated");
		}
	}
}

// This function will cycle the LEDs illuminating each word, changing shades every (x) milliseconds
void CycleAllLEDs(unsigned long DelayMillis) // Cycle the LEDs every (x) milliseconds
{
  if(millis() > (LastLEDCycleMillis + DelayMillis))
  {
    CycleLEDs(LED_MANUAL_START, LED_MANUAL_END, LED_MANUAL_COL);
    CycleLEDs(LED_TEMP_START, LED_TEMP_END, LED_TEMP_COL);
    CycleLEDs(LED_PRUSA_START, LED_PRUSA_END, LED_PRUSA_COL);
    CycleLEDs(LED_LIGHTS_START, LED_LIGHTS_END, LED_LIGHTS_COL);
    CycleLEDs(LED_PRINTER_START, LED_PRINTER_END, LED_PRINTER_COL);
    LastLEDCycleMillis = millis();
  }
}

// This function reads the humidity and temperature from the DHT11 Sensor and returns it to the caller
// via two ByRef parameters, so it can be displayed as needed
void GetHumidityAndTemp(float & Humidity, float & Temp)
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and report it
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    lcd.setCursor(0, 1);
    lcd.print("Unable to read DHT11");
    PrintLn("Failed to read from DHT sensor!");
  }
  else
  {
    // Report the humidity and temperature
    Humidity = h;
    Temp = f;
  }
}

// This function checks the temp via the DS18B20 digital temp sensor, and then sets up a "timer",
// constantly watching to see if the specified interval time has passed. It returns an unsigned long,
// so it may not display in the COM window correctly without using Serial.print() directly.
unsigned long CheckDS18B20TempSensor(float & Temp)
{
  if(millis() > EndTempTime)
  {
    //It's time to check and report the ambient temp
    sensors.requestTemperatures(); // Send the command to get temperatures
    float ambiTempC = sensors.getTempCByIndex(0);
    float ambiTempF = (1.8 * ambiTempC) + 32.0;
    Temp = ambiTempF;
    // Reset the timer using the value in CheckTempInterval at the top
    StartTempTime = millis();
    EndTempTime = StartTempTime + CheckTempInterval;
  }
  return (EndTempTime - millis());
}

// This function calls ToggleLED for each of the two toggle switches, and reports a problem if there is one
String TakeCareOfToggleLEDs()
{
  // First, check the first toggle/led pair
  uint8_t Toggle1Reading = digitalRead(PIN_Relay_Power_Toggle1);
  if(Toggle1Reading == HIGH)
  {
    if(lastToggle1 != HIGH)
    {            
      digitalWrite(PIN_Relay_Power_LED1, HIGH);
      lastToggle1 = HIGH;
      PrintLn("Toggle1 read HIGH, LED1 has been turned ON");    
    }
  }  
  else
  {
    if(lastToggle1 == HIGH)
    {            
      digitalWrite(PIN_Relay_Power_LED1, LOW);
      lastToggle1 = LOW;
      PrintLn("Toggle1 read LOW, LED1 has been turned OFF");    
    }
  }
  // Now, check the second toggle/led pair
  uint8_t Toggle2Reading = digitalRead(PIN_Relay_Power_Toggle2);
  if(Toggle2Reading == HIGH)
  {
    if(lastToggle2 != HIGH)
    {            
      digitalWrite(PIN_Relay_Power_LED2, HIGH);
      lastToggle2 = HIGH;
      PrintLn("Toggle2 read HIGH, LED2 has been turned ON");    
    }
  }  
  else
  {
    if(lastToggle2 == HIGH)
    {            
      digitalWrite(PIN_Relay_Power_LED2, LOW);
      lastToggle2 = LOW;
      PrintLn("Toggle2 read LOW, LED2 has been turned OFF");    
    }
  }
  return "OK";
}


// This function will assign color values to a specified range of digital LEDs,
// rotating through the range of colors
void CycleLEDs(uint8_t StartPos, uint8_t EndPos, uint16_t & ColNum)
{
    uint16_t i;
    for(i = StartPos; i <= EndPos; i++) 
    {
      strip.setPixelColor(i, Wheel((i + ColNum) & 255));
    }
    strip.show();
    ColNum++;
    if(ColNum > 255)
    {
      ColNum = 0;
    }
}

// Makes the pixels rotate through all of the colors
void rainbow(long wait) 
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


/*** COMMON COM PORT PRINTING FUNCTIONS ***/
void Print(String Msg)
{                                                                                                                                                                       
  if(PrintToCOM)
  {
    //Get connected at the specific speed if we're not already
    // Setup the USB COM port if it's not already set
    if(!AlreadyConnected)
    {
        Serial.begin(COM_Speed);
        AlreadyConnected = true;
    }
    Serial.print(Msg);
  }
}
void Print(byte Msg)
{                                                                                                                                                                       
  if(PrintToCOM)
  {
    //Get connected at the specific speed if we're not already
    // Setup the USB COM port if it's not already set
    if(!AlreadyConnected)
    {
        Serial.begin(COM_Speed);
        AlreadyConnected = true;
    }
    Serial.print(Msg);
  }
}

void PrintLn(String Msg)
{
  if(PrintToCOM)
  {
    //Get connected at the specific speed if we're not already
    // Setup the USB COM port if it's not already set
    if(!AlreadyConnected)
    {
        Serial.begin(COM_Speed);
        AlreadyConnected = true;
    }
    Serial.println(Msg);
  }
}

void PrintLn(byte Msg)
{
  if(PrintToCOM)
  {
    //Get connected at the specific speed if we're not already
    // Setup the USB COM port if it's not already set
    if(!AlreadyConnected)
    {
        Serial.begin(COM_Speed);
        AlreadyConnected = true;
    }
    Serial.println(Msg);
  }
}

// ******************************************************************************************************************
// *****************************     REFERENCE CODE THAT MIGHT BE USED LATER    *************************************
// ******************************************************************************************************************
      // THIS IS RELATED TO THE TEMP/HUMIDITY SENSOR SHOWING OTHER OPTIONS/FEATURES
//    // Compute heat index in Fahrenheit (the default)
//    float hif = dht.computeHeatIndex(f, h);
//    // Compute heat index in Celsius (isFahreheit = false)
//    float hic = dht.computeHeatIndex(t, h, false);
//    Serial.print("Humidity: ");
//    Serial.print(h);
//    Serial.print(" %\t");
//    Serial.print("Temperature: ");
//    Serial.print(t);
//    Serial.print(" *C ");
//    Serial.print(f);
//    Serial.print(" *F\t");
//    Serial.print("Heat index: ");
//    Serial.print(hic);
//    Serial.print(" *C ");
//    Serial.print(hif);
//    Serial.println(" *F");

  // This is how it's done with the thermistor, but then we're left with a voltage reading that would need
  // to be accurately converted into a temperature reading.  Hopefully the DS18B20 Digital Sensor can do that for us...
  //float ambiTemp = analogRead(PIN_Ambient_Air_Signal);
  //lcd.setCursor(0, 0);
  //lcd.print(ambiTemp);

/*** NEO-PIXEL FUNCTIONS TO BE USED LATER ON! ***/
/*
void SetRandomColor(long LastRotationTime)
{
    byte Red = random(255); // Holds the red level
    byte Green = random(255); // Holds the green level
    byte Blue = random(255); // Holds the blue level
    for(byte b = 0; b < LED_COUNT; b++) 
    {
        _pixelColors[b] = strip.Color(Red, Green, Blue);
        strip.setPixelColor(b, _pixelColors[b]);
        Print("Pixel ");
        Print(b);
        Print("'s color: ");
        PrintLn(_pixelColors[b]);
    }
    strip.show();
}
*/


