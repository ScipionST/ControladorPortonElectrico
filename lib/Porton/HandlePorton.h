#ifndef PORTON_H
#define PORTON_H

#include <Arduino.h>
#include "ESPGlobalStates.h"
#include "ESPLogger.h"

extern volatile bool sensorOpen;
extern volatile bool sensorClose;
extern volatile bool flagOpen;
extern volatile bool flagClose;
extern volatile bool flagObstacle;

void flagSensorOpen(); // IRAM_ATTR (interrupciones)
void flagSensorClose(); // IRAM_ATTR
void flagSensorObstacle(); // IRAM_ATTR

class ESPLogger; // Declaración anticipada

class Porton
{
public:
  Porton(ESPLogger& logger);

  bool sensorObstacle = false;

  bool relayOpen = false;
  bool relayClose = false;

  bool stopMode = false;

  void setup();

  String open();
  String close();
  String stop();

  void handleCommand();
  void handleOpen();
  void handleClose();
  void handleStop();
  void autoClose();

private:
  ESPLogger _logger; // logger interno

  enum ComandoPorton {
    ABRIR,
    CERRAR,
    NINGUNO,
  };
  ComandoPorton comandoPorton;

  enum EstadoPorton {
    DETENIDO,
    ABRIENDO,
    CERRANDO,
    STOPMODE
  };
  EstadoPorton estadoPorton;

  enum EstadoSensorStop {
    CERRADO,
    ABIERTO,
    MEDIO
  };
  EstadoSensorStop estadoSensorStop;

  const int timeoutEstado = 1000;
  unsigned long timerEstado = 0;

  int closeTimeout = 6000;
  unsigned long timerToClose = 5000;
  unsigned long lastObstacle = 5000;

  /**
   * Verifica el estado del pin del sensor de obstáculo.
   * @return 0 si no hay obstáculo, 1 si hay un obstáculo. Si detecta un obstáculo, asigna millis() a lastObstacle
   */
  int obstacle();
};

#endif
