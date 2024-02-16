#include "main.h"

// Fonction pour changer la couleur du texte d'un bouton
void drawButton(WINDOW* buttonwin, char* text, const BH, const BW, bool reverse) {

    wclear(buttonwin);
    box(buttonwin, 0, 0);
    if(reverse) wattron(buttonwin, A_REVERSE);
    mvwprintw(buttonwin, BH/2, BW/2 - (strlen(text)/2), text);
    if (reverse) wattroff(buttonwin, A_REVERSE);

    wrefresh(buttonwin);
}

void initWin(WINDOW* questwin, WINDOW* buttonList[], ToPrint print, const BH, const BW) {

  mvwprintw(questwin, BH/2, BW - (strlen(print.question)/2), print.question);
  box(questwin, 0,0);
  wrefresh(questwin);
  printf("question : %s\n", print.question);

  drawButton(buttonList[0], print.answer1, BH, BW, 0);
  drawButton(buttonList[1], print.answer2, BH, BW, 0);
  drawButton(buttonList[2], print.answer3, BH, BW, 0);
  drawButton(buttonList[3], print.answer4, BH, BW, 0);

  refresh();
}

void mainFather(WINDOW* mainwin, int HEIGHT, int WIDTH, int statePipe[],  int toPrintPipe[], int nQues, int nAns) {  
  
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_RED);

  int state =1;
  int state_old=1;
  
  const BH = HEIGHT/10; //Button Height
  const BW = WIDTH/4; //Button Width
  const AW = WIDTH/3; //Answer Width

  ToPrint print;

  close(toPrintPipe[1]);
  read(toPrintPipe[0], &print, sizeof(ToPrint));
  
  //définition et placement des fenètres ---------------------------------------
  WINDOW* questwin = newwin(2*BH, WIDTH/2, BH, WIDTH/4);

  WINDOW* buttonA = newwin(3*BH, AW, 4 * BH , WIDTH/6);
  WINDOW* buttonB = newwin(3*BH, AW, 4 * BH , 3 * WIDTH/6);
  WINDOW* buttonC = newwin(3*BH, AW, 7 * BH , WIDTH/6);
  WINDOW* buttonD = newwin(3*BH, AW, 7 * BH , 3 * WIDTH/6);

  WINDOW* buttonList[4] = {buttonA, buttonB, buttonC, buttonD};

  char* listAttribute[4] = {print.answer1, print.answer2, print.answer3, print.answer4};

  initWin(questwin, buttonList, print, BH, BW);
  
  refresh();

  int ch = -1;
  do {
    switch (ch) {
      
      case -1 :
        
        initWin(questwin, buttonList, print, BH, BW);
        break;

      case KEY_RIGHT :
        if (state < nAns) {
          state_old = state;
          state ++;
          drawButton(buttonList[state_old-1], listAttribute[state_old-1], BH, BW, 0);
          drawButton(buttonList[state-1], listAttribute[state-1], BH, BW, 1);
        }
        break;

      case KEY_LEFT :
        if (state > 1) {
          state_old = state;
          state --;
          drawButton(buttonList[state_old-1], listAttribute[state_old-1], BH, BW, 0);
          drawButton(buttonList[state-1], listAttribute[state-1], BH, BW, 1);
        }
        break;
      
      case KEY_ENTER :
        close(statePipe[0]);
        write(statePipe[1], &state, sizeof(int));
        close(statePipe[1]);

      default: break;
    } 

    
      
    // Afficher les textes --------------------------------------------------------
    //mvwprintw(questwin, BH/2, BW/2 - (strlen(print.question)/2), print.question);
    
    // Afficher les box --------------------------------------------------------
    //box(questwin,0,0);

    //box(buttonA,0,0);
    //box(buttonB,0,0);

    //permet l'affichage tu texte et des box 
    /*if (nAns == 3 || nAns == 4) {
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
    }*/
    cbreak;
    keypad(stdscr, TRUE);

  } while ((ch = getch()) != 'q' );
  
  endwin();

   
}
