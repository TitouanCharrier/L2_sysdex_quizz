#include "main.h"


#define PIPE_SIZE 1000

// Fonction de dynamisation de la fenètre ncurses
void handle_resize(int sig) {
    endwin();
    refresh();
    clear();
    refresh();
}

int main(int argc, char *argv[]) {

  setlocale(LC_ALL, "fr_FR.UTF-8");

  int pipefd[2];
  
  //initialisation du pipe
  if (pipe(pipefd) == -1) {
    perror("Erreur lors du pipe\n");
    exit(EXIT_FAILURE);
  }
  
  //initialisation du fork
  pid_t filsPid;
  filsPid = fork();
  
  //verification du fork
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
  
  // ----------------------------------- Début de la mise en page ncurses -------------------------

   initscr();
    signal(SIGWINCH, handle_resize);

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW *mainwin = newwin(height, width, 0, 0);
    box(mainwin, 0, 0);
    wrefresh(mainwin);

    while (1) {
        getmaxyx(stdscr, height, width);
        werase(mainwin);
        box(mainwin, 0, 0);
        wrefresh(mainwin);
    }

    endwin();

  //------------------------------------ Fin de la mise en page ncurses --------------------------- 

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

