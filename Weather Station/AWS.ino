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

// Credits to the DHT11 library
// 0.1.0 by Rob Tillaart (01/04/2011)
// inspired by DHT11 library

#include "dht.h"

// MQ 135 Library credits
//https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

#include "MQ135.h"
int ANALOGPIN = A2;
MQ135 gasSensor = MQ135(ANALOGPIN);

//UV Conversion values: http://www.arduinoprojects.net/sensor-projects/arduino-uno-guva-s12sd-uv-sensor-example.php

// MQ 7 library credits: https://github.com/swatish17/MQ7-Library
#include "MQ7.h"
MQ7 mq7(A4, 5);

// ESP8266 module arrangement credits: https://medium.com/@cgrant/using-the-esp8266-wifi-module-with-arduino-uno-publishing-to-thingspeak-99fc77122e82

#include <SoftwareSerial.h>

#define RX 10
#define TX 11

String AP = "moto";       
String PASS = "1234567890"; 

String API = "OHEZX5HA71A2O7Z4";  
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";
String field5 = "field5";

float temp, humi;
float CO_conc, air_quality, uv_index;

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

SoftwareSerial esp8266(RX,TX); 


/////////////////////////////////////////////////////////////////////////
int signal_pin = A0; // or any other ANALOG input you wish to have for DHT11 sensor
dht DHT; // object of the DHT class to enable analog inputs

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
    // Serial.print() prints to the serial monitor
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
 
  Serial.println(rtc.getTimeStr());
  Serial.print("The temperature measured: ");
  Serial.println(rtc.getTemp());

  } 

void dht_loop(){
    DHT.read11(signal_pin); // reads data from pin A0
    temp = DHT.temperature;
    humi = DHT.humidity;
}

void mq135_loop(){
    float ppm = gasSensor.getPPM();
    air_quality = ppm;
}

void guva_loop(){
    float mv = analogRead(A1);
    float uv = (mv/1024)*5;
    uv_index =  uv; 
}


void mq7_loop(){
  float ppm = mq7.getPPM();
  CO_conc = ppm;
  
}
void setup()
{
  Serial.begin(9600); // Setting up the serial communication with a bps of 9600
  rtc_setup();
  esp8266.begin(9600);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}


void esp8266_loop(){
  String getData = "GET/update?api_key="+ API +"&"+ field1 +"="+String(temp);
  getData = getData + "&" + field2 + "=" + String(humi);
  getData = getData + "&" + field3 + "=" + String(CO_conc);
  getData = getData + "&" + field4 + "=" + String(air_quality);
  getData = getData + "&" + field5 + "=" + String(uv_index);

  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");  
  esp8266_loop();
}

void loop()
{
  //rtc_loop();
  dht_loop();
  mq135_loop();
  guva_loop();
  mq7_loop();
  esp8266_loop();
  delay(2000);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
