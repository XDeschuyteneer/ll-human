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
#include <unistd.h>
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

void scprint(char* mot, char* output, int style, int color) {
  sprintf(output, "%c[%d;%dm%s%c[%dm", 27, style, color, mot, 27, 0);
} 

void B_to_str(int B, char* repr) {
  char type;
  if (B < 1000) {
    sprintf(repr, "%iB", B);
  } else if (B < 1000000) {
    sprintf(repr, "%.1fK", B / 1000.);
  } else if (B < 1000000000) {
    sprintf(repr, "%.1fM", B / 1000000.);
  } else {
    sprintf(repr, "%fG", B / 1000000000.);
  }
}

void printdate(time_t old) {
  time_t  curr;
  time(&curr);
  double diff = difftime(curr, old);
  if (diff < 60) {
    printf(" | %-8s", "< 1 min");
  } else if (diff < 600) {
    printf(" | %-8s", "< 10 min");
  } else if (diff < 3600) {
    printf(" | %-8s", "< 1h");
  } else if (diff < 2592000) {
    printf(" | %-8s", "< 1 mois");
  } else if (diff < 32140800) {
    printf(" | %-8s", "< 1 an");
  } else {
    printf(" | %-8s", "> 1 an");
  }
}

char* lister(char* loc, int all) {
  DIR *rep = opendir(loc);
  chdir(loc);
  if (rep != NULL) { 
    struct dirent  *dp;
    struct stat     statbuf;
    struct passwd  *pwd;
    struct group   *grp;
    struct tm      *tm;
    char            datestring[256]; 
    int nums[6];
    int bin[3];
    char repr[6];
    char size[5];
    int i;
    int j;

    while ((dp = readdir (rep))) { 
      if (stat(dp->d_name, &statbuf) == -1)
        continue;

      if (dp->d_name[0] != '.' || all) {
        printf("| ");

        //permitions
        cprint( (S_ISDIR(statbuf.st_mode)) ? "d" : "-", BOLD, BLUE);
        cprint( (statbuf.st_mode & S_IRUSR) ? "r" : "-", BOLD, GREEN);
        cprint( (statbuf.st_mode & S_IWUSR) ? "w" : "-", BOLD, RED);
        cprint( (statbuf.st_mode & S_IXUSR) ? "x" : "-", BOLD, BLUE);
        cprint( (statbuf.st_mode & S_IRGRP) ? "r" : "-", BOLD, GREEN);
        cprint( (statbuf.st_mode & S_IWGRP) ? "w" : "-", BOLD, RED);
        cprint( (statbuf.st_mode & S_IXGRP) ? "x" : "-", BOLD, BLUE);
        cprint( (statbuf.st_mode & S_IROTH) ? "r" : "-", BOLD, GREEN);
        cprint( (statbuf.st_mode & S_IWOTH) ? "w" : "-", BOLD, RED);
        cprint( (statbuf.st_mode & S_IXOTH) ? "x" : "-", BOLD, BLUE);

        //date modif
        printdate(statbuf.st_mtime);

        B_to_str(statbuf.st_size, size);
        printf(" | %7s | ", size);
        if (S_ISDIR(statbuf.st_mode))
            cprint(dp->d_name, BOLD, BLUE);
        else
            cprint(dp->d_name, BOLD, BLACK);
        printf(" \n");
      }
        
    }
    closedir (rep), rep = NULL; 
  } else {
    printf("impossible d'ouvrir rep");
  }
}



int main(int argc, char **argv){
  if (argc == 1) {
    lister(".", 0);
  } else if (argc == 2 && (strcmp(argv[1], "-a") == 0)) {
    lister(".", 1);
  } else if (argc == 2) {
    lister(argv[1], 0);
  } else if (argc == 3 && strcmp(argv[1], "-a") == 0) {
    lister(argv[2], 1);
  } else {
    printf("erreur parametres");
  }
  return 0;
}
