#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

const char *ssid =      "chip&dale";      
const char *pass =      "BwAx45$%";
WiFiServer server(80);
IPAddress ip(192,168,1,128);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup() {
  pinMode(A0,INPUT);
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, using IP address: ");
  Serial.println(WiFi.localIP()); 
  server.begin();

bool status;
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
}

void loop() {
  WiFiClient client = server.available();
   if (!client) {
    return;
  }
   while(!client.available()){
    delay(1);
  }
  delay(1500);
  float h = bme.readHumidity();
  float t = bme.readTemperature();
  float p = (bme.readPressure() / 100.0F) * 0.75006375541921;
  int s = analogRead(A0);
  String req = client.readStringUntil('\r');
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head></head><body>");
  client.print(t);
  client.print(":");
  client.print(h);
  client.print(":");
  client.print(s);
  client.print(":");
  client.print(p);
  client.println("</body></html>");
  delay(1);
  client.stop();  
}
