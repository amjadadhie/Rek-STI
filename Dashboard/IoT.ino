#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include "DFRobot_ESP_PH.h"
#include "EEPROM.h"

// Pin Input ESP32
#define pinPH 34
#define pinUV 27
#define pinMois 35

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
int push_button1 = 5;
int push_button2 = 18;
int push_button3 = 19;
int mode = 0;
int flag = 0;
int switch1;
int switch2;
int switch3;
int sensor;

// PH
DFRobot_ESP_PH ph;
HTTPClient http;
#define ESPADC 4096.0   // ESP ADC value
#define ESPVOLTAGE 3300 // ESP voltage supply value
float voltage, phValue, temperature = 25;

// Variabel Pengukuran pH
// float Po = 0.0;
// float PH_step;
// float nilai_analog_PH;
// float TeganganPh;
float PH4 = 13.59; // Tegangan Output ketika pH = 7
float PH7 = 15.90; // Tegangan Output ketika pH = 4

// Network
const char* ssid = "hahaha";
const char* password = "hahahahaha";
// const char* mqtt_server = "192.168.18.3";
const int mqtt_port = 1883;
const char* topic_pH = "pHgeming";
const char* topic_moist = "moistgeming";
const char* topic_uv = "uvgeming";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
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

// void reconnect() {
//     while (!client.connected()) {
//         Serial.print("Attempting MQTT connection...");
//         String clientId = "ESP32Client-";
//         clientId += String(random(0xffff), HEX);
//         if (client.connect(clientId.c_str())) {
//             Serial.println("connected");
//         } else {
//             Serial.print("failed, rc=");
//             Serial.print(client.state());
//             Serial.println(" try again in 5 seconds");
//             delay(5000);
//         }
//     }
// }

void updateLCD() {
    if (mode == 0) {
        lcd.setCursor(0, 0);
        lcd.print("01 Pengukuran  ");
        lcd.print("               ");
        lcd.setCursor(0, 1);
        lcd.print("10 Kalibrasi");
    } else if (mode == 1) {
        // Soil Moisture
        float valueMoiss = analogRead(pinMois);
        //valueMois = valueMois * 1000.00;
        float moisturePercentagee = -1.00 * (valueMoiss - 2650.00) * 100.00 / 1600.00;
        Serial.print("Moisture : ");
        Serial.println(moisturePercentagee);

        // UV
        float uvSensorValuee = analogRead(pinUV);
        float uvIndexx = uvSensorValuee / 4.5;
        Serial.print("UV Index : ");
        Serial.println(uvIndexx, 3);

        // pH
        float nilai_analog_PHH = analogRead(pinPH);
        float TeganganPhh = nilai_analog_PHH * 5.000 / 1023.000;
        float PH_stepp = (PH4 - PH7) / 3;
        float Poo = 7.00 + ((TeganganPhh - PH7) / (2 * PH_stepp));
        Serial.print("pH Value: ");
        Serial.println(Poo, 3); // value pH

        // Membuat string data yang akan dikirim
        char dataString[150];
        snprintf(dataString, sizeof(dataString), "Moisture: %d, UV Index: %.2f, pH: %.2f", moisturePercentagee, uvIndexx, Poo);

        //client.publish(topic, dataString);

        lcd.setCursor(0, 0);
        lcd.print("pH=");
        lcd.print(Poo);
        lcd.print("UV=");
        lcd.print(uvIndexx);
        lcd.setCursor(0, 1);
        lcd.print("Moist=");
        lcd.print(moisturePercentagee);
        delay(3000);
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("                ");

    } else if (mode == 2) {
        lcd.setCursor(0, 0);
        lcd.print("Kalibrasi");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Pilih sensor");
        lcd.setCursor(0, 1);
        lcd.print("yang ingin");
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("dikalibrasi");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("01 Sensor pH");
        lcd.setCursor(0, 1);
        lcd.print("10 Sensor UV");
        lcd.setCursor(0, 0);
        lcd.print("11 Sensor Hygro");
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    // client.setServer(mqtt_server, 1883);
    EEPROM.begin(32);
    ph.begin();

    Wire.begin();
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the backlight (if available)

    pinMode(push_button1, INPUT);
    pinMode(push_button2, INPUT);
    pinMode(push_button3, INPUT);

    lcd.setCursor(0, 0);
    lcd.print("Selamat datang!");

    updateLCD();
    delay(5000);
}

void loop() {
    // if (!client.connected()) {
    //     reconnect();
    // }
    client.loop();

    float valueMois = analogRead(pinMois);
    //valueMois = valueMois * 1000.00;
    float moisturePercentage = -1.00 * (valueMois - 2650.00) * 100.00 / 1600.00;

    // UV
    float uvSensorValue = analogRead(pinUV);
    
    float uvIndex = random(0, 101) / 4.5;
    // pH
    float nilai_analog_PH = analogRead(pinPH);
    float TeganganPh = nilai_analog_PH * 5.000 / 1023.000;
    float PH_step = (PH4 - PH7) / 3;
    float Po = 7.00 + ((TeganganPh - PH7) / (2 * PH_step));

    int value = 0;
    for (int i = 0; i <= 10; i++) {
        client.publish(topic_pH, String(Po).c_str());
        client.publish(topic_moist, String(moisturePercentage).c_str());
        client.publish(topic_uv, String(uvIndex).c_str());
    }

    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 0);

    switch1 = digitalRead(push_button1);
    switch2 = digitalRead(push_button2);
    switch3 = digitalRead(push_button3);
    flag = 0;

    if (switch1 == 0 && switch2 == 0 && switch3 == 1) {
        mode = 0;
        updateLCD();
    } else if (switch1 == 0 && switch2 == 1 && switch3 == 1) {
        while (flag == 0) {
            mode = 1;
            updateLCD();

            switch1 = digitalRead(push_button1);
            switch2 = digitalRead(push_button2);
            switch3 = digitalRead(push_button3);
            if (switch1 == 0 && switch2 == 0 && switch3 == 1) {
                flag = 1;
            }
        }
    } else if (switch1 == 1 && switch2 == 0 && switch3 == 1) {
        mode = 2;
        updateLCD();
        delay(2000);
        while (flag == 0) {

            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 1);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Menunggu...");

            switch1 = digitalRead(push_button1);
            switch2 = digitalRead(push_button2);
            switch3 = digitalRead(push_button3);

            if (switch1 == 0 && switch2 == 1 && switch3 == 1) {
                sensor = 1;
                lcd.setCursor(0, 0);
                lcd.print("Sensor pH");
                //ph air garam ph7
                //bacaan sensor (pastikan sensor sudah ada di tempat yang sesuai)
                // ambil 3 kali dengan delay 1s
                // ambil rata-rata
                //offset = nilai pasti - bacaan sensor
                // nanti nilai pengukuran = bacaan sensor - offset
                delay(2000);
                lcd.print("Selesai 01");

            } else if (switch1 == 1 && switch2 == 0 && switch3 == 1) {
                sensor = 2;
                lcd.setCursor(0, 0);
                lcd.print("Sensor UV");
                // UV senter UV yang diketahui lumen
                //bacaan sensor (pastikan sensor sudah ada di tempat yang sesuai)
                // ambil 3 kali dengan delay 1s
                // ambil rata-rata
                //offset = nilai pasti - bacaan sensor
                // nanti nilai pengukuran = bacaan sensor - offset
                delay(2000);
                lcd.print("Selesai 10");
            } else if (switch1 == 1 && switch2 == 1 && switch3 == 1) {
                sensor = 3;
                lcd.setCursor(0, 0);
                lcd.print("Sensor Hygro");
                //air dan tanah 1:1
                //bacaan sensor (pastikan sensor sudah ada di tempat yang sesuai)
                // ambil 3 kali dengan delay 1s
                // ambil rata-rata
                //offset = nilai pasti - bacaan sensor
                // nanti nilai pengukuran = bacaan sensor - offset
                delay(2000);
                lcd.print("Selesai 11");
            } else if (switch1 == 0 && switch2 == 0 && switch3 == 1) {
                flag = 1;
                lcd.setCursor(0, 0);
                lcd.print("Selesai");
                lcd.setCursor(0, 1);
                lcd.print("Keluar");
                delay(2000);
            }
        }
    }
    sendToSupabase(moisturePercentage,uvIndex,Po );
    delay(5000);
}

void sendToSupabase(float moisture, float uvIndex, float pH) {
    if (WiFi.status() == WL_CONNECTED) {
        http.begin("https://zohwotklgcwnxhrvjdbc.supabase.co/rest/v1/data_iot");
        http.addHeader("Content-Type", "application/json");
        http.addHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpvaHdvdGtsZ2N3bnhocnZqZGJjIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MTc0MzA3MDYsImV4cCI6MjAzMzAwNjcwNn0.qjhyxaCesSBL4QU-SOaSbhCJX-iWG3mwGv4zNOCYlrk");

        String jsonData = "{\"humidity\":" + String(moisture) + ", \"uv\":" + String(random(10, 101) / 4.5) + ", \"ph\":" + String(pH) + "}";

        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("Error in WiFi connection");
    }
}