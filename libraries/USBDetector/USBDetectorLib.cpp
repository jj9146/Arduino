/*
 Name:		USBDetectorLib.cpp
 Created:	7/22/2016 11:41:01 PM
 Author:	J.J. Slowik
 Editor:	http://www.visualmicro.com
 Description:
 Library that helps to determine if a Nano is being powered by the USB port, or a dedicated supply.  A USB-connected
 Nano can only muster enough juice to run a bank of 10 RGB LEDs, and even then it can get quite hot.  By soldering a
 10k resistor between Pin 1 of the USB jack and pin D12 (configurable) on the Nano, a digital read on that pin will
 tell you insantly if you're on a dedicated power supply or not.  This has two very useful effects:
	 (1.)  If your project uses too much power to run off of the USB, you can conditionally constrain its pull by using this
	 indicator, allowing you to power up just enough of the circuit to test it without frying it
	 (2.)  If you tend to dump status messages to the Serial Monitor while your process is running to aid in debugging, you
	 often have to go in and remove all of them to avoid slowing your finaly project.  This library includes functions that
	 you can call to write to the Serial Monitor and leave in, as they'll be automatically disabled once it detects that
	 it's not plugged into a USB adapter (and thus not a computer).
*/

#include "USBDetectorLib.h"


USBDetectorLib::USBDetectorLib(int usbPowerPin, long COM_BPS)
{
	pinMode(usbPowerPin, INPUT);
	_usbPowerPin = usbPowerPin;
	_haveCOMConnection = false;
	_COM_Speed = COM_BPS;
}

bool USBDetectorLib::ConnectAndValidateUSBPower()
{
	//Check the USB power pin to see if we have a USB connection
	int USBRead = digitalRead(_usbPowerPin);
	if (USBRead == HIGH)
	{
		//Setup the connection
		Serial.begin(_COM_Speed);
		_haveCOMConnection = true;
	}
	else
	{
		//We don't have a connection - bail
		_haveCOMConnection = false;
	}
	return _haveCOMConnection;
}

void USBDetectorLib::print(String strValue)
{
	if (_haveCOMConnection && !_halting) { Serial.print(strValue); }
}

void USBDetectorLib::print(int intValue)
{
	if (_haveCOMConnection && !_halting) { Serial.print(intValue); }
}

void USBDetectorLib::print(long lngValue)
{
	if (_haveCOMConnection && !_halting) { Serial.print(lngValue); }
}

void USBDetectorLib::print(unsigned long ulngValue)
{
	if (_haveCOMConnection && !_halting) { Serial.print(ulngValue); }
}

void USBDetectorLib::print(float ufltValue)
{
	if (_haveCOMConnection && !_halting) { Serial.print(ufltValue); }
}

void USBDetectorLib::print(byte ubyte)
{
	if (_haveCOMConnection && !_halting) { Serial.print(ubyte); }
}

void USBDetectorLib::println(String strValue)
{
	if (_haveCOMConnection && !_halting) { Serial.println(strValue); }
}

void USBDetectorLib::println(int intValue)
{
	if (_haveCOMConnection && !_halting) { Serial.println(intValue); }
}

void USBDetectorLib::println(long lngValue)
{
	if (_haveCOMConnection && !_halting) { Serial.println(lngValue); }
}

void USBDetectorLib::println(unsigned long ulngValue)
{
	if (_haveCOMConnection && !_halting) { Serial.println(ulngValue); }
}

void USBDetectorLib::println(float ufltValue)
{
	if (_haveCOMConnection && !_halting) { Serial.println(ufltValue); }
}

void USBDetectorLib::println(byte ubyte)
{
	if (_haveCOMConnection && !_halting) { Serial.println(ubyte); }
}

void USBDetectorLib::HaltPrintToCOM(bool Halting)
{
	_halting = Halting;
}
