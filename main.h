#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <signal.h>

typedef struct ToPrint {
  char* question;
  char* answer1;
  char* answer2;
  char* answer3;
  char* answer4;
  int goodState;
} ToPrint;

void mainFather(WINDOW* mainwin, int HEIGHT, int WIDTH, int statePipe[], int toPrintPipe[], int nQues, int nAns);
void mainSon(int statePipe[], int toPrintPipe[]);

