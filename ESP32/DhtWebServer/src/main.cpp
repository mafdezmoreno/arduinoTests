/*********
Based on:
 https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-web-server-arduino-ide/
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "wifi_credentials.h"
// Uncomment and replace with your network credentials inside "wifi_credentials.h"
//const char* ssid = "ssid";
//const char* password = "password";
#include "email.h"
#include "index_html.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define DHTPIN      2       // Digital pin connected to the DHT sensor
#define DHTTYPE     DHT11   // DHT11 or DHT22 or DHT21
#define DHTPOWER    15      // Digital pin to give power to DHT
#define PIN_BAT     34      // Pin to monitor battery voltage level
//#define DEBUG_MODE
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void debPrint(const String& msg){
#ifdef DEBUG_MODE
    Serial.println(msg);
#endif
}

String readDHTTemperature() {

    digitalWrite(DHTPOWER, HIGH);
    delay(2000);
    float t = dht.readTemperature();
    digitalWrite(DHTPOWER, LOW);

    if (isnan(t)) {
        debPrint("Failed to read from DHT sensor!");
        return "--";
    }
    else {
        debPrint(String(t));
        return String(t);
    }
}

String readDHTHumidity() {

    digitalWrite(DHTPOWER, HIGH);
    delay(2000);
    float h = dht.readHumidity();
    digitalWrite(DHTPOWER, LOW);

    if (isnan(h)) {
        debPrint("Failed to read from DHT sensor!");
        return "--";
    }
    else {
        debPrint(String(h));
        return String(h);
    }
}

String getTimeString() {
    //timeClient.update(); // produces timeout here
    String hour = "";
    String min = "";
    unsigned h = timeClient.getHours();
    unsigned m = timeClient.getMinutes();
    hour = String(h);
    if (h<10){
        hour = "0"+ String(h);
    }
    min = String(m);
    if (m < 10){
        min = "0" + min;
    }
    String time = hour + ":" + min;
    debPrint(time);
    return time;
}

// Replaces placeholder with DHT values
String processor(const String& var){

    if(var == "TEMPERATURE"){
        return readDHTTemperature();
    }
    else if(var == "HUMIDITY"){
        return readDHTHumidity();
    }
    return "";
}

// Returns battery level in Volt
String readBatLevel(){

    float batteryLevel = 0;
    unsigned meas = map(analogRead(PIN_BAT), 0, 4095, 0, 450);
    String batLevel = String(meas/100.);
    debPrint("Bat: ");
    debPrint(batLevel);
    return batLevel;
}


void setup(){
#ifdef DEBUG_MODE
    Serial.begin(115200);
#endif
    dht.begin();

    // DHT power
    pinMode(DHTPOWER, OUTPUT);
    digitalWrite(DHTPOWER, LOW);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        debPrint("Connecting to WiFi..");
    }

    // To get time from internet
    timeClient.begin();

    // Print ESP32 Local IP Address
    String IP = (WiFi.localIP()).toString();
    debPrint(IP);

    // Sending IP by email
    sendEmail(IP);
    delay(200);
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        debPrint("Sending temperature");
        request->send_P(200, "text/plain", readDHTTemperature().c_str());
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        debPrint("Sending humidity");
        request->send_P(200, "text/plain", readDHTHumidity().c_str());
    });
    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
        debPrint("Sending time");
        request->send_P(200, "text/plain", getTimeString().c_str());
    });
    server.on("/bat", HTTP_GET, [](AsyncWebServerRequest *request){
        debPrint("Sending bat");
        request->send_P(200, "text/plain", readBatLevel().c_str());
    });
    // Start server
    server.begin();
}

void loop() {
    timeClient.update();
    delay(5000);
}
