#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>


#include <Settings.h>

const char* ssid = SSID;
const char* password = PASSWORD;
const char* mqtt_server = MQTT_SERVER;
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;
const char* ca_cert = CA_CERT

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637.h>
#include <Adafruit_NeoPixel.h>

#define PIN            3 //digitaler PWM Pin 3 am Arduino Nano
#define NUMPIXELS      1 //Anzahl der angeschlossenen RGB LEDs
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// TM1637 Display Driver connection GPIO
#define CLK 15
#define DIO 16

TM1637 tm(CLK, DIO);

// GPIO where the DS18B20 digital temperature sensor is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection... ");
    //Serial.print("Wifi-Status = ");
    //Serial.print(WiFi.status());
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.print(clientId.c_str());
      Serial.println(" connected");
    } else {
      Serial.print(clientId.c_str());
      Serial.print(" not connected, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // initialize NeoPixel library for rgb led
  pixels.begin(); 
  pixels.setPixelColor(0, pixels.Color(0,255,0));  //green,RED,blue
  pixels.show();
  // start 7-seg-Display
  tm.begin();
  tm.setBrightness(1);
  tm.display("8888");
  // Start the Serial Monitor
  Serial.begin(9600);
  setup_wifi();
  espClient.setCACert(ca_cert);
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
  
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  if (!client.connected()) {
    pixels.setPixelColor(0, pixels.Color(0,0,255));  //green,red,BLUE
    pixels.show();
    reconnect();
  }
  pixels.setPixelColor(0, pixels.Color(255,0,0));  //GREEN,red,blue
  pixels.show();
  //client.loop();  // checking for mqtt messages (there should not come something)
  pixels.setPixelColor(0, pixels.Color(0,0,0));  //green,red,blue 
  pixels.show();

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  tm.display(temperatureC);
  Serial.print(temperatureC);
  Serial.println("ºC");
  snprintf (msg, MSG_BUFFER_SIZE, "%.2f", temperatureC);  // i.e. 19.91
  //snprintf (msg, MSG_BUFFER_SIZE, "%+1.7E", temperatureC);  // i.e. +1.9062500E+01
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("temp/temp0", msg);
  delay(5000);
}
