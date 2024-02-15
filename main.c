#include "main.h"


#define PIPE_SIZE 1000

int main(int argc, char *argv[]) {


  fprintf(stdout,"debug0\n");
  
  int pipefd[2];
  
  if (pipe(pipefd) == -1) {
    perror("Erreur lors du pipe\n");
    exit(EXIT_FAILURE);
  }
  
  
  printf("debug1\n");

  pid_t filsPid;
  filsPid = fork();

  if (filsPid < 0) {
    fprintf(stderr, "Erreur lors du fork\n");
    return 1;
  } 

  // Code du fils
  else if (filsPid == 0) {
    mainSon(pipefd); 
    

    
  } 

  //Code du père
  else {
  printf("Debug Début Père\n");

  sleep(1);
  
  Entry mainEntry;

  close(pipefd[1]);
  read(pipefd[0], &mainEntry, sizeof(Entry));
  
  //printf("pointeur père : %p\n", mainEntry);  
  printf("test père : %s\n", mainEntry.qList[0].body);
  printf("debug Fin père\n");
 
  }

  return 0;

  return EXIT_SUCCESS;
}

