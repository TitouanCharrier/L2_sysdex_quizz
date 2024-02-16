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

void reverseButtons(WINDOW* buttonList[], int state, const BH, const BW, char* text) {
  reverseColor(buttonList[state], ("%s", text ), BH, BW);
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

  ToPrint print;

  close(toPrintPipe[1]);
  read(toPrintPipe[0], &print, sizeof(Entry));
  
  //définition et placement des fenètres ---------------------------------------
  WINDOW* questwin = newwin(2*BH, WIDTH/2, BH, WIDTH/4);

  WINDOW* buttonA = newwin(3*BH, AW, 4 * BH , WIDTH/6);
  WINDOW* buttonB = newwin(3*BH, AW, 4 * BH , 3 * WIDTH/6);
  WINDOW* buttonC = newwin(3*BH, AW, 7 * BH , WIDTH/6);
  WINDOW* buttonD = newwin(3*BH, AW, 7 * BH , 3 * WIDTH/6);

  WINDOW* buttonList[4] = {buttonA, buttonB, buttonC, buttonD};

  // Afficher les textes --------------------------------------------------------
  mvwprintw(questwin, BH/2, BW/2 - (strlen(print.question)/2), print.question);
  mvwprintw(buttonA, BH/2, BW/2, print.answer1);
  mvwprintw(buttonB, BH/2, BW/2, print.answer2);
  
  
  // Afficher les box --------------------------------------------------------
  box(questwin,0,0);

  box(buttonA,0,0);
  box(buttonB,0,0);

  //permet l'affichage tu texte et des box 
  refresh();

  wrefresh(mainwin);
  wrefresh(questwin);
 
  wrefresh(buttonA);
  wrefresh(buttonB);
  
  if (nAns == 3 || nAns == 4) {
    mvwprintw(buttonC, BH/2, BW/2, print.answer3);
    box(buttonC,0,0);
    refresh();
    wrefresh(buttonC);
  }
  if (nAns == 4) {
    mvwprintw(buttonD, BH/2, BW/2, print.answer4);
    box(buttonD,0,0);
    refresh();
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
          reverseButtons(buttonList, state, BH, BW, "mouai...");
        }
        break;

      case KEY_LEFT :
        if (state > 1) {
          state_old = state;
          state --;
          restoreButtons(buttonList, state_old, BH, BW);
          reverseButtons(buttonList, state, BH, BW, "testons ce truc");
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
