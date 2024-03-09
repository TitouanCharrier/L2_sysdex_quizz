#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lib/ncurses.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <signal.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>

#define PIPE_SIZE 1000
#define PIPE_PRINT "./pipefolder/pipe_print"
#define PIPE_STATE "./pipefolder/pipe_state"
#define PIPE_RES   "./pipefolder/pipe_res"

typedef struct ToPrint {
  char* question;
  char* answer1;
  char* answer2;
  char* answer3;
  char* answer4;
  int goodState;
} ToPrint;

void mainFather(WINDOW* mainwin, int HEIGHT, int WIDTH, int nQues, int nAns, int* mainMem);
void mainSon( int* mainMem, int nQues);

