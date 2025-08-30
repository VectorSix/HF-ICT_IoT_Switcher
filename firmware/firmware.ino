/**
*                                           _____________________
*                         _________________/                     \
*                        /                              _        |
*   __             __   /        __                    |_> |     |
*  |  |         __|__| |       |__|              __    |_> |_    /
*  |  |__      |  |__  |        __       ____   |  |__          /
*  |   __|__   |   __|  \___   |  |   __|____|  |   __|        /
*  |  |  |  |  |  |       __|  |  |  |  |       |  |          /
*  |  |  |  |  |  |       \    |  |  |  |       |  |         /
*  |  |  |  |  |  |        \   |  |  |__|____   |__|__      /
*  |__|  |__|  |__|         \  |__|     |____|     |__|    /
*                            \_________                   /
*                                      \_________________/
*
*                   HF-ICT- IoT Switcher - 2025 by Severin Holm 
*/

// ====== CONFIG ZONE ======
// Captive Portal
#define CP_SSID_NAME "⚡ IoT-Switcher Setup ⚡" 

// Servo Config
#define SERVO_PIN 19
#define SERVO_FREQ 50
#define SERVO_RESOLUTION 16
#define SERVO_MIN_US 2400 // 0°
#define SERVO_MAX_US 500  // 180°

// Servo Position Fine tuning
#define SERVO_POS_0 10
#define SERVO_POS_90 115
#define SERVO_POS_180 180

// ====== DO NOT CHANGE ANYTHING FROM HERE ======
#include <Arduino.h> 
#include "webpages.h"
#include "WiFiManagerESP32.h"

// Global objects
WiFiManagerESP32 wifiManager;

// Global states
int currentPosition = SERVO_POS_0;

// ESP Setup Routine
void setup() {
  // Serial Setup
  Serial.begin(115200);
  delay(1000);
  
  // Servo Setup
  servoSetup();
  delay(1000);

  // Wifi Setup
  wifiManager.setSsid(CP_SSID_NAME);
  wifiManager.begin();
  Serial.println("ESP32 Setup Done");
  delay(1000);

  // REST CMD Handler
  restSetup();
  delay(1000);
}

// REST API Setup Routine
void restSetup() {
  wifiManager.setCmdCallback([](WebServer& srv){
    if (srv.hasArg("action")) {
      String action = srv.arg("action");        
      if (action == "pos_0") {
          currentPosition = SERVO_POS_0;
          srv.send(200, "application/json", "{\"status\":\"Position 0\"}");
      } else if (action == "pos_90") {
          currentPosition = SERVO_POS_90;
          srv.send(200, "application/json", "{\"status\":\"Position 90\"}");
      } else if (action == "pos_180") {
          currentPosition = SERVO_POS_180;        
          srv.send(200, "application/json", "{\"status\":\"Position 180\"}");            
      } else {
          srv.send(400, "application/json", "{\"error\":\"Unknown action\"}");
      }
      setServoAngle(currentPosition);
    } else {
        srv.send(400, "application/json", "{\"error\":\"Missing action\"}");
    }
  });  
}

// Servo Setup Routine
void servoSetup() {
  if (!ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RESOLUTION)) {
    Serial.println("LEDC Attach failed!");
  }
  delay(500);
  setServoAngle(currentPosition);
  Serial.println("Servo Setup Done");
  delay(500);
}

// ESP Main Loop
void loop() {
  wifiManager.process();
}

// Native LEDC Servo Driver
void setServoAngle(int angle) {
  int us = map(angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);
  uint32_t duty = (uint32_t)us * ((1 << SERVO_RESOLUTION) - 1) / 20000;
  ledcWrite(SERVO_PIN, duty);
}