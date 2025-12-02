// Bibliotecas e Includes
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

// Pinout – Placa 1
const int PINO_DHT        = 4;   // DHT
const int PINO_ULTRA_ECHO = 23;  // Sensor ultrassônico - Echo
const int PINO_ULTRA_TRIG = 22;  // Sensor ultrassônico - Trigger
const int PINO_LED        = 19;  // LED
const int PINO_LDR        = 34;  // LDR
const int PINO_RGB_R      = 14;  // LED RGB - R
const int PINO_RGB_G      = 26;  // LED RGB - G
const int PINO_RGB_B      = 25;  // LED RGB - B


// Inicialização de objetos
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);


// LED e variáveis de piscar
const int ledPin = 19;
volatile bool blinkRequested = false;
unsigned long blinkUntil = 0;


// Conexão WiFi
void setup() {
  pinMode(PINO_DHT, INPUT);
  pinMode(PINO_ULTRA_ECHO, INPUT);
  pinMode(PINO_ULTRA_TRIG, OUTPUT);
  pinMode(PINO_LED, OUTPUT);
  pinMode(PINO_LDR, INPUT);
  pinMode(PINO_RGB_R, OUTPUT);
  pinMode(PINO_RGB_G, OUTPUT);
  pinMode(PINO_RGB_B, OUTPUT);
  
  
  Serial.begin(115200);    // inicia a Serial para ver mensagens de debug
  espClient.setInsecure();  // desativa verificação do certificado (somente para testes)
  WiFi.begin(WIFI_SSID, WIFI_PASS);  // inicia conexão WiFi
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("\nConectado com sucesso!");


// Conexão MQTT
  mqttClient.setServer(BROKER_URL, BROKER_PORT);
  String userID = "S1";
  userID += String(random(0xffff), HEX);
  while (!mqttClient.connect(userID.c_str(), BROKER_USER_ID, BROKER_USER_PASS)) {
    Serial.print(".");
    delay(200);
  }


// Inscrição e Callback
  mqttClient.setCallback(callback); // define a função chamada quando chegar mensagem no tópico
  mqttClient.subscribe(TOPIC_PRESENCA); // inscreve no tópico configurado


// Configuração de pino
  Serial.println("\n Conectado com sucesso!");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}


// Loop principal
void loop() {
  String mensagem = "";  // lê uma linha do Serial (até '\\n')

  if (Serial.available() > 0) {
  mensagem = Serial.readStringUntil('\n');
  mqttClient.publish(TOPIC_PRESENCA, mensagem.c_str());  // publica a mensagem no tópico MQTT
  Serial.println(mensagem);
  }

  mqttClient.loop();
  unsigned long now = millis();


  // Piscar LED (não bloqueante)
  if (blinkRequested) {
    if (now <= blinkUntil) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
      blinkRequested = false;
    }
  }
}


// Função Callback
void callback(char* topic, byte* payload, unsigned int length) {  // tópico, payload e tamanho
  // monta uma string C a partir do payload (evita usar String no callback)
  const unsigned int maxLen = 255;
  unsigned int copyLen = (length < maxLen) ? length : maxLen;
  char msg[maxLen + 1];
  for (unsigned int i = 0; i < copyLen; i++) {
    msg[i] = (char)payload[i];
  }
  msg[copyLen] = '\0';

  Serial.print("Mensagem recebida no tópico ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  // pede para piscar o LED por 1s (feito no loop, sem bloquear)
  blinkRequested = true;
  blinkUntil = millis() + 1000;
}
