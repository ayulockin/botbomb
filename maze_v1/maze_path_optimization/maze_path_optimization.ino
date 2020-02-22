#include<string.h>

char path[100] = "LBLLLBSBLLBSLL";
struct mazepath{
  char eepath[100]="";
  int pathl = 0;
};
char path_new[10];
int pathlength=0;
mazepath maze;

int i=0;

void setup(){
  Serial.begin(9600);
  
  for(int i=0;i<30;i++){
    rec_intersection(path[i]);
  }
  Serial.println(maze.eepath);
  
}

void loop(){/*
  rec_intersection(path[i]);
  i++;
  Serial.println(maze.pathl);
  Serial.println(pathlength);
  delay(2000);
  */
}

void rec_intersection(char dir){
  maze.eepath[pathlength]=dir;
  maze.pathl = pathlength;
  simplify_path();
  pathlength++;
  Serial.println(maze.pathl);
  Serial.println(pathlength);
}

void simplify_path(){
  if(maze.eepath[maze.pathl-1]!='B' || maze.pathl<2){
    Serial.println("i am returned");
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








///////////////////////////////////////////////////////////////////

/*
void maze_optimize(){
  char n;
  int a;
  int i,j,k;
  char prev_char;
  
  for(i=0;i<maze.pathl;i++){
    for(i=0;i<=maze.pathl;i++){
      if(maze.eepath[i]=='B'){
        //for(k=0;k<1;k++){
          path_new[0] = maze.eepath[i-1];
          path_new[1] = 'B';
          path_new[2] = maze.eepath[i+1];
        //}
        if (strcmp(path_new, "LBL") == 0){
          path_add = 'S';
        }
        else if (strcmp(path_new, "LBS") == 0){
          path_add = 'R';
        }
        else if (strcmp(path_new, "LBR") == 0){
          path_add = 'B';
        }
        else if (strcmp(path_new, "RBL") == 0){
          path_add = 'B';
        }
        else if (strcmp(path_new, "SBS") == 0){
          path_add = 'B';
        }
        else if (strcmp(path_new, "SBL") == 0){
          path_add = 'R';
        }
        break;  
      } 
  
        
      maze.pathl = strlen(maze.eepath);
    
      prev_char = maze.eepath[i];
    }
    replace(maze.eepath,path_new,&path_add);
    //printf("\n\nThe replaced string\n*************************************\n");
    //puts(path);
  }
  EEPROM.put(eeAddress+1,maze.eepath);
}

char replace(char * o_string, char * s_string, char * r_string) {
      char buffer[100];
      
      char * ch;
      //printf("%c",*r_string);
 
      if(!(ch = strstr(o_string, s_string)))
              return *o_string;
             
      strncpy(buffer, o_string, ch-o_string);
      buffer[ch-o_string] = 0;
      sprintf(buffer+(ch - o_string), "%s%s", r_string, ch + strlen(s_string));
      o_string[0] = 0;
      strcpy(o_string, buffer);

      return *o_string;
}
*/









