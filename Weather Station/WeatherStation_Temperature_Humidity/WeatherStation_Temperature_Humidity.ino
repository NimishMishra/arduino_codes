// Credits to the DHT11 library
// 0.1.0 by Rob Tillaart (01/04/2011)
// inspired by DHT11 library

#include "dht.h"

int signal_pin = A0; // or any other ANALOG input you wish to have
dht DHT; // object of the DHT class to enable analog inputs

void dht_loop(){
    DHT.read11(signal_pin); // reads data from pin A0
    Serial.print("Temperature =");
    Serial.print(DHT.temperature);
    Serial.print("  Humidity = ");
    Serial.println(DHT.humidity);
 }
void setup(){
  // Inializing the Serial Monitor with 9600 bps 
  Serial.begin(9600); 
  delay(500);//Delay to let system boot
 
}//end "setup()"
 
void loop(){ 
    
    dht_loop();
    delay(2000);
}
