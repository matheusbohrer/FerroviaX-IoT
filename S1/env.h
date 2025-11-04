#ifndef ENV_H
#define ENV_H

// --- WiFi ---
#define WIFI_SSID "FIESC_IOT_EDU"
#define WIFI_PASS "8120gv08"

// --- HiveMQ ---
#define BROKER_URL "bbbe195380654bf482d3272ea1f6c91f.s1.eu.hivemq.cloud"
#define BROKER_PORT 8883
#define BROKER_USER_ID "bohrer"
#define BROKER_USER_PASS "Matheus"

// --- Identificação do Dispositivo
#define DEVICE_NAME "S1"

// --- Tópicos MQTT para S1 ---
#define TOPIC_TEMP "FerroviaX/S1/temp"
#define TOPIC_UMID "FerroviaX/S1/umid"
#define TOPIC_LUMINOSIDADE "FerroviaX/S1/luminosidade"
#define TOPIC_PRESENCA "FerroviaX/S1/presenca"
#define TOPIC_LED "FerroviaX/S1/led"
#define TOPIC_STATUS "FerroviaX/S1/status"

#endif