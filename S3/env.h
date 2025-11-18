#ifndef ENV_H
#define ENV_H

// --- WiFi ---
#define WIFI_SSID "FIESC_IOT_EDU"
#define WIFI_PASS "8120gv08"

// --- HiveMQ ---
#define BROKER_URL "179.222.224.26"
#define BROKER_PORT 6883
#define BROKER_USER_ID "nextrain"
#define BROKER_USER_PASS "nextrain"

// --- Identificação do Dispositivo
#define DEVICE_NAME "S3"

// --- Tópicos MQTT para S3 ---
#define TOPIC_PRESENCA3 "FerroviaX/S3/presenca3"
#define TOPIC_LED "FerroviaX/S3/led"
#define TOPIC_SERVO1 "FerroviaX/S3/servo1"
#define TOPIC_SERVO2 "FerroviaX/S3/servo2"
#define TOPIC_STATUS "FerroviaX/S3/status"

#endif