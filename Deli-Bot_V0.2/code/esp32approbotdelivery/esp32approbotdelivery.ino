#include "BluetoothSerial.h"

// Bluetooth parameters
BluetoothSerial ESP_BT;

// Pins for LED control
int led_pin_1 = 4;
int led_pin_2 = 0;
int led_pin_3 = 2;
int led_pin_4 = 16; // Define pin for LED 4
int led_pin_5 = 17; // Define pin for LED 5
int led_pin_6 = 18; // Define pin for LED 6

int incoming;

// Function prototypes
void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Bluetooth setup
  ESP_BT.begin("ESP32_BT_Slave");

  // LED pin setup
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  pinMode(led_pin_3, OUTPUT);
  pinMode(led_pin_4, OUTPUT); // Setup pin for LED 4
  pinMode(led_pin_5, OUTPUT); // Setup pin for LED 5
  pinMode(led_pin_6, OUTPUT); // Setup pin for LED 6
}

void loop() {
  if (ESP_BT.available()) {
    incoming = ESP_BT.read(); // Read what we receive 

    // Print incoming value to serial monitor for button controls
    if (incoming == 10 || incoming == 20 || incoming == 30 || incoming == 21 || incoming == 31 || incoming == 1 || 
        incoming == 40 || incoming == 41 || incoming == 50 || incoming == 51 || incoming == 60 || incoming == 61) {
      if (incoming == 10) Serial.println(1);
      if (incoming == 20) Serial.println(2);
      if (incoming == 30) Serial.println(3);
      if (incoming == 40) Serial.println(4);
      if (incoming == 50) Serial.println(5);
      if (incoming == 60) Serial.println(6);
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
      case 40:  
        digitalWrite(led_pin_4, 0);
        break;
      case 41:  
        digitalWrite(led_pin_4, 1);
        break;
      case 50:  
        digitalWrite(led_pin_5, 0);
        break;
      case 51:  
        digitalWrite(led_pin_5, 1);
        break;
      case 60:  
        digitalWrite(led_pin_6, 0);
        break;
      case 61:  
        digitalWrite(led_pin_6, 1);
        break;
    }
  }
}
