#pragma once

void webSetup();
void webLoop();

int  webGetLastValue();
void webSetLastValue(int v);

void webSetNotificationSent(bool v);
bool webGetNotificationSent();
