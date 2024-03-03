#include "main.h"

#define PIPE_SIZE 1000

int main(int argc, char *argv[]) {

  //Permet l'utilisation de caractères UTF-8
  setlocale(LC_ALL, "fr_FR.UTF-8");
  
  //Définition des variables principales
  int statePipe[2]; //pipe gérant la réponse renvoyé
  int toPrintPipe[2]; //pipe gérant la question a afficher
  int resultPipe[2]; //pipe gérant le résultat a afficher
  int nQues= 10;
  int nAns = 2;

  //initialisation du pipe
  pipe(toPrintPipe); 
  pipe(statePipe);
  pipe(resultPipe);
  
  //fonction de débug
  for (int i=0; i<argc; ++i) {
    printf("argv %d = %s\n", i, argv[i]);
  }
   
  //affichage de l'aide et prise en compte du -h
  if (argc == 1 ) {
  }

  else if (argv[1][1] == 'r') {
    printf("Ceci est un quizz, répondez bien à la question, gagnez un point, sinon 0\n");
    return 0;
  }

  else if (argv[1][1] == 's') {
    printf("Travail réalisé par Alexy Bechade et Titouan Charrier (AKA les meilleurs)\n");
    return 0;
  }

  else if (argc == 2 && argv[1][1] == 'a') {
    nAns = argv[1][2] - '0';
    printf("nAns %d\n", nAns);
  } 
  
  else if (argc == 2 && argv[1][1] == 'q') {
    nQues = atoi(argv[1]+2*sizeof(char));
    printf("nQues %d\n", nQues);

  }

  else if (argc == 3) {
    if ( argv[2][1] == 'a') {
      nAns = argv[2][2] - '0';
      printf("nAns %d\n", nAns);
    }

    if ( argv[1][1] == 'a') {
      nAns = argv[1][2] - '0';
      printf("nAns %d\n", nAns);
    }
    
    if (argv[1][1] == 'q') {
    nQues = atoi(argv[1]+2*sizeof(char));
    printf("nQues %d\n", nQues);
    }
    
    if (argv[2][1] == 'q') {
    nQues = atoi(argv[2]+2*sizeof(char));
    printf("nQues %d\n", nQues);
    }
  } 

  else {
    printf("Usage: ./quiz [OPTIONS]...\n Quiz game via IPC.\n\n Options:\n \t\t-h, --help display this help and exit\n \t\t-r, --rules display the rules of the game\n \t\t-s, --student display the name of the students who implemented the game\n \t\t-a, --answers set the number of possible answers (by default 2, max 4)\n \t\t-q, --questions set the number of questions in the quiz (by default 4, max 10)\n WARNING : 2 arguments max, args like -a and -q must be as showned :\n \t\t-a3 -q10\n");
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
    mainSon(statePipe, toPrintPipe, 0, resultPipe, nQues); 
  } 

  //Code du père
  else {
  debug_log("Début du père");  
  // ----------------------------------- Début de la mise en page ncurses -------------------------
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
  box(mainwin, 0, 0); 


  mainFather(mainwin, HEIGHT, WIDTH, statePipe, toPrintPipe, resultPipe, 10, nAns, 0);

  //------------------------------------ Fin de la mise en page ncurses --------------------------
 
  }

  return 0;

  return EXIT_SUCCESS;
}

