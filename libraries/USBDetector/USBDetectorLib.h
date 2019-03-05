/*
 Name:		USBDetectorLib.h
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

#ifndef _USBDetectorLib_h
#define _USBDetectorLib_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class USBDetectorLib
{
public:
	USBDetectorLib(int usbPowerPin, long COM_BPS);
	bool ConnectAndValidateUSBPower();
	void print(String strValue);
	void print(int intValue);
	void print(long lngValue);
	void print(unsigned long ulngValue);
	void print(float ufltValue);
	void print(byte ubyte);
	void println(String strValue);
	void println(int intValue);
	void println(long lngValue);
	void println(unsigned long ulngValue);
	void println(float ufltValue);
	void println(byte ubyte);
	void HaltPrintToCOM(bool Halting);
private:
	bool _haveCOMConnection;
	bool _halting;
	long _COM_Speed;
	int _usbPowerPin;
};

#endif

