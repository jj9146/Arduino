
bool RanOnce = false;

void setup() {
  // put your setup code here, to run once:
Serial.begin(250000);

}

void loop() {
  if(RanOnce)
  {
    return;
  }
  else
  {
      // put your main code here, to run repeatedly:
      float IntensityMultiplier = .785;
      byte Red = 65;
      Red *= IntensityMultiplier;
      Serial.println(Red);
  }
}
