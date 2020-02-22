#define lc 0 // black 
#define sc 1 //white

int s1, s2, s3, s4, s5, s6, s7, s8;

float error = 0;


void readsensors() {
  s1 = digitalRead(A0);
  s2 = digitalRead(A1);
  s3 = digitalRead(A2);
  s4 = digitalRead(A3);
  s5 = digitalRead(A4);
  s6 = digitalRead(8);
  s7 = digitalRead(2);
  s8 = digitalRead(4);
}

void setup(){
  Serial.begin(9600);
}

void loop(){
  readsensors();
  error_map();
  
 // calculate();
  Serial.print(s1);
  Serial.print("\t");
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
  //Serial.print(avg);
  Serial.print(s8);
  Serial.print("\t");
  Serial.println("\n");
  
  /*
  Serial.println(error);
  Serial.print(" ");*/
  delay(1000);
}


void error_map(){ 
  if((s3==sc)&&(s4==lc)&&(s5==lc)&&(s6==sc))
   error=0;
   else if((s3==lc)&&(s4==lc)&&(s5==lc)&&(s6==sc))
   error=-0.5;
  else if((s3==sc)&&(s4==lc)&&(s5==sc)&&(s6==sc))
   error=-0.5;
  else if((s3==sc)&&(s4==lc)&&(s5==lc)&&(s6==lc))
   error=0.5;
  else if((s3==sc)&&(s4==sc)&&(s5==lc)&&(s6==sc))
   error=0.5;
  
  else if((s2==sc)&&(s3==lc)&&(s4==lc)&&(s5==sc))
   error=-1; 
  else if((s4==sc)&&(s5==lc)&&(s6==lc)&&(s7==sc))
   error=1;
  
  else if((s2==lc)&&(s3==lc)&&(s4==lc)&&(s5==sc))
   error=-1.5;
  else if((s2==sc)&&(s3==lc)&&(s4==sc)&&(s5==sc))
   error=-1.5; 
  else if((s4==sc)&&(s5==lc)&&(s6==lc)&&(s7==lc))
   error=1.5;
  else if((s4==sc)&&(s5==sc)&&(s6==lc)&&(s7==sc))
   error=1.5;
   
  else if((s1==sc)&&(s2==lc)&&(s3==lc)&&(s4==sc))
   error=-2;
  else if((s5==sc)&&(s6==lc)&&(s7==lc)&&(s8==sc))
   error=2;
  
  else if((s1==lc)&&(s2==lc)&&(s3==lc)&&(s4==sc))
   error=-2.5;
  else if((s1==sc)&&(s2==lc)&&(s3==sc)&&(s4==sc))
   error=-2.5;
  else if((s5==sc)&&(s6==lc)&&(s7==lc)&&(s8==lc))
   error=2.5;
  else if((s5==sc)&&(s6==sc)&&(s7==lc)&&(s8==sc))
   error=2.5;
   
  else if((s1==lc)&&(s2==lc)&&(s3==sc))
   error=-3;
  else if((s6==sc)&&(s7==lc)&&(s8==lc))
   error=3;
      
  if((s1==lc)&&(s2==sc)&&(s8==sc))
   error=-3.5; 
  if((s1==sc)&&(s7==sc)&&(s8==lc))
   error=3.5;
}







