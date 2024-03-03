#include "main.h"

void mainSon(int statePipe[], int toPrintPipe[], int* mainMem, int resultPipe[], int nQues) {

  debug_log("début du fils");
  //key_t key = ftok(const char ".", int "SysDexTpNote");
  //int mainMem_v = shmget(key_t key, size_t 1024, IPC_CREAT | 0666);
  //int* mainMem = shmat(int mainMem_v, const void NULL, int 0);
  
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
  print2.answer2 = "Indien";
  print2.answer3 = "Pacifique";
  print2.answer4 = "Arctique";
  print2.goodState = 3;
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
  print6.answer2 = "6";
  print6.answer3 = "7";
  print6.answer4 = "9";
  print6.goodState = 3;
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
  print8.answer1 = "Pablo Picasso";
  print8.answer2 = "Vincent van Gogh";
  print8.answer3 = "Leonardo da Vinci";
  print8.answer4 = "Raphael";
  print8.goodState = 3;
  printList[7] = print8;

  ToPrint print9;
  print9.question = "Quel est le plus long fleuve du monde ?";
  print9.answer1 = "Nil";
  print9.answer2 = "Mississippi";
  print9.answer3 = "Amazone";
  print9.answer4 = "Yangtsé";
  print9.goodState = 3;
  printList[8] = print9;

  ToPrint print10;
  print10.question = "Quel est l'organe principal du système respiratoire ?";
  print10.answer1 = "Le cœur";
  print10.answer2 = "Les poumons";
  print10.answer3 = "Le foie";
  print10.answer4 = "Le cerveau";
  print10.goodState = 2;
  printList[9] = print10;

  debug_log("le fils est initialisé");
  int_log("le fils a reçu l'ordre d'éxécuter le nombre de questions", nQues);

  int state = -1;
  int check;
  
  for (int actualQuestion = 0; actualQuestion<nQues; ++actualQuestion) {

    int_log("le fils execute la question numéro : ", actualQuestion);
    debug_log("le fils écrit");
    //ecrire la question à afficher
    close(toPrintPipe[0]);
    check = write(toPrintPipe[1], &printList[actualQuestion], sizeof(ToPrint));
    int_log("le fils à écrit : ", check);
    close(toPrintPipe[1]);

    debug_log("le fils finit d'écrire");
    debug_log(printList[actualQuestion].question);

    while (1) {
      usleep(100000);
    
      //lire l'état actuel
      close(statePipe[1]);
      read(statePipe[0], &state, sizeof(int));
      close(statePipe[0]);
    
      if (state == printList[actualQuestion].goodState) {
        //ecrire Victoire
        int victory = 1;
        state = -1;

        close(resultPipe[0]);
        write(resultPipe[1], &victory, sizeof(int));
        close(resultPipe[1]);

        close(statePipe[0]);
        write(statePipe[1], &state, sizeof(int));
        close(statePipe[1]);
        break;
      }
      else if(state != -1) {
        //ecrire Défaite
        int defeat = 0;
        state = -1;

        close(resultPipe[0]);
        write(resultPipe[1], &defeat, sizeof(int));
        close(resultPipe[1]);

        close(statePipe[0]);
        write(statePipe[1], &state, sizeof(int));
        close(statePipe[1]);

        break;

      }
    }
    debug_log("le fils est sorti de la boucle while");
  }
  debug_log("le fils est sorti de la boucle for");
}

