#include<string.h>
char path[100] = "LBLLLBSBLLBSLLBSRLBL";
struct mazepath{
  char eepath[100]="";
  int pathl = 6;
};
char path_new[10];
int pathlength=0;
mazepath maze;

void setup(){
  Serial.begin(9600);
  for(int i=0;i<30;i++){
    rec_intersection(path[i]);
  }
  Serial.println(maze.eepath);
}

void loop(){
  //Serial.println(maze.eepath);
  //maze.pathl = pathlength;
  //simplify_path();
  //pathlength++;
  //Serial.println(maze.pathl);
  //Serial.println(pathlength);
  //delay(1000);
}

void rec_intersection(char dir){
  maze.eepath[pathlength]=dir;
  maze.pathl = pathlength;
  simplify_path();
  pathlength++;
  //Serial.println(maze.pathl);
  //Serial.println(pathlength);
}

void simplify_path(){
  Serial.println("m here!");
  if(maze.eepath[maze.pathl-1]!='B' || maze.pathl<2){
    Serial.println("m returned!");
    return;  
  }
  else if(maze.eepath[maze.pathl-1]=='B'){
    path_new[0]=maze.eepath[maze.pathl-2];
    path_new[1]=maze.eepath[maze.pathl-1];
    path_new[2]=maze.eepath[maze.pathl];

    Serial.println(path_new);

    if (strcmp(path_new, "LBL") == 0){
      maze.eepath[maze.pathl-2] = 'S';
    }
    else if (strcmp(path_new, "LBS") == 0){
      maze.eepath[maze.pathl-2] = 'R';
    }
    else if (strcmp(path_new, "LBR") == 0){
      maze.eepath[maze.pathl-2] = 'B';
    }
    else if (strcmp(path_new, "RBL") == 0){
      maze.eepath[maze.pathl-2] = 'B';
    }
    else if (strcmp(path_new, "SBS") == 0){
      maze.eepath[maze.pathl-2] = 'B';
    }
    else if (strcmp(path_new, "SBL") == 0){
      maze.eepath[maze.pathl-2] = 'R';
    }

    maze.pathl = maze.pathl-1;
    pathlength = pathlength-2;
    
  }
}
