//DigitalReadSerial

//Reads a digital input on pin 2, prints the result to the Serial Monitor
// Resistance used is 10 kilo Ohm

const int pin_number = 7;
void setup(){
// runs once every reset button
  Serial.begin(9600); // begin a 9600 bps information transfer
  pinMode(pin_number, INPUT); // Changing the state of the pin to be an input
}


void loop(){
  
  int output = digitalRead(pin_number);
  if(output == 1)
    Serial.println("Button was pushed");
  delay(1);
}

/*
* Takeaways:
  1. Push buttons tend to join any two points on the board.
    2. By some tinkering, it is obvious that the points are diagonally
    opposite (the point diagonal to the VCC works only when the button is pushed)
    3. The point in front of the VCC is always on (short circuited).
*/
