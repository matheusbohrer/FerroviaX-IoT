#include <WiFi.h>
WifiClient client;
const String SSID = "nome da rede";
const String PASS = "nome da rede";

void setup() {
  Serial.begin(115200);
  client.begin(nome, senha);
  Serial.println("Conectando no WiFi");
  while(cliente.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
