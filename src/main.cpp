#include <Arduino.h>

// definición de los pines

#define LED_PIN LED_BUILTIN
#define BTN_PIN D2

// Definición constante de tiempo debounce

const unsigned long debounceDelay = 50; // 50 milisegundos

// Definición de variables

unsigned long lastDebounceTime = 0; 

bool buttonState = HIGH;
bool lastButtonReading = HIGH; 

void setup() {
  // Comunicación serie
  Serial.begin(115200);
  // Configuración de los pines
  pinMode(BTN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  // Estado inicial del LED --> apagado
  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
  bool currentButtonReading = digitalRead(BTN_PIN); // Lectura del botón con cada búcle

  if (currentButtonReading != lastButtonReading) // Si el estado del botón cambia 
  {
    lastDebounceTime = millis(); // la variable lastDebounceTime toma t1 de pulsar el botón
  }

  if ((millis()- lastDebounceTime) > debounceDelay) // si el tiempo que se ha pulsado el botón es superior a 50 s.
  {
    if (currentButtonReading == LOW && buttonState == HIGH) // Se pulsa el botón (antes estaba sin pulsar)
    {
      digitalWrite(LED_PIN, HIGH);
    }
    
    buttonState = currentButtonReading;
    
  }
  
  lastButtonReading = currentButtonReading;
  
}

