#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"  // arquivo com as configurações

// --- Pinos ---
#define PIN_PRESENCA2 34
#define PIN_PRESENCA4 35
#define PIN_LED 2

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastMsg = 0;
int intervalo = 2000; // 2 segundos

// --- Callback MQTT ---
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.print("Mensagem recebida em ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  // Controle LED
  if (String(topic) == TOPIC_LED) {
    if (msg == "1") {
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED ligado!");
    } else {
      digitalWrite(PIN_LED, LOW);
      Serial.println("LED desligado!");
    }
  }
}

// --- Reconexão MQTT ---
void reconnect() {
  while (!mqtt.connected()) {
    Serial.print("Tentando conectar ao broker MQTT...");
    if (mqtt.connect(DEVICE_NAME, BROKER_USER_ID, BROKER_USER_PASS)) {
      Serial.println("Conectado!");
      mqtt.subscribe(TOPIC_LED);
      mqtt.subscribe(TOPIC_SERVO3);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(mqtt.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PRESENCA2, INPUT);
  pinMode(PIN_PRESENCA4, INPUT);
  pinMode(PIN_LED, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando-se ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());

  mqtt.setServer(BROKER_URL, BROKER_PORT);
  mqtt.setCallback(callback);
}

void loop() {
  if (!mqtt.connected()) reconnect();
  mqtt.loop();

  unsigned long agora = millis();
  if (agora - lastMsg > intervalo) {
    lastMsg = agora;

    int presenca2 = digitalRead(PIN_PRESENCA2);
    int presenca4 = digitalRead(PIN_PRESENCA4);

    char msgPresenca2[8];
    char msgPresenca4[8];
    sprintf(msgPresenca2, "%d", presenca2);
    sprintf(msgPresenca4, "%d", presenca4);

    mqtt.publish(TOPIC_PRESENCA2, msgPresenca2);
    mqtt.publish(TOPIC_PRESENCA4, msgPresenca4);

    Serial.println("Dados enviados:");
    Serial.print("Presença 2: ");
    Serial.println(presenca2);
    Serial.print("Presença 4: ");
    Serial.println(presenca4);
  }
}
