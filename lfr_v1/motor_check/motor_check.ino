#define lm1 3 
#define lm2 5
#define rm1 6
#define rm2 9 

void setup() {
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
}

void loop(){
  straight();
}


void straight(){
  analogWrite(lm1,0);
  analogWrite(lm2,130);
  analogWrite(rm1,0);
  analogWrite(rm2,140);
}



