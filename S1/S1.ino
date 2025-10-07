#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifi_client;
PubSubClient mqtt(wifi_client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.println("Conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
Serial.println("Conectado com sucesso!")
mqtt.setServer(brokerURL.c_str(),brokerPort);
String clientID = "S1-";
clientID += String(random(0xffff),HEX);
Serial.println("Conectado ao broker!")
while(mqtt.connect(clientID.c_str())) == 0){
  Serial.print(".");
  delay(200);
}
Serial.println("\mConectado ao broker!")
}

void loop() {
  // put your main code here, to run repeatedly:

}
