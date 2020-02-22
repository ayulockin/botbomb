#define lc 1 // black 
#define sc 0 //white

int s1,s2,s3,s4,s5,s6,s7,s8;
int sum=0;
float avg = 0.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //analogtodigital();
  readsensors();
  calculate();
  //Serial.print(s1);
  //Serial.print("\t");
  Serial.print(s2);
  Serial.print("\t");
  Serial.print(s3); 
  Serial.print("\t");
  Serial.print(s4);
  Serial.print("\t");
  Serial.print(s5);
  Serial.print("\t");
  Serial.print(s6);
  Serial.print("\t");
  Serial.print(s7);
  Serial.print("\t");
  Serial.print(avg);
  //Serial.print(s8);
  //Serial.print("\t");
  Serial.println("\n");
  delay(1000);
}

void calculate(){
  sum = 10000*s2+1000*s3+100*s4-100*s5-1000*s6-10000*s7;
  avg = sum/6;
}

void readsensors(){
  //s1 = analogRead(A0);
  s2 = analogRead(A0);
  s3 = analogRead(A1);
  s4 = analogRead(A2);
  s5 = analogRead(A3);
  s6 = analogRead(A4);
  s7 = analogRead(A5);
  //s8 = analogRead(7);
}

void analogtodigital(){
  readsensors();
  calculate();
/*
  if(s2>avg){s2=1;}
  else if(s2<avg){s2=0;}
  if(s3>avg){s3=1;}
  else if(s3<avg){s3=0;}
  if(s4>avg){s4=1;}
  else if(s4<avg){s4=0;}
  if(s5>avg){s5=1;}
  else if(s5<avg){s5=0;}
  if(s6>avg){s6=1;}
  else if(s6<avg){s6=0;}
  if(s7>avg){s7=1;}
  else if(s7<avg){s7=0;}
  */
  if(s2>avg){s2=1;}
  else{s2=0;}
  if(s3>avg){s3=1;}
  else{s3=0;}
  if(s4>avg){s4=1;}
  else{s4=0;}
  if(s5>avg){s5=1;}
  else{s5=0;}
  if(s6>avg){s6=1;}
  else{s6=0;}
  if(s7>avg){s7=1;}
  else{s7=0;}
  
}



