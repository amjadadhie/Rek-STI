#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <BH1750.h>
// Tambahkan library sensor sesuai kebutuhan

// Define Wifi
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// DEFINE PIN INPUT & VARIABLES PH
const int pH_pin = A3; // DIGANTI PH INPUT BERAPA PINNNYA

// DEFINE PIN INPUT & VARIABLES MOISTURE
const int moisture_pin = A1; // DIGANTI MOISTURE BERAPA PINNYA
const int AirValue = 2650;
const int WaterValue = 980;
int soilMoistureValue = 0;
int soilmoist = 0;

// DEFINE PIN INPUT & VARIABLES LIGHT
const int light_pin = A2; // DIGANTI DENGAN LIGHT BERAPA PINNYA
BH1750 LightMeter;
float light = 0.0;
float humidity = 0.0;

// the XML array size needs to be bigger that your maximum expected size. 2048 is way too big for this example
char XML[2048];

// just some buffer holder for char operations
char buf[32];

// variable for the IP reported when you connect to your homes intranet (during debug mode)
IPAddress Actual_IP;
// definitions of your desired intranet created by the ESP32
IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;
AsyncWebServer server(80);





void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Inisialisasi sensor

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Live Monitoring</title></head><body><h1>Sensor Data</h1><div><p>Light: " + String(light) + "</p><p>Humidity: " + String(humidity) + "</p><p>pH: " + String(ph) + "</p></div></body></html>";
    request->send(200, "text/html", html);
  });

  // these calls will handle data coming back from your web page
  // this one is a page request, upon ESP getting / string the web page will be sent
  server.on("/", SendWebsite);

  // upon esp getting /XML string, ESP will build and send the XML, this is how we refresh
  // just parts of the web page
  server.on("/xml", SendXML);

  server.begin();

  // BEGIN LightMeter
  LightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE_2);
  pinMode(pH_pin, INPUT);

}

void loop() {
  light = readLightSensor();
  humidity = readHumiditySensor();
  ph = readPHSensor();

  read_SoilMoist(); // LOOP SOILMOIST
  read_pHSensor();
  read_LightSensor();

  delay(100); // Update setiap 10 detik
}

// SOIL MOIST FUNCTION

const int AirValue = 2650;
const int WaterValue = 980;
int soilMoistureValue = 0;
int soilmoist = 0;

void read_SoilMoist() {
  soilMoistureValue = analogRead(A6); // pin berapa
  soilmoist= map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if(soilmoist >= 100)
    {
      soilmoist=100;
    }
  else if(soilmoist <=0)
    {
      soilmoist=0;
    }
  
  lcd.setCursor(0,0);
  lcd.print("Soil Humid =");
  lcd.setCursor(13,0);
  lcd.print(soilmoist);   
  lcd.print("% ");

  // print ke terminal / web
  Serial.print(soilmoist);

}

// SENSOR CAHAYA FUNCTION 
void read_LightSensor(){
  uint16_t lux = LightMeter.readLightLevel(1);
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.print(" lx");
}


// SENSOR PH FUNCTION
void read_pHSensor(){
  int nilai_analog_pH = analogRead(pH_pin);
  
  pH_Value = analogRead(pH_pin); // DIGANTI BERAPA INPUT
  Voltage = pH_Value * (5 / 1023.0);
  PH_step
  Serial.print("")
  Serial.print(Voltage);
}

