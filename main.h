#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
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
#define PIPE_PRINT "pipe_print"
#define PIPE_STATE "pipe_state"
#define PIPE_RES   "pipe_res"

typedef struct ToPrint {
  char* question;
  char* answer1;
  char* answer2;
  char* answer3;
  char* answer4;
  int goodState;
} ToPrint;

void mainFather(int nQues, int nAns);
void mainSon(int nQues);
void debug_log(const char* message);
void int_log(const char *message, int value);

