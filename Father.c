#include "main.h"

// Fonction pour changer la couleur du texte d'un bouton
void reverseColor(WINDOW* buttonwin, char* text, const BH, const BW) {

    wattron(buttonwin, A_REVERSE); //active le surlignage
    mvwprintw(buttonwin, BH/2, BW/2 - (strlen(text)/2), ("%s", text));
    wattroff(buttonwin, A_REVERSE);

    wrefresh(buttonwin);
}

// Fonction pour changer la couleur du texte d'un bouton
void restoreColor(WINDOW* buttonwin, char* text, const BH, const BW) {

    wattron(buttonwin, COLOR_PAIR(1)); //active le surlignage
    mvwprintw(buttonwin, BH/2, BW/2 - (strlen(text)/2), ("%s", text));
    wattroff(buttonwin, COLOR_PAIR(1));

    wrefresh(buttonwin);
}

void reverseButtons(WINDOW* buttonList[], int state, const BH, const BW) {
  switch (state) {
    case 1 :
      reverseColor(buttonList[0], "Answer A", BH, BW);
      break;
    case 2 :
      reverseColor(buttonList[1], "Answer B", BH, BW);
      break;  
    case 3 :
      reverseColor(buttonList[2], "Answer C", BH, BW);
      break;  
    case 4 :
      reverseColor(buttonList[3], "Answer D", BH, BW);
      break;
  }
}

void restoreButtons(WINDOW* buttonList[], int state, const BH, const BW) {
  switch (state) {
    case 1 :
      restoreColor(buttonList[0], "Answer A", BH, BW);
      break;
    case 2 :
      restoreColor(buttonList[1], "Answer B", BH, BW);
      break;  
    case 3 :
      restoreColor(buttonList[2], "Answer C", BH, BW);
      break;  
    case 4 :
      restoreColor(buttonList[3], "Answer D", BH, BW);
      break;
  }
}


void mainFather(WINDOW* mainwin, int HEIGHT, int WIDTH, int statePipe[],  int toPrintPipe[], int nQues, int nAns) {  
  
  init_pair(1, COLOR_WHITE, COLOR_BLACK);

  int state =1;
  int state_old;
  
  const BH = HEIGHT/10; //Button Height
  const BW = WIDTH/4; //Button Width
  const AW = WIDTH/3; //Answer Width
  
  //définition et placement des fenètres ---------------------------------------
  WINDOW* questwin = newwin(BH, WIDTH/2, BH, WIDTH/4);

  WINDOW* answin1 = newwin(BH, AW, 3 * BH , WIDTH/6);
  WINDOW* answin2 = newwin(BH, AW, 3 * BH , 3 * WIDTH/6);
  WINDOW* answin3 = newwin(BH, AW, 4 * BH , WIDTH/6);
  WINDOW* answin4 = newwin(BH, AW, 4 * BH , 3 * WIDTH/6);
  WINDOW* buttonA = newwin(BH, BW, 6 * BH, 2);
  WINDOW* buttonB = newwin(BH, BW, 6 * BH, 1 * BW - 1);
  WINDOW* buttonC = newwin(BH, BW, 6 * BH, 2 * BW - 1 );
  WINDOW* buttonD = newwin(BH, BW, 6 * BH, 3 * BW - 1);

  WINDOW* buttonList[4] = {buttonA, buttonB, buttonC, buttonD};

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
    mvwprintw(answin3, BH/2, AW/2-4, "Bouton A");
    mvwprintw(buttonC, BH/2, BW/2, "C");
    box(answin3,0,0);
    box(buttonC,0,0);
    refresh();
    wrefresh(answin3);
    wrefresh(buttonC);
  }
  if (nAns == 4) {
    mvwprintw(answin4, BH/2, AW/2-4, "Bouton A");
    mvwprintw(buttonD, BH/2, BW/2, "D");
    box(answin4,0,0);
    box(buttonD,0,0);
    refresh();
    wrefresh(answin4);
    wrefresh(buttonD);
  }
  cbreak;
  keypad(stdscr, TRUE);



  int ch;
  while ((ch = getch()) != 'q' ) {
    switch (ch) {

      case KEY_RIGHT :
        if (state < nAns) {
          state_old = state;
          state ++;
          restoreButtons(buttonList, state_old, BH, BW);
          reverseButtons(buttonList, state, BH, BW);
        }
        break;

      case KEY_LEFT :
        if (state > 1) {
          state_old = state;
          state --;
          restoreButtons(buttonList, state_old, BH, BW);
          reverseButtons(buttonList, state, BH, BW);
        }
        break;
      
      case KEY_ENTER :
        close(statePipe[0]);
        write(statePipe[1], state, sizeof(int));
        close(statePipe[1]);
    }
  }
  
  endwin();

   
}
