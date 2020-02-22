#define lc 0 //BLACK
#define sc 1 //WHITE

/////////////motor pinout/////////////////

#define lm1 3
#define lm2 5
#define rm1 10
#define rm2 11

int s1, s2, s3, s4, s5, s6, s7, s8;

/////////////pid parameters////////////////

float error = 0;
float prev_error = 0;
float Kp = 37.5;
float Kd = 75; //100/150
float P = 0;
float D = 0;
float pd = 0;

int lms, rms; //left motor speed, right motor speed
int bs = 160; //base speed

int left_max = 200;
int right_max = 202; //based on motor speed diff
int left_min = 0;
int right_min = 0;

///////////////sharp turn parameters///////////////

int left = 0, right = 0;
int leftthres = 300;    //150
int rightthres = 300;   //150
int tthres = 200;       // for the T section only
int left_t_delay = 250; //delay for left T

/////////////brake parameters///////////////

int brake_count = 0;
int brake_thres = 400;

////////////setup///////////////////////////
void setup()
{
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);

  left = 1000;
  right = 1000;
}

///////////////////LOOP STARTS///////////////

void loop()
{
  readsensors();
  error_map();

  // This is for the left most and right most sensors
  if (s1 == lc)
  {
    left = 0;
  }
  if (s8 == lc)
  {
    right = 0;
  }

  // For break_count checking
  if (left == 0 && right == 0)
  {
    if (brake_count > 15000)
    {
      brake_count = 15000;
    }
    brake_count++;
  }
  else
  {
    brake_count = 0;
  }

  // WHEN SENSOR OVERSHOOTS AND FINDS NOTHING
  // RIGHT ONLY
  // LEFT ONLY
  // T
  // DEAD END
  if ((s1 != lc) && (s2 != lc) && (s3 != lc) && (s4 != lc) && (s5 != lc) && (s6 != lc) && (s7 != lc) && (s8 != lc))
  {
    // RIGHT SHARP TURNS
    if ((left > leftthres) && (right < rightthres))
    {
      brake();
      readsensors();
      while ((s4 != lc) && (s5 != lc))
      {
        sharp_right_turn();
        readsensors();
      }
      brake();
      left = 1000;
      right = 1000;
    }

    // LEFT SHARP TURN
    else if ((left < leftthres) && (right > rightthres))
    {
      brake();
      readsensors();
      while ((s4 != lc) && (s5 != lc))
      {
        sharp_left_turn();
        readsensors();
      }
      brake();
      left = 1000;
      right = 1000;
    }

    // 'T' -> go left
    else if ((left < tthres) && (right < tthres))
    {
      T_intersection();
    }

    // DEAD END
    else
    {
      dead_end();
    }
  }

  // WHEN SENSOR OVERSHOOTS AND FINDS A LINE
  // STRAIGHT OR LEFT
  // STRAIGHT OR RIGHT
  // CROSS

  else if ((s1 != lc) && (s4 == lc) && (s5 == lc) && (s8 != lc))
  {
    // straight or left -> left
    if ((left < 500) && (right > 1000))
    {
      left_T();
    }
    // straight or right -> straight
    else if ((left > 1000) && (right < 500))
    {
      straight();
    }
    //cross -> left
    else if ((left < 500) && (right < 500))
    {
      left_T();
    }
  }
  // WHEN SENSOR OVERSHOOTS AND FINDS ALL BLACK
  else if ((s1 == lc) && (s2 == lc) && (s3 == lc) && (s4 == lc) && (s5 == lc) && (s6 == lc) && (s7 == lc) && (s8 == lc))
  {
    // END OF MAZE
    // TO DO:
    // CHANGE break_thres in accordance to stopping at intersection and end maze.
    if (brake_count > brake_thres)
    {
      stop_end();
    }
  }

  pid();
  left = left + 1;
  right = right + 1;
  if (left > 18000)
  {
    left = 1000;
  }
  if (right > 18000)
  {
    right = 1000;
  }
}

//////////////////////sensor get value//////////////////////

void readsensors()
{
  s1 = digitalRead(A0);
  s2 = digitalRead(A1);
  s3 = digitalRead(A2);
  s4 = digitalRead(A3);
  s5 = digitalRead(A4);
  s6 = digitalRead(A5);
  s7 = digitalRead(2);
  s8 = digitalRead(4);
}

///////////////////////error definition/////////////////////

void error_map()
{
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

void pid()
{
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

  analogWrite(lm1, lms);
  analogWrite(lm2, 0);
  analogWrite(rm1, rms);
  analogWrite(rm2, 0);

  prev_error = error;
}

/////////////////////////intersection handling////////////////////

void T_intersection()
{
  brake();
  readsensors();
  while ((s4 != lc) && (s5 != lc))
  {
    sharp_left_turn();
    readsensors();
  }
  brake();
  left = 1000;
  right = 1000;
}

void left_T()
{
  brake();
  // TAKE A LEFT TO GET OUT OF LINE
  sharp_left_turn();
  delay(left_t_delay);
  readsensors();
  while (s4 != lc && s5 != lc)
  {
    sharp_left_turn();
    readsensors();
  }
  brake();
  left = 1000;
  right = 1000;
}

void dead_end()
{
  brake();
  readsensors();
  while ((s4 != lc) && (s5 != lc))
  {
    sharp_right_turn();
    readsensors();
  }
  brake();
  left = 1000;
  right = 1000;
}

void stop_end()
{
  brake();
  delay(10000);
}

////////////////////////////turn functions////////////////////////

void straight()
{ // Changed from 110 -> 90
  analogWrite(lm1, 90);
  analogWrite(lm2, 0);
  analogWrite(rm1, 90);
  analogWrite(rm2, 0);
}

void sharp_left_turn()
{ // Changed from 110 -> 90
  analogWrite(lm1, 0);
  analogWrite(lm2, 90);
  analogWrite(rm1, 90);
  analogWrite(rm2, 0);
}

void sharp_right_turn()
{
  analogWrite(lm1, 90); // Changed from 110 -> 90
  analogWrite(lm2, 0);
  analogWrite(rm1, 0);
  analogWrite(rm2, 90);
}

void brake()
{
  analogWrite(lm1, 255);
  analogWrite(lm2, 255);
  analogWrite(rm1, 255);
  analogWrite(rm2, 255);
}
