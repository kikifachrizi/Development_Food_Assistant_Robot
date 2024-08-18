#include "MPU9250.h"
#define ENCODER_A_1 32 
#define ENCODER_B_1 33 

#define ENCODER_A_2 25 
#define ENCODER_B_2 26 

volatile int encoder_value_1 = 0; 
volatile int encoder_value_2 = 0; 
float pi = 3.14159265358979323846;
float magx,magy,heading;
float accZ;
int status;

void encoder_isr_1() {
  int A_1 = digitalRead(ENCODER_A_1);
  int B_2 = digitalRead(ENCODER_B_1);

  if ((A_1 == HIGH) != (B_2 == LOW)) {
    encoder_value_1--;
  } else {
    encoder_value_1++;
  }
}

void encoder_isr_2() {
  int A_3 = digitalRead(ENCODER_A_2);
  int B_4 = digitalRead(ENCODER_B_2);

  if ((A_3 == HIGH) != (B_4 == LOW)) {
    encoder_value_2--;
  } else {
    encoder_value_2++;
  }
}


MPU9250 IMU(Wire,0x68);
float accelX,accelY,accelZ;
float yaw,pitch ,roll;

void setup() {
  Serial.begin(115200); 
  Serial2.begin(38400);

  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  
  pinMode(ENCODER_A_1, INPUT_PULLUP);
  pinMode(ENCODER_B_1, INPUT_PULLUP);
  pinMode(ENCODER_A_2, INPUT_PULLUP);
  pinMode(ENCODER_B_2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_1), encoder_isr_1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_2), encoder_isr_2, CHANGE);
}

void loop(){
    IMU.readSensor();
//    Serial.print(IMU.getMagX_uT(),6);
//    Serial.print(" ");
//    Serial.print(IMU.getMagY_uT(),6);
//    Serial.println(" ");
//    Serial.print("Encoder value 1: " + String(encoder_value_1));
//    Serial.print(" ");
//    Serial.print("Encoder value 2: " + String(encoder_value_2));
//    Serial.println(" ");
//    read_ultra();
//    int xAng = map(IMU.getGyroX_rads(), min_, max_,-90,90);

    accelX = IMU.getAccelX_mss();
    accelY = IMU.getAccelY_mss();
    accelZ = IMU.getAccelZ_mss();

    pitch = 180 * atan(-accelX / sqrt(accelY*accelY + accelZ*accelZ))/pi;
    roll  = 180 * atan(accelY / sqrt(accelX*accelX + accelZ*accelZ))/pi;
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    delay(100);
}
