// Objective: To design a psuhbutton monitored LED

const int pinNumber = 13; 

void setup(){
// runs once every restart
  
  Serial.begin(9600); // begin a 9600 bps information transfer
  pinMode(pinNumber, INPUT);
  
}

void loop(){
  int input = digitalRead(pinNumber);
    if (input == 1)
      Serial.println("The LED is lit");
    delay(1);
}


/*
* 1. The LED bulb lights up when the push button is pressed.
*/
