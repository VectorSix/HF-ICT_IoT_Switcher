/**
*                                          ____________________
*                        _________________/                    \
*                       /                              _       |
*  __             __   /        __                    |_> |    |
* |  |         __|__| |       |__|              __    |_> |_   /
* |  |__      |  |__  |        __       ____   |  |__         /
* |   __|__   |   __|  \___   |  |   __|____|  |   __|       /
* |  |  |  |  |  |       __|  |  |  |  |       |  |         /
* |  |  |  |  |  |       \    |  |  |  |       |  |        /
* |  |  |  |  |  |        \   |  |  |__|____   |__|__     /
* |__|  |__|  |__|         \  |__|     |____|     |__|   /
*                           \_________                  /
*                                     \________________/
*
*                   HF-ICT- IoT Switcher - 2025 by Severin Holm 
*/

// ====== CONFIG ZONE ======
// Captive Portal
#define CP_SSID_NAME "⚡ IoT-Switcher Setup ⚡" 

// Blynk Credentials
#define BLYNK_TEMPLATE_ID   "TMPL45WOt0r58"
#define BLYNK_TEMPLATE_NAME "IoT Switcher"
#define BLYNK_AUTH_TOKEN    "b1UHtQmqQdbm-cdk29CJCBzoRvUzoHWk"

// Blynk Servo Config
#define VP_SERVO_CONTROL  V1
#define VP_SERVO_FEEDBACK V2

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
#include <BlynkSimpleEsp32.h>
#include "webpages.h"
#include "WiFiManagerESP32.h"

// Global objects
WiFiManagerESP32 wifiManager;

// Global states
bool isBlynkSetup = false;
int currentPosition = SERVO_POS_0;
char auth[] = BLYNK_AUTH_TOKEN;

// ESP Setup Routine
void setup() {
  Serial.begin(115200);
  delay(1000);
  wifiManager.setSsid(CP_SSID_NAME);
  wifiManager.begin();
  Serial.println("ESP32 Setup Done");
  servoSetup();
}

// Blynk Setup Routine
void blynkSetup() {
  isBlynkSetup = true;  
  Blynk.config(auth);
  Serial.println("Blynk Setup Done");
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
  if (!wifiManager.isConnected()) wifiManager.process();
  else {
    // Blynk Setup
    if(!isBlynkSetup) { 
      blynkSetup(); 
    }

    // Blynk Loop
    if (!Blynk.connected()) { 
      Blynk.connect(5000); 
    }
    Blynk.run();
  }
}

// Native LEDC Servo Driver
void setServoAngle(int angle) {
  int us = map(angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);
  uint32_t duty = (uint32_t)us * ((1 << SERVO_RESOLUTION) - 1) / 20000;
  ledcWrite(SERVO_PIN, duty);
}

void smoothMove(int startAngle, int endAngle, int angleStep) {
  if (startAngle < endAngle) {
    for (int angle = startAngle; angle <= endAngle; angle += angleStep) {
      setServoAngle(angle);
      delay(20);
    }
  } else {
    for (int angle = startAngle; angle >= endAngle; angle -= angleStep) {
      setServoAngle(angle);
      delay(20);
    }
  }
}

// Blynk Hooks: Blynk Connected
BLYNK_CONNECTED() {
  Serial.println("Connected to Blynk!");
  Blynk.virtualWrite(VP_SERVO_FEEDBACK, currentPosition);
}

// Blynk Hooks: Pin handler
BLYNK_WRITE(VP_SERVO_CONTROL) {
  int value = param.asInt();
  Serial.print("Blynk Command incomming: "); Serial.println(value);
  switch(value) {
    case 0: currentPosition = SERVO_POS_0; break;
    case 1: currentPosition = SERVO_POS_90; break;
    case 2: currentPosition = SERVO_POS_180; break;
    default: return;
  }
  setServoAngle(currentPosition);
  Blynk.virtualWrite(VP_SERVO_FEEDBACK, currentPosition);
  delay(500);  
}