#include "telegram.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include "secret.h"

static String jsonEscape(String s) {
  s.replace("\\", "\\\\");
  s.replace("\"", "\\\"");
  s.replace("\n", "\\n");
  s.replace("\r", "");
  return s;
}

void telegramSend(const String& message) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, cannot send Telegram message.");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure(); // quick TLS approach for projects

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(TELEGRAM_BOT_TOKEN) + "/sendMessage";

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  String safeMsg = jsonEscape(message);

  String payload = "{";
  payload += "\"chat_id\":\"" + String(TELEGRAM_CHAT_ID) + "\",";
  payload += "\"text\":\"" + safeMsg + "\"";
  payload += "}";

  Serial.println("Sending Telegram message...");
  int httpCode = http.POST(payload);

  Serial.print("Telegram HTTP response code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    Serial.println(http.getString());
  } else {
    Serial.println("Error sending Telegram request");
  }

  http.end();
}
