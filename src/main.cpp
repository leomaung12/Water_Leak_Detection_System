#include <Arduino.h>

#include "config.h"
#include "alarm.h"
#include "web_server.h"
#include "telegram.h"

static unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);

  alarmSetup();
  webSetup();
}

void loop() {
  int value = alarmReadSensor();
  webSetLastValue(value);

  // periodic serial print
  if (millis() - lastPrint > SERIAL_PRINT_MS) {
    Serial.print("ADC: ");
    Serial.println(value);
    lastPrint = millis();
  }

  // Trigger notification once per leak event
  if (alarmIsTriggered() && !webGetNotificationSent()) {
    String msg = "🚨 WATER LEAK DETECTED!\n"
                 "Sensor value: " + String(value) + "\n"
                 "Threshold: " + String(THRESHOLD);
    telegramSend(msg);
    webSetNotificationSent(true);
  }

  // Update alarm + buzzer pattern
  alarmUpdate(value);

  // Handle incoming web requests
  webLoop();

  delay(LOOP_DELAY_MS);
}
