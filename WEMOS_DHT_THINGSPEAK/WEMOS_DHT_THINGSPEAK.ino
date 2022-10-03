#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN D8
#define DHTTYPE DHT11 
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "CZDZP798N2EONW7H";
const char* ssid = "RAINET"; //nama wifi
const char* password = "c1pul1r18"; //password wifi
const char* server = "api.thingspeak.com"; //biarkan isinya ga usah dirubah apithingspeak



DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
void setup() 
{
 Serial.begin(115200);
 dht.begin();
 WiFi.begin(ssid, password);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{
 delay(500);
 Serial.print(".");
}
 Serial.println("");
 Serial.println("WiFi connected");
}
void loop() 
{
 //byte h = dht.readHumidity();
 byte t = dht.readTemperature();
 byte f = dht.readTemperature(true);
 /*if (isnan(h) || isnan(t)) 
 {
 Serial.println("Failed to read from DHT sensor!");
 return;
 }*/
 
 if (client.connect(server,80)) {
 String postStr = apiKey;
 postStr +="&field1=";
 postStr += String(t);
 postStr += "\r\n\r\n\r\n";
 
 client.print("POST /update HTTP/1.1\n");
 client.print("Host: api.thingspeak.com\n");
 client.print("Connection: close\n");
 client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
 client.print("Content-Type: application/x-www-form-urlencoded\n");
 client.print("Content-Length: ");
 client.print(postStr.length());
 client.print("\n\n\n");
 client.print(postStr);
 
 Serial.print("Temperature: ");
 Serial.print(t);
 Serial.println(" Sending data to Thingspeak"); 
 }
 client.stop();
 Serial.println("Waiting 20 secs");
 // thingspeak needs at least a 15 sec delay between updates
 // 20 seconds to be safe
 
 delay(20000);
}
