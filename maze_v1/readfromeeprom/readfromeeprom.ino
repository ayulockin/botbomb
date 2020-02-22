#include <EEPROM.h>

struct mazepath{
  char eepath[100];
  int pathl;
  char path[100];
};

mazepath maze;

int eeAddress=0;

void setup() {

  Serial.begin(9600);
/*
  for(int i=0;i<11;i++){
   Serial.println("Read custom object from EEPROM: ");
   char val = path[i];
   EEPROM.get(eeAddress, dir);
   Serial.println(dir);
   eeAddress++; 
  }*/  
  readfromeeprom();
}


void loop() {
  /* Empty loop */
}

void readfromeeprom(){
  EEPROM.get(eeAddress,maze);
  Serial.println("path!");
  Serial.println(maze.path);
  Serial.println(maze.pathl);
  Serial.println(maze.eepath);
  
/*
  EEPROM.get(eeAddress+1,maze.eepath);
  Serial.println("optimized path!");
  Serial.println(maze.eepath);
*/
}

