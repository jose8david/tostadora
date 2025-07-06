#include <Arduino.h>

// Pines
#define BTN_ONOFF      D4
#define BTN_SET        D1
#define LIMIT_INFERIOR D3
#define LIMIT_SUPERIOR D2

#define MOTOR_PIN      D8
#define LED_VERDE      D5
#define LED_AMARILLO   D6
#define LED_ROJO       D7

// Debounce
unsigned long lastBtnTime = 0;
const unsigned long debounceDelay = 50;

// Duraciones
const unsigned long TIME_SHORT = 20000;
const unsigned long TIME_MEDIUM = 30000;
const unsigned long TIME_LONG = 40000;
unsigned long toastingTime = TIME_MEDIUM;  // Valor por defecto

// Estado de programa
enum Estado {ESPERA, BAJANDO, TOSTANDO, SUBIENDO, FINALIZADO};
Estado estado = ESPERA;

// Última opción de duración (0 = 20s, 1 = 30s, 2 = 40s)
int tiempoIndex = 1;

void apagarTodo() {
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARILLO, LOW);
  digitalWrite(LED_ROJO, LOW);
}


void mostrarLED() {
  digitalWrite(LED_VERDE, tiempoIndex == 0 ? HIGH : LOW);
  digitalWrite(LED_AMARILLO, tiempoIndex == 1 ? HIGH : LOW);
  digitalWrite(LED_ROJO, tiempoIndex == 2 ? HIGH : LOW);
}


void setup() {
  Serial.begin(115200);

  // Configuración de pines
  pinMode(BTN_ONOFF, INPUT);
  pinMode(BTN_SET, INPUT);
  pinMode(LIMIT_INFERIOR, INPUT);
  pinMode(LIMIT_SUPERIOR, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);

  apagarTodo();
  Serial.println("Tostadora lista. Esperando botón ON/OFF...");
}

void loop() {
  unsigned long tiempoActual = millis();

  // Botón SET para elegir tiempo de tostado
  if (digitalRead(BTN_SET) == LOW && (tiempoActual - lastBtnTime > debounceDelay)) {
    tiempoIndex = (tiempoIndex + 1) % 3;  // ciclo entre 0, 1, 2
    lastBtnTime = tiempoActual;

    switch (tiempoIndex) {
      case 0: toastingTime = TIME_SHORT; break;
      case 1: toastingTime = TIME_MEDIUM; break;
      case 2: toastingTime = TIME_LONG; break;
    }

    mostrarLED();
    Serial.print("Tiempo de tostado seleccionado: ");
    Serial.println(toastingTime / 1000);
  }

  switch (estado) {
    case ESPERA:
      if (digitalRead(BTN_ONOFF) == LOW) {
        estado = BAJANDO;
        Serial.println("Iniciando bajada...");
        digitalWrite(MOTOR_PIN, HIGH);
        delay(200);  // estabilizar motor
      }
      break;

    case BAJANDO:
      if (digitalRead(LIMIT_INFERIOR) == LOW) {
        digitalWrite(MOTOR_PIN, LOW);
        estado = TOSTANDO;
        Serial.println("Bandeja abajo. Iniciando tostado...");
        delay(200);  // estabilizar relé
        lastBtnTime = tiempoActual;
      }
      break;

    case TOSTANDO:
      if ((tiempoActual - lastBtnTime) >= toastingTime) {
        estado = SUBIENDO;
        Serial.println("Tostado completo. Subiendo bandeja...");
        digitalWrite(MOTOR_PIN, HIGH);
        delay(200);
      }
      break;

    case SUBIENDO:
      if (digitalRead(LIMIT_SUPERIOR) == LOW) {
        digitalWrite(MOTOR_PIN, LOW);
        estado = FINALIZADO;
        Serial.println("Proceso terminado. Bandeja arriba.");
      }
      break;

    case FINALIZADO:
      if (digitalRead(BTN_ONOFF) == LOW) {
        estado = BAJANDO;
        Serial.println("Reiniciando proceso...");
        digitalWrite(MOTOR_PIN, HIGH);
        delay(200);
      }
      break;
  }
}



