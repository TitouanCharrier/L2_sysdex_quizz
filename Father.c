#include "main.h"

#include <stdio.h>

void int_log(const char *message, int value) {
    FILE *file = fopen("debug.log", "a"); // Ouvre le fichier en mode ajout
    if (file != NULL) {
        fprintf(file, "%s %d\n", message, value); // Écrit le message et l'entier dans le fichier
        fclose(file); // Ferme le fichier
    } else {
        printf("Impossible d'ouvrir le fichier de journalisation des débogages.\n");
    }
}

void debug_log(const char* message) {
  int_log(message, -100);
}

// Fonction pour changer la couleur du texte d'un bouton
void drawButton(WINDOW* buttonwin, char* text, int BH, int BW, bool reverse) {
    wclear(buttonwin);
    box(buttonwin, 0, 0);
    if(reverse) wattron(buttonwin, A_REVERSE);
    else wattron(buttonwin, COLOR_PAIR(1));
    mvwprintw(buttonwin, 3*BH/2, BW/2 - (strlen(text)/2), text);
    if (reverse) wattroff(buttonwin, A_REVERSE);
    else wattroff(buttonwin, COLOR_PAIR(1));

    wrefresh(buttonwin);
}

void initWin(WINDOW* questwin, WINDOW* buttonList[], ToPrint print, int BH, int BW, int nAns) {
  
  mvwprintw(questwin, 2*BH/2,  (3*BW)/4 - (strlen(print.question)/2), print.question);
  box(questwin, 0,0);
  wrefresh(questwin);
  printf("question : %s\n", print.question);

  drawButton(buttonList[0], print.answer1, BH, BW, 0);
  drawButton(buttonList[1], print.answer2, BH, BW, 0);
  if (nAns>2) drawButton(buttonList[2], print.answer3, BH, BW, 0);
  if (nAns>3) drawButton(buttonList[3], print.answer4, BH, BW, 0);

  refresh();
}

void resultWin(WINDOW* questwin, int BH, int BW, bool result) {
  wclear(questwin);
  box(questwin, 0, 0);
  if (result) {
    wattron(questwin, COLOR_PAIR(3));
    mvwprintw(questwin, 2*BH/2,  (3*BW)/4 -7 , "Féliciations !");
    wattroff(questwin, COLOR_PAIR(3));
  }
  else {
    wattron(questwin, COLOR_PAIR(4));
    mvwprintw(questwin, 2*BH/2,  (3*BW)/4 -4 , "Dommage !");
    wattroff(questwin, COLOR_PAIR(4));
  }
  wrefresh(questwin);

}

void mainFather(WINDOW* mainwin, int HEIGHT, int WIDTH, int statePipe[],  int toPrintPipe[], int resultPipe[], int nQues, int nAns, int* mainMem) {  
  
  start_color(); 
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);

  int state = 1;
  int state_old = 1;
  
  int BH = HEIGHT/10; //Button Height
  int BW = WIDTH/3; //Button Width
  int AW = WIDTH/3; //Answer Width

  //key_t key = ftok(const char ".", int "SysDexTpNote");
  //int mainMem_v = shmget(key_t key, size_t 1024, IPC_CREAT | 0666);
  //int* mainMem = shmat(int mainMem_v, const void NULL, int 0);
  
  //initialisation de print et prise en compte d'un défaut de chargment du fils
  ToPrint print;
  print.question = "erreur";
  print.answer1 = "answer erreur";
  print.answer2 = "answer erreur";
  print.answer3 = "answer erreur";
  print.answer4 = "answer erreur";
  print.goodState = 1;
  
  int check;

  debug_log("lecture du print initial par le père");
  close(toPrintPipe[1]);
  check = read(toPrintPipe[0], &print, sizeof(ToPrint));
  int_log("le père à lu initialement : ", check);
  close(toPrintPipe[0]);

  //définition et placement des fenètres ---------------------------------------
  WINDOW* questwin = newwin(2*BH, WIDTH/2, BH, WIDTH/4);

  WINDOW* buttonA = newwin(3*BH, AW, 4 * BH , WIDTH/6);
  WINDOW* buttonB = newwin(3*BH, AW, 4 * BH , 3 * WIDTH/6);
  WINDOW* buttonC = newwin(3*BH, AW, 7 * BH , WIDTH/6);
  WINDOW* buttonD = newwin(3*BH, AW, 7 * BH , 3 * WIDTH/6);

  WINDOW* buttonList[4] = {buttonA, buttonB, buttonC, buttonD};
  
  char* listAttribute[4] = {print.answer1, print.answer2, print.answer3, print.answer4};
  
  //initialisation de la fenètre
  //initWin(questwin, buttonList, print, BH, BW, nAns);
  bkgd(COLOR_PAIR(2));
  refresh();

  int ch = -1;
  do {

    //lire le print
    debug_log("lecture du print par le père");
    close(toPrintPipe[1]);
    check = read(toPrintPipe[0], &print, sizeof(ToPrint));
    close(toPrintPipe[0]);
    int_log("le père à lu : ", check);
    debug_log(print.question);

    debug_log("affichage de la question");
    initWin(questwin, buttonList, print, BH, BW, nAns);
    refresh();

    switch (ch) {
      
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
      
      case '\n' :
        //envoyer la réponse dans le tube state 
        close(statePipe[0]);
        write(statePipe[1], &state, sizeof(int));
        close(statePipe[1]);
        
        //lire la fenetre a afficher
        int actualResult;
        close(resultPipe[1]);
        read(resultPipe[0], &actualResult, sizeof(int));
        close(resultPipe[0]);

        resultWin(questwin, BH, BW, actualResult); 

        break;

      default: break;
    } 

      
    cbreak;
    keypad(stdscr, TRUE);

  } while ((ch = getch()) != 'q' );
  
  endwin();
   
}
