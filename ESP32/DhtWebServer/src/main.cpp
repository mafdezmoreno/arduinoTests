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

#define DHTPIN 2        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT11 or DHT22 or DHT21
#define DHTPOWER 15     // Digital pin to give power to DHT

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {

    digitalWrite(DHTPOWER, HIGH);
    delay(2000);
    float t = dht.readTemperature();
    digitalWrite(DHTPOWER, LOW);

    if (isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return "--";
    }
    else {
        Serial.println(t);
        return String(t);
    }
}

String readDHTHumidity() {

    digitalWrite(DHTPOWER, HIGH);
    delay(2000);
    float h = dht.readHumidity();
    digitalWrite(DHTPOWER, LOW);

    if (isnan(h)) {
        Serial.println("Failed to read from DHT sensor!");
        return "--";
    }
    else {
        Serial.println(h);
        return String(h);
    }
}

String getTimeString() {

    String h = String(timeClient.getHours());
    String m = String(timeClient.getMinutes());

    return h + ":"  + m;
}

// Replaces placeholder with DHT values
String processor(const String& var){
    //Serial.println(var);
    if(var == "TEMPERATURE"){
        return readDHTTemperature();
    }
    else if(var == "HUMIDITY"){
        return readDHTHumidity();
    }
    return String();
}

void setup(){
    // Serial port for debugging purposes
    Serial.begin(115200);

    dht.begin();

    // DHT power
    pinMode(DHTPOWER, OUTPUT);
    digitalWrite(DHTPOWER, LOW);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    // To get time from internet
    timeClient.begin();

    // Print ESP32 Local IP Address
    String IP = (WiFi.localIP()).toString();
    Serial.println(IP);

    // Sending IP by email
    sendEmail(IP);

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", readDHTTemperature().c_str());
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", readDHTHumidity().c_str());
    });
    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getTimeString().c_str());
    });
    // Start server
    server.begin();
}

void loop() {
    timeClient.update();

    //Serial.print(timeClient.getHours());
    //Serial.print(timeClient.getMinutes());
    Serial.println(getTimeString());
    delay(5000);
}