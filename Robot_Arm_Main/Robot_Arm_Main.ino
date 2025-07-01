#include <ESP32Servo.h>
#include <PS4Controller.h>

Servo servo1;  // X-axis
Servo servo2;  // Y-axis
//heelloo

const int servo1Pin = 13;  // GPIO for X-axis servo
const int servo2Pin = 15;  // GPIO for Y-axis servo

int angleX = 90;  // Initial neutral angles
int angleY = 90;

void setup() {
  Serial.begin(115200);

  // Replace with your ESP32 MAC address if needed
  PS4.begin("F4:65:0B:57:2D:22");  
  Serial.println("Waiting for PS4 Controller...");

  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);

  servo1.attach(servo1Pin, 500, 2500);  // 0° to 180°
  servo2.attach(servo2Pin, 500, 2500);

  servo1.write(angleX);
  servo2.write(angleY);
}

void loop() {
  if (PS4.isConnected()) {
    int stickX = PS4.LStickX();  // -128 (left) to +127 (right)
    int stickY = PS4.LStickY();  // -128 (up) to +127 (down)

    // Map to 0–180, inverted Y axis to match natural motion
    angleX = map(stickX, -128, 127, 0, 180);
    angleY = map(stickY, -128, 127, 180, 0);  

    // Limit to servo range just in case
    angleX = constrain(angleX, 0, 180);
    angleY = constrain(angleY, 0, 180);

    servo1.write(angleX);
    servo2.write(angleY);

    Serial.print("X: ");
    Serial.print(angleX);
    Serial.print(" | Y: ");
    Serial.println(angleY);

    delay(50);  // smooth update
  }
}
