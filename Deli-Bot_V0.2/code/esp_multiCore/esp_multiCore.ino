#include "MPU9250.h"
#include <PS4Controller.h>
#define ENCODER_A_1 32 
#define ENCODER_B_1 33 

#define ENCODER_A_2 25 
#define ENCODER_B_2 26 

#define PPR_ENC 1200


#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

volatile int encoder_value_1 = 0; 
volatile int encoder_value_2 = 0; 

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

const int trigPin = 5;
const int echoPin = 18;

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

bool lStickX1 = false;
bool lStickX2 = false;
bool lStickY1 = false;
bool lStickY2 = false;
bool rStickX1 = false;
bool rStickX2 = false;
bool rStickY1 = false;
bool rStickY2 = false;
bool upStick = false;
bool downStick = false;
bool rightStick = false;
bool leftStick = false;
bool triangleStick = false;
bool crossStick = false;
bool circleStick = false;
bool squareStick = false;
bool shareStick = false;
bool optionStick = false;
bool l1State = false;
bool r1State = false;

TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
const int led1 = 2;
const int led2 = 4;

MPU9250 IMU(Wire,0x68);
int status;

void read_ultra(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  distanceInch = distanceCm * CM_TO_INCH;
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
}

void setup() {
  Serial.begin(115200); 
  Serial2.begin(115200);
  PS4.begin("94:E6:86:38:07:50");
//  SerialBT.begin("ESP32test"); //Bluetooth device name
//  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

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
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    IMU.readSensor();
    Serial.print(IMU.getGyroX_rads(),6);
    Serial.print(" ");
    Serial.print(IMU.getGyroY_rads(),6);
    Serial.print(" ");
    Serial.print(IMU.getGyroZ_rads(),6);
//    Serial.print(" ");
//    Serial.print("Encoder value 1: " + String(encoder_value_1));
//    Serial.print(" ");
//    Serial.print("Encoder value 2: " + String(encoder_value_2));
    Serial.println(" ");

//    float dist_trvld1 = (((2*3.14*2.5)/PPR_ENC) * encoder_value_1);
//    float dist_trvld2 = (((2*3.14*2.5)/PPR_ENC) * encoder_value_2); //6.00 adalah nilai offside
//    Serial.print(" ");
//    Serial.print("Encoder value 1: " + String(encoder_value_1));
//    Serial.print(" ");
//    Serial.print("Encoder value 2: " + String(encoder_value_2));
//    Serial.println(" ");
//    SerialBT.print("enc1 : ");
//    SerialBT.print(encoder_value_1);
//    SerialBT.print(" ; ");
//    SerialBT.print("enc2 : ");
//    SerialBT.print(encoder_value_2);
//    SerialBT.print(" ; ");
//    SerialBT.print("distance travelled 1: ");
//    SerialBT.print(dist_trvld1);
//    SerialBT.print(" ; ");
//    SerialBT.print("distance travelled 2: ");
//    SerialBT.print(dist_trvld2);
//    SerialBT.println(" ; ");

    
//    read_ultra();
    delay(100);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    if (PS4.isConnected()) {
      //tombol
      if (PS4.Right()){
        rightStick = true;
        Serial2.write('D');
        delay(10);
      }else{
        rightStick = false;
      }
      
      if (PS4.Down()){
        downStick = true;
        Serial2.write('B');
        delay(10);
      }else{
        downStick = false;
      }
      
      if (PS4.Up()){
        upStick = true;
        Serial2.write('A');
        delay(10);
      }else{
        upStick = false;
      }
      
      if (PS4.Left()){
        leftStick = true;
        Serial2.write('C');
        delay(10);
      }else{
        leftStick = false;
      }
  
      if (PS4.Square()){
        squareStick = true;
        Serial2.write('L');
        delay(10);
      }else{
        squareStick = false;
      }
      if (PS4.Cross()){
        crossStick = true;
        Serial2.write('K');
        delay(10);
      }else{
        crossStick = false;
      }
      if (PS4.Circle()){
        circleStick = true;
        Serial2.write('M');
        delay(10);
      }else{
        circleStick = false;
      }
      if (PS4.Triangle()){
        triangleStick = true;
        Serial2.write('J');
        delay(10);
      }else{
        triangleStick = false;
      }
  
      if (PS4.Share()) {
        shareStick = true;
        Serial2.write('I');
        delay(10);
      }else{
        shareStick = false;
      }
      if (PS4.Options()) {
        optionStick = true;
        Serial2.write('Q');
        delay(10);
      }else{
        optionStick = false;
      }
  
      
      //analog kiri
      if (PS4.LStickX()) {
          if(PS4.LStickX() < -110){
              lStickX2 = true;
              Serial2.write('U');
//              Serial.println('U');
              delay(10);
          }else if(PS4.LStickX() > 110){
              lStickX1 = true;
              Serial2.write('T');
//              Serial.println('T');
              delay(10);
          }  
      }else{
        lStickX1 = false;lStickX2 = false;lStickY1 = false;lStickY2 = false;  
      }
      if (PS4.LStickY()) {
          if(PS4.LStickY() < -110){
              lStickY1 = true;
              Serial2.write('S');
//              Serial.println('S');
              delay(10);
          }else if(PS4.LStickY() > 110){
              lStickY2 = true;
              Serial2.write('R');
//              Serial.println('R');
              delay(10);
          }
      }else{
        lStickY1 = false;lStickY2 = false;lStickX1 = false;lStickX2 = false;    
      }
  
      //analog kanan
          if (PS4.RStickX()) {
          if(PS4.RStickX() < -110){
              rStickX2 = true;
              Serial2.write('3');
              delay(10);
          }else if(PS4.RStickX() > 110){
              rStickX1 = true;
              Serial2.write('4');
              delay(10);
          }
      }else{
        rStickX1 = false;rStickX2 = false;rStickY1 = false;rStickY2 = false;  
      }
      if (PS4.RStickY()) {
          if(PS4.RStickY() < -110){
              rStickY1 = true;
              Serial2.write('2');
              delay(10);
          }else if(PS4.RStickY() > 110){
              rStickY2 = true;
              Serial2.write('1');
              delay(10);
          }
      }else{
        rStickY1 = false;rStickY2 = false;rStickX1 = false;rStickX2 = false;    
      }
  
    if(l1State == false && r1State == false && rStickX1 == false && rStickX2 == false && rStickY1 == false && rStickY2 == false && lStickX1 == false && lStickX2 == false  && lStickY1 == false && lStickY2 == false && upStick == false && downStick == false && rightStick == false && leftStick == false && triangleStick == false && crossStick == false && squareStick == false && circleStick == false && optionStick == false && shareStick == false){
      Serial2.write('X');  
//      Serial.println('X');
      delay(10);
    }
    }
  }
}

void loop() {
  
}
