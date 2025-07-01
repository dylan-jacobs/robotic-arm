#include <ESP32Servo.h>
#include <PS4Controller.h>

// to get ESP32 MAC ADDRESS
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>

const int SERVO1_PIN = 13;
const int SERVO2_PIN = 15;
const double CONTROLLER_SENSITIVITY = 0.33;
const int DRIFT_THRESHOLD = 5;

Servo servo1;
Servo servo2;
double servo1_pos = 90;
double servo2_pos = 90;

/*
void get_ESP32_MAC1() {
  const uint8_t* address = esp_bt_dev_get_address();
  char str[100];
  sprintf(str, "ESP32 Bluetooth MAC address: %02x:%02x:%02x:%02x:%02x:%02x", address[0], address[1], address[2], address[3], address[4], address[5]);
  Serial.println(str);
}*/

void get_ESP32_MAC() {
  if (!btStarted()) {
    if (!btStart()) {
      Serial.println("Failed to start Bluetooth controller.");
      return;
    }
  }

  if (!esp_bluedroid_get_status()) {
    if (esp_bluedroid_init() != ESP_OK) {
      Serial.println("Failed to init Bluedroid.");
      return;
    }
    if (esp_bluedroid_enable() != ESP_OK) {
      Serial.println("Failed to enable Bluedroid.");
      return;
    }
  }

  const uint8_t* address = esp_bt_dev_get_address();

  if (address == nullptr) {
    Serial.println("Failed to get BT MAC address (still null).");
    return;
  }

  char str[100];
  sprintf(str, "ESP32 Bluetooth MAC address: %02X:%02X:%02X:%02X:%02X:%02X",
          address[0], address[1], address[2],
          address[3], address[4], address[5]);

  Serial.println(str);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  PS4.begin("F4:65:0B:57:2D:22");  // start Bluetooth pairing
  Serial.println("Waiting for PS4 controller...");

  // init servos
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo1.attach(SERVO1_PIN, 500, 2500);
  servo2.attach(SERVO2_PIN, 500, 2500);

  servo1.write(0);
  servo2.write(0);
}

void loop() {
  if (PS4.isConnected()) {
    // Serial.println("PS4 Controller connected!");

    double stickX = PS4.LStickX();
    double stickY = PS4.LStickY();

    if (abs(stickX) >  DRIFT_THRESHOLD) {
      servo1_pos += (stickX*CONTROLLER_SENSITIVITY);
    }
    if (abs(stickY) > DRIFT_THRESHOLD) {
      servo2_pos += (stickY*CONTROLLER_SENSITIVITY);
    }
    servo1_pos = constrain(servo1_pos, 0, 180);
    servo2_pos = constrain(servo2_pos, 0, 180);

    servo1.write(servo1_pos);
    servo2.write(servo2_pos);

    Serial.println(servo1_pos);


    delay(50);  // slow down output
  }
  
}
