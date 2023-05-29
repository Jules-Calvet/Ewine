#ifndef __NET_CONF_H
#define __NET_CONF_H

// Please modify those macro definitions to meet needs

#define WIFI_TAR 1

#if WIFI_TAR == 0

#define AP_SSID     (const char*)"Jules"//"IntervIsen"//"OnePlus9_ben"//
#define AP_PSWD     (const char*)"montazac"//"Bi1venueYM"//"lemot2passdebg!"//

#elif WIFI_TAR == 1

#define AP_SSID     (const char*)"Reseau du KGB"
#define AP_PSWD     (const char*)"12345678"

#endif

#define CONNECT_MODE 0

#if CONNECT_MODE == 0

#define IpServer       "broker.hivemq.com"//"test.mosquitto.org"//"de9e95f0ec1b4d449ff5cbe8461a42f6.s2.eu.hivemq.cloud"//
#define ServerPort     "1883"//"1884"//"8883"//

#elif CONNECT_MODE == 1

#define IpServer       "YOUR OWN PC TCP SERVER ADDRESS"
#define ServerPort     "YOUR OWN PC TCP SERVER PORT"

#endif

#define MQTT_DEVICE_ID (uint8_t*)"YOUR OWN MQTT DEVICE ID"
#define MQTT_SECRET    (uint8_t*)"YOUR OWN MQTT DEVICE SECRET CODE"

#define MQTT_CLIENTID   "ewine"
#define MQTT_USERNAME   ""//"rw"//"yncrea"//
#define MQTT_PASSWORD   ""//"readwrite"//"Yncrea1234"//

#endif
