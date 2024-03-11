#include "main.h"

void mainSon(int nQues) {

  debug_log("début du fils");

  //------------------------- Initialisation des questions ---------------------------------------

  ToPrint printList[10];
  
  ToPrint print1;
  print1.question = "Quelle est la capitale de la France ?";
  print1.answer1 = "Paris";
  print1.answer2 = "Berlin";
  print1.answer3 = "Londres";
  print1.answer4 = "Rome";
  print1.goodState = 1; 
  printList[0] = print1;

  ToPrint print2;
  print2.question = "Quel est le plus grand océan ?";
  print2.answer1 = "Atlantique";
  print2.answer2 = "Pacifique";
  print2.answer3 = "Indien";
  print2.answer4 = "Arctique";
  print2.goodState = 2;
  printList[1] = print2;

  ToPrint print3;
  print3.question = "Quelle est la monnaie du Japon ?";
  print3.answer1 = "Dollar";
  print3.answer2 = "Yen";
  print3.answer3 = "Euro";
  print3.answer4 = "Livre";
  print3.goodState = 2;
  printList[2] = print3;

  ToPrint print4;
  print4.question = "Quel est le plus grand désert du monde ?";
  print4.answer1 = "Sahara";
  print4.answer2 = "Atacama";
  print4.answer3 = "Gobi";
  print4.answer4 = "Antarctique";
  print4.goodState = 1;
  printList[3] = print4;

  ToPrint print5;
  print5.question = "Qui a écrit 'Romeo et Juliette' ?";
  print5.answer1 = "William Shakespeare";
  print5.answer2 = "Victor Hugo";
  print5.answer3 = "Charles Dickens";
  print5.answer4 = "Jane Austen";
  print5.goodState = 1;
  printList[4] = print5;

  ToPrint print6;
  print6.question = "Combien de continents y a-t-il sur Terre ?";
  print6.answer1 = "4";
  print6.answer2 = "7";
  print6.answer3 = "6";
  print6.answer4 = "9";
  print6.goodState = 2;
  printList[5] = print6;

  ToPrint print7;
  print7.question = "Quel est le plus haut sommet du monde ?";
  print7.answer1 = "Mont Blanc";
  print7.answer2 = "Mont Everest";
  print7.answer3 = "K2";
  print7.answer4 = "Matterhorn";
  print7.goodState = 2;
  printList[6] = print7;

  ToPrint print8;
  print8.question = "Qui a peint 'La Joconde' ?";
  print8.answer1 = "Leonardo da Vinci";
  print8.answer2 = "Pablo Picasso";
  print8.answer3 = "Vincent van Gogh";
  print8.answer4 = "Raphael";
  print8.goodState = 1;
  printList[7] = print8;

  ToPrint print9;
  print9.question = "Quel est le plus long fleuve du monde ?";
  print9.answer1 = "Nil";
  print9.answer2 = "Amazone";
  print9.answer3 = "Mississippi";
  print9.answer4 = "Yangtsé";
  print9.goodState = 2;
  printList[8] = print9;

  ToPrint print10;
  print10.question = "Quel est l'organe pour respirer ?";
  print10.answer1 = "Le cœur";
  print10.answer2 = "Les poumons";
  print10.answer3 = "Le foie";
  print10.answer4 = "Le cerveau";
  print10.goodState = 2;
  printList[9] = print10;

  debug_log("le fils est initialisé");
  int_log("le fils a reçu l'ordre d'éxécuter le nombre de questions", nQues);

  //------------------------------ Déclaration des variables ------------------------------------- 
  
  int victory;
  int state = -1;
  int check = 0;
  int score = 0;

  //--------------------------------- Ouverture des pipes ----------------------------------------

  int toPrintPipe = open(PIPE_PRINT, O_WRONLY);
  int_log("le fils à ouvert le print: ", toPrintPipe); 
  int statePipe = open(PIPE_STATE, O_RDONLY);
  int_log("le fils ouvre le state : ", statePipe); 
  int resultPipe = open(PIPE_RES, O_WRONLY);
  int_log("le fils ouvre le result : ", resultPipe);
 
  if (resultPipe == -1 || statePipe == -1 || toPrintPipe == -1) {
    debug_log("erreur d'ouverture de 1/plusieurs pipes\n");
    return;
  }

  //--------------------------- Début de la boucle principale ------------------------------------
  
  // Cette boucle fait une itération par question, change le texte a afficher
  for (int actualQuestion = 0; actualQuestion<nQues; ++actualQuestion) {

    int_log("le fils execute la question numéro : ", actualQuestion);
    debug_log("le fils écrit");
    //ecrire la question à afficher
    check = write(toPrintPipe, &printList[actualQuestion], sizeof(ToPrint));
    int_log("le fils à écrit le print: ", check);
    debug_log(printList[actualQuestion].question);
    
    //cette boucle lit le state et agit lorsqu'une réponse est envoyée
    while (1) {
      debug_log("début de la boucle du fils");
    
      //lire l'état actuel
      check = read(statePipe, &state, sizeof(int));
      int_log("le fils lit le state : ", check); //blocant
      if (check == 0) {
        debug_log("erreur dans la lecture du state, state fermé ?");
        break;
      } 

      if (state == printList[actualQuestion].goodState) {
        //ecrire Victoire
        victory = 1;
      }
      else {
        victory = 0;
      }

      if (state != -1) {
        
        check = write(resultPipe, &victory, sizeof(int));
        score += victory;
        int_log("le fils écrit le result : ", check);
        state = -1;

        break;

      }
    }
    debug_log("le fils est sorti de la boucle while");
  }
  debug_log("le fils est sorti de la boucle for");

  //ecrire le score à afficher
  
  //----------------------------------- Initialisation de la shm ---------------------------------

  char* adr1;
  int status, cle = 5;

  if(( status = shmget(cle, 80*sizeof(char),
    IPC_CREAT|IPC_EXCL|0600))==-1) {
    debug_log(strerror(errno));
    exit(1);
  }

  if(( adr1 =(char*) shmat(status, NULL, 0)) == (char*)-1){
    debug_log(strerror(errno));
    exit(1);
  }
  debug_log("le fils écrit dans l'adresse");

  sprintf(adr1,"%d / %d ", score, nQues); 
  debug_log(adr1);


  if(shmdt(adr1) == -1) {
  debug_log(strerror(errno));
  exit(1);
  }

  debug_log("le fils a bien ecrit la shm");
  debug_log("attendre le père");
  check = read(statePipe, &state, sizeof(int));
  
  if (score * 2 >= nQues) {
    if (kill(getppid(), SIGUSR1) == -1) {
      debug_log("echec de l'envoie du signal");
      return;
    }
  }

  else {
    if (kill(getppid(), SIGUSR2) == -1) {
      debug_log("echec de l'envoie du signal");
      return;
    }
  }

  close(toPrintPipe);
  close(statePipe);
  close(resultPipe);

  debug_log("Fin du Fils ------------------------------------");
  exit(0);
}

