#include "BluetoothSerial.h"

// Bluetooth parameters
BluetoothSerial ESP_BT;

// Pins for LED control
int led_pin_1 = 4;
int led_pin_2 = 0;
int led_pin_3 = 2; 

// Motor control pins
#define enA 13
#define in1 14
#define in2 27
#define in3 26
#define in4 25
#define enB 12

int incoming;
int xAxis = 140, yAxis = 140, aAxis = 140;
int motorSpeedA = 0;
int motorSpeedB = 0;

// Function prototypes
void forward();
void backward();
void turnRight();
void turnLeft();
void Stop();

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Bluetooth setup
  ESP_BT.begin("ESP32_BT");

  // LED pin setup
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  pinMode(led_pin_3, OUTPUT);

  // Motor pin setup
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // Handle Bluetooth data
  if (ESP_BT.available() >= 2) {
    xAxis = ESP_BT.read();
    delay(10);
    yAxis = ESP_BT.read();
    Serial.print(" ");
    Serial.print(xAxis);
    Serial.print("; ");
    Serial.println(yAxis);
  }
  delay(10);

  // Motor control based on received Bluetooth data
  if (xAxis > 130 && xAxis < 150 && yAxis > 130 && yAxis < 150) {
    Stop();
  } else if (yAxis > 130 && yAxis < 150) {
    if (xAxis < 130) {
      turnRight();
      motorSpeedA = map(xAxis, 130, 60, 0, 255);
      motorSpeedB = map(xAxis, 130, 60, 0, 255);
    }
    if (xAxis > 150) {
      turnLeft();
      motorSpeedA = map(xAxis, 150, 220, 0, 255);
      motorSpeedB = map(xAxis, 150, 220, 0, 255);
    }
  } else {
    if (xAxis > 130 && xAxis < 150) {
      if (yAxis < 130) {
        forward();
      }
      if (yAxis > 150) {
        backward();
      }
      if (yAxis < 130) {
        motorSpeedA = map(yAxis, 130, 60, 0, 255);
        motorSpeedB = map(yAxis, 130, 60, 0, 255);
      }
      if (yAxis > 150) {
        motorSpeedA = map(yAxis, 150, 220, 0, 255);
        motorSpeedB = map(yAxis, 150, 220, 0, 255);
      }
    } else {
      if (yAxis < 130) {
        forward();
      }
      if (yAxis > 150) {
        backward();
      }
      if (xAxis < 130) {
        motorSpeedA = map(xAxis, 130, 60, 255, 50);
        motorSpeedB = 255;
      }
      if (xAxis > 150) {
        motorSpeedA = 255;
        motorSpeedB = map(xAxis, 150, 220, 255, 50);
      }
    }
  }


  if (ESP_BT.available()) {
    incoming = ESP_BT.read(); // Read what we receive 

    // Print incoming value to serial monitor for button controls
    if (incoming == 10 || incoming == 20 || incoming == 30 || incoming == 21 || incoming == 31 || incoming == 1) {
    if (incoming == 10)
    Serial.println(1);
    if (incoming == 20)
    Serial.println(2);
    if (incoming == 30)
    Serial.println(3);
    }

    // Handle LED control based on incoming value
    switch (incoming) {
      case 1:  
        digitalWrite(led_pin_1, 1);
        break;
      case 20:  
        digitalWrite(led_pin_2, 0);
        break;
      case 21:  
        digitalWrite(led_pin_2, 1);
        break;
      case 30:  
        digitalWrite(led_pin_3, 0);
        break;
      case 31:  
        digitalWrite(led_pin_3, 1);
        break;
    }
  }
}

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
