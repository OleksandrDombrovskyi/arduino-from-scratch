#include "TOF10120.h"

TOF10120 sensor;

//constant
const byte BUILD_IN_LED_PIN = 13;
const byte PWM_PIN = 10;

const int SUBROUTINE_DELAY = 10000;
const int LIGHTING_TIMEOUT = 500;
const int FLASH_TIMEOUT = 100;

//increments
volatile int subroutineInc = 0;
volatile int lightingTimeInc = 0;
volatile int flashTimeInc = 0;

//flashing state
bool isFlashOn = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting...");

  pinMode(BUILD_IN_LED_PIN, OUTPUT);
  digitalWrite(BUILD_IN_LED_PIN, LOW);

  //remove when add PWM
  pinMode(PWM_PIN, OUTPUT);

  //init lazer sensor
  sensor.initSensor();
}


void loop() {
  if (subroutineInc < SUBROUTINE_DELAY) {
    subroutineInc++;
  } else {
    subroutineInc = 0;
    doSubroutine();
  }
}

void doSubroutine() {
  if (sensor.isMovement()) {
    lightingTimeInc = 0;
  }
  
  if (lightingTimeInc < LIGHTING_TIMEOUT) {
    lightingTimeInc++;
    analogWrite(PWM_PIN, 1);
  } else {
    analogWrite(PWM_PIN, 0);
  }

  blinkLed();
  sensor.clk();
}

void blinkLed() {
  if (flashTimeInc < FLASH_TIMEOUT) {
    flashTimeInc++;
  } else {
    flashTimeInc = 0;
    // blink for test
    isFlashOn = !isFlashOn;
    digitalWrite(BUILD_IN_LED_PIN, isFlashOn);
  }
}
