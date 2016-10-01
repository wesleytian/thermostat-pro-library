// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Wire.h>
#include "SSD1306.h"
#include "font.h"
#include <RCSwitch.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "html_home.h"

#define BLYNK_PRINT Serial // Enables Serial Monitor
#include <BlynkSimpleESP8266.h> // This part is for Ethernet stuff



RCSwitch mySwitch = RCSwitch();


#define DHTPIN 14     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.


SSD1306  display(0x3c, SCL, SDA);

DHT dht(DHTPIN, DHTTYPE);

bool state = false;
int last = -180000;
int highHeatLimit = 75;
int lowHeatLimit =75;
int onSignal = 1381827;
int offSignal = 1381836;
int currentSignal = 0;

int lowHumid = 0;

char auth[] = "cc7e38a7f1024960aca086a910777bcb"; // Put your Auth Token here. (see Step 3 above)


const char* ssid = "freefly";
const char* password = "lrtsucks";


ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", html);
  // digitalWrite(led, 0);
}

void handleSet() {
  Serial.println("set command received");
  if(server.hasArg("high")) {
    highHeatLimit = server.arg("high").toInt();
    Serial.println("High set to: " + highHeatLimit);

  }
  if(server.hasArg("low")) {
    lowHeatLimit = server.arg("low").toInt();
    Serial.println("Low set to: " + lowHeatLimit);
  }
}

void handleBlog() {
  String sometext = R"(<head><META http-equiv="refresh" content="1"></head>)";
  sometext += "<h1>";
  sometext += millis();
  sometext += "</h1>";
  server.send(200, "text/html", sometext);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  // digitalWrite(led, 0);
}

void setup() {
  Serial.begin(115200);
#if 0
  Blynk.begin(auth, ssid, password);
   pinMode(led, OUTPUT);
  // digitalWrite(led, 0);
  
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/blog.html", handleBlog);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  Serial.println("DHTxx test!");

  // Transmitter is connected to Arduino Pin #16  
  mySwitch.enableTransmit(16);
  mySwitch.setPulseLength(184);
#endif

  dht.begin();
  display.init();
  display.flipScreenVertically();
  display.setFont(Dialog_plain_16);
  //digitalWrite(led, 1);

}

void loop() {
  //server.handleClient();


  // Wait a few seconds between measurements.
  Serial.println("delay begin");
  delay(2000);
  Serial.println("done");
  //display.clear();

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("nan!!!");
    //display.drawString(64,22,"Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0,0,String(h) + " %");
  Serial.println("Humidity: " + String(h) + " %");

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.drawString(0,24,String(f) + " °F");
  Serial.println("Temperature: " + String(t) + " °C\t" + String(f) + " °F");

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.drawString(0,48,String(hif) + " °F");
  Serial.println("Heat Index: " + String(hic) + " °C\t" + String(hif) + " °F");

  #if 0
    65-72 night nosebleed
    68-75
  #endif

  #if 0
  if ((state == false) && (millis() > last + 180000) && (hif > highHeatLimit)) {
    currentSignal = onSignal;
    mySwitch.send(currentSignal, 24);
    delay(2000);
    last = millis();
    state = true;
    Serial.println("on" + millis());
  }

  else if ((state == true) && (hif < lowHeatLimit || h < lowHumid)) {
    currentSignal = offSignal;
    mySwitch.send(currentSignal, 24);
    delay(2000);
    state = false;
    Serial.println("off" + millis());
  }

 if ((state == false) && (millis() > last + 180000) && (f > 77.5)) {
    currentSignal = onSignal;
    mySwitch.send(currentSignal, 24);
    delay(2000);
    last = millis();
    Serial.println("on" + millis());
  }

  else if ((state == true) && (f < 76.5)) {
    currentSignal = offSignal;
    mySwitch.send(currentSignal, 24);
    delay(2000);
    state = false;
    Serial.println("off" + millis());
  }


   // Blynk.run();
   // Blynk.virtualWrite(V0, h);
   // Blynk.virtualWrite(V1, f);
   // Blynk.virtualWrite(V2, hif);
   // Blynk.virtualWrite(V3, currentSignal);
  #endif
  //display.display();
}
