#include "main.h"


void mainSon(int pipefd[]) {
  
  Entry mainEntry;
  mainEntry.qList;

  printf("debug Début Fils\n");

  //Chargement des questions
  Question question1;
  question1.q_id = 1;
  question1.body = "question1"; 
  mainEntry.qList[0] = question1;

  Question question2;
  question2.q_id = 2;
  question2.body = "question2"; 
  mainEntry.qList[1] = question2;
  
  Question question3;
  question3.q_id = 3;
  question3.body = "question3"; 
  mainEntry.qList[2] = question3;

  Question question4;
  question4.q_id = 4;
  question4.body = "question4"; 
  mainEntry.qList[3] = question4;

  Question question5;
  question5.q_id = 5;
  question5.body = "question5"; 
  mainEntry.qList[4] = question5;

  Question question6;
  question6.q_id = 6;
  question6.body = "question6"; 
  mainEntry.qList[5] = question6;

  Question question7;
  question7.q_id = 7;
  question7.body = "question7"; 
  mainEntry.qList[6] = question7;

  Question question8;
  question8.q_id = 8;
  question8.body = "question8"; 
  mainEntry.qList[7] = question8;

  Question question9;
  question9.q_id = 9;
  question9.body = "question9"; 
  mainEntry.qList[8] = question9;

  Question question10;
  question10.q_id = 10;
  question10.body = "question10"; 
  mainEntry.qList[9] = question10;


  close(pipefd[0]);
  write(pipefd[1], &mainEntry, sizeof(Entry));
  close(pipefd[1]);

  printf("test fils : %s\n", mainEntry.qList[0].body);  

  printf("Debug Fin fils\n");
}

  /*
  //CHargement des réponses
  Answer answer1;
  answer1.a_id = 1;
  answer1.choice1 = "choice1";
  answer1.choice2 = "choice2";
  answer1.choice3 = "choice3";
  answer1.choice4 = "choice4";
  */


