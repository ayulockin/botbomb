int b_detect;

void readsensors_block(){
  b_detect=analogRead(A5);
}

void setup(){
  Serial.begin(9600);
}

void loop(){
  readsensors_block();
  Serial.println(b_detect);
  delay(1000);
}



