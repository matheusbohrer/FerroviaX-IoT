#ifndef ENV_H
#define ENV_H

// --- WiFi ---
#define WIFI_SSID "FIESC_IOT_EDU"
#define WIFI_PASS "8120gv08"

// --- HiveMQ ---
#define BROKER_URL "bbbe195380654bf482d3272ea1f6c91f.s1.eu.hivemq.cloud"
#define BROKER_PORT 8883
#define BROKER_USER_ID "bohrer2"
#define BROKER_USER_PASS "Matheus2"

// --- Identificação do Dispositivo
#define DEVICE_NAME "TREM"

// --- Tópicos MQTT para Trem ---
#define TOPIC_STATUS "FerroviaX/TREM/status"
#define TOPIC_PONTEH "FerroviaX/TREM/ponteH"

#endif