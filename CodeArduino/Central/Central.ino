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

Adafruit_BMP280 bmp; // I2C
DFRobot_DHT11 DHT;

WebServer server(80);

void setup() {
  Serial.begin(115200);

  
  WiFi.softAP("ESP32_AP", "senha123");
  Serial.println("Ponto de acesso criado. IP:");
  Serial.println(WiFi.softAPIP());

  //Inicializa BMP280
  Serial.println(F("BMP280 test"));
  unsigned status;
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Não foi possível encontrar um sensor BMP280 válido!"));
    while (1) delay(10);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   
                  Adafruit_BMP280::SAMPLING_X2,     
                  Adafruit_BMP280::SAMPLING_X16,    
                  Adafruit_BMP280::FILTER_X16,      
                  Adafruit_BMP280::STANDBY_MS_500); 

  
  server.on("/dados", []() {

    int leituraSolo = analogRead(SENSOR_SOLO);
    int leituraAgua = analogRead(SENSOR_AGUA);
    int leituraLuz = analogRead(SENSOR_LUZ);

    //DHT11
    DHT.read(DHT11_PIN);
    int leituraHumi = DHT.humidity;
    int leituraTemp = DHT.temperature;

    //UV
    int SensorUV = analogRead(UV_SENSOR_PIN);
    float voltage = SensorUV * (3.3 / 4095.0);
    float leituraUV = voltage * 7.0 / 1.0;

    //BMP280
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