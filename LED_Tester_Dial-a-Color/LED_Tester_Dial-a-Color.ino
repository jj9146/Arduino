/*
  This is a sketch for controlling a standard RGB LED with three variable resistors to
  dial-in any color manually.
*/
  bool Reporting = true;
  
  //Define the pins
  int R_Pin_Out = 3;
  int G_Pin_Out = 5;
  int B_Pin_Out = 6;
  int R_Pin_In = 0;
  int G_Pin_In = 1;
  int B_Pin_In = 2;
  double RCol = 0;
  double GCol = 0;
  double BCol = 0;

// the setup function runs once when you press reset or power the board
void setup() 
{
  if(Reporting) { Serial.begin(115200); }

  //Setup the pins
  pinMode(R_Pin_Out, OUTPUT);
  pinMode(G_Pin_Out, OUTPUT);
  pinMode(B_Pin_Out, OUTPUT);
  pinMode(R_Pin_In, INPUT);
  pinMode(G_Pin_In, INPUT);
  pinMode(B_Pin_In, INPUT);

}

// the loop function runs over and over again forever
void loop() 
{
  
  //Read the Red, Green, and Blue values from the three potentiometers, scaling them between 0 and 254
  RCol = map(analogRead(R_Pin_In), 0, 1023, 0, 254);
  GCol = map(analogRead(G_Pin_In), 0, 1023, 0, 254);
  BCol = map(analogRead(B_Pin_In), 0, 1023, 0, 254);

  analogWrite(R_Pin_Out, int(RCol));   // Set the Red LED level
  analogWrite(G_Pin_Out, int(GCol));   // Set the Green LED level
  analogWrite(B_Pin_Out, int(BCol));   // Set the Blue LED level

  if(Reporting)
  {
    Serial.println(RCol);
    Serial.println(GCol);
    Serial.println(BCol);
    Serial.println("");
  }

}






