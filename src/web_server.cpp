#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "web_server.h"
#include "web_ui.h"
#include "secret.h"   

static WebServer server(80);

static int  g_lastValue = 0;
static bool g_notificationSent = false;

// --- Handlers, sendP -> flas(progmem), not ram
static void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

static void handleData() {
  String json = "{";
  json += "\"value\":" + String(g_lastValue) + ",";
  json += "\"leak\":false";
  json += "}";
  server.send(200, "application/json", json);
}

// --- WiFi connect ---
static void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - start > 20000) {  // 20s timeout
      Serial.println("\nWiFi connect timeout (will retry in loop)");
      return;
    }
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void webSetup() {
  connectWiFi();

  // Only start server if WiFi connected
  if (WiFi.status() == WL_CONNECTED) {
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    Serial.println("HTTP server started");
  } else {
    Serial.println("WiFi not connected yet; server not started.");
  }
}

void webLoop() {
  // If WiFi drops, you can optionally reconnect here
  if (WiFi.status() != WL_CONNECTED) return;
  server.handleClient();
}

void webSetLastValue(int v) { g_lastValue = v; }
bool webGetNotificationSent() { return g_notificationSent; }
void webSetNotificationSent(bool v) { g_notificationSent = v; }
