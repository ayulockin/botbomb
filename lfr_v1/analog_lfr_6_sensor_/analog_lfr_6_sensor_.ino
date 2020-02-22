#define lc 0 // black 
#define sc 1 //white

/////////////motor pinout/////////////////

#define lm1 3 
#define lm2 5
#define rm1 6
#define rm2 9

int s2,s3,s4,s5,s6,s7;

/////////////pid parameters////////////////

float error = 0;
float prev_error = 0;
float Kp = 100;
float Kd = 0;  //100/150 
float P = 0;
float D = 0;
float pd = 0;

int lms,rms;
int bs = 130;

int left_max = 200;
int right_max = 202; //based on motor speed diff
int left_min = 0;
int right_min = 0;

///////////////sharp turn parameters///////////////

int left=0,right=0;
int leftthres = 200;    //150
int rightthres = 200;   //150

int leftacutethres = 700;
int rightacutethres = 700;

int sum=0;
float avg=0;


/////////////brake parameters///////////////

int brake_count = 0;
int brake_thres = 400;

void setup() {
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  left = 1000;
  right = 1000;
}

void loop() {
  pid();
  /*
  analogtodigital();
  error_map();

  if (s2==lc){
    left = 0;
  }
  if (s7==lc){
    right = 0;
  }

  if((s3==lc)&&(s4==lc)&&(s5==lc)&&(s6==lc)){     //intersection blink
     digitalWrite(LED_BUILTIN,HIGH);
  }
  else{
     digitalWrite(LED_BUILTIN,LOW);
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

  if ((s2!=lc)&&(s3!=lc)&&(s4!=lc)&&(s5!=lc)&&(s6!=lc)&&(s7!=lc)){
    if ((left>leftthres)&&(right<rightthres)){//right sharp turns
    brake();
    delay(5);
    analogtodigital();
    while((s4!=lc)&&(s5!=lc)){
      sharp_right_turn();
      analogtodigital();
    }
    brake();
    //delay(10);
    //left = 1000;
    //right = 1000;
    }
    else if ((left<leftthres)&&(right>rightthres)){//left sharp turn
      brake();
      delay(5);
      analogtodigital();
      while((s4!=lc)&&(s5!=lc)){
        sharp_left_turn();
        analogtodigital();
      }
      brake();
      //delay(10);
      //left = 1000;
      //right = 1000;
    }
    else if (left<leftacutethres && right>rightacutethres){//accute left turn
      brake();
      delay(5);
      analogtodigital();
      while((s4!=lc)&&(s5!=lc)){
        sharp_accute_left_turn();
        analogtodigital();
      }
      brake();
      //delay(10);

      //left = 500;
      //right = 500;
    }
    else if (left>leftacutethres && right<rightacutethres){//accute right turn
      brake();
      delay(5);
      analogtodigital(); 
      while((s4!=lc)&&(s5!=lc)){
        sharp_accute_right_turn();
        analogtodigital();
     }
     brake();
     //delay(10);
    }
    else if(left<200 && right<200){T_intersection();}

    else if(left>1000 && right>1000){dead_end();}
  }
  else if((s2!=lc)&&((s3==lc)||(s4==lc)||(s5==lc)||(s6==lc))&&(s7!=lc)){
    if (left>100 && left<1000 && right>1000){  //straight or left -> left
        left_T();
    }                              
    else if (left>1000 && right<1000 && right>100){   //right_T(); straight or right -> straight
        right_T();
        //straight();
    }
    else if(left<100 && right<100){   //cross -> left
        cross();
    }
  }
  else if((s2==lc)&&(s3==lc)&&(s4==lc)&&(s5==lc)&&(s6==lc)&&(s7==lc)){
    if (brake_count>brake_thres){  //end of maze
        stop_end();
     }            
  }

  pid();
  left=left+1;
  right=right+1;
  if(left>20000){//20000
    left=1000;
  }
  if(right>20000){
    right=1000;
  }*/
}


//////////////////////sensor get value//////////////////////

void readsensors(){
  s2 = analogRead(A0);
  s3 = analogRead(A1);
  s4 = analogRead(A2);
  s5 = analogRead(A3);
  s6 = analogRead(A4);
  s7 = analogRead(A5); 
}

void calculate(){
  sum = s2+s3+s4+s5+s6+s7;
  avg = sum/6;
}

void analogtodigital(){
  readsensors();
  calculate();
  
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
///////////////////////error definition/////////////////////
 
void error_map(){
  if((s3==sc)&&(s4==lc)||(s5==lc)&&(s6==sc))
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
   
  else if((s2==lc)&&(s3==lc)&&(s4==sc))
   error=-2;
  else if((s5==sc)&&(s6==lc)&&(s7==lc))
   error=2;
      
  if((s2==lc)&&(s2==sc)&&(s7==sc))
   error=-2.5; 
  if((s2==sc)&&(s7==sc)&&(s7==lc))
   error=2.5;
}


///////////////////////pid computation/////////////////////

void pid(){
  analogtodigital();
  error_map();

  pd = Kp*error + Kd*(error - prev_error);
 
  lms = bs - pd;
  rms = bs + pd;

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

void T_intersection(){
  brake();
  delay(30);
  while((s4!=lc)&&(s5!=lc)){                 // 'T' -> left turn
    sharp_left_turn();
    analogtodigital();
  }
  brake();
  //delay(20);
  //left = 1000;
  //right = 1000;
}

void left_T(){
  brake();
  delay(10);
  sharp_left_turn();
  delay(40);             //to be tuned
  while(s4!=lc && s5!=lc){ 
    sharp_left_turn();
    analogtodigital();
  }
  brake();
  //delay(20);
  
  left = 1000;
  right = 1000;
}

void right_T(){
  brake();
  delay(10);
  sharp_right_turn();
  delay(40);             //to be tuned
  while(s4!=lc && s5!=lc){ 
    sharp_right_turn();
    analogtodigital();
  }
  brake();
  //delay(20);
  
  left = 1000;
  right = 1000;
}

void cross(){
  brake();
  delay(10);
  sharp_right_turn();
  //sharp_left_turn();
  delay(40);         //to be tuned
  /*while(s4!=lc && s5!=lc){ //left on cross
    sharp_left_turn();
    analogtodigital();
  }*/
  while(s4!=lc && s5!=lc){ //right on cross
    sharp_right_turn();
    analogtodigital();
  }
  brake();
  //delay(20);
  
  left = 1000;
  right = 1000;
}

void dead_end(){
  brake();
  delay(10); 
  while(s4!=lc && s5!=lc){ //take right turn at deadend
    sharp_right_turn();
    analogtodigital();
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
  analogWrite(lm1,127);
  analogWrite(lm2,0);
  analogWrite(rm1,127);
  analogWrite(rm2,0);
}

void sharp_left_turn(){ 
  analogWrite(lm1,0);
  analogWrite(lm2,127);
  analogWrite(rm1,127);
  analogWrite(rm2,0);  
}

void sharp_right_turn(){
  analogWrite(lm1,127);//90->127->140
  analogWrite(lm2,0);
  analogWrite(rm1,0);
  analogWrite(rm2,127);  
}

void sharp_accute_left_turn(){
  analogWrite(lm1,0);
  analogWrite(lm2,127);
  analogWrite(rm1,127);
  analogWrite(rm2,0);  
}

void sharp_accute_right_turn(){
  analogWrite(lm1,127);
  analogWrite(lm2,0);
  analogWrite(rm1,0);
  analogWrite(rm2,127);  
}

void about_turn(){
  analogWrite(lm1,0);
  analogWrite(lm2,127);
  analogWrite(rm1,127);
  analogWrite(rm2,0);  
}

void brake(){
  analogWrite(lm1,255);
  analogWrite(lm2,255);
  analogWrite(rm1,255);
  analogWrite(rm2,255); 
}
