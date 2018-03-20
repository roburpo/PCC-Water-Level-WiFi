/* Water Level Sensor System
   Author: PCC ENGR114 Student Spring 2017
   Date: 06/01/2017
   Version: 2.0
   Description: This program reads the analog voltage signal from the eTape 
   and displays the measurements digitally in Arduino.
   Connect pin #2 of the sensor to ground, then pin #3 to a 560 ohm resistor. 
   The other side of the 560 ohm resistor to VCC 5V to create a resistor 
   divider. The ADC pin connects to the point between the resistor and sensor.
*/ 
// the value of the resistor (not the eTape)
#define SERIESRESISTOR 470    
 
// Sensor pin input
#define SENSORPIN A0 

// resistance values
#define zero_depth_resistance 550 // Resistance value when no liquid is present. 
#define max_resistance    60    // Resistance when liquid is at max line. 

#include <SoftwareSerial.h>
#define RX 2
#define TX 3
SoftwareSerial esp8266(RX,TX);

String AP="Samsung Galaxy S7 6335";
String PASS="A1234567";

String API = "R03IAJF3KWV0HC4M";
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

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
 
void setup(void) {
  Serial.begin(9600);
  esp8266.begin(9600);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}
 
void loop(void) {
  
  float reading;
 
  reading = analogRead(SENSORPIN); // voltage reading from eTape

 
  // convert the value to resistance
 
  reading = (1023 / reading)  - 1;
  reading = SERIESRESISTOR / reading;
  char buffer[8];
  String resistance = dtostrf(reading, 1, 4, buffer);
  //Serial.println(reading);


// connect and send data to ThingSpeak
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+resistance;

 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 
  delay(16000);

 

}
