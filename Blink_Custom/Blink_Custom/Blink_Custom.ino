/*
  Blink
 */

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  int D1 = random(500);
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(23, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(D1);              // wait for a second
  int D2 = random(100);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(23, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(D2);              // wait for a second
  Serial.print(D1);
  Serial.print(" - ");
  Serial.println(D2);
}
