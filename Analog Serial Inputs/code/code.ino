// Objective: Read Analog serial input and print to serial monitor


void setup() // runs once the reset button is pressed
{
  Serial.begin(9600); // initializing the serial communiation bps
}

void loop()
{
   int input = analogRead(A0); // Reads the analog input from A0 pin
    Serial.println(input);
    delay(1); // delay in milli-seconds for stability
}


/*Takeaways- 
* 1. The potentiometer's dial can be turned to simulate voltage
  values from 0V to 5V. On the Serial monitor, this range is represented by
  0 - 1023
* 2. The potentiometer resistance is at 250 kilo-ohms
*/
