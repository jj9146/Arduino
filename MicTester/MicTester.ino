/*
  This is an app for testing audio input
*/
  int AnalogMic;
  int Min;
  int Max;
  int Avg;
  long AvgCount;
  long AvgTotal;
  String ReportingMode;

  int ResetPin = 11;
  int AnalogMicPin = A7;
  //unsigned long start_time;
  //int ResetTime = 5000; //5 seconds

// *********************************************************************
// The setup function runs once when you press reset or power the board
// *********************************************************************
void setup() 
{
  Serial.begin(250000);

  pinMode(AnalogMicPin, INPUT);  
  pinMode(ResetPin, INPUT);  
  //ReportingMode = "raw";
  ReportingMode = "minmax";
  //start_time = millis();
  
}

// ********************************************************
// The main loop function runs over and over again forever
// ********************************************************
void loop() 
{
  Serial.print("+");
  Serial.print(analogRead(AnalogMicPin));
  Serial.println(".");
}

void originalLoop()
{
  // If the "reset" button was hit, reset the min/max
  if(digitalRead(ResetPin) == HIGH)
  {
    //Time to calc and reset
    Serial.println("Recalculating...");      
    Avg = AvgTotal/AvgCount;
    Min = Avg;
    Max = Avg;
    AvgTotal = 0;
    AvgCount = 0;
  }

  AnalogMic = analogRead(AnalogMicPin);
  if(ReportingMode == "raw") {Serial.println(AnalogMic);} //Use this to watch raw data

  AvgTotal = AvgTotal + AnalogMic;
  AvgCount++;
  
  if(AnalogMic > Max)
  {
    Max = AnalogMic;
  }

  if(AnalogMic < Min)
  {
    Min = AnalogMic;
  }
  
  if(ReportingMode == "minmax")
  {
    Serial.print("Min: ");      
    Serial.print(Min);      
    Serial.print("  Max: ");      
    Serial.print(Max);      
    Serial.print("  Avg: ");      
    Serial.print(Avg);      
    Serial.print("  Current: ");      
    Serial.println(AnalogMic);      
  }
}

