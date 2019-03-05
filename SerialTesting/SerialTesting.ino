#include <USBDetectorLib.h>

//Create the USBDetectorLib class
//  Use pin 7 for the USB Power Indicator LED, pin 12 for the USB Power Connection (via 10k resistor)
USBDetectorLib usbDetectorLib(12, 250000); 
int LEDPin = 7;
bool _haveCOMConnection;                             

void setup() 
{
    pinMode(LEDPin, OUTPUT);
    _haveCOMConnection = usbDetectorLib.ConnectAndValidateUSBPower(); //Tells us if we're hooked up to a PC or battery/power supply 
    if(_haveCOMConnection)
    {
        digitalWrite(LEDPin, HIGH);
    }
}

void loop() 
{
    // put your main code here, to run repeatedly:
    usbDetectorLib.print("Timer: ");
    usbDetectorLib.println(millis());
    delay(1000);
    // triple-blink the built-in LED on pin 13 to let them know it's running
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
}



