#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial NodeMCU(D2, D1);
float preasure;
const char* ssid     = "HMIF STT YBSI";
const char* password = "SarasaSadataSakata";

const char* host = "sim.pdamciamis.co.id";

WiFiClient client;
const int httpPort = 80;
String url;
long duration, distance;
unsigned long timeout;
  
void setup() {
  Serial.begin(9600);
  delay(10);
  
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  NodeMCU.begin(4800);
  pinMode(D2, INPUT);
  pinMode(D1, OUTPUT);
}

void loop() {
  
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //return;
  }
  
 while (NodeMCU.available() > 0) {
    float val = NodeMCU.parseFloat();
    if (NodeMCU.read() == '\n') {
    Serial.print("Preasure ");
    Serial.println(val);
    preasure = val;
    }
 }  
    // We now create a URI for the request
  url = "/iottirga/press/index.php?data=";
  url += preasure;
 
  Serial.print("Requesting URL: ");
  Serial.println(url);

// This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

// Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  Serial.println();
  delay(60000);
  
}
