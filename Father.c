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
  int_log(message, -1);
}

// Fonction pour changer la couleur du texte d'un bouton
void drawButton(WINDOW* buttonwin, char* text, int BH, int BW, bool reverse) {
    debug_log("drawButton débute");
    wclear(buttonwin);
    box(buttonwin, 0, 0);
    debug_log("drawButton à affiché la box");
    if(reverse) wattron(buttonwin, A_REVERSE);
    else wattron(buttonwin, COLOR_PAIR(1));
    debug_log("drawButton est initialisé");
    mvwprintw(buttonwin, 3*BH/2, BW/2 - (strlen(text)/2), text);
    debug_log("drawButton a affiché le bouton");
    if (reverse) wattroff(buttonwin, A_REVERSE);
    else wattroff(buttonwin, COLOR_PAIR(1));

    wrefresh(buttonwin);
    debug_log("drawButton prend fin");
}

void initWin(WINDOW* questwin, WINDOW* buttonList[], ToPrint print, int BH, int BW, int nAns) {
  
  debug_log("début de initWin");

  mvwprintw(questwin, 2*BH/2,  (3*BW)/4 - (strlen(print.question)/2), print.question);
  box(questwin, 0,0);
  wrefresh(questwin);
  debug_log("nous sommes arrivés a la question");
  debug_log(print.question);
  debug_log("nous avons passé la question");
  printf("question : %s\n", print.question);

  drawButton(buttonList[0], print.answer1, BH, BW, 0);
  drawButton(buttonList[1], print.answer2, BH, BW, 0);
  if (nAns>2) drawButton(buttonList[2], print.answer3, BH, BW, 0);
  if (nAns>3) drawButton(buttonList[3], print.answer4, BH, BW, 0);

  refresh();
  debug_log("fin de initWin ");
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
  
  printf("debugPère-1");
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

  usleep(100000);
  close(toPrintPipe[1]);
  read(toPrintPipe[0], &print, sizeof(ToPrint));
  debug_log("le Pipe affiche : ");
  debug_log(print.question);
  debug_log("fin de la lecture du toPrintPipe");
  //définition et placement des fenètres ---------------------------------------
  WINDOW* questwin = newwin(2*BH, WIDTH/2, BH, WIDTH/4);

  WINDOW* buttonA = newwin(3*BH, AW, 4 * BH , WIDTH/6);
  WINDOW* buttonB = newwin(3*BH, AW, 4 * BH , 3 * WIDTH/6);
  WINDOW* buttonC = newwin(3*BH, AW, 7 * BH , WIDTH/6);
  WINDOW* buttonD = newwin(3*BH, AW, 7 * BH , 3 * WIDTH/6);

  WINDOW* buttonList[4] = {buttonA, buttonB, buttonC, buttonD};
  
  printf("debugPère1");

  char* listAttribute[4] = {print.answer1, print.answer2, print.answer3, print.answer4};


  printf("debugPère2");

  initWin(questwin, buttonList, print, BH, BW, nAns);


  printf("debugPère3");

  bkgd(COLOR_PAIR(2));
  
  refresh();
  printf("debugPère4");
  sleep(1);

  int ch = -1;
  do {

    //lire le print
    close(toPrintPipe[1]);
    read(toPrintPipe[0], &print, sizeof(ToPrint));

    switch (ch) {
      
      case -1 :
        
        initWin(questwin, buttonList, print, BH, BW, nAns);
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
      
      case '\n' :
        //envoyer la réponse dans le tube state 
        close(statePipe[0]);
        write(statePipe[1], &state, sizeof(int));
        close(statePipe[1]);
        
        //lire la fenetre a afficher
        int actualResult;
        close(resultPipe[1]);
        read(resultPipe[0], &actualResult, sizeof(int));

        resultWin(questwin, BH, BW, actualResult); 

        sleep(2);

        break;

      default: break;
    } 

      
    cbreak;
    keypad(stdscr, TRUE);

  } while ((ch = getch()) != 'q' );
  
  endwin();
   
}
