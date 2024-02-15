#include "main.h"

void mainFather(WINDOW* mainwin, int HEIGHT, int WIDTH, int pipefd[], int nQues, int nAns) {  

  
  const BH = HEIGHT/10; //Button Height
  const BW = WIDTH/4; //Button Width
  const AW = WIDTH/3; //Answer Width
  

  //définition et placement des fenètres ---------------------------------------
  WINDOW* questwin = newwin(BH, WIDTH/2, BH, WIDTH/4);

  WINDOW* answin1 = newwin(BH, AW, 3 * BH , WIDTH/6);
  WINDOW* answin2 = newwin(BH, AW, 3 * BH , 3 * WIDTH/6);

  WINDOW* buttonA = newwin(BH, BW, 6 * BH, 2);
  WINDOW* buttonB = newwin(BH, BW, 6 * BH, 1 * BW - 1);


  // Afficher les textes --------------------------------------------------------
  mvwprintw(answin1, BH/2, AW/2-4, "Bouton A");
  mvwprintw(answin2, BH/2, AW/2-4, "Bouton A");

  mvwprintw(buttonA, BH/2, BW/2, "A");
  mvwprintw(buttonB, BH/2, BW/2, "B");
  
  
  // Afficher les box --------------------------------------------------------
  box(questwin,0,0);

  box(answin1,0,0);
  box(answin2,0,0);

  box(buttonA,0,0);
  box(buttonB,0,0);

  //permet l'affichage tu texte et des box 
  refresh();

  wrefresh(mainwin);
  wrefresh(questwin);
  wrefresh(answin1);
  wrefresh(answin2);
 
  wrefresh(buttonA);
  wrefresh(buttonB);
  
  if (nAns == 3 || nAns == 4) {
    WINDOW* answin3 = newwin(BH, AW, 4 * BH , WIDTH/6);
    WINDOW* buttonC = newwin(BH, BW, 6 * BH, 2 * BW - 1 );
    mvwprintw(answin3, BH/2, AW/2-4, "Bouton A");
    mvwprintw(buttonC, BH/2, BW/2, "C");
    box(answin3,0,0);
    box(buttonC,0,0);
    refresh();
    wrefresh(answin3);
    wrefresh(buttonC);
  }
  if (nAns == 4) {
    WINDOW* answin4 = newwin(BH, AW, 4 * BH , 3 * WIDTH/6);
    WINDOW* buttonD = newwin(BH, BW, 6 * BH, 3 * BW - 1);
    mvwprintw(answin4, BH/2, AW/2-4, "Bouton A");
    mvwprintw(buttonD, BH/2, BW/2, "D");
    box(answin4,0,0);
    box(buttonD,0,0);
    refresh();
    wrefresh(answin4);
    wrefresh(buttonD);
  }

  getch();
  endwin();

   
}
