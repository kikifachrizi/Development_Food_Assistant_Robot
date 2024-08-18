#define ENCODER_A_1 32 
#define ENCODER_B_1 33 

#define ENCODER_A_2 25 
#define ENCODER_B_2 26 

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

void setup() {
  Serial.begin(115200); 
  pinMode(ENCODER_A_1, INPUT_PULLUP);
  pinMode(ENCODER_B_1, INPUT_PULLUP);
  pinMode(ENCODER_A_2, INPUT_PULLUP);
  pinMode(ENCODER_B_2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_1), encoder_isr_1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_2), encoder_isr_2, CHANGE);
}

void loop() {
  Serial.println("Encoder value 1: " + String(encoder_value_1));
  Serial.println("Encoder value 2: " + String(encoder_value_2));
  delay(100);
}
