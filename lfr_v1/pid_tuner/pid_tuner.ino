#define lc 0 // black 
#define sc 1 //white

/////////////motor pinout/////////////////

#define lm1 3 
#define lm2 5
#define rm1 6
#define rm2 9 

int s1,s2,s3,s4,s5,s6,s7,s8;

/////////////pid parameters////////////////

float error = 0;
float prev_error = 0;
float Kp = 58;
float Kd = 300;  //100/150   //40, 70
float P = 0;
float D = 0;
float pd = 0;

int lms,rms; 
int bs = 200;  

int left_max = 200;
int right_max = 202; //based on motor speed diff
int left_min = 0;
int right_min = 0;

///////////////sharp turn parameters///////////////

int left=0,right=0;
int leftthres = 300;    //150
int rightthres = 300;   //150 

int leftacutethres = 700;
int rightacutethres = 700;


/////////////brake parameters///////////////

int brake_count = 0;
int brake_thres = 400;

void setup() {
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);

}

void loop() {
  readsensors();
  error_map();

  if (s1==lc){
    left = 0;
  }
  if (s8==lc){
    right = 0;
  }

  if (left==0 && right ==0){ //for braking
    if (brake_count>15000){
      brake_count=15000;
    }
    brake_count++;
  }
  else{
    brake_count = 0;
  }

  if ((s1!=lc)&&(s2!=lc)&&(s3!=lc)&&(s4!=lc)&&(s5!=lc)&&(s6!=lc)&&(s7!=lc)&&(s8!=lc)){
    if ((left>leftthres)&&(right<rightthres)){//right sharp turns
    brake();
    //delay(5);
    readsensors();
    while((s4!=lc)&&(s5!=lc)){
      sharp_right_turn();
      readsensors();
    }
    brake();
    //delay(10);
    //left = 1000;
    //right = 1000;
    }
    else if ((left<leftthres)&&(right>rightthres)){//left sharp turn
      brake();
      //delay(5);
      readsensors();
      while((s4!=lc)&&(s5!=lc)){
        sharp_left_turn();
        readsensors();
      }
      brake();
      //left = 1000;
      //right = 1000;
     
    }
  }/*
  else if((s1==lc)&&(s2==lc)&&(s3==lc)&&(s4==lc)&&(s5==lc)&&(s6==lc)&&(s7==lc)&&(s8==lc)){
    if (brake_count>brake_thres){ 
      stop_end();
     }            
  }*/

  pid();
  left=left+1;
  right=right+1;
  if(left>20000){//20000
    left=1000;
  }
  if(right>20000){
    right=1000;
  }
}


//////////////////////sensor get value//////////////////////

void readsensors(){
  s1 = digitalRead(A0);
  s2 = digitalRead(A1);
  s3 = digitalRead(A2);
  s4 = digitalRead(A3);
  s5 = digitalRead(A4);
  s6 = digitalRead(8);
  s7 = digitalRead(2);
  s8 = digitalRead(4);
}

///////////////////////error definition/////////////////////
 
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

///////////////////////pid computation/////////////////////

void pid(){
  readsensors();
  error_map();

  pd = Kp*error + Kd*(error - prev_error);
 
  lms = bs + pd;
  rms = bs - pd;

  if(lms>=left_max)
  {
   lms=left_max;
  }
 
  else if(lms<=left_min)
  {
   lms=left_min;
  }
   
  if(rms>=right_max)
  {
   rms=right_max;
  }
  
  else if(rms<=right_min)
  {
   rms=right_min;
  }

  analogWrite(lm1,lms+2);
  analogWrite(lm2,0);
  analogWrite(rm1,rms);
  analogWrite(rm2,0);
 
  prev_error = error;

}

/////////////////////////intersection handling////////////////////

void dead_end(){
  brake();
  delay(10); 
  while(s4!=lc && s5!=lc){ //take right turn at deadend
    sharp_right_turn();
    readsensors();
  }
  brake();
  //delay(20)
}

void stop_end(){
  brake();
  delay(10);
  delay(10000);
}

////////////////////////////turn functions////////////////////////

void straight(){
  analogWrite(lm1,90);
  analogWrite(lm2,0);
  analogWrite(rm1,90);
  analogWrite(rm2,0);
}

void sharp_left_turn(){ 
  analogWrite(lm1,0);
  analogWrite(lm2,90);
  analogWrite(rm1,90);
  analogWrite(rm2,0);  
}

void sharp_right_turn(){
  analogWrite(lm1,90);//90->90->140
  analogWrite(lm2,0);
  analogWrite(rm1,0);
  analogWrite(rm2,90);  
}


void brake(){
  analogWrite(lm1,255);
  analogWrite(lm2,255);
  analogWrite(rm1,255);
  analogWrite(rm2,255); 
}
