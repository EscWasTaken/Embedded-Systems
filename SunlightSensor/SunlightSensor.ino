#include "arduino_secrets.h" // Header file for ssid and password, aswell as api key
#include <WiFiNINA.h> // https://www.arduino.cc/reference/en/libraries/wifinina/
#include <Arduino.h>
#include <hp_BH1750.h>  //  https://github.com/Starmbi/hp_BH1750
 
//Please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;                // your network SSID (name)
char pass[] = SECRET_PASS;                // your network password (use for WPA, or use as key for WEP)
hp_BH1750 BH1750;   
WiFiSSLClient client; // NOTE: Different to just WiFiClient beause HTTPS acutally works!
String eventName = "sun_event";
 
char HOST_NAME[] = "hook.us1.make.com";
String PATH_NAME   = "/" + String(SECRET_KEY);
 
//lux of 400 for testing simulated with phone tourch, Real sunlight bean over 20,000 lux
const float sunlightLux = 400;
bool sunlight = false;
 
// 1 sec poll interval for soft testing, the sun dosn't move that fast so we could poll it evey minute to avoid the status changing too quickly
const int pollInterval = 1000;
 
void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
 
  Serial.begin(9600);
  while (!Serial);
 
  bool avail = BH1750.begin(BH1750_TO_GROUND);// init the sensor with address pin connetcted to ground
                                              // result (bool) wil be be "false" if no sensor found
  if (!avail) {
    Serial.println("No BH1750 sensor found!");
    while (true) {};                                        
  }
}
 
void loop() {
  getLUX();
  delay(pollInterval);
}
 
void serverSend(String sun, float lux) {
  String queryString = "?value1=" + sun + "&value2=" + lux;

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 443)) {
    // if connected:
    Serial.println("Connected to server");

    // make a HTTP request:
    // send HTTP header
    client.println("POST " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    int statusCode = getStatusCode(client);
    if (statusCode == 301) {
      // Handle redirect
      String newLocation = client.readStringUntil('\n');
      newLocation.trim();
      newLocation.remove(0, 9); // Remove "Location: "
      Serial.println("Redirecting to: " + newLocation);
      client.stop();
    } else {
      // Read the response from the server
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          Serial.print(c);
        }
      }
      Serial.println();
      Serial.println("disconnected");
    }
  } else {
    Serial.println("connection failed");
  }

  // the server's disconnected, stop the client:
  client.stop();
}
 
void getLUX(){
  BH1750.start();   //starts a measurement
  float lux=BH1750.getLux();  //  waits until a conversion finished
  Serial.println(String(lux));        
  lightCheck(lux);
}
 
void lightCheck(float lux){
  bool sunlightCurrentStatus = sunlight;
  String sun = "off";
  if (lux >= sunlightLux) {
    sunlight = true;
    sun = "on";
  }
  else {
    sunlight = false;
    sun = "off";
  }
  if (sunlightCurrentStatus != sunlight) {
    Serial.println("Sun Changed to " + sun);
    serverSend(sun, lux);
  }
}

int getStatusCode(WiFiClient& client) {
  int statusCode = 0;
  while (!client.available()) {
    // Wait for data
  }
  String line = client.readStringUntil('\r');
  statusCode = line.toInt();
  return statusCode;
}
