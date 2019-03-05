/* 
  Hookup Configuration:
  ---------------
  The stepper motor should be attached to digital pins 8 - 11 of the Arduino.
  Servo #1 goes to pin 3, Servo #2 goes to pin 2.
  Servos #3, #4, and #5 go to pins 4, 5, and 6, respectively
  The left joystick should be connected to pins A0, A1, and A2 (horizontal, vertical, and click)
  The right joystick should be connected to pins A3, A4, and A5 (horizontal, vertical, and click)
  The left trigger should be connected to pin A6, and the right trigger connected to pin A7
  Debug data will be sent on COM port at 57,600 BPS
*/
 
// STEPPER-RELATED VARIABLES
#include <Stepper.h>
int StepperPin1 = 8;
int StepperPin2 = 9;
int StepperPin3 = 10;
int StepperPin4 = 11;
const int intStepSize = 1;  //Sets how big each step is
const int stepsPerRevolution = 200;  // This is what my stepper is rated for
int intStepperRPM = 60;  //Sets the speed of the stepper motor
Stepper myStepper(stepsPerRevolution, StepperPin1, StepperPin2, StepperPin3, StepperPin4); // initialize the stepper library on pins 8 through 11
int intCurrentStep = 0;  //Tracks the position of the stepper
unsigned long lngLastStepperMove = 0; //Keeps tabs on the last time the stepper moved, so it can be turned off when not needed (IMPORTANT!!)

// SERVO-RELATED VARIABLES
#include <Servo.h> 
// *** FIX THE DESCRIPTIONS OF ALL THE NEW SERVOS!
Servo Servo1;  // create servo object to control the BASE servo
Servo Servo2;  // create servo object to control the ELBOW servo
Servo Servo3;  // create servo object to control the ELBOW servo
Servo Servo4;  // create servo object to control the ELBOW servo
Servo Servo5;  // create servo object to control the ELBOW servo
int Servo1StartAngle = 0;  // start position for base servo
int Servo1WriteAngle = 0; //Variable that holds the base servo's angle
int Servo2StartAngle = 175;  // start position for elbow servo
int Servo2WriteAngle = 0; //Variable that holds the elbow servo's angle
int Servo3StartAngle = 90;  // start position for base servo
int Servo3WriteAngle = 0; //Variable that holds the base servo's angle
int Servo4StartAngle = 90;  // start position for base servo
int Servo4WriteAngle = 0; //Variable that holds the base servo's angle
int Servo5StartAngle = 90;  // start position for base servo
int Servo5WriteAngle = 0; //Variable that holds the base servo's angle
int intServo1MaxAngle = 180; //Max base servo angle
int intServo1MinAngle = 0; //Min base servo angle
int intServo2MaxAngle = 175; //Max elbow servo angle
int intServo2MinAngle = 68; //Min elbow servo angle
int intServo3MaxAngle = 150; //Max verticle wrist servo angle
int intServo3MinAngle = 30; //Min vertcle wrist servo angle
int intServo4MaxAngle = 150; //Max elbow servo angle
int intServo4MinAngle = 30; //Min elbow servo angle
int intServo5MaxAngle = 150; //Max elbow servo angle
int intServo5MinAngle = 30; //Min elbow servo angle
int intServo1ChangeRate = 1; // controls how fast the base servo changes direction
int intServo2ChangeRate = 1; // controls how fast the elbow servo changes direction
int intServo3ChangeRate = 1; // controls how fast the elbow servo changes direction
int intServo4ChangeRate = 1; // controls how fast the elbow servo changes direction
int intServo5ChangeRate = 1; // controls how fast the elbow servo changes direction
int Servo1Pin = 3; // The pin the first servo's signal wire is connected to
int Servo2Pin = 2; // The pin the second servo's signal wire is connected to
int Servo3Pin = 4; // The pin the second servo's signal wire is connected to
int Servo4Pin = 5; // The pin the second servo's signal wire is connected to
int Servo5Pin = 6; // The pin the second servo's signal wire is connected to

// JOYSTICK-RELATED VARIABLES
int intLeftJoystickXPin = 0; // The analog pin the Joystick's X-axis is connected to
int intLeftJoystickYPin = 1; // The analog pin the Joystick's Y-axis is connected to
int intLeftJoystickClickPin = 2; // The analog pin the Joystick's "click" is connected to
int intLeftXMidPoint = 0; // Used to get the joystick's "X" at-rest value
int intLeftYMidPoint = 0; // Used to get the joystick's "Y" at-rest value
int intRightJoystickXPin = 3; // The analog pin the Joystick's X-axis is connected to
int intRightJoystickYPin = 4; // The analog pin the Joystick's Y-axis is connected to
int intRightJoystickClickPin = 5; // The analog pin the Joystick's "click" is connected to
int intRightXMidPoint = 0; // Used to get the joystick's "X" at-rest value
int intRightYMidPoint = 0; // Used to get the joystick's "Y" at-rest value
int intLeftTriggerPin = 6; // The analog pin the Joystick's X-axis is connected to
int intRightTriggerPin = 7; // The analog pin the Joystick's Y-axis is connected to
int intLeftTriggerStartPoint = 0; // Used to get the left trigger's at-rest value
int intRightTriggerStartPoint = 0; // Used to get the right trigger's at-rest value
unsigned long lngLastServo2Move = 0; //Keeps tabs on the last time the base servo moved, so it can be turned off when not needed (IMPORTANT!!)
unsigned long lngLastServo1Move = 0; //Keeps tabs on the last time the elbow servo moved, so it can be turned off when not needed (IMPORTANT!!)
unsigned long lngLastServo3Move = 0; //Keeps tabs on the last time the first wrist servo moved, so it can be turned off when not needed (IMPORTANT!!)
unsigned long lngLastServo4Move = 0; //Keeps tabs on the last time the second wrist servo moved, so it can be turned off when not needed (IMPORTANT!!)
unsigned long lngLastServo5Move = 0; //Keeps tabs on the last time the third wrist servo moved, so it can be turned off when not needed (IMPORTANT!!)

// OTHER VARIABLES
boolean Debugging; // Set to True to have data spit out on the COM port

// MAIN SETUP FUNCTION
void setup() 
{
  //If this is set to "true", debug info will be reported via the COM port
  Debugging = true; 
  if(Debugging) Serial.begin(57600); //Begin transmitting to the serial port

  //Setup the stepper  
  myStepper.setSpeed(intStepperRPM);  // set the stepper speed (RPM)
  //Setup the servos
  pinMode(Servo1Pin, OUTPUT); // initialize a digital output for the base servo
  pinMode(Servo2Pin, OUTPUT); // initialize a digital output for the elbow servo
  pinMode(Servo3Pin, OUTPUT); // initialize a digital output for a wrist servo
  pinMode(Servo4Pin, OUTPUT); // initialize a digital output for a wrist servo
  pinMode(Servo5Pin, OUTPUT); // initialize a digital output for a wrist servo
  //Setup the joystick
  pinMode(intLeftJoystickClickPin, INPUT); // initialize a digtal input for the left click
  pinMode(intRightJoystickClickPin, INPUT); // initialize a digtal input for the right click
  Servo1.attach(Servo1Pin);  // attaches the servo on pin Servo1Pin to the servo object 
  Servo2.attach(Servo2Pin);  // attaches the servo on pin Servo2Pin to the servo object 
  Servo3.attach(Servo3Pin);  // attaches the servo on pin Servo2Pin to the servo object 
  Servo4.attach(Servo4Pin);  // attaches the servo on pin Servo2Pin to the servo object 
  Servo5.attach(Servo5Pin);  // attaches the servo on pin Servo2Pin to the servo object 
  Servo1WriteAngle = Servo1StartAngle;
  Servo2WriteAngle = Servo2StartAngle;
  Servo3WriteAngle = Servo3StartAngle;
  Servo4WriteAngle = Servo4StartAngle;
  Servo5WriteAngle = Servo5StartAngle;
  Servo1.write(Servo1WriteAngle); //Jump to the start position
  Servo2.write(Servo2WriteAngle); //Jump to the start position
  Servo3.write(Servo2WriteAngle); //Jump to the start position
  Servo4.write(Servo2WriteAngle); //Jump to the start position
  Servo5.write(Servo2WriteAngle); //Jump to the start position
  intLeftXMidPoint = analogRead(intLeftJoystickXPin); // Get the left joystick's "X" at-rest value
  intLeftYMidPoint = analogRead(intLeftJoystickYPin); // Get the left joystick's "Y" at-rest value
  intRightXMidPoint = analogRead(intRightJoystickXPin); // Get the left joystick's "X" at-rest value
  intRightYMidPoint = analogRead(intRightJoystickYPin); // Get the left joystick's "Y" at-rest value
  intLeftTriggerStartPoint = analogRead(intLeftTriggerPin); // Get the left trigger's at-rest value
  intRightTriggerStartPoint = analogRead(intRightTriggerPin); // Get the right trigger's at-rest value
  DebugPrint("Left X-axis at-rest reading: ", intLeftXMidPoint);
  DebugPrint("Left Y-axis at-rest reading: ", intLeftYMidPoint);
  DebugPrint("Right X-axis at-rest reading: ", intRightXMidPoint);
  DebugPrint("Right Y-axis at-rest reading: ", intRightYMidPoint);
  DebugPrint("Left Trigger at-rest reading: ", intLeftTriggerStartPoint);
  DebugPrint("Right Trigger at-rest reading: ", intRightTriggerStartPoint);
}

void loop() 
{
  boolean isOn = true; //(digitalRead(intPowerPin) == HIGH); //You can use a pin to detect power if that'll help in the future
  if(isOn)
  {
    int ControllerMode = 0;
    int intLeftTrigger = analogRead(intLeftTriggerPin);
    int intRightTrigger = analogRead(intRightTriggerPin);
    DebugPrint("Left Trigger Read: ", intLeftTrigger);
    DebugPrint("Right Trigger Read: ", intRightTrigger);

    int intX = 0;
    int intY = 0;
    if(intLeftTrigger < (intLeftTriggerStartPoint - 100))
    {
      //They're pulling the left trigger...
      ControllerMode=+1;
    }
    if(intRightTrigger < (intRightTriggerStartPoint - 100))
    {
      //They're pulling the right trigger...
      ControllerMode=+2;      
    }
    DebugPrint("ControllerMode: ", ControllerMode);

    switch(ControllerMode)
    {
      // ******************************************************************************************************************
      // *** NO TRIGGER PULLED - CONTROL STEPPER MOTOR, BASE SERVO (LEFT THUMBSTICK) AND ELBOW SERVO (RIGHT THUMBSTICK) ***
      // ******************************************************************************************************************
      case 0:
        //MOVE THE STEPPER/ROTATE THE BASE
        //See where the X-axis of the left joystick is
        intX = analogRead(intLeftJoystickXPin);
        if(intX < (intLeftXMidPoint - 300)) //Force them to move farther to avoid accidently movements
        {
          //Moving Left on Left Joystick
          myStepper.step(intStepSize);
          lngLastStepperMove = millis();
          intCurrentStep += intStepSize;
          if(intCurrentStep > stepsPerRevolution) intCurrentStep = 0;
          DebugPrint("Stepped Left to step #", intCurrentStep);
        }
        if(intX > (intLeftXMidPoint + 300)) //Force them to move farther to avoid accidently movements
        {
          //Moving Right on Left Joystick
          myStepper.step(intStepSize * -1);
          lngLastStepperMove = millis();
          intCurrentStep -= intStepSize;
          if(intCurrentStep < 0) intCurrentStep = stepsPerRevolution;
          DebugPrint("Stepped Right to step #", intCurrentStep);
        }
        //This should turn the coils off after a second, so the driver doesn't overheat
        if((millis() - lngLastStepperMove) > 1000)
        {
          digitalWrite(StepperPin1, LOW);
          digitalWrite(StepperPin2, LOW);
          digitalWrite(StepperPin3, LOW);
          digitalWrite(StepperPin4, LOW);
        }
        //MOVE SERVO 1/TILT THE BASE OF THE ARM
        //See where the Y-axis of the left joystick is
        intY = analogRead(intLeftJoystickYPin);
        if(intY < (intLeftYMidPoint - 100))
        {
          //Moving Up on Joystick
          if(!Servo1.attached()) Servo1.attach(Servo1Pin);
          Servo1WriteAngle += intServo1ChangeRate; // Increase the angle
          if(Servo1WriteAngle >= intServo1MaxAngle) Servo1WriteAngle = intServo1MaxAngle;
          Servo1.write(Servo1WriteAngle); // Change the servo's angle
          lngLastServo1Move = millis();
          DebugPrint("Angled base servo degrees up to ", Servo1WriteAngle);
        }
        if(intY > (intLeftYMidPoint + 100))
        {
          //Moving Down on Joystick
          if(!Servo1.attached()) Servo1.attach(Servo1Pin);
          Servo1WriteAngle -= intServo1ChangeRate; // Decrease the angle
          if(Servo1WriteAngle <= intServo1MinAngle) Servo1WriteAngle = intServo1MinAngle;
          Servo1.write(Servo1WriteAngle); // Change the base servo's angle
          lngLastServo1Move = millis();
          DebugPrint("Angled base servo degrees down to ", Servo1WriteAngle);
        }
        //This should turn the base servo off after a second, so it doesn't overheat
        if((millis() - lngLastServo1Move) > 1000)
        {
          Servo1.detach();
        }
        //MOVE SERVO 2/TILT THE ELBOW OF THE ARM
        //See where the Y-axis of the right joystick is
        intY = analogRead(intRightJoystickYPin);
        if(intY > (intRightYMidPoint + 100))
        {
          //Moving Up on right Joystick
          if(!Servo2.attached()) Servo2.attach(Servo2Pin);
          Servo2WriteAngle += intServo2ChangeRate; // Increase the angle
          if(Servo2WriteAngle >= intServo2MaxAngle) Servo2WriteAngle = intServo2MaxAngle;
          Servo2.write(Servo2WriteAngle); // Change the elbow servo's angle
          lngLastServo2Move = millis();
          DebugPrint("Angled elbow servo degrees up to ", Servo2WriteAngle);
        }
        if(intY < (intRightYMidPoint - 100))
        {
          //Moving Down on right Joystick
          if(!Servo2.attached()) Servo2.attach(Servo2Pin);
          Servo2WriteAngle -= intServo2ChangeRate; // Decrease the angle
          if(Servo2WriteAngle <= intServo2MinAngle) Servo2WriteAngle = intServo2MinAngle;
          Servo2.write(Servo2WriteAngle); // Change the elbow servo's angle
          lngLastServo2Move = millis();
          DebugPrint("Angled elbow servo degrees down to ", Servo2WriteAngle);
        }
        //This should turn the elbow servo off after a second, so it doesn't overheat
        if((millis() - lngLastServo2Move) > 1000)
        {
          Servo2.detach();
        }
        delay(10);
      break;
      
      
      // Left Teigger Pulled
      case 1:
      // ************************************************************
      // *** LEFT TRIGGER PULLED - CONTROL THE THREE WRIST SERVOS ***
      // ************************************************************
        //MOVE THE Horizontal Wrist Servo (#3)
        //See where the X-axis of the left joystick is
        intX = analogRead(intLeftJoystickXPin);
        if(intX < (intLeftXMidPoint - 100))
        {
          //Moving Left on Left Joystick
          if(!Servo3.attached()) Servo3.attach(Servo3Pin);
          Servo3WriteAngle -= intServo3ChangeRate; // Decrease the angle
          if(Servo3WriteAngle <= intServo3MinAngle) Servo3WriteAngle = intServo3MinAngle;
          Servo3.write(Servo3WriteAngle); // Change the first wrist servo angle
          lngLastServo3Move = millis();
          DebugPrint("Angled wrist servo (#3) degrees down to ", Servo3WriteAngle);
        }
        if(intX > (intLeftXMidPoint + 100)) 
        {
          //Moving Right on Left Joystick
          if(!Servo3.attached()) Servo3.attach(Servo1Pin);
          Servo3WriteAngle += intServo3ChangeRate; // Increase the angle
          if(Servo3WriteAngle >= intServo3MaxAngle) Servo3WriteAngle = intServo3MaxAngle;
          Servo3.write(Servo3WriteAngle); // Change the servo's angle
          lngLastServo3Move = millis();
          DebugPrint("Angled wrist servo (#3) degrees up to ", Servo3WriteAngle);
        }
        //This should turn the elbow servo off after a second, so it doesn't overheat
        if((millis() - lngLastServo3Move) > 1000)
        {
          Servo3.detach();
        }

        //MOVE SERVO 4/TILT THE END OF THE ARM
        //See where the Y-axis of the left joystick is
        intY = analogRead(intLeftJoystickYPin);
        if(intY < (intLeftYMidPoint - 100))
        {
          //Moving Up on Joystick
          if(!Servo4.attached()) Servo4.attach(Servo4Pin);
          Servo4WriteAngle += intServo4ChangeRate; // Increase the angle
          if(Servo4WriteAngle >= intServo4MaxAngle) Servo4WriteAngle = intServo4MaxAngle;
          Servo4.write(Servo4WriteAngle); // Change the servo's angle
          lngLastServo4Move = millis();
          DebugPrint("Angled wrist servo #4 degrees up to ", Servo4WriteAngle);
        }
        if(intY > (intLeftYMidPoint + 100))
        {
          //Moving Down on Joystick
          if(!Servo4.attached()) Servo4.attach(Servo4Pin);
          Servo4WriteAngle -= intServo4ChangeRate; // Decrease the angle
          if(Servo4WriteAngle <= intServo4MinAngle) Servo4WriteAngle = intServo4MinAngle;
          Servo4.write(Servo4WriteAngle); // Change the base servo's angle
          lngLastServo4Move = millis();
          DebugPrint("Angled base servo degrees down to ", Servo4WriteAngle);
        }
        //This should turn the base servo off after a second, so it doesn't overheat
        if((millis() - lngLastServo4Move) > 1000)
        {
          Servo4.detach();
        }
    
        //MOVE SERVO 5/Spin the servo at the end of the arm
        //See where the Y-axis of the right joystick is
        intY = analogRead(intRightJoystickYPin);
        if(intY > (intRightYMidPoint + 100))
        {
          //Moving Up on right Joystick
          if(!Servo5.attached()) Servo5.attach(Servo5Pin);
          Servo5WriteAngle += intServo5ChangeRate; // Increase the angle
          if(Servo5WriteAngle >= intServo5MaxAngle) Servo5WriteAngle = intServo5MaxAngle;
          Servo5.write(Servo5WriteAngle); // Change the elbow servo's angle
          lngLastServo5Move = millis();
          DebugPrint("Spun servo at end of arm degrees up to ", Servo5WriteAngle);
        }
        if(intY < (intRightYMidPoint - 100))
        {
          //Moving Down on right Joystick
          if(!Servo5.attached()) Servo5.attach(Servo5Pin);
          Servo5WriteAngle -= intServo5ChangeRate; // Decrease the angle
          if(Servo5WriteAngle <= intServo5MinAngle) Servo5WriteAngle = intServo5MinAngle;
          Servo5.write(Servo5WriteAngle); // Change the elbow servo's angle
          lngLastServo5Move = millis();
          DebugPrint("Angled servo at end of arm degrees down to ", Servo5WriteAngle);
        }
        //This should turn the elbow servo off after a second, so it doesn't overheat
        if((millis() - lngLastServo5Move) > 1000)
        {
          Servo5.detach();
        }
        delay(10);

      break;

      // Right Trigger Pulled
      case 2:

      break;

      // Both Triggers Pulled
      case 3:

      break;
    }
    
  }
}

// Sends a debug message out on the COM port, if debugging is turned on
void DebugPrint(String strMsg)
{
  if(Debugging)
  {
    Serial.println(strMsg);
  }
}

// Sends a debug message and integer value out on the COM port, if debugging is turned on
void DebugPrint(String strMsg, int intVal)
{
  if(Debugging)
  {
    Serial.print(strMsg);
    Serial.println(intVal);
  }
}

