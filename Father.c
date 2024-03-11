#include "main.h"

#include <stdio.h>

// fonction de débug générée par ia
void int_log(const char *message, int value) {
    FILE *file = fopen("debug.log", "a"); // Ouvre le fichier en mode write
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
void drawButton(WINDOW* buttonwin, char* text, int BH, int BW, int reverse) {
    wclear(buttonwin);
    box(buttonwin, 0, 0);
    if(reverse == 0) wattron(buttonwin, A_REVERSE);
    else wattron(buttonwin, COLOR_PAIR(1));
    mvwprintw(buttonwin, 3*BH/2, BW/2 - (strlen(text)/2), text);
    if (reverse == 0) wattroff(buttonwin, A_REVERSE);
    else wattroff(buttonwin, COLOR_PAIR(1));

    wrefresh(buttonwin);
}

void initWin(WINDOW* listWin[], ToPrint print, int BH, int BW, int nAns, int state) {
  
  wclear(listWin[4]);
  mvwprintw(listWin[4], 2*BH/2,  (3*BW)/4 - (strlen(print.question)/2), print.question);
  debug_log("la question est : ");
  debug_log(print.question);
  box(listWin[4], 0,0);
  wrefresh(listWin[4]);

  drawButton(listWin[0], print.answer1, BH, BW, 1-state);
  drawButton(listWin[1], print.answer2, BH, BW, 2-state);
  if (nAns>2) drawButton(listWin[2], print.answer3, BH, BW, 3-state);
  if (nAns>3) drawButton(listWin[3], print.answer4, BH, BW, 4-state);

  for (int i=0; i<6; ++i) {
    wrefresh(listWin[i]);
  }
}

void initquitWin(WINDOW* listWin[], char* score, int BH, int BW) {
  
  debug_log("entrée dans initquitWin");
  werase(listWin[0]);
  werase(listWin[1]);
  werase(listWin[2]);
  werase(listWin[3]);
  wclear(listWin[4]);

  box(listWin[4], 0,0);
  mvwprintw(listWin[4], 2*BH/2,  (3*BW)/4 - (strlen(score)/2), score);
  for (int i=0; i<6; ++i) {
    wrefresh(listWin[i]);
  }
}

void resultWin(WINDOW* listWin[], int BH, int BW, bool result) {
  wclear(listWin[4]);
  box(listWin[4], 0, 0);
  if (result) {
    wattron(listWin[4], COLOR_PAIR(3));
    mvwprintw(listWin[4], 2*BH/2,  (3*BW)/4 -7 , "Féliciations !");
    wattroff(listWin[4], COLOR_PAIR(3));
  }
  else if (!result){
    wattron(listWin[4], COLOR_PAIR(4));
    mvwprintw(listWin[4], 2*BH/2,  (3*BW)/4 -4 , "Dommage !");
    wattroff(listWin[4], COLOR_PAIR(4));
  }

  for (int i=0; i<6; ++i) {
    wrefresh(listWin[i]);
  }
  
}

char* getScore() {
  
  char* scoreboard = malloc(500*sizeof(char));
  char* adr2;
  int status, cle = 5;

  if(( status = shmget(cle, sizeof(int), 0))==-1) {
    printf("shm2.shmget: %s\n", strerror(errno));
    exit(1);
  }

  if(( adr2 =(char*) shmat(status, NULL, 0)) == (char*)-1){
    printf("shm2.shmat: %s\n", strerror(errno));
    exit(1);
  }

  sprintf(scoreboard,"%s",adr2);
  debug_log("le père à lu dans la shared memory");
  debug_log(adr2);
  debug_log(scoreboard);
  
  if(shmdt(adr2) == -1) {
    printf("shm2.shmdt: %s\n", strerror(errno));
    exit(1);
  }

  if(shmctl(status, IPC_RMID, NULL) == -1) {
    printf("shm2.shmctl: %s\n", strerror(errno));
    exit(1);
  }

  return scoreboard;
}

void mainFather(int nQues, int nAns) {  

   // initialisation de Ncurses -------------------------------------------------------------------
  
  // initialisation de la fenetre
  initscr();
  
  //supression du curseur et de l'affichage des touches
  noecho();
  curs_set(0);
    
  //calcul des bords de l'écran et 
  int HEIGHT, WIDTH;
  getmaxyx(stdscr, HEIGHT, WIDTH);
  
  // Définition de la fenetre principale
  WINDOW* mainwin = newwin(HEIGHT -1, WIDTH -1, 1, 1);

  start_color(); 
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);

 
  int BH = HEIGHT/10; //Button Height
  int BW = WIDTH/3; //Button Width
  int AW = WIDTH/3; //Answer Width

  //définition et placement des fenètres 
  WINDOW* questwin = newwin(2*BH, WIDTH/2, BH, WIDTH/4);

  WINDOW* buttonA = newwin(3*BH, AW, 4 * BH , WIDTH/6);
  WINDOW* buttonB = newwin(3*BH, AW, 4 * BH , 3 * WIDTH/6);
  WINDOW* buttonC = newwin(3*BH, AW, 7 * BH , WIDTH/6);
  WINDOW* buttonD = newwin(3*BH, AW, 7 * BH , 3 * WIDTH/6);

  WINDOW* listWin[6] = {buttonA, buttonB, buttonC, buttonD, questwin, mainwin};
  
  
  bkgd(COLOR_PAIR(2));
  box(mainwin, 0, 0); 
  wrefresh(mainwin);
  refresh();
  
  // initialisation des Pipes --------------------------------------------------------------------
  int toPrintPipe = open(PIPE_PRINT, O_RDONLY);
  int_log("le père à ouvert le print : ", toPrintPipe);
  int statePipe = open(PIPE_STATE, O_WRONLY);
  int_log("le père à ouvert le state : ", toPrintPipe);
  int resultPipe = open(PIPE_RES, O_RDONLY);
  int_log("le père à ouvert le result : ", toPrintPipe);

  // initialisation des variables ----------------------------------------------------------------
  int check = 0;
  int state = 1;
  int state_old = 1;
  int actualResult = 0;

  //initialisation de print et prise en compte d'un défaut de chargment du fils
  ToPrint print;
  print.question = "erreur";
  print.answer1 = "answer erreur";
  print.answer2 = "answer erreur";
  print.answer3 = "answer erreur";
  print.answer4 = "answer erreur";
  print.goodState = 1;
  
  int ch = -1;
  int qCount = 0;
  
  //Début de la boucle principale ----------------------------------------------------------------
  do {
    
    if (ch == -1) { //lire le print
      debug_log("lecture du print par le père");
      check = read(toPrintPipe, &print, sizeof(ToPrint));
      int_log("le père à lu le print : ", check);
      debug_log(print.question);

    }      

    initWin(listWin, print, BH, BW, nAns, state);
    refresh();

    char* listAttribute[4] = {print.answer1, print.answer2, print.answer3, print.answer4};

    switch (ch) {
 
      case KEY_RIGHT :
        debug_log("flèche droite enfoncé");
        if (state < nAns) {
          state_old = state;
          state ++;
          initWin(listWin, print, BH, BW, nAns, state);
          refresh();
        }
        break;

      case KEY_LEFT :
        debug_log("flèche gauche enfoncé");
        if (state > 1) {
          state_old = state;
          state --;
          initWin(listWin, print, BH, BW, nAns, state);
          refresh();
        }
        break;
      
      case '\n' :
        debug_log("Entrée enfoncé");
        //envoyer la réponse dans le tube state 
        check = write(statePipe, &state, sizeof(int));
        int_log("le père à écrit le state : ", check);
        
        //lire la fenetre a afficher
        check = read(resultPipe, &actualResult, sizeof(int)); //bloquant
        int_log("le père lit le result : ", check); 

        resultWin(listWin, BH, BW, actualResult); 
        refresh();

        //passage a la question suivante 
        qCount ++;
       
        if (qCount == nQues) {
          getch();
          debug_log("starting to get score");
          char* score = malloc(500*sizeof(char));
          usleep(500000);
          score = getScore();
          debug_log("score given");
          debug_log(score);
          initquitWin(listWin, score, BH, BW); 
          refresh();
          getch();
          goto END;
        }
        debug_log("lecture du print par le père lors de Entrée");
        check = read(toPrintPipe, &print, sizeof(ToPrint));
        int_log("le père à lu le print lors de Entrée: ", check);
        if (check == 0) {
          debug_log("erreur lors de la lecture d'un print");
          return 1;
        }
        debug_log(print.question);


        break;

      default: break;
    } 


    cbreak;
    keypad(stdscr, TRUE);

  } while ((ch = getch()) != 'q' );
  
END:

  check = close(toPrintPipe);
  int_log("le père à fermé le print : ", check);
  close(statePipe);
  close(resultPipe);

  endwin();
   
}
