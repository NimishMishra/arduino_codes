// Libraries used:

// DS3231 (the ZS-042 RTC1 module as designed in Fritzing):
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/

// Arduino Mega:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL pin
//         bps used  -> 9600 instead of the original 115200 in the original library
 
#include <DS3231.h>

DS3231  rtc(SDA, SCL); 
// Initializing the Real Time Clock object using Serial Data and Serial Clock pins. rtc is the DS3231 class constructor

void rtc_setup(){
    rtc.begin(); // Iniliaze the Real Time Clock object
  
    // The following three lines are run only when the DS3231 module is calibrated the first time. 
    // Change the parameters to set to the first instance when we begin to record the time
    
    rtc.setDOW(TUESDAY);     
    rtc.setTime(5, 43, 0);     
    rtc.setDate(2, 4, 2019);   
}

void rtc_loop(){
  } 

void setup()
{
  Serial.begin(9600); // Setting up the serial communication with a bps of 9600
  rtc_setup();
}

void loop()
{
  // Serial.print() prints to the serial monitor
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
 
  Serial.println(rtc.getTimeStr());
  Serial.print("The temperature measured: ");
  Serial.println(rtc.getTemp());
  // The parameter of delay function is in milliseconds
  delay (1000); // A delay of 1 second
}
