#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "env.h"  // configurações de rede e tópicos MQTT

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// pinos de hardware
const servo1_pin = 26; // pino Servo 1;
const servo2_pin = 27; // pino Servo 2;
const byte TRIGGER_PIN = 5; // pino Sensor Ultrassônico;
const byte ECHO_PIN = 18; // pino Sensor Ultrassônico;
const int LED = 14; // pino LED

Servo1 servo1;
Servo2 servo2;

void setRGB(int r, int g, int b) {
  analogWrite(PIN_LED_R, r);
  analogWrite(PIN_LED_G, g);
  analogWrite(PIN_LED_B, b);
}

// callback MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Mensagem recebida em ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  // LED de iluminação
  if (String(topic) == TOPIC_LED) {
    if (msg == "ON") {
      digitalWrite(PIN_LED_ILUMINACAO, HIGH);
      ledIluminacao = true;
    } else if (msg == "OFF") {
      digitalWrite(PIN_LED_ILUMINACAO, LOW);
      ledIluminacao = false;
    }
  }

  // Servo 1
  if (String(topic) == TOPIC_SERVO1) {
    int pos = msg.toInt();
    servo1.write(pos);
  }

  // Servo 2
  if (String(topic) == TOPIC_SERVO2) {
    int pos = msg.toInt();
    servo2.write(pos);
  }

  // LED RGB
  if (String(topic) == TOPIC_STATUS) {
    if (msg == "OK") setRGB(0, 255, 0);         // Verde
    else if (msg == "ALERTA") setRGB(255, 255, 0); // Amarelo
    else if (msg == "ERRO") setRGB(255, 0, 0);  // Vermelho
    else if (msg == "OFF") setRGB(0, 0, 0);     // Desligado
  }
}

// Conexão WiFi e MQTT

void connectWiFi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao broker MQTT... ");
    String clientId = String(DEVICE_NAME) + "-" + String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), BROKER_USER_ID, BROKER_USER_PASS)) {
      Serial.println("Conectado ao HiveMQ Cloud!");

      // Inscrições nos tópicos
      mqttClient.subscribe(TOPIC_LED);
      mqttClient.subscribe(TOPIC_SERVO1);
      mqttClient.subscribe(TOPIC_SERVO2);
      mqttClient.subscribe(TOPIC_STATUS);

      setRGB(0, 255, 0);  // Verde = conectado
    } else {
      Serial.print("Falha na conexão. Código de erro: ");
      Serial.println(mqttClient.state());
      setRGB(255, 0, 0);  // Vermelho = erro
      delay(5000);
    }
  }
}

// Setup
void setup() {
  Serial.begin(115200);

  // Configura pinos
  pinMode(PIN_LED_ILUMINACAO, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_PRESENCA, INPUT);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);

  // Estado inicial
  setRGB(0, 0, 255); // Azul = iniciando

  // Conecta WiFi
  connectWiFi();

  // Configura conexão segura (TLS)
  espClient.setInsecure(); // desabilita verificação de certificado
  mqttClient.setServer(BROKER_URL, BROKER_PORT);
  mqttClient.setCallback(callback);

  // Conecta MQTT
  reconnectMQTT();
}

// Loop principal
void loop() {
  if (!mqttClient.connected()) reconnectMQTT();
  mqttClient.loop();

  // Lê o sensor de presença a cada 1 segundo
  unsigned long now = millis();
  if (now - lastRead > 1000) {
    lastRead = now;
    int presenca = digitalRead(PIN_PRESENCA);

    if (presenca != presencaAnterior) {
      presencaAnterior = presenca;
      mqttClient.publish(TOPIC_PRESENCA3, presenca ? "1" : "0");
      Serial.print("Presença detectada: ");
      Serial.println(presenca ? "Sim" : "Não");
    }
  }
}
