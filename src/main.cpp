#include <Arduino.h>
#include "wifi_info.h"
#include "dht_info.h"
#include <arduino_homekit_server.h>
#include <ESPmDNS.h>

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_temperature;
extern "C" homekit_characteristic_t cha_humidity;

extern "C" homekit_characteristic_t cha_humidity;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

static DHTHumiture last_report_humiture;

#define LOG_D(fmt, ...) printf_P(PSTR(fmt "\n"), ##__VA_ARGS__);

int random_value(int min, int max)
{
  return min + random(max - min);
}

void my_homekit_setup()
{
  arduino_homekit_setup(&config);
}

void my_homekit_report()
{

  DHTHumiture th = dht_sensor_read();
  float temperature_value = th.temperature;
  float humidity_value = th.humidity;

  if (temperature_value == -1 || humidity_value == -1)
  {
    return;
  }

  if (th.temperature != last_report_humiture.temperature || th.humidity != last_report_humiture.humidity)
  {
    Serial.print(F("Humidity: "));
    Serial.print(th.humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(th.temperature);
    Serial.print(F("°C "));
    Serial.println();
  
    cha_temperature.value.float_value = temperature_value;
    cha_humidity.value.float_value = humidity_value;
    homekit_characteristic_notify(&cha_temperature, cha_temperature.value);
    homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
    last_report_humiture = th;
  }
}

void my_homekit_loop() {

  arduino_homekit_loop();

  const uint32_t t = millis();
  /*
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  */

  my_homekit_report();

  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 10 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
          ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}

void setup() {

  Serial.begin(115200);

  // 连接 WiFi
  wifi_connect();
  // 初始化 DTH22
  dht_sensor_setup();

  my_homekit_setup();
}

void loop() {
  delay(delayMS + 2000);
  my_homekit_loop();
}