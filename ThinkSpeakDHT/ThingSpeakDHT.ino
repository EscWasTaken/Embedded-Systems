#include "arduino_secrets.h" // Header for you WiFi SSID, WiFi Password, ThingSpeak Channel ID, and API Key
#include "ThingSpeak.h" // https://github.com/mathworks/thingspeak-arduino
#include <DHT.h> // https://github.com/adafruit/DHT-sensor-library + https://github.com/adafruit/Adafruit_Sensor
#include <DHT_U.h> // https://github.com/adafruit/DHT-sensor-library + https://github.com/adafruit/Adafruit_Sensor
#include <WiFiNINA.h> //https://www.arduino.cc/reference/en/libraries/wifinina/

#define DHTPIN 3 
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
 
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
 
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_API_KEY;
String myStatus = "";
 
void setup() {

  Serial.begin(115200);      // Initialize serial 

  while (!Serial) {

    ; // wait for serial port to connect. Needed for Leonardo native USB port only

  }
  ThingSpeak.begin(client);
  dht.begin();  // Initialize ThingSpeak 
}
 
void loop() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    
    while(WiFi.status() != WL_CONNECTED){ // May need several attempts

      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 

    Serial.println("\nConnected.");
  }
 
  delay(10000); // Any shorter and ThingSpeak throws HTTP errors

  // Read humidity percent
  float Humidity = dht.readHumidity();
  Serial.println(Humidity);
  // Read temperature as Celsius (the default)
  float Temperature = dht.readTemperature();
  Serial.println(Temperature);
  
  // set the fields with the values
  ThingSpeak.setField(1, Temperature);
  ThingSpeak.setField(2, Humidity);

  // set the status
  ThingSpeak.setStatus(myStatus);

  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(x == 200){
    Serial.println("Channel update successful.");
  }

  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(5000); // Any shorter and ThingSpeak throws HTTP errors
}
