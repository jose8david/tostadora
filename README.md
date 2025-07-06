# TostadoraButtonControl 🚀

Este proyecto controla una tostadora mediante una placa Wemos D1 Mini (ESP8266). Incluye prevención de rebotes y está desarrollado en PlatformIO con control de versiones usando Git.

## Características
- Plataforma: PlatformIO + ESP8266
- Lógica: C++ con debounce manual
- Documentación: Markdown
- Versionado: Git

## Enceder el LED mientras se mantiene pulsado el botón 

```Arduino
#include <Arduino.h>

#define BUTTON_PIN D2      // Pin donde está el botón
#define LED_PIN LED_BUILTIN // LED de la placa Wemos

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

bool buttonState = HIGH;
bool lastButtonReading = HIGH;

void setup() {
  pinMode(BUTTON_PIN, INPUT);       // botón con resistencia pull-up externa
  pinMode(LED_PIN, OUTPUT);         // LED interno
  digitalWrite(LED_PIN, LOW);       // asegúrate de que empieza apagado
  Serial.begin(115200);
}

void loop() {
  bool currentReading = digitalRead(BUTTON_PIN);

  if (currentReading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentReading == LOW && buttonState == HIGH) {
      digitalWrite(LED_PIN, HIGH);  // Encender LED
      Serial.println("Botón pulsado, LED encendido");
    }
    buttonState = currentReading;
  }

  lastButtonReading = currentReading;
}


```
