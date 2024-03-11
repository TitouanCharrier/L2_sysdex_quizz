#include "main.h"

void displayHelp() {
  printf("Usage: ./quiz [OPTIONS]...\n Quiz game via IPC.\n\n");
  printf("Options:\n"); 
  printf("\t-h, --help display this help and exit\n"); 
  printf("\t-r, --rules display the rules of the game\n");
  printf("\t-s, --student display the name of the students who implemented the game\n");
  printf("\t-a, --answers set the number of possible answers (by default 2, max 4)\n");
  printf("\t-q, --questions set the number of questions in the quiz (by default 4, max 10)\n");
}

int main(int argc, char *argv[]) {

  //Permet l'utilisation de caractères UTF-8
  setlocale(LC_ALL, "fr_FR.UTF-8");
  
  //---------------------------------- Prise en compte des arguments ------------------------------

  //Définition des variables principales
  int nQues = 4;
  int nAns = 2;
  int opt = 0; 
  debug_log("début du programme ----------------------------------------------------------------");

   
  while ((opt = getopt(argc, argv, "hrsq:a:")) != -1) {
    debug_log("enter the while arg");
    int_log("arg is : ", opt);
    switch (opt) {
      case 'h' :
        displayHelp();
        return 0;
    
      case 'r' :
        
        printf("Use [righ arrow] and [left arrow] to select the right answer\n");
        printf("then press [ENTER]\n");
        printf("your result is printed and you can press any key except [ENTER]\n");
        printf("to get the next question\n");
        printf("Quit using [q]\n");
        return 0;
    
      case 's' :
        printf("Work done by Alexy Bechade and Titouan Charrier (AKA les meilleurs)\n");
        return 0;

      case 'a' :
        nAns = atoi(optarg);
        break;

      case 'q':
        nQues = atoi(optarg);

    }

  }
  
  if (nQues < 4 || nQues > 10 || nAns < 2 || nAns > 4) {
    displayHelp();
    return 0;
  }

  //---------------------------------- Définition des pipes --------------------------------------

  int check = mkfifo(PIPE_PRINT, 0666);
  int_log("Print Pipe initialisé :", check);
  check = mkfifo(PIPE_STATE, 0666);
  int_log("State Pipe initialisé :", check);
  check = mkfifo(PIPE_RES, 0666);
  int_log("Res Pipe initialisé :", check);

  //-------------------------------- Création du processus fils ----------------------------------

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
    // appel de la fonction mainson décrite dans Son.c
    mainSon( 0, nQues); 
  } 

  //Code du père
  else {
  // appel de la fonction mainFather décrite dans Father.c
  mainFather(nQues, nAns);
  }

  return 0;
}

