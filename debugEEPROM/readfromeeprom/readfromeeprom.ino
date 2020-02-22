#include<EEPROM.h>

struct mazepath
{
  char eepath[100] = "";
  int pathl = 0;
};

char path_new[10];
int pathlength = 0;
mazepath maze;

// EEPROM address
int flag=0; // 0 for 1st run, 1 for optimized run

void setup(){
  EEPROM.get(0, maze);
  EEPROM.get(1, flag);
  Serial.print(maze.eepath);
  Serial.print(flag);
}

void loop(){
  
}
