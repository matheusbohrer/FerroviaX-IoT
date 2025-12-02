// Bibliotecas e Includes
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"
#include <DHT.h>

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
DHT dht(PINO_DHT, DHT11);


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
  
  
  Serial.begin(115200);    // inicia a Serial para debug
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
  mqttClient.subscribe(TOPIC_PRESENCA);
  mqttClient.subscribe(TOPIC_LED);


// Configuração de pino
  Serial.println("\n Conectado com sucesso!");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

// Leitura do ultrassonico
float lerDistancia() {
  // Debug dos pinos configurados
  Serial.print("Testando sensor - Trigger: ");
  Serial.print(PINO_ULTRA_TRIG);
  Serial.print(", Echo: ");
  Serial.println(PINO_ULTRA_ECHO);
  
  // Garantir que o trigger está LOW por mais tempo
  digitalWrite(PINO_ULTRA_TRIG, LOW);
  delayMicroseconds(5);
  
  // Enviar pulso de trigger
  digitalWrite(PINO_ULTRA_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_ULTRA_TRIG, LOW);
  
  // Ler duração do pulso no pino echo com timeout maior
  long duracao = pulseIn(PINO_ULTRA_ECHO, HIGH, 50000); // timeout de 50ms
  
  // Debug da duração lida
  Serial.print("Duração do pulso: ");
  Serial.print(duracao);
  Serial.println(" microsegundos");
  
  // Verificar se houve timeout
  if (duracao == 0) {
    Serial.println("Erro: timeout - sensor não responde ou mal conectado");
    
    // Teste adicional - verificar se o pino Echo está funcionando
    Serial.print("Estado atual do pino Echo: ");
    Serial.println(digitalRead(PINO_ULTRA_ECHO));
    
  }
  
  // Verificar se a duração está em um alcance válido (2cm até 400cm)
  if (duracao < 116 || duracao > 23200) {
    Serial.print("Duração fora do alcance válido: ");
    Serial.println(duracao);
    return -1;
  }
  
  // Calcular distância
  // Tempo de ida e volta
  float distancia = (duracao * 0.0343) / 2;
  
  Serial.print("Distância calculada: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  return distancia;
}

 // Leitura do LDR
 float lerIluminacao() {

  int raw = analogRead(PINO_LDR);
  float iluminacao = raw;
  return iluminacao;
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
