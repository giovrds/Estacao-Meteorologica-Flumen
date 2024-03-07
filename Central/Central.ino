#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 8

//looping
int tempo;
int menor;
int maior;

//Sensor de água
#define sensor A1
int humidade = 0;

//Sensor de luminosidade
int foto = A2;
int valor;
int umidadesolo = A5;
int valorumidadesolo;

void setup() {
  Serial.begin(9600);
  //Sensor de luminosidade
  pinMode(foto, INPUT);
  pinMode(umidadesolo, INPUT);
}

void loop() {

  //Sensor de água
  //humidade = analogRead(sensor);
  //Serial.print(" |Sensor de água: ");
  //Serial.print(humidade);
  //Serial.println("|");
  //Serial.println();
  //delay(55);

  //DHT11 - temperatura e umidade
  DHT.read(DHT11_PIN);
  Serial.print(" |Humidade: ");
  Serial.print(DHT.humidity);
  Serial.println("|");
  Serial.println();
  Serial.print(" |Temperatura: ");
  Serial.print(DHT.temperature);
  Serial.println("ºC |");
  Serial.println();

  //luminosidade
  valor = analogRead(foto);

  if (valor < 400) {
    Serial.print(" |Luminosidade baixa!|");
    Serial.println();
    Serial.println();
  } else {
    Serial.print(" |Luminosidade alta!|");
    Serial.println();
    Serial.println();
  }

  //umidade do solo
  valorumidadesolo = analogRead(umidadesolo);

  Serial.print("Valor Solo ");
  Serial.print(valorumidadesolo);

  if (valorumidadesolo < 400) {
    Serial.print(" Umidade: Baixa!");
    Serial.println();
    Serial.println();
  }
  if (valorumidadesolo > 400 && valorumidadesolo < 700) {
    Serial.print(" Umidade: Moderada");
    Serial.println();
    Serial.println();
  }
  if (valorumidadesolo > 700 && valorumidadesolo <1200) {
    Serial.print(" Umidade: Adequada");
    Serial.println();
    Serial.println();
  }
  delay(1000);
  if (millis() - tempo > 60000) {  // 10000 milissegundos = 10 segundos
    menor = maior = valorumidade solo = valor = 0;  // Limpar os valores das variáveis
  tempo = millis();
  }
}
