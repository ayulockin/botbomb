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

void loop() {
  analogWrite(lm1, 255);
  analogWrite(lm2, 0);
  analogWrite(rm1, 255);
  analogWrite(rm2, 0);

}
