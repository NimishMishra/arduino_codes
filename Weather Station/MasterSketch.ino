// Initializing the Real Time Clock object using Serial Data and Serial Clock pins. rtc is the DS3231 class constructor

#include <DS3231.h>
DS3231  rtc(SDA, SCL); 
void rtc_setup(){
    rtc.begin(); // Iniliaze the Real Time Clock object

    //You need to setup the clock date and time everytime the clock is inialized.
    rtc.setDOW(SUNDAY);     
    rtc.setTime(12, 35, 0);     
    rtc.setDate(21, 4, 2019);   
}



// The serial monitor print statements serve to test the arduino sketch before sending data 
// to thingspeak.com. I have commented out all print statements for better performance


// Libraries used:

// DS3231 (the ZS-042 RTC1 module as designed in Fritzing):
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/

// Arduino Mega:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL pin
//         bps used  -> 9600 instead of the original 115200 in the original library

// https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

 
#include <SoftwareSerial.h>
#include "dht.h"
#include <GUVA-S12SD.h>
#include "MQ135.h"
#define RX1 10
#define TX1 11

int signal_pin = A0; // or any other ANALOG input you wish to have
dht DHT; // object of the DHT class to enable analog inputs
GUVAS12SD uv(A12); // UV sensor to analog pin A12


// MQ 135 Library credits
//https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

#include "MQ135.h"
int ANALOGPIN = A2;
MQ135 gasSensor = MQ135(ANALOGPIN);


// MQ 7 library credits: https://github.com/swatish17/MQ7-Library
#include "MQ7.h"
MQ7 mq7(A4, 5);


String HOST = "api.thingspeak.com";
String PORT = "80";
String AP = "moto";
String PASS = "1234567890";

String API = "OHEZX5HA71A2O7Z4";
String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";
String field5 = "field5";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int temperatureValue;
int humidityValue;
int COconcentrationValue;
int airqualityValue;
int uvindexValue;
SoftwareSerial esp8266(RX1,TX1);


const int AOUTpin = 4;
const int DOUTpin = 8;
void mq_7_setup(){
     pinMode(DOUTpin, INPUT);
 }

void esp8266_setup(){
  esp8266.begin(115200);

  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
 }

 void mq_7_loop(){
  float ppm = mq7.getPPM();
  Serial.print("Carbon monoxide ");
  Serial.print(ppm);
  COconcentrationValue = ppm;
 }

 void mq135_loop(){
    float ppm = gasSensor.getPPM();
    Serial.print("Air quality ");
    Serial.print(ppm);
}


void rtc_loop(){

  // Serial.print() prints to the serial monitor
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
 
  Serial.print(rtc.getTimeStr());
  Serial.print(" Temp (RTC): ");
  Serial.print(rtc.getTemp());
  Serial.print(" -- ");

  } 

void dht_loop(){
    DHT.read11(signal_pin); // reads data from pin A0
    Serial.print("Temp(DHT11) = ");
    Serial.print(DHT.temperature);
    temperatureValue = DHT.temperature;
    Serial.print("  Humidity = ");
    humidityValue = DHT.humidity;
    Serial.print(DHT.humidity);
    Serial.print(" % --");
 }


//UV Conversion values: http://www.arduinoprojects.net/sensor-projects/arduino-uno-guva-s12sd-uv-sensor-example.php
 void uv_loop(){
    float mv = uv.read();
    float uv_index = uv.index(mv);
    Serial.print("UV Voltage observed: ");
    Serial.print(mv*0.001);
    Serial.print("V --corresponding UV Index: ");
    Serial.println(uv_index);
    uvindexValue = uv_index;
}


void air_quality_loop(){
    Serial.print("Air Quality: ");
    float ppm = gasSensor.getPPM();
    Serial.print(ppm);
    Serial.print(" PPM-- ");
    airqualityValue = ppm;
}
void sendCommand(String command, int maxTime, char readReplay[]) {

//Serial.print(countTrueCommand);
//Serial.print(". at command => ");
//Serial.print(command);
//Serial.print(" ");
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

//Serial.println("OYI");

countTrueCommand++;
countTimeCommand = 0;

}
if(found == false)
{

//Serial.println("Fail");
countTrueCommand = 0;
countTimeCommand = 0;

}
found = false;

}



void esp8266_loop(){
    String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(temperatureValue)+"&"+field2+"="+String(humidityValue);
    getData += "&" + field3 + "=" + String(COconcentrationValue);
    getData += "&" + field4 + "=" + String(airqualityValue);
    getData += "&" + field5 + "=" + String(uvindexValue);
    sendCommand("AT+CIPMUX=1",5,"OK");
    sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
    sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
    esp8266.println(getData);
    delay(1500);
    countTrueCommand++;
    sendCommand("AT+CIPCLOSE=0",5,"OK");
}

void dust_sensor_loop(){
    int dustPin = 50;
    float voltageMeasured = analogRead(dustPin);
    float calcVoltage = voltageMeasured * (5.0/1024.0);
    calcVoltage = (0.17 * calcVoltage) - 0.1;
    Serial.print("-- Dust value: ");
    Serial.print(calcVoltage);
}

void setup()
{
  Serial.begin(9600); // Setting up the serial communication with a bps of 9600
  delay(500);//Delay to let system boot
  rtc_setup();
  mq_7_setup();
  //esp8266_setup();
}

void loop()
{
  rtc_loop();
  dht_loop();
  uv_loop(); // takes 2 seconds to return data.
  air_quality_loop();
  mq_7_loop();
  //Serial.println();
  //esp8266_loop();
  delay (2000);
  dust_sensor_loop();
  Serial.println();
  
}