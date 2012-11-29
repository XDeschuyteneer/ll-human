#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <xlocale.h>
#include <stdlib.h>
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
const int BLUE = 34;
const int MAGENTA = 35;
const int CYAN = 36;
const int WHITE = 37;

void cprint(char* mot, int style, int color) {
  printf("%c[%d;%dm%s%c[%dm", 27, style, color, mot, 27, 0);
}

char digit_to_char(int digit) {
  char s[2];
  sprintf(s, "%d", digit);
  return s[0];
}

void printnbr(int nums[], char repr[], int size) {
  int i;
  for (i = 0; i < size; i++) {
    repr[i] = digit_to_char(nums[i]);
  }
}

int arrToInt(int nums[], int base, int cnt) { 
  int tmp = 0; 
  for (cnt = cnt - 1; cnt >= 0; cnt--) 
    { 
      tmp += nums[cnt] * pow(base, cnt); 
    } 
  return tmp; 
} 

void b10_to_bX(int num, int nums[], int base, int bits) {
  int cnt = bits - 1; 
  while (cnt < bits && cnt >= 0) { 
    if (num != 0) {
      nums[cnt] = num % base; 
      num = num / base;
    } else {
      nums[cnt] = 0;
    }
    cnt--;
  }
}

char* lister(char* loc) {
  DIR *rep = opendir(loc);
  if (rep != NULL) 
    { 
      struct dirent  *dp;
      struct stat     statbuf;
      struct passwd  *pwd;
      struct group   *grp;
      struct tm      *tm;
      char            datestring[256]; 
      int nums[6];
      int bin[3];
      char repr[6];
      int i;
      int j;
      while ((dp = readdir (rep))) { 
        /* Get entry's information. */
        if (stat(dp->d_name, &statbuf) == -1)
          continue;

        b10_to_bX(statbuf.st_mode, nums, 8, 6);
        printf("|");
        if (nums[0] == 0 && nums[1] == 4 && nums[2] == 0) {
          printf("d");
        } else if (nums[0] == 1 && nums[1] == 0 && nums[2] == 0) {
          printf("-");
        }
        for (i = 3; i < 6; i++) {
          b10_to_bX(nums[i], bin, 2, 3);
          if (bin[0] == 0) {
            printf("-");
          } else {
            cprint("r", BOLD, GREEN);
          }
          if (bin[1] == 0) {
            printf("-");
          } else {
            cprint("w", BOLD, BLUE);
          }
          if (bin[2] == 0) {
            printf("-");
          } else {
            cprint("x", BOLD, RED);
          }
        }
        printf("| %i | %s |\n", statbuf.st_size, dp->d_name);
        
      } 
        closedir (rep), rep = NULL; 
      } 
    }



  int main(void){
    cprint("hello, world!\n", BOLD, RED);
    lister(".");
    return 0;
  }
