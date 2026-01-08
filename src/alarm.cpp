#include "alarm.h"
#include "config.h"

static bool g_alarmTriggered = false;
static unsigned long g_alarmStart = 0;

void alarmSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

int alarmReadSensor() {
  return analogRead(SENSOR_PIN);
}

void alarmUpdate(int value) {
  // Latch alarm on threshold crossing
  if (!g_alarmTriggered && value > THRESHOLD) {
    g_alarmTriggered = true;
    g_alarmStart = millis();
  }

  // Buzzer behavior
  if (g_alarmTriggered) {
    unsigned long elapsed = millis() - g_alarmStart;

    if (elapsed < BUZZ_WINDOW_MS) {
      unsigned long cycle = elapsed % BUZZ_CYCLE_MS;
      digitalWrite(BUZZER_PIN, (cycle < BUZZ_ON_MS) ? HIGH : LOW);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

bool alarmIsTriggered() {
  return g_alarmTriggered;
}

unsigned long alarmStartTime() {
  return g_alarmStart;
}

void alarmReset() {
  g_alarmTriggered = false;
  digitalWrite(BUZZER_PIN, LOW);
}
