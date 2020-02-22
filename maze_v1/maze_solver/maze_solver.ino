#include<EEPROM.h>

#define lc 0 // black  
#define sc 1 //white

/////////////motor pinout/////////////////

#define lm1 3
#define lm2 5
#define rm1 6
#define rm2 9

int s1, s2, s3, s4, s5, s6, s7, s8;

/////////////pid parameters////////////////

float error = 0;
float prev_error = 0;
float Kp = 37.5;
float Kd = 75;  //100/150
float P = 0;
float D = 0;
float pd = 0;

int lms, rms;
int bs = 130;

int left_max = 200;
int right_max = 202; //based on motor speed diff
int left_min = 0;
int right_min = 0;

///////////////sharp turn parameters///////////////

int left = 0, right = 0;
int leftthres = 400;    //150
int rightthres = 400;   //150

int leftacutethres = 700;
int rightacutethres = 700;

//int overshoot_delay = 100; //80

/////////////brake parameters///////////////

int brake_count = 0;
int brake_thres = 200;

/////////////////maze parameters/////////////

char path_new[3];
int pathlength = 0;
int pathlen = 0;
int no_of_steps = 100;

bool state = false;
bool flag = false; 

struct mazepath {                   //data saved on eeprom
  char eepath[100] = "";            //optimized path array
  int pathl = 0;                    //total path length
  char path[100] = "";          //path stored using LSRB and 
                                    //simplified path 
};

mazepath maze;
int eeAddress = 0;

void setup() {
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);

  pinMode(10,OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  left = 1000;
  right = 1000;

  Serial.begin(9600);
}

//////////////////////sensor get value//////////////////////

void readsensors() {
  s1 = digitalRead(A0);
  s2 = digitalRead(A1);
  s3 = digitalRead(A2);
  s4 = digitalRead(A3);
  s5 = digitalRead(A4);
  s6 = digitalRead(A5);
  s7 = digitalRead(2);
  s8 = digitalRead(7);
}

///////////////////////error definition/////////////////////

void error_map() {
  if ((s3 == sc) && (s4 == lc) && (s5 == lc) && (s6 == sc))
    error = 0;
  else if ((s3 == lc) && (s4 == lc) && (s5 == lc) && (s6 == sc))
    error = -0.5;
  else if ((s3 == sc) && (s4 == lc) && (s5 == sc) && (s6 == sc))
    error = -0.5;
  else if ((s3 == sc) && (s4 == lc) && (s5 == lc) && (s6 == lc))
    error = 0.5;
  else if ((s3 == sc) && (s4 == sc) && (s5 == lc) && (s6 == sc))
    error = 0.5;

  else if ((s2 == sc) && (s3 == lc) && (s4 == lc) && (s5 == sc))
    error = -1;
  else if ((s4 == sc) && (s5 == lc) && (s6 == lc) && (s7 == sc))
    error = 1;

  else if ((s2 == lc) && (s3 == lc) && (s4 == lc) && (s5 == sc))
    error = -1.5;
  else if ((s2 == sc) && (s3 == lc) && (s4 == sc) && (s5 == sc))
    error = -1.5;
  else if ((s4 == sc) && (s5 == lc) && (s6 == lc) && (s7 == lc))
    error = 1.5;
  else if ((s4 == sc) && (s5 == sc) && (s6 == lc) && (s7 == sc))
    error = 1.5;

  else if ((s1 == sc) && (s2 == lc) && (s3 == lc) && (s4 == sc))
    error = -2;
  else if ((s5 == sc) && (s6 == lc) && (s7 == lc) && (s8 == sc))
    error = 2;

  else if ((s1 == lc) && (s2 == lc) && (s3 == lc) && (s4 == sc))
    error = -2.5;
  else if ((s1 == sc) && (s2 == lc) && (s3 == sc) && (s4 == sc))
    error = -2.5;
  else if ((s5 == sc) && (s6 == lc) && (s7 == lc) && (s8 == lc))
    error = 2.5;
  else if ((s5 == sc) && (s6 == sc) && (s7 == lc) && (s8 == sc))
    error = 2.5;

  else if ((s1 == lc) && (s2 == lc) && (s3 == sc))
    error = -3;
  else if ((s6 == sc) && (s7 == lc) && (s8 == lc))
    error = 3;

  if ((s1 == lc) && (s2 == sc) && (s8 == sc))
    error = -3.5;
  if ((s1 == sc) && (s7 == sc) && (s8 == lc))
    error = 3.5;
}

///////////////////////pid computation/////////////////////

void pid() {
  readsensors();
  error_map();

  pd = Kp * error + Kd * (error - prev_error);

  lms = bs + pd;
  rms = bs - pd;

  if (lms >= left_max)
  {
    lms = left_max;
  }

  else if (lms <= left_min)
  {
    lms = left_min;
  }

  if (rms >= right_max)
  {
    rms = right_max;
  }

  else if (rms <= right_min)
  {
    rms = right_min;
  }

  analogWrite(lm1, lms + 2);
  analogWrite(lm2, 0);
  analogWrite(rm1, rms);
  analogWrite(rm2, 0);

  prev_error = error;

}

void loop() {
  readsensors();
  error_map();

  if (s1 == lc) {
    left = 0;
  }
  if (s8 == lc) {
    right = 0;
  }

  if (left == 0 && right == 0) { //for braking
    if (brake_count > 15000) {
      brake_count = 15000;
    }
    brake_count++;
  }
  else {
    brake_count = 0;
  }

  if (flag == false) {
    if ((s1 != lc) && (s2 != lc) && (s3 != lc) && (s4 != lc) && (s5 != lc) && (s6 != lc) && (s7 != lc) && (s8 != lc)) {
      if ((left > leftthres) && (right < rightthres)) { //right sharp turns
        brake();
        delay(5);
        readsensors();
        while ((s4 != lc) && (s5 != lc)) {
          sharp_right_turn();
          readsensors();
        }
        brake();
        delay(5);
        left = 1000;
        right = 1000;
      }
      else if ((left < leftthres) && (right > rightthres)) { //left sharp turn
        brake();
        delay(5);
        readsensors();
        while ((s4 != lc) && (s5 != lc)) {
          sharp_left_turn();
          readsensors();
        }
        brake();
        left = 1000;
        right = 1000;
      }/*
      else if(left>50 && left<200 && right>50 && right<200){ // 'T' -> go left
        T_intersection();
        rec_intersection('L');
      }*/
      else if (left < 200 && right < 200) { // 'T' -> go left
        T_intersection();
        rec_intersection('L');
      }
      else if (left > 950 && right > 950) { //dead end
        dead_end();
        rec_intersection('B');
      }
    }
    else if ((s1 != lc) && (s2 != lc) && ((s3 == lc) || (s4 == lc) || (s5 == lc) || (s6 == lc)) && (s7 != lc) && (s8 != lc)) {

      if (left > 100 && left < 1000 && right > 1000) { //straight or left -> left
        left_T();
        rec_intersection('L');
      }

      else if (left > 1000 && right < 1000 && right > 100) { //right_T(); straight or right -> straight
        straight();
        delay(10);
        //pid();
        rec_intersection('S');
      }

      else if (left < 100 && right < 100) { //cross -> left
        cross();
        rec_intersection('L');
      }
    }
    else if ((s1 == lc) && (s2 == lc) && (s3 == lc) && (s4 == lc) && (s5 == lc) && (s6 == lc) && (s7 == lc) && (s8 == lc)) {

      if (brake_count > brake_thres) { //end of maze
        //maze_optimize();
        //savetoeeprom();
        stop_end();
        flag = true;
      }
    }
  }
/*
  else if (flag == true) {
    optimized_path_control();
  }*/

  pid();
  left = left + 1;
  right = right + 1;
  if (left > 20000) { //20000
    left = 1000;
  }
  if (right > 20000) {
    right = 1000;
  }
}

///////////////////path control based on optimized path/////////////////

void optimized_path_control() {
  int i = 0;
  if ((s1 != lc) && (s2 != lc) && (s3 != lc) && (s4 != lc) && (s5 != lc) && (s6 != lc) && (s7 != lc) && (s8 != lc)) {
    if ((left > leftthres) && (right < rightthres)) { //right sharp turns
      brake();
      delay(10);
      readsensors();
      while (s6 != lc) {
        sharp_right_turn();
        readsensors();
      }
      brake();
      delay(20);
      left = 1000;
      right = 1000;
    }
    else if ((left < leftthres) && (right > rightthres)) { //left sharp turn
      brake();
      delay(10);
      readsensors();
      while (s3 != lc) {
        sharp_left_turn();
        readsensors();
      }
      brake();
      delay(20);
      left = 1000;
      right = 1000;
    }
    else if (left < 200 && right < 200) { // 'T' -> go left or right based on path[i]
      if (maze.eepath[i] == 'L') {
        sharp_left_turn();
      }
      else if (maze.eepath[i] == 'R') {
        sharp_right_turn();
      }
      i++;
    }
    else if (left > 950 && right > 950) { //dead end
      dead_end();
    }
  }
  else if ((s1 != lc) && (s2 != lc) && ((s3 == lc) || (s4 == lc) || (s5 == lc) || (s6 == lc)) && (s7 != lc) && (s8 != lc)) {

    if (left > 100 && left < 1000 && right > 1000) { //straight or left -> left
      if (maze.eepath[i] == 'L') {
        left_T();
      }
      else if (maze.eepath[i] == 'S') {
        straight();
      }
      i++;
    }

    else if (left > 100 && left < 1000 && right > 1000) { //right_T(); straight or right -> straight
      if (maze.eepath[i] == 'R') {
        right_T();
      }
      else if (maze.eepath[i] == 'S') {
        straight();
      }
      i++;
    }

    else if (left < 100 && right < 100) { //cross -> left
      if (maze.eepath[i] == 'L') {
        left_T();
      }
      else if (maze.eepath[i] == 'R') {
        right_T();
      }
      else if (maze.eepath[i] == 'S') {
        straight();
      }
      i++;
    }
  }
  else if ((s1 == lc) && (s2 == lc) && (s3 == lc) && (s4 == lc) && (s5 == lc) && (s6 == lc) && (s7 == lc) && (s8 == lc)) {

    if (brake_count > brake_thres) { //end of maze
      stop_end();
      flag = false;
    }
  }

}

////////////////////record the direction taken by he bot/////////////////////

void rec_intersection(char dir) {
  maze.path[pathlength] = dir;
  //maze.pathl = pathlength;
  simplify_path();
  pathlength++;
}

//////////////////////simplify the path while storing it///////////////////

void simplify_path() {
  if (maze.eepath[maze.pathl - 1] != 'B' || maze.pathl < 2){
    return;
  }
  else if (maze.eepath[maze.pathl - 1] == 'B') {
    path_new[0] = maze.eepath[maze.pathl - 2];
    path_new[1] = maze.eepath[maze.pathl - 1];
    path_new[2] = maze.eepath[maze.pathl];

    if (strcmp(path_new, "LBL") == 0) {
      maze.eepath[maze.pathl - 2] = 'S';
    }
    else if (strcmp(path_new, "LBS") == 0) {
      maze.eepath[maze.pathl - 2] = 'R';
    }
    else if (strcmp(path_new, "LBR") == 0) {
      maze.eepath[maze.pathl - 2] = 'B';
    }
    else if (strcmp(path_new, "RBL") == 0) {
      maze.eepath[maze.pathl - 2] = 'B';
    }
    else if (strcmp(path_new, "SBS") == 0) {
      maze.eepath[maze.pathl - 2] = 'B';
    }
    else if (strcmp(path_new, "SBL") == 0) {
      maze.eepath[maze.pathl - 2] = 'R';
    }

    maze.pathl = maze.pathl - 1;
    pathlen = pathlen - 2;

  }

}

//////////////////////storing the optimized path/////////////////

void rec_intersec(char dir) {
  maze.eepath[pathlen] = dir;
  maze.pathl = pathlen;
  simplify_path();
  pathlen++;
}

/////////////////optimize the already simplified path//////////////

void maze_optimize(){
  for(int i = 0; i < no_of_steps; i++){
    rec_intersec(maze.path[i]);
  }
}

////////////////////save the optimized path in EEPROM///////////////////

void savetoeeprom() {
  EEPROM.put(eeAddress, maze);
}

/////////////////////////intersection handling////////////////////

void T_intersection() {
  brake();
  delay(30);
  while ((s4 != lc) && (s5 != lc)) {         // 'T' -> left turn
    sharp_left_turn();
    readsensors();
  }
  brake();
  //delay(20);
  left = 1000;
  right = 1000;
}

void left_T() {
  brake();
  delay(30);
  sharp_left_turn();
  delay(250);              //to be tuned
  while (s4 != lc && s5 != lc) {
    sharp_left_turn();
    readsensors();
  }
  brake();
  //delay(20);

  left = 1000;
  right = 1000;


}

void right_T() {
  brake();
  delay(30);
  sharp_right_turn();
  delay(250);             //to be tuned
  while (s4 != lc && s5 != lc) {
    sharp_right_turn();
    readsensors();
  }
  brake();
  //delay(20);

  left = 1000;
  right = 1000;
}

void cross() {
  brake();
  delay(10);
  sharp_right_turn();
  //sharp_left_turn();
  delay(650);               //to be tuned
  while (s4 != lc && s5 != lc) { //go left
    sharp_left_turn();
    readsensors();
  }
  brake();
  //delay(20);

  left = 1000;
  right = 1000;
}

void dead_end() {
  brake();
  delay(100);

  while ((s4 != lc) && (s5 != lc)) {
    sharp_left_turn();
    readsensors();
  }
  brake();
  delay(10);

  left = 1000;
  right = 1000;
}

void stop_end() {
  brake();
  delay(10000);
}

////////////////////////////turn functions////////////////////////

void straight() {
  analogWrite(lm1, 90);
  analogWrite(lm2, 0);
  analogWrite(rm1, 90);
  analogWrite(rm2, 0);
}

void sharp_left_turn() {
  analogWrite(lm1, 0);
  analogWrite(lm2, 90);
  analogWrite(rm1, 90);
  analogWrite(rm2, 0);
}

void sharp_right_turn() {
  analogWrite(lm1, 90); //90->90->140
  analogWrite(lm2, 0);
  analogWrite(rm1, 0);
  analogWrite(rm2, 90);
}
 
void about_turn() {
  analogWrite(lm1, 0);
  analogWrite(lm2, 90);
  analogWrite(rm1, 90);
  analogWrite(rm2, 0);
}

void brake() {
  analogWrite(lm1, 255);
  analogWrite(lm2, 255);
  analogWrite(rm1, 255);
  analogWrite(rm2, 255);
}
