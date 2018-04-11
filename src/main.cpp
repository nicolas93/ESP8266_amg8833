#include <Arduino.h>
#include "Adafruit_AMG88xx.h"
#include <ESP8266WiFi.h>



const char* ssid = "";
const char* password = "";



Adafruit_AMG88xx amg;
WiFiServer server(80);


void setup() {

  Serial.begin(9600);
  Serial.println("!");
    // put your setup code here, to run once:
    int status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
  Serial.println(":)");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  String val;
  if (req.indexOf("/tmpcam") != -1) {
    val = "<!DOCTYPE HTML><head>\n<script src=https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js></script>\n</head><body><center><table><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr><tr><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td><td>00.00</td></tr></table><div id=a></div></center><script>function valtocol(e){max=45;if(e>max){return\"#ff0000\"}min=0;mid=max/2;var d,c,a=0;d=e<mid?255*(2*e/max):255;c=e>mid?255-(255*((2*e-max)/max)):255;sr=d>15?(Math.round(d)).toString(16):\"0\"+(Math.round(d)).toString(16);sg=c>15?(Math.round(c)).toString(16):\"0\"+(Math.round(c)).toString(16);sb=\"00\";return\"#\"+sr+sg+sb}var i=0;w=$(window).width();h=$(window).height();m=h>w?w:h;m=m/8;$(\"td\").css(\"height\",m+\"px\");$(\"td\").css(\"width\",m+\"px\");var array=[];$.get(\"test.json\",function(a){console.log(a.vals.length);array=a.vals},\"json\");window.setInterval(function(){i++;if(i>80){i=0}for(i=1;i<=8;i++){for(j=1;j<=8;j++){console.log(window.array[(i-1)*8+(j-1)]);$(\"table tr:nth-child(\"+i+\") td:nth-child(\"+j+\")\").css(\"background-color\",valtocol(array[(i-1)*8+(j-1)]));$(\"table tr:nth-child(\"+i+\") td:nth-child(\"+j+\")\").html(array[(i-1)*8+(j-1)]+\"\"+valtocol(array[(i-1)*8+(j-1)]))}}},2000);</script></body>";
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
    val = json;
  } else {
    client.stop();
    return;
  }


  client.flush();


  // Send the response to the client
  Serial.println("::" +val);
  client.print(val);
  delay(1);
  Serial.println("Client disonnected");




  // put your main code here, to run repeatedly:


}
