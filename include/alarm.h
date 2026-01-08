#pragma once
#include <Arduino.h>

void alarmSetup();
int  alarmReadSensor();
void alarmUpdate(int value);

bool alarmIsTriggered();
unsigned long alarmStartTime();
void alarmReset();
