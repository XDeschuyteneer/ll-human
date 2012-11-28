#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h> 
#include <dirent.h> 
#include <sys/stat.h> 
#include <time.h> 

/*
 * attributs
 */
const int NONE = 0;
const int BOLD = 1;
const int UNDERSCORE = 4;
const int BLINK = 5;
const int REVERSE = 7;
const int CONCEALED = 8;

/*
 * colors
 */ 
const int BLACK = 30;
const int RED = 31;
const int GREEN = 32;
const int YELLOW = 33;
const int Blue = 34;
const int MAGENTA = 35;
const int CYAN = 36;
const int WHITE = 37;

void cprint(char* mot, int style, int color) {
  printf("%c[%d;%dm%s%c[%dm\n", 27, style, color, mot, 27, 0);
}

char* lister(char* loc) {
  DIR *rep = opendir(loc); 
 
  if (rep != NULL) 
    { 
      struct dirent *lecture; 
 
      while ((lecture = readdir (rep))) 
        { 
          struct stat st; 
 
          stat (lecture->d_name, &st);
          time_t t = st.st_mtime; 
          struct tm tm = *localtime (&t); 
          char s[32]; 
          strftime (s, sizeof s, "%d/%m/%Y %H:%M:%S", &tm); 
 
          printf ("%-14s %s\n", lecture->d_name, s);
        } 
      closedir (rep), rep = NULL; 
    } 
}

int main(void)
{
  cprint("hello, world!", BOLD, RED);
  lister(".");
  return 0;
}
