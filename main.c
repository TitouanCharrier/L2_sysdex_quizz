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

   noecho();
    curs_set(0);
    
    //calcul des bords de l'écran et 
    int HEIGHT, WIDTH;
    getmaxyx(stdscr, HEIGHT, WIDTH);
    
    const BH = HEIGHT/10;
    const BW = WIDTH/4;
    const AW = WIDTH/3;
    

    //définition et placement des fenètres ---------------------------------------
    WINDOW* mainwin = newwin(HEIGHT -1, WIDTH -1, 1, 1);

    WINDOW* questwin = newwin(BH, WIDTH/2, BH, WIDTH/4);

    WINDOW* answin1 = newwin(BH, AW, 3 * BH , WIDTH/6);
    WINDOW* answin2 = newwin(BH, AW, 3 * BH , 3 * WIDTH/6);
    WINDOW* answin3 = newwin(BH, AW, 4 * BH , WIDTH/6);
    WINDOW* answin4 = newwin(BH, AW, 4 * BH , 3 * WIDTH/6);

    WINDOW* buttonA = newwin(BH, BW, 6 * BH, 2);
    WINDOW* buttonB = newwin(BH, BW, 6 * BH, 1 * BW);
    WINDOW* buttonC = newwin(BH, BW, 6 * BH, 2 * BW);
    WINDOW* buttonD = newwin(BH, BW, 6 * BH, 3 * BW - 1);

   

    // Afficher les textes --------------------------------------------------------
    mvwprintw(mainwin, HEIGHT / 8, (WIDTH - 20) / 2, "Bienvenue Dans le QUIZZ");
    mvwprintw(mainwin, (HEIGHT / 8) +1, (WIDTH - 20) / 2, "    faites vos choix");

    mvwprintw(answin1, BH/2, AW/2-4, "Bouton A");
    mvwprintw(answin2, BH/2, AW/2-4, "Bouton A");
    mvwprintw(answin3, BH/2, AW/2-4, "Bouton A");
    mvwprintw(answin4, BH/2, AW/2-4, "Bouton A");

    mvwprintw(buttonA, BH/2, BW/2, "A");
    mvwprintw(buttonB, BH/2, BW/2, "B");
    mvwprintw(buttonC, BH/2, BW/2, "C");
    mvwprintw(buttonD, BH/2, BW/2, "D");
    
    
    // Afficher les box --------------------------------------------------------
    box(mainwin, 0, 0); 

    box(questwin,0,0);

    box(answin1,0,0);
    box(answin2,0,0);
    box(answin3,0,0);
    box(answin4,0,0);

    box(buttonA,0,0);
    box(buttonB,0,0);
    box(buttonC,0,0);
    box(buttonD,0,0);

    //permet l'affichage tu texte et des box 
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

