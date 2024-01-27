#include <ESP8266HTTPClient.h>
#include <DFRobot_PAJ7620U2.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>

const char* ssid = "Get-72F2E0";
const char* password = "gjmltncdmf";
const char* hostname = "GestureRecognizeEsp01";
const int udpPort = 1234;
const int httpPort = 5000;

String TextGesture = "";
WiFiUDP udp;

AsyncWebServer server(80);

DFRobot_PAJ7620U2 paj;

String hostnameserver = "";


void UdpPasrse() {
  while (true) {
    if (udp.parsePacket()) {
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      hostnameserver = udp.remoteIP().toString();
      Serial.println(udp.remoteIP());
      Serial.println(udp.remotePort());
      udp.print("ESP_OK");
      udp.endPacket();
      break;
    }
  }
}


void sendHTTPRequest() {
  WiFiClient client;
  HTTPClient http;

  String url = "http://" + hostnameserver + ":" + String(httpPort) + "/DataGesPAJ";
  Serial.println("Sending request to: " + url);

  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "gesture=" + TextGesture;
  Serial.println("POST data: " + postData);

  int httpCode = http.POST(postData);

  if (httpCode > 0) {
    Serial.printf("HTTP POST request successful, status code: %d\n", httpCode);
    String response = http.getString();
    Serial.println("Server response: " + response);
  } else {
    Serial.printf("HTTP POST request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}


void InitialWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.hostname(hostname);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void InitialServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", TextGesture);
    String esp_ip = request->client()->remoteIP().toString();
    Serial.println(esp_ip);
  });
  server.begin();
}

void InitialPaj() {
  Wire.begin(2, 0);

  delay(300);
  Serial.println("Gesture recognition system based on PAJ7620U2");
  while (paj.begin() != 0) {
    TextGesture = ("initial PAJ7620U2 failure! Please check if all the connections are fine, or if the wire sequence is correct?");
    delay(500);
  }
  TextGesture = ("PAJ7620U2 init completed, start to test the gesture recognition function");

  paj.setGestureHighRate(true);
}

void setup() {
  Serial.begin(115200);
  InitialWifi();
  InitialServer();
  InitialPaj();
  udp.begin(udpPort);
  UdpPasrse();
  TextGesture = "Hi";
  sendHTTPRequest();
}



void loop() {
  DFRobot_PAJ7620U2::eGesture_t gesture = paj.getGesture();
  if (gesture != paj.eGestureNone) {
    String description = paj.gestureDescription(gesture);
    TextGesture = paj.gestureDescription(gesture);
    sendHTTPRequest();
  }
}
