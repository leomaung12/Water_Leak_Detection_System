# Water_Leak_Detection_System

ESP32 Water Leak Detector
A real-time water leak detection system built on ESP32 that provides
local audible alerts, a live web dashboard, and Telegram notifications.

Features
Analog water sensor monitoring
Latched alarm with buzzer alert
Live web dashboard (HTML/CSS/JS)
Real-time sensor graph
Telegram notification on leak detection
Manual alarm reset and notification test

Tech Stack
ESP32 (Arduino / C++)
HTML, CSS, JavaScript
REST-style HTTP endpoints
Telegram Bot API

How It Works
ESP32 continuously reads an analog water sensor
When readings exceed a threshold, the alarm latches
Buzzer alerts locally
Telegram sends a one-time notification
Web dashboard shows live sensor data and alarm state

Demo

Future Improvements
Adjustable threshold via UI
Multiple sensors
WebSocket live updates
