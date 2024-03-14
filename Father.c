#include "main.h"

#include <stdio.h>

// fonction de débug générée par ia 
// génère un fichier de débug appelé debug.log
// le fichier contient l'historique des événements
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

// Fonction principale d'affichage et de modification des boutons
// en Ncurses les boutons sont des fenètres avec un contour appelé "box"
// cette fonction prend une fenètre en entrée et en réécrit le contenu
void drawButton(WINDOW* buttonwin, char* text, int BH, int BW, int reverse) {
    wclear(buttonwin);
    box(buttonwin, 0, 0);
    if(reverse == 0) wattron(buttonwin, A_REVERSE);
    else wattron(buttonwin, COLOR_PAIR(1));
    mvwprintw(buttonwin, 3 * BH / 2, BW / 2 - (strlen(text) / 2), text);
    if (reverse == 0) wattroff(buttonwin, A_REVERSE);
    else wattroff(buttonwin, COLOR_PAIR(1));

    wrefresh(buttonwin);
}

// Fonction de mise a jour des fenètres
// prends une liste de fenètres en entrée, affiche la question et met a jour les réponses
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

// Fonction d'appel de la fenètre de définition
// efface les fenètres précédentes et modifie questwin
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

// Fonction d'affichage du résultat temporaire
// Efface le contenu de questwin et le remplace par le text adapté
void resultWin(WINDOW* listWin[], int BH, int BW, bool result) {
  wclear(listWin[4]);
  box(listWin[4], 0, 0);
  if (result) {
    wattron(listWin[4], COLOR_PAIR(3));
    mvwprintw(listWin[4], 2*BH/2,  (3*BW)/4 -7 , "Feliciations !");
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

// Fonction de lecture du score dans la shm
// lit le score et retourne sous la forme d'un string
char* getScore() {
  char* scoreboard = malloc(500*sizeof(char));
  char* adr2;
  int status, cle = 5;

  if(( status = shmget(cle, sizeof(int), 0))==-1) {
    debug_log(strerror(errno));
    exit(1);
  }

  if(( adr2 =(char*) shmat(status, NULL, 0)) == (char*)-1){
    debug_log(strerror(errno));
    exit(1);
  }

  sprintf(scoreboard,"%s",adr2);
  debug_log("le père à lu dans la shared memory");
  debug_log(adr2);
  debug_log(scoreboard);
  
  if(shmdt(adr2) == -1) {
    debug_log(strerror(errno));
    exit(1);
  }

  if(shmctl(status, IPC_RMID, NULL) == -1) {
    debug_log(strerror(errno));
    exit(1);
  }

  return scoreboard;
}

void sigHandlerV() {

  char* args[] = {"./finalV", NULL};
  execvp(args[0], args);
}

void sigHandlerD() {
  char* args[] = {"./finalD", NULL};
  execvp(args[0], args);
}

// Fonction principale du père
// Est en charge de l'initialisation et de l'actualisation
// des fenètres ncurses en fonction des données contenu
// dans le ToPrint envoyé par le fils
void mainFather(int nQues, int nAns) {  

  //-------------------------------- initialisation de Ncurses ----------------------------------
  
  // initialisation de l'écran
  initscr();
  
  //supression du curseur et de l'affichage des touches
  noecho();
  curs_set(0);
    
  //calcul des bords de l'écran
  int HEIGHT, WIDTH;
  getmaxyx(stdscr, HEIGHT, WIDTH);
  
  // Définition de la fenetre principale
  WINDOW* mainwin = newwin(HEIGHT -1, WIDTH -1, 1, 1);

  // Définition des couleurs
  start_color(); 
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);

  // Définition des tailles 
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
  
  // Définition du fond
  bkgd(COLOR_PAIR(2));
  wrefresh(mainwin);
  refresh();
  
  //--------------------------------- initialisation des Pipes -----------------------------------
  int toPrintPipe = open(PIPE_PRINT, O_RDONLY);
  int_log("le père à ouvert le print : ", toPrintPipe);
  int statePipe = open(PIPE_STATE, O_WRONLY);
  int_log("le père à ouvert le state : ", toPrintPipe);
  int resultPipe = open(PIPE_RES, O_RDONLY);
  int_log("le père à ouvert le result : ", toPrintPipe);

  //------------------------------- initialisation des variables ---------------------------------
  int check = 0;
  int state = 1;
  int actualResult = 0;
  int ch = -1;
  int qCount = 0;

  //initialisation de print et prise en compte d'un défaut de chargment du fils
  ToPrint print;
  print.question = "erreur";
  print.answer1 = "answer erreur";
  print.answer2 = "answer erreur";
  print.answer3 = "answer erreur";
  print.answer4 = "answer erreur";
  print.goodState = 1;
  
  // Gestion des signaux
  signal(SIGUSR1, sigHandlerV);
  signal(SIGUSR2, sigHandlerD);
  
  //------------------------------ Début de la boucle principale ---------------------------------
  do {
    
    if (ch == -1) { //lire le print
      debug_log("lecture du print par le père");
      check = read(toPrintPipe, &print, sizeof(ToPrint));
      int_log("le père à lu le print : ", check);
      debug_log(print.question);

    }      
    
    // Rafraichir la fenetre
    initWin(listWin, print, BH, BW, nAns, state);
    refresh();
    
    // Prise en compte des touche préssées
    switch (ch) {
 
      case KEY_RIGHT : //séléctionner la réponse suivante
        debug_log("flèche droite enfoncé");
        if (state < nAns) {
          state ++;
          initWin(listWin, print, BH, BW, nAns, state);
          refresh();
        }
        break;

      case KEY_LEFT : //séléctionner la réponse précédente
        debug_log("flèche gauche enfoncé");
        if (state > 1) {
          state --;
          initWin(listWin, print, BH, BW, nAns, state);
          refresh();
        }
        break;
      
      case '\n' : //Envoyer la répose séléctionné au fils
        debug_log("Entrée enfoncé");
        //envoyer la réponse dans le tube state 
        check = write(statePipe, &state, sizeof(int));
        int_log("le père à écrit le state : ", check);
        
        //lire la fenetre a afficher
        check = read(resultPipe, &actualResult, sizeof(int)); //bloquant
        int_log("le père lit le result : ", check); 
        
        // Afficher la fenètre résultat
        resultWin(listWin, BH, BW, actualResult); 
        refresh();

        qCount ++;
        
        // Prise en charge de la fin du jeu
        if (qCount == nQues) {
          getch();
          debug_log("starting to get score");
          char* score = malloc(80*sizeof(char));
          usleep(50000);
          score = getScore();
          debug_log("score given");
          debug_log(score);

          //affichage de l'écran de fin
          initquitWin(listWin, score, BH, BW); 
          refresh();
          sleep(2);
          free(score);
          goto END;
        }
        
        //lecture de la question suivante
        debug_log("lecture du print par le père lors de Entrée");
        check = read(toPrintPipe, &print, sizeof(ToPrint));
        int_log("le père à lu le print lors de Entrée: ", check);

        if (check == 0) {
          debug_log("erreur lors de la lecture d'un print");
          return;
        }

        debug_log(print.question);
        break;

      default: break;
    } 

    keypad(stdscr, TRUE);

  } while ((ch = getch()) != 'q' );
  
END:

  endwin();
   
  write(statePipe, &state, sizeof(int));
  close(toPrintPipe);
  close(statePipe);
  close(resultPipe);

  debug_log("wait for signal");
  usleep(50000);

  exit(0);
  
}
