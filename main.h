#include <stdio.h>
#include <stdlib.h>
//#include <assert.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

typedef struct Question {
  int q_id;
  char* body;
} Question;

typedef struct Answer {
  int a_id;
  char* choice1;
  char* choice2;
  char* choice3;
  char* choice4;
} Answer;

typedef struct Entry {
  Question qList[10];
  Answer aList[10];
} Entry;

void startFather();
void mainSon(int pipefd[]);

