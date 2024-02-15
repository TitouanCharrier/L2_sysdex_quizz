#include "main.h"

#define PIPE_SIZE 1000

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

    int height, width;
    getmaxyx(stdscr, height, width);
    
    int bh = height/10;
    int bw = width/4;

    WINDOW* mainwin = newwin(height -1, width -1, 1, 1);

    WINDOW* questwin = newwin(bh, width/2, bh, width/4);

    WINDOW* answin1 = newwin(height/10, width/3, 3 * bh , width/6);
    WINDOW* answin2 = newwin(height/10, width/3, 3 * bh , 3 * width/6);
    WINDOW* answin3 = newwin(height/10, width/3, 4 * bh , width/6);
    WINDOW* answin4 = newwin(height/10, width/3, 4 * bh , 3 * width/6);

    WINDOW* buttonA = newwin(bh, bw, 6 * bh, 1 * bw);
    WINDOW* buttonB = newwin(bh, bw, 6 * bh, 1 * bw);
    WINDOW* buttonC = newwin(bh, bw, 6 * bh, 1 * bw);
    WINDOW* buttonD = newwin(bh, bw, 6 * bh, 2 * bw);

    noecho();
    curs_set(0);

    mvwprintw(mainwin, height / 8, (width - 20) / 2, "Bienvenue Dans le QUIZZ");
    mvwprintw(mainwin, (height / 8) +1, (width - 20) / 2, "    faites vos choix");

    mvwprintw(buttonA, bh/2, bw/2-4, "Bouton A");
    box(buttonA,0,0);

    mvwprintw(buttonB, bh/2, bw/2-4, "Bouton B");
    box(buttonB,0,0);

    box(questwin,0,0);

    box(answin1,0,0);
    box(answin2,0,0);
    box(answin3,0,0);
    box(answin4,0,0);

    box(buttonC,0,0);
    box(buttonD,0,0);

    
    //affichage de la bordure, le refresh() est REQUIS
    box(mainwin, 0, 0); 
    refresh();

    wrefresh(mainwin);
    wrefresh(questwin);
    wrefresh(answin1);
    wrefresh(answin2);
    wrefresh(answin3);
    wrefresh(answin4);
    wrefresh(buttonA);
    wrefresh(buttonB);
    wrefresh(buttonC);
    wrefresh(buttonD);
    

    getch();

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

