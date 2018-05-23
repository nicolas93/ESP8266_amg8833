#include <Arduino.h>
#include "Adafruit_AMG88xx.h"
#include <ESP8266WiFi.h>
#include <creds.h> // define SSID and PASS in seperate file
#include "FS.h"



const char* ssid = SSID;
const char* password = PASS;



Adafruit_AMG88xx amg;
WiFiServer server(80);

void sendFile(WiFiClient c, String path) {
  if (SPIFFS.exists(path)) {
    File f = SPIFFS.open(path, "r");
    while (f.available()) {
      Serial.println("[http] Sending " + path);
      Serial.println("[http] bytes:" + String((int)f.size()));
      int max = f.size() / 1027;
      int leftover = f.size() % 1027;
      char buff[1028];
      Serial.print("[http]");
      for (int i = 0; i < max; i++) {
        Serial.print(".");
        f.readBytes(buff, 1027);
        buff[1027] = 0;
        c.print(buff);
      }
      Serial.println();
      Serial.println("[http] done.");
      f.readBytes(buff, leftover);
      buff[leftover] = 0;
      c.print(buff);
    }
    f.close();
  } else {
    c.print("404");
  }
}


void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println();
  Serial.println("[AMG8833] Set up temp sensor..");
  int status = amg.begin();
  if (!status) {
      Serial.println("[AMG8833] Could not find a valid AMG88xx sensor, check wiring!");
      while (1);
  }
  Serial.println("[AMG8833] Connected.");

  Serial.println("[WiFi] Set up connection..");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("[WiFi] connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("[WiFi] connected");

  // Start the server
  server.begin();
  Serial.println("[Server] Server started");

  // Print the IP address
  Serial.println("[Server] IP:" + WiFi.localIP().toString());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("[Server] new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println( "[Server] " +req);
  client.flush();

  // Match the request
  String val;
  if (req.indexOf("/tmpcam") != -1) {
    sendFile(client, "/index.html");
  } else if (req.indexOf("jquery.min.js") != -1) {
    sendFile(client, "jquery.min.js");
  } else if (req.indexOf("/test.json") != -1) {
    float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
    amg.readPixels(pixels);
    int i = 0;
    char tmp0 [10]= "";
    dtostrf(pixels[0],4,4,tmp0);
    String json = "{\n\"vals\":[";
    json += tmp0;
    for(i=1; i <64; i++){
      char tmp [10] = "";
      dtostrf(pixels[i],4,4,tmp);
      json += ", " + String(tmp);
    }
    json += "]\n}";
    client.print(json);
    //client.print("{\"vals\":[10,10,10,10,10,10,10,10,10,0,30,40,50,60,70,10,10,20,30,40,50,60,70,10,10,20,30,40,50,60,70,10,10,20,30,40,50,60,70,10,10,20,30,40,50,60,70,10,10,20,30,40,50,60,80,10,10,10,10,10,10,10,10,10]}");
  } else {
    client.stop();
    return;
  }


  client.flush();



  delay(1);
  Serial.println("[Server] Client disonnected");




  // put your main code here, to run repeatedly:


}
