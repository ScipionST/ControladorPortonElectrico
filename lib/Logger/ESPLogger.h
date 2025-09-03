#ifndef ESPLOGGER_H
#define ESPLOGGER_H

#include "Arduino.h"
#include "ArduinoJson.h"
#include "HTTPClient.h"

class ESPWebServer;

/**
 * Permite almacenar mensajes en formato JSON y luego ser enviados hacia un servidor
 */
class ESPLogger
{
public:
  ESPLogger();
  /**
   * @param text espera un String para almacenar
   *
   */
  void log(String text);
  /**
   * @return 0 si hubo error, 1 si los datos se recibieron con éxito
   */
  int sendLogs();

private:
  static const int MAX_LOGS = 5;
  String logs[MAX_LOGS];

  int index = 0;
  int logsCounter = 0;

  // Es necesario establecer un servidor local al cual será enviada la data
  const String ESPLoggerServer = "http://192.168.0.100:5100/serialprintESP32";
};

#endif
