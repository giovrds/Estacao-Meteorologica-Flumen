// Biblioteca do sensor de temperatura e umidade
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 8

// Biblioteca do sensor de pressão atmosférica
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Sensor de pressão atmosférica
#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp;

// Looping
int tempo;

// Sensor de água
#define sensor A1
int umidade = 0;

// Sensor de luminosidade
int luz = A2;
int valor;

// Sensor de umidade do solo
int umidadesolo = A5;
int valorumidadesolo;

void setup() {
  Serial.begin(9600);

  // Sensor de luminosidade
  pinMode(luz, INPUT);
  pinMode(umidadesolo, INPUT);

  // Sensor de pressão atmosférica
  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println(F("O Sensor BMP280 não está respondendo!"));
    while (1); 
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  // Sensor de pressão atmosférica
  float pressao = bmp.readPressure();
  pressao /= 100.0F; // Converte para hPa
  Serial.print(F(" |Pressão Atmosférica: "));
  Serial.print(pressao);
  Serial.print(" hPa");
  Serial.println("|");
  Serial.println();

  // Sensor de água
  umidade = analogRead(sensor);
  Serial.print(" |Sensor de água: ");
  Serial.print(umidade);
  Serial.println("|");
  Serial.println();
  delay(55);

  // DHT11 - temperatura e umidade
  DHT.read(DHT11_PIN);
  Serial.print(" |Umidade: ");
  Serial.print(DHT.humidity);
  Serial.println("|");
  Serial.println();
  Serial.print(" |Temperatura: ");
  Serial.print(DHT.temperature);
  Serial.println("ºC |");
  Serial.println();

  // Sensor de luminosidade
  valor = analogRead(luz);
  if (valor < 400) {
    Serial.print(" |Luminosidade: baixa!|");
    Serial.println();
    Serial.println();
  } else {
    Serial.print(" |Luminosidade: alta!|");
    Serial.println();
    Serial.println();
  }

  // Sensor de umidade do solo
  valorumidadesolo = analogRead(umidadesolo);
  Serial.print(" |Valor Solo: ");
  Serial.print(valorumidadesolo);
  Serial.print("|");
  Serial.println();
  Serial.println();

  if (valorumidadesolo < 400) {
    Serial.print(" |Umidade do Solo: Baixa!|");
    Serial.println();
    Serial.println();
  }
  if (valorumidadesolo > 400 && valorumidadesolo < 700) {
    Serial.print(" |Umidade do Solo: Moderada|");
    Serial.println();
    Serial.println();
  }
  if (valorumidadesolo > 700 && valorumidadesolo < 1200) {
    Serial.print(" |Umidade do Solo: Adequada|");
    Serial.println();
    Serial.println();
  }
  delay(5000);
  if (millis() - tempo > 60000) {  // 10000 milissegundos = 10 segundos
    valorumidadesolo = valor = 0;  // Limpar os valores das variáveis
    tempo = millis();
  }
}