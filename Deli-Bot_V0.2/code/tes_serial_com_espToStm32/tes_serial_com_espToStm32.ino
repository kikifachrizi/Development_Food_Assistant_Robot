void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
}

void loop() {
  Serial2.print('p');
  if(Serial2.available()){
    Serial.println(Serial2.read());
  }
//  delay(100);
//  Serial.write('1');
//  delay(1000);
//  Serial.write('2')
//  delay(1000);
//  Serial.write('3');
//  delay(1000);
//  Serial.write('?');
//  delay(1000);
}
