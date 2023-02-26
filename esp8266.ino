#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>  // Add OneWire library for DS18B20 temperature sensor
#include <DallasTemperature.h>  // Add DallasTemperature library for DS18B20 temperature sensor
#include <DHT.h> // Include the DHT library

#define DHTPIN D7 // Pin where the DHT11 humidity and temperature sensor is connected
#define DHTTYPE DHT11 // DHT type is DHT11
int moistureSensor = A0; 
int moisture;
const int waterPump = D3;
const char* ssid = "Livebox-F908";
const char* password = "bCsDN4xzG82NJRbJ9G";
const char* serverName = "http://iot.bluehost-testsite-nunyvega.blog/post-data.php";
const char* auth = "ysit2llfpoJBW55hcEZVTpDnrkDDhA_d";
const String apiKeyValue = "bCsDN4xzG82NJRbJ9G222";

OneWire oneWire(D5);  // Specify the pin of the DS18B20 temperature sensor
DallasTemperature sensors(&oneWire);  // Create a DallasTemperature object to interact with the DS18B20
float soilTemperature;

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;
bool relay = false;



void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("System Loading");

  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }

  lcd.clear();

  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  sensors.begin();
  dht.begin(); // Initialize DHT sensor
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
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }

  HTTPClient http;  
  WiFiClient wifiClient;
  http.begin(wifiClient, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  moisture = analogRead(moistureSensor);
  moisture = map(moisture, 0, 1024, 0, 100);
  moisture = (moisture - 100) * -1;

  float humidity = dht.readHumidity(); // Read humidity from DHT sensor
  float airTemperature = dht.readTemperature(); // Read temperature from DHT sensor

  // Read temperature from DS18B20
  sensors.requestTemperatures();
  soilTemperature = sensors.getTempCByIndex(0);
 
  String httpRequestData = 
    "api_key=" + apiKeyValue + 
    "&moisture=" + String(moisture) + 
    "&humidity=" + String(humidity) + 
    "&airTemperature=" + String(airTemperature) +
    "&soilTemperature=" + String(soilTemperature) + "";

  Blynk.virtualWrite(V0, moisture);
  lcd.setCursor(0, 0);
  lcd.print("M");
  lcd.print(moisture);
  lcd.print("H");
  lcd.print(humidity);

  lcd.setCursor(0, 1);
  lcd.print("AT");
  lcd.print(airTemperature);
  lcd.print("ST");
  lcd.print(soilTemperature);
  lcd.print(" ");

  Blynk.run();

  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);
  
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  delay(15000);
}
