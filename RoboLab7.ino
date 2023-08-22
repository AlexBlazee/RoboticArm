#include "Buzzer.h"
#include "ESPMax.h"
#include "_espmax.h"
#include "ESP32PWMServo.h"
#include "SuctionNozzle.h"
#include "LobotSerialServoControl.h"

// Sound sensor placement

#define sensor_pin 32  // Define the pin for the sound sensor

void setup() {
  // Initialize driver libraries
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  PWMServo_init();
  analogReadResolution(10);
  analogSetClockDiv(ADC_11db);
  Serial.begin(115200);
  Serial.println("start...");
  setBuzzer(100);              // Set the buzzer to sound for 100 milliseconds
  go_home(2000);               // Move the robotic arm to the initial position
  SetPWMServo(1, 1500, 2000);  // Set the suction nozzle servo to the initial position
}

void loop() {
  int num = 0;
  float pos[3];
  bool num_st = false;
  long int time_ms = millis();
 
  while (true) {
    float soundValue = analogRead(sensor_pin);
    if (soundValue > 50) {
      if (num == 0 || (millis() - time_ms) < 1000) {
        time_ms = millis();
        delay(80);
        num += 1;
      }
      Serial.println(soundValue);
    }

    if (num > 0 && (millis() - time_ms) > 1500) {
      num_st = true;
      Serial.println(num);
    }

    if (num_st) {    
      setBuzzer(100);  // Set the buzzer to sound for 100ms
      pos[0] = 200;
      pos[1] = -200;
      pos[2] = 85;
      set_position(pos, 3000);  // Move the arm above the colored block
      delay(3000);
      pos[0] = -200;
      pos[1] = -200;
      pos[2] = 85;
      set_position(pos, 3000);  // Engage the suction nozzle to pick up the colored block
      delay(3000);
      go_home(1500);  // Reset the robotic arm to the initial position
      delay(100);
      num_st = false;
      num = 0;
    } else {
      delay(20);
    }
  }
}
