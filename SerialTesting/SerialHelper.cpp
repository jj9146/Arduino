/*
  SerialHelper.cpp - Library for auto-magically determining
  if a board is being powered by the USB port, and will halt
  all attempts to connect to and send data over COM when
  there is no computer/device to listen or respond.
  *** For Arduino Nano boards ***
  * NOTE:  Requires a 10k resistor soldered between Pin 1 of the Micro USB jack and pin D12 on the
  * arduino.  Attach it to the shotsky diode directly below the USB jack (easiest to solder), and 
  * let it sit vertically along the left side of the Micro USB jack, with the top wire bent down and
  * over to the 12 pin.  You could put the entire resistor in shrink-wrap tubing, but there's really
  * nothing to short on, so that might be a lost cause.
*/
#include "Arduino.h"
#include "SerialHelper.h"

SerialHelper::SerialHelper(int indicatorPin, int usbPowerPin, long COM_BPS)
{
    pinMode(indicatorPin, OUTPUT);
    pinMode(usbPowerPin, INPUT);
    _serialIndicatorPin = indicatorPin;
    _usbPowerPin = usbPowerPin;
    _haveCOMConnection = false;
	_COM_Speed = COM_BPS;
}

/*
 * ConnectAndValidateUSBPower will look for power from the USB jack to be fed into D12
 * If it finds power and can make the COM connection, then it'll set "_haveCOMConnection"
 * to true, and return it as the response to the caller
 */
void SerialHelper::ConnectAndValidateUSBPower()
{
    //Check pin 12 to see if we have a USB connection
    int USBRead = digitalRead(_usbPowerPin);
    if(USBRead == HIGH)
    {
        //Setup the connection
        Serial.begin(_COM_Speed);
        _haveCOMConnection = true;
        //Setup the connection indicator pin
        pinMode(_serialIndicatorPin, OUTPUT);
        //Turn the indicator on
        digitalWrite(_serialIndicatorPin, HIGH);
    }
    else
    {
        //We don't have a connection - bail
        _haveCOMConnection = false;
    }
    //return _haveCOMConnection;
}

// Print text to COM port without a CrLf
void SerialHelper::PrintStr(String text)
{
    if(_haveCOMConnection)
    {
        Serial.print(text);
    }
}

// Print text to COM port with a CrLf
void SerialHelper::PrintStrLn(String text)
{
    if(_haveCOMConnection)
    {
        Serial.println(text);
    }
}

 
