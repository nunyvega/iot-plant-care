#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>  // Library for DS18B20 temperature sensor
#include <DallasTemperature.h>  // Library for DS18B20 temperature sensor
#include <DHT.h> // Library for DHT humidity and temperature sensor

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

// Blynk virtual write function for controlling the water pump
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

  // End HTTP connection and delay for 15 seconds
  http.end();
  delay(15000);
}