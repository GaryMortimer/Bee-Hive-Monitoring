//  Adapted from the ThingSpeak - WriteMultipleFields example
//  With Adding support for DS18B20 on two pins Rob Tillaart 
//  URL: http://forum.arduino.cc/index.php?topic=216835.msg1764333#msg1764333  
//  Requires
//  An ESP 32 with two DS18B20 
//  https://hobbyking.com/en_us/keyes-digital-temperature-sensor-module-for-arduino.html
//  Free ThingSpeak account

#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_1 2
#define ONE_WIRE_BUS_2 4

OneWire oneWire_in(ONE_WIRE_BUS_1);
OneWire oneWire_out(ONE_WIRE_BUS_2);

DallasTemperature sensor_inhouse(&oneWire_in);
DallasTemperature sensor_outhouse(&oneWire_out);

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
String myStatus = "";

void setup() {
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  sensor_inhouse.begin();
  sensor_outhouse.begin();
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

    Serial.print("Requesting temperatures...");
    sensor_inhouse.requestTemperatures();
    sensor_outhouse.requestTemperatures();
    Serial.println(" done");

    Serial.print("Inhouse: ");
    Serial.println(sensor_inhouse.getTempCByIndex(0));

    Serial.print("Outhouse: ");
    Serial.println(sensor_outhouse.getTempCByIndex(0));
  
  ThingSpeak.setField(1, sensor_inhouse.getTempCByIndex(0));
  ThingSpeak.setField(2, sensor_outhouse.getTempCByIndex(0));
  
  // write to the ThingSpeak channel
int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
if(x == 200){
Serial.println("Channel update successful.");
}
else{
Serial.println("Problem updating channel. HTTP error code " + String(x));
}
  
  delay(20000); // Wait 20 seconds to update the channel again
}
