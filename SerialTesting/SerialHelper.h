/*
  SerialHelper.h - Library for auto-magically determining
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
#ifndef SerialHelper_h
#define SerialHelper_h

#include "Arduino.h"

class SerialHelper
{
  public:
    SerialHelper(int indicatorPin, int usbPowerPin, long COM_BPS);
    void ConnectAndValidateUSBPower();
    void PrintStr(String text);
    void PrintStrLn(String textln);
  private:
    bool _haveCOMConnection;
    long _COM_Speed;
    int _serialIndicatorPin;
    int _usbPowerPin;
};

#endif


