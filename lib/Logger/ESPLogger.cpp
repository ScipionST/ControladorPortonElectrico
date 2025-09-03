#include "ESPLogger.h"

ESPLogger::ESPLogger() {}

void ESPLogger::log(String text) {
  logs[index++] = text;
  logsCounter++;

  if (index >= MAX_LOGS) {
    int sendStatus = sendLogs();

    if (!sendStatus) {
      logsCounter++;
      return;
    }
    else {
      index = 0;
      logsCounter = 0;
    }
  }
}

int ESPLogger::sendLogs() {
  JsonDocument doc;

  for (int i = 0; i < logsCounter; i++) {
    String key = "data" + String(i + 1);
    doc[key] = logs[i];
  }

  String json;
  serializeJson(doc, json);

  HTTPClient http;
  http.setTimeout(3000);
  http.begin(ESPLoggerServer);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(json);
  if (httpResponseCode != 201) {
    http.end();
    return 0;
  }
  else {
    http.end();
    return 1;
  }
}
