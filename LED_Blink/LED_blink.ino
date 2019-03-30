/* Objective:
Turns an LED on for one second, then off for one second, repeatedly.

Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
the correct LED pin independent of which board is used.
If you want to know what pin the on-board LED is connected to on your Arduino
model, check the Technical Specs of your board at: https://www.arduino.cc/en/Main/Products

*/

void setup(){
// Runs once when the reset button is pressed
  pinMode(LED_BUILTIN, OUTPUT); // initialises the builtin LED as the output
  Serial.begin(9600);
}

void loop(){
  digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED turned on");
  delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("LED turned off");
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED turned on");
    delay(1000);
  
}
