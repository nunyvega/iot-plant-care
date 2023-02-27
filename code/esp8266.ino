/*
ESP8266 board code for the PLANT CARE INTEGRAL SYSTEM project.

This program reads data from various sensors including a moisture sensor, a DHT11 humidity and temperature sensor, and a DS18B20 temperature sensor.
The program controls a water pump based on the moisture level of the soil, and sends sensor data to a server.
The host name and the API key are defined in the program.
The current host name is "iot.bluehost-testsite-nunyvega.blog".
The program also displays sensor data on an LCD display and on a Blynk app.

Dependency libraries required:
- ESP8266WiFi.h - Library for connecting to a WiFi network
- ESP8266HTTPClient.h - Library for HTTP requests
- LiquidCrystal_I2C.h - Library for LCD display
- BlynkSimpleEsp8266.h - Library for Blynk app
- OneWire.h - Library for DS18B20 temperature sensor
- DallasTemperature.h - Library for DS18B20 temperature sensor
- DHT.h - Library for DHT humidity and temperature sensor
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define DHTPIN D7 // Pin where the DHT11 sensor is connected
#define DHTTYPE DHT11 // DHT type is DHT11

// Declare pin and variable names
const int moistureSensor = A0;
int moisture;
const int waterPump = D3;

// Declare WiFi credentials, server URL and API key
const char * ssid = "Livebox-F908";
const char * password = "bCsDN4xzG82NJRbJ9G";
const char * serverName = "http://iot.bluehost-testsite-nunyvega.blog/post-data.php";
const char * auth = "ysit2llfpoJBW55hcEZVTpDnrkDDhA_d";
const String apiKeyValue = "bCsDN4xzG82NJRbJ9G222";

// Create objects for the sensors
OneWire oneWire(D5);
DallasTemperature sensors( & oneWire);
float soilTemperature;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;
bool relay = false;
bool looped = false;
bool screen = true;

void setup() {
  // Start serial communication and set pin modes
  // Serial communication is started with a baud rate of 9600.
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);

  // Initialize LCD display
  // The LCD display is initialized and a loading animation is displayed.
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("System Loading");

  // Display loading animation
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }

  // Clear LCD display and start Blynk connection
  lcd.clear();
  // Blynk connection is established with the specified authentication token
  // WiFi credentials, server URL and port
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);

  // Sensors are initialized
  sensors.begin();
  dht.begin();

  // Connect to WiFi network
  Serial.print("Connecting to WiFi network ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

// Function to control the water pump
BLYNK_WRITE(V1) {
  relay = param.asInt();
  if (relay) {
    digitalWrite(waterPump, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON ");
  } else {
    digitalWrite(waterPump, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
  }
}

// Function to control the screen backlight in case it bothers
BLYNK_WRITE(V5) {
  screen = param.asInt();
  if (screen) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
}

void loop() {
  // Check if WiFi is still connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  // Make HTTP POST request to server
  HTTPClient http;
  WiFiClient wifiClient;
  http.begin(wifiClient, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Read moisture, temperature and humidity sensor data
  moisture = analogRead(moistureSensor);
  moisture = map(moisture, 0, 1024, 0, 100);
  moisture = (moisture - 100) * -1;

  float humidity = dht.readHumidity();
  float airTemperature = dht.readTemperature();

  sensors.requestTemperatures();
  soilTemperature = sensors.getTempCByIndex(0);

  // Construct HTTP POST request data
  String httpRequestData =
    "api_key=" + apiKeyValue +
    "&moisture=" + String(moisture) +
    "&humidity=" + String(humidity) +
    "&airTemperature=" + String(airTemperature) +
    "&soilTemperature=" + String(soilTemperature) + "";

  // Write moisture data to Blynk app
  Blynk.virtualWrite(V0, moisture);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, airTemperature);
  Blynk.virtualWrite(V4, soilTemperature);

  // Write temperature data to LCD display
  //loop between moisture and humidity or temperature values
  if (looped) {
    lcd.setCursor(0, 0);
    lcd.print("Moisture: ");
    lcd.print(moisture);
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    looped = false;
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Air Temp: ");
    lcd.print(airTemperature);
    lcd.setCursor(0, 1);
    lcd.print("Soil Temp: ");
    lcd.print(soilTemperature);
    looped = true;
  }
  // Run Blynk and print HTTP POST request data to serial monitor
  Blynk.run();
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  // Send HTTP POST request to server and check for response
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // End HTTP connection and delay for 5 seconds
  http.end();
  delay(5000);
}