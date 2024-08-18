int x; 
int dirA1 = 11;
int dirA2 = 9;
int pwmA = 10;

int dirB1 = 4;
int dirB2 = 7;
int pwmB = 5;

int ls = 8;

void setup() { 
  Serial.begin(115200); 
  Serial.setTimeout(1); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(dirA1, OUTPUT);
  pinMode(dirA2, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dirB1, OUTPUT);
  pinMode(dirB2, OUTPUT);

  pinMode(ls, INPUT_PULLUP);
} 
void loop() { 
  int data_ = ls.read();
  Serial.println(data_);
//  while (!Serial.available()); 
//  x = Serial.readString().toInt(); 
////  Serial.print(x + 1); 
//  if(x == 1){
////    digitalWrite(LED_BUILTIN, HIGH); 
//    forwardA();   
////      right_();
//  }else if(x == 2){    
////    digitalWrite(LED_BUILTIN, LOW);    
//    backwardA();
////      left_();
//  }else if(x == 0){
//    stop_A();
////      stop_B();
//  }
////  while(Serial.available() > 0){
////    char data_ = Serial.read();
////    if(data_ == '1'){
////      forward();
////    }else if(data_ == '2'){
////      backward();
////    }else if(data_ == '0'){
////      stop_();
////    }
////  }
} 

void forwardA(){
    Serial.println("Maju");
    analogWrite(pwmB, 255);
    digitalWrite(dirB1, HIGH);
    digitalWrite(dirB2, LOW); 
}
void backwardA(){
    Serial.println("Mundur");
    analogWrite(pwmB, 255);
    digitalWrite(dirB1, LOW);
    digitalWrite(dirB2, HIGH); 
}
void stop_A(){
    Serial.println("Stop");
    analogWrite(pwmB, 0);
    digitalWrite(dirB1, LOW);
    digitalWrite(dirB2, LOW); 
}

void stop_B(){
    Serial.println("Stop");
    analogWrite(pwmA, 0);
    digitalWrite(dirA1, LOW);
    digitalWrite(dirA2, LOW); 
}

void right_(){
    Serial.println("buka");
    analogWrite(pwmA, 70);
    digitalWrite(dirA1, HIGH);
    digitalWrite(dirA2, LOW); 
}

void left_(){
    Serial.println("tutup");
    analogWrite(pwmA, 70);
    digitalWrite(dirA1, LOW);
    digitalWrite(dirA2, HIGH); 
}
