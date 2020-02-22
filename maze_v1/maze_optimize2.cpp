#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
char path[100] = "LBLLLBSBLLBSLL";
int pathlength = 14;
int pathindex = 0;
char path_new[100];
char path_add;

char replace (char *, char *, char *);
 
int main() {
    char n;
	int a;
	int i,j,k;
	char prev_char;
 	int pathlength = 14;
 	
	puts(path);
	printf("***********************\n");
	
	for(i=0;i<pathlength;i++){
    	for(i=0;i<=pathlength;i++){
			if(path[i]=='B'){
				for(k=0;k<1;k++){
					path_new[k] = path[i-1];
					path_new[k+1] = 'B';
					path_new[k+2] = path[i+1];
					puts(path_new);
					printf("\n");
				}
				if (strcmp(path_new, "LBL") == 0){
					path_add = 'S';
					printf("%c\n", path_add);
				}
				else if (strcmp(path_new, "LBS") == 0){
					path_add = 'R';
					printf("%c\n", path_add);
				}
				else if (strcmp(path_new, "LBR") == 0){
					path_add = 'B';
					printf("%c\n", path_add);
				}
				else if (strcmp(path_new, "RBL") == 0){
					path_add = 'B';
					printf("%c\n", path_add);
				}
				else if (strcmp(path_new, "SBS") == 0){
					path_add = 'B';
					printf("%c\n", path_add);
				}
				else if (strcmp(path_new, "SBL") == 0){
					path_add = 'R';
					printf("%c\n", path_add);
				}
				break;	
			}	
	
				
			pathlength = strlen(path);
			printf("%d",pathlength);
			printf("\n"); 
		
			prev_char = path[i];
		}
		replace(path,path_new,&path_add);
		printf("\n\nThe replaced string\n*************************************\n");
    	puts(path);
	}
}
 
/**
 * The replace function
 *
 * Searches all of the occurrences using recursion
 * and replaces with the given string
 * @param char * o_string The original string
 * @param char * s_string The string to search for
 * @param char * r_string The replace string
 * @return void The o_string passed is modified
 */
char replace(char * o_string, char * s_string, char * r_string) {
      char buffer[100];
      
      char * ch;
 
      if(!(ch = strstr(o_string, s_string)))
              return *o_string;
			       
      strncpy(buffer, o_string, ch-o_string);
      buffer[ch-o_string] = 0;
      sprintf(buffer+(ch - o_string), "%s%s", r_string, ch + strlen(s_string));
      o_string[0] = 0;
      strcpy(o_string, buffer);

      return *o_string;
 }
