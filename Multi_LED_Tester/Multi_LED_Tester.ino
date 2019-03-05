/*
  This is an app for testing multiple RGB LEDs
*/
  unsigned long StartTime;
  unsigned long StartBlinkTime;
  unsigned long BlinkInterval;
  unsigned long BlinkPatternChangeInterval = 7000;
  bool BlinkOn = false;
  bool Reporting = false;
  
  //Define the pins
  int Constant_Pin = 2;
  int Blink_Pin = 4;
  int R_Pin = 3;
  int G_Pin = 5;
  int B_Pin = 6;
  double RCol = 0;
  double GCol = 0;
  double BCol = 0;
  double ColInc[3] = {.01, .03, .09};
  int SmallestInc = 0;
  int RInc = 0;
  int GInc = 1;
  int BInc = 2;
  bool TimeToShift = false;

// the setup function runs once when you press reset or power the board
void setup() 
{
  if(Reporting) { Serial.begin(115200); }

  //Setup the pins
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  //Turn on the constantly on pin
  digitalWrite(Constant_Pin, HIGH);  

  //Come up with an initial random blink time
  BlinkInterval = random(800); //Milliseconds
  //Start the timers
  StartTime = millis();
  StartBlinkTime = millis();
}

// the loop function runs over and over again forever
void loop() 
{
  //**** DETERMINE AND TRACK THE BLINK RATES ****
  //See if we've been showing the blink pattern for more than 7 seconds,
  //and if so, then reset the timers and come up with another
  if((millis() - StartTime) > BlinkPatternChangeInterval)
  {
    //It's time to change the blink rate (every <BlinkPatternChangeInterval> milliseconds)
    BlinkInterval = random(800); //Milliseconds
    //Reset the timers
    StartTime = millis();
    StartBlinkTime = millis();
  }
  // See if we need to turn the blinker on or off
  if((millis() - StartBlinkTime) > BlinkInterval)
  {
    if(BlinkOn == true)
    {
      BlinkOn = false;
      StartBlinkTime = millis();
    }
    else
    {
      BlinkOn = true;
      StartBlinkTime = millis();
    }
  }

  //Adjust the analog colors
  RCol = RCol + ColInc[RInc];
  GCol = GCol + ColInc[GInc];
  BCol = BCol + ColInc[BInc];

  //Make sure they haven't gone too far in one direction or another
  if(RCol > 256 or RCol < 0)
  {
    ColInc[RInc] = ColInc[RInc] * -1;
    RCol = RCol + ColInc[RInc];
    if ((RCol < 0) && (SmallestInc == RInc)) { TimeToShift = true; }
  }
  if(GCol > 256 or GCol < 0)
  {
    ColInc[GInc] = ColInc[GInc] * -1;
    GCol = GCol + ColInc[GInc];
    if ((GCol < 0) && (SmallestInc == GInc)) { TimeToShift = true; }
  }
  if(BCol > 256 or BCol < 0)
  {
    ColInc[BInc] = ColInc[BInc] * -1;
    BCol = BCol + ColInc[BInc];
    if ((BCol < 0) && (SmallestInc == BInc)) { TimeToShift = true; }
  }
  
  //**** TURN THE PINS ON AND OFF ****
  if(BlinkOn==true)
  {
    digitalWrite(Blink_Pin, HIGH);   // turn the LED ON (HIGH = 5V)
  }
  else
  {
    digitalWrite(Blink_Pin, LOW);   // turn the LED OFF (LOW = 0V)
  }

    analogWrite(R_Pin, int(RCol));   // Set the Red LED level
    analogWrite(G_Pin, int(GCol));   // Set the Green LED level
    analogWrite(B_Pin, int(BCol));   // Set the Blue LED level

  if(Reporting)
  {
    Serial.println(RCol);
    Serial.println(GCol);
    Serial.println(BCol);
    Serial.println("");
  }

  // If it's time to change things up, then move their rates
  // around so they all have the same chances to produce the same patterns
  if(TimeToShift == true)
  {
    //Bump them all up by one postion
    RInc = RInc + 1;
    GInc = GInc + 1;
    BInc = BInc + 1;
    //If one was bumped out, make it the 0 index
    if (RInc > 2){RInc = 0;}
    if (GInc > 2){GInc = 0;}
    if (BInc > 2){BInc = 0;}
    TimeToShift = false;
  } 
  
}






