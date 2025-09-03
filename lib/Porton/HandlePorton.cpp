#include "HandlePorton.h"

volatile bool sensorOpen = false;
volatile bool sensorClose = false;
volatile bool flagOpen = false;
volatile bool flagClose = false;
volatile bool flagObstacle = false;

class ESPLogger;

Porton::Porton(ESPLogger& logger) : _logger(logger) {}

void Porton::setup() {
  pinMode(pinRelayOpen, OUTPUT);
  pinMode(pinRelayClose, OUTPUT);
  pinMode(pinSensorOpen, INPUT_PULLDOWN);
  pinMode(pinSensorClose, INPUT_PULLDOWN);
  pinMode(pinObstacle, INPUT_PULLDOWN);

  digitalWrite(pinRelayOpen, HIGH);
  digitalWrite(pinRelayClose, HIGH);

  attachInterrupt(digitalPinToInterrupt(pinSensorOpen), flagSensorOpen, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinSensorClose), flagSensorClose, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinObstacle), flagSensorObstacle, CHANGE);

  estadoSensorStop = digitalRead(pinSensorOpen) ? ABIERTO : digitalRead(pinSensorClose) ? CERRADO : MEDIO;
  estadoPorton = DETENIDO;
}

// ABRIR
String Porton::open()
{
  if (estadoSensorStop == ABIERTO) return "El portón está abierto";

  if (estadoPorton == ABRIENDO) return "El portón ya se está abriendo";

  handleOpen();
  return "Abriendo...";
}

// CERRAR
String Porton::close()
{
  if (estadoSensorStop == CERRADO) return "El portón ya está cerrado";

  if (estadoPorton == CERRANDO) return "El portón ya se está cerrando";

  if (obstacle()) return "Obstáculo detectado";

  handleClose();
  return "Cerrando...";
}

// DETENER
String Porton::stop() {
  if (estadoPorton != STOPMODE || !digitalRead(pinRelayOpen) || !digitalRead(pinRelayClose)) {
    digitalWrite(pinRelayOpen, HIGH);
    digitalWrite(pinRelayClose, HIGH);

    estadoPorton = STOPMODE;
    return "Detenido...";
  }

  return "El portón ya está en modo detenido...";
}

// CONTROL DE COMANDOS
void Porton::handleCommand()
{
  if (comandoPorton == ABRIR && millis() - timerEstado >= timeoutEstado) {
    if (!digitalRead(pinSensorOpen)) {
      digitalWrite(pinRelayOpen, LOW);
      estadoPorton = ABRIENDO;
      comandoPorton = NINGUNO;
    }
    else {
      estadoSensorStop = ABIERTO;
      estadoPorton = DETENIDO;
      comandoPorton = NINGUNO;
      return;
    }
  }

  if (comandoPorton == CERRAR && millis() - timerEstado >= timeoutEstado) {
    if (!digitalRead(pinSensorClose)) {
      digitalWrite(pinRelayClose, LOW);
      estadoPorton = CERRANDO;
      comandoPorton = NINGUNO;
    }
    else {
      estadoSensorStop = CERRADO;
      estadoPorton = DETENIDO;
      comandoPorton = NINGUNO;
      return;
    }
  }
}

// CONTROL APERTURA
void Porton::handleOpen()
{
  digitalWrite(pinRelayClose, HIGH);

  comandoPorton = ABRIR;
  timerEstado = millis();
}

// CONTROL CIERRE
void Porton::handleClose()
{
  digitalWrite(pinRelayOpen, HIGH);

  comandoPorton = CERRAR;
  timerEstado = millis();
}

void Porton::handleStop()
{
  // SENSOR DE ABIERTO
  if (flagOpen) {
    flagOpen = false;

    if (estadoPorton != ABRIENDO) return;

    static unsigned long last = 0;
    unsigned long now = millis();

    if (now - last > 50) {
      digitalWrite(pinRelayOpen, HIGH);
      estadoPorton = DETENIDO;
      estadoSensorStop = ABIERTO;
    }
  }

  // SENSOR DE CERRADO
  if (flagClose) {
    flagClose = false;

    if (estadoPorton != CERRANDO) return;

    static unsigned long last = 0;
    unsigned long now = millis();

    if (now - last > 50) {
      digitalWrite(pinRelayClose, HIGH);
      estadoPorton = DETENIDO;
      estadoSensorStop = CERRADO;
    }
  }

  // MODO CERRANDO
  if (estadoPorton == CERRANDO)
  {
    if (obstacle())
    {
      digitalWrite(pinRelayClose, HIGH);

      estadoPorton = DETENIDO;
      comandoPorton = ABRIR;
    }
  }
}

int Porton::obstacle()
{ // NEGADO - Mi sensor de obstáculo fué invertido para entregar LOW cuando detecta obstáculo o no tiene energía.
  // Anteriormente el sensor de obstáculo mantenía su relay apagado y pasaba a HIGH cuando detectaba un obstáculo
  // Si el sensor se daña en este caso, el sensor jamás podría avisar de que hay un obstáculo (podría ocasionar un accidente al cerrarse con un vehículo en medio del sensor)
  if (!digitalRead(pinObstacle))
  {
    lastObstacle = millis();
    return 1;
  }

  return 0;
}

void Porton::autoClose()
{
  if (estadoPorton == ABRIENDO) {
    timerToClose = millis();
    return;
  }

  if (estadoSensorStop == CERRADO || estadoPorton == CERRANDO || estadoPorton == STOPMODE) return;

  obstacle();

  unsigned long timer = millis();

  if (timer - lastObstacle >= closeTimeout && timer - timerToClose >= closeTimeout)
  {
    comandoPorton = CERRAR;
  }
}

void IRAM_ATTR flagSensorOpen() {
  flagOpen = true;
}

void IRAM_ATTR flagSensorClose() {
  flagClose = true;
}

void IRAM_ATTR flagSensorObstacle() {
  flagObstacle = true;
}
