#pragma once

// pins
constexpr int SENSOR_PIN = 32;
constexpr int BUZZER_PIN = 15;

// leak setting
constexpr int THRESHOLD = 1000;

// timing
constexpr unsigned long SERIAL_PRINT_MS = 1000;
constexpr unsigned long LOOP_DELAY_MS   = 50;

// buzzer pattern
constexpr unsigned long BUZZ_WINDOW_MS  = 10000;  // 10 seconds
constexpr unsigned long BUZZ_CYCLE_MS   = 1000;
constexpr unsigned long BUZZ_ON_MS      = 200;