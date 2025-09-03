#ifndef ESPGLOBALSTATES_H
#define ESPGLOBALSTATES_H

#include <Arduino.h>

#define pinLed 2

#define pinRelayOpen 16
#define pinRelayClose 17

#define pinObstacle 4
#define pinSensorOpen 19
#define pinSensorClose 18

#define pinZeroCross 27

class ESPWebServer; // Declaración anticipada (SIN INCLUDE)

/**
 * Permite recibir y ejecutar comandos internos aislados de otras funciones
 *
 * @param ComandReset   Establecer en true para indicar que se debe reiniciar el dispositivo.
 */
class ESPGlobalStates
{
private:
  friend class ESPWebServer;

  bool CommandReset = false;
  const int TimeoutReset = 1000; // Tiemp de espera antes de reiniciar
  unsigned long TimerReset = 0; // Marca de tiempo para calcular si ha pasado el tiempo necesario para poder reiniciar
  void reset();

public:
  ESPGlobalStates();
  // ~ESPGlobalStates();

  /**
   * Verifica si se han activado variables de comandos. Realiza acciones según comandos.
   */
  void checkCommands();
};

#endif
