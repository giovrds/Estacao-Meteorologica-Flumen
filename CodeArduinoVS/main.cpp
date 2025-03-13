#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <DFRobot_DHT11.h>

#define DHT11_PIN 19
#define SENSOR_SOLO 23
#define SENSOR_LUZ 34
#define SENSOR_AGUA 35
#define UV_SENSOR_PIN 34

const char* ssid = "Giogio";
const char* password = "giory@2409";

Adafruit_BMP280 bmp;
DFRobot_DHT11 DHT;
WebServer server(80);

void setup() {
    Serial.begin(115200);

    // Conectar ao Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Conectando ao Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConectado ao Wi-Fi!");
    Serial.print("IP do ESP32: ");
    Serial.println(WiFi.localIP());

    // Inicializa BMP280
    if (!bmp.begin(0x76)) {
        Serial.println("Erro: Não foi possível encontrar o sensor BMP280!");
        while (1) delay(10);
    }

    // Configuração do servidor web
    server.on("/dados", []() {
        int leituraSolo = analogRead(SENSOR_SOLO);
        int leituraAgua = analogRead(SENSOR_AGUA);
        int leituraLuz = analogRead(SENSOR_LUZ);

        DHT.read(DHT11_PIN);
        int leituraHumi = DHT.humidity;
        int leituraTemp = DHT.temperature;

        int SensorUV = analogRead(UV_SENSOR_PIN);
        float voltage = SensorUV * (3.3 / 4095.0);
        float leituraUV = voltage * 7.0;

        float leituraPressao = bmp.readPressure() / 100.0;
        float leituraAltitude = bmp.readAltitude(1013.2);

        String jsonData = "{";
        jsonData += "\"umidade_solo\":" + String(leituraSolo) + ",";
        jsonData += "\"nivel_agua\":" + String(leituraAgua) + ",";
        jsonData += "\"umidade_ar\":" + String(leituraHumi) + ",";
        jsonData += "\"luminosidade\":" + String(leituraLuz) + ",";
        jsonData += "\"temperatura\":" + String(leituraTemp) + ",";
        jsonData += "\"uv\":" + String(leituraUV) + ",";
        jsonData += "\"pressao\":" + String(leituraPressao) + ",";
        jsonData += "\"altitude\":" + String(leituraAltitude);
        jsonData += "}";

        server.send(200, "application/json", jsonData);
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
