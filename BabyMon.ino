#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
const char *ssid =      "wlan_144";      
const char *pass =      "qscvhi@#$%qsc";
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
  dht.begin();
}

void loop() {
  WiFiClient client = server.available();
   if (!client) {
    return;
  }
   while(!client.available()){
    delay(1);
  }
  delay(3000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
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
  client.println("</body></html>");
  delay(1);
  client.stop();  
}
