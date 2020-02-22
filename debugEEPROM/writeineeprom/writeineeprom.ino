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
  EEPROM.put(0, maze);
  EEPROM.put(1, flag);
}

void loop(){
  
}
