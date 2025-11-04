#ifndef ENV_H
#define ENV_H

// --- WiFi ---
#define WIFI_SSID "FIESC_IOT_EDU"
#define WIFI_PASS "8120gv08"

// --- HiveMQ ---
#define BROKER_URL "bbbe195380654bf482d3272ea1f6c91f.s1.eu.hivemq.cloud"
#define BROKER_PORT 8883
#define BROKER_USER_ID "chupel"
#define BROKER_USER_PASS "Caua"

// --- Identificação do Dispositivo
#define DEVICE_NAME "S2"

// --- Tópicos MQTT para S2 ---
#define TOPIC_PRESENCA2 "FerroviaX/S2/presenca2"
#define TOPIC_PRESENCA4 "FerroviaX/S2/presenca4"
#define TOPIC_LED "FerroviaX/S2/led"
#define TOPIC_SERVO3 "FerroviaX/S2/servo3"
#define TOPIC_STATUS "FerroviaX/S2/status"

#endif