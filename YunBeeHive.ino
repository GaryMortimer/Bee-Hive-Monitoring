// No solder Arduino Yun data feeder via ThingSpeak, can send data to HoneyPi
// Cobbled code from all over the web,made from stuff on my desk it's not pretty. 
// Also not the latest board, but it works.
// It communicates via the Yun's built in WiFi they cost Around $56 if you can find one.

// You can find the sensors and connecting wires at HobbyKing 
// Radio control servo extentions 
// https://hobbyking.com/en_us/futaba-60cm-26awg-servo-extension-lead-with-retaining-clip-5pcs.html
// Carefully cut away the clip end to reveal the pins Todays price $2.81

// DS18B20 
// https://hobbyking.com/en_us/keyes-digital-temperature-sensor-module-for-arduino.html

// Connected to 5V power and digital pwm pin 4 Todays price $1.89

// LM35 (That might not actually be it, but the code works)
// https://hobbyking.com/en_us/kingduino-analog-temperature-sensor-module.html

// Connected to 3.3V power and analog A0 Todays price $1.35


// Gary Mortimer December 2021, modify it all you like!


// Add the sensors

#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Start the WiFi
#include <BridgeClient.h>

#define DS18B20PIN 4
#define sensorPin A0 

BridgeClient client;

// Your ThingSpeak details
unsigned long myChannelNumber = 000000;
const char * myWriteAPIKey = "Yourkey";

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(DS18B20PIN);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  Bridge.begin();
  ThingSpeak.begin(client);
}

void loop() {
  sensors.requestTemperatures(); 
  // Get the voltage reading from the LM35
  int reading = analogRead(sensorPin);

  // Convert that reading into voltage
  float voltage = reading * (3.3 / 1024.0);

  // Convert the voltage into the temperature in Celsius
  float Hiveprobe = voltage * 19.21;

  // Print the temperature in Celsius

  Serial.print("Hiveprobe: ");
  Serial.print(Hiveprobe);
  Serial.print("\xC2\xB0"); // shows degree symbol
  Serial.print("C  |  ");

  // Inside temperature (DS18B20 sensor)
  float temperaturein = sensors.getTempCByIndex(0);
  
  Serial.print("Under Box ");
  Serial.print(temperaturein);
  Serial.println("ºC");

  // Send data to ThingSpeak
  
  ThingSpeak.setField(1,temperaturein);
  ThingSpeak.setField(2,Hiveprobe);
  
  ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
  
  // Send data very 12 minutes ie 5 times an hour
  delay(720000);
  
}
