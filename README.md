# L2_sysdex_quizz
Voici le travail demandé au second semestre de L2. 
Ce project traite de la communication inter-processus.

Ce projet est un quiz. 

Il requière la bibliothèque NCurses


Archlinux et dérivés :

$ sudo pacman -S ncurses


Fedora, RHEL, CentOS, AlmaLinux, Rocky Linux:

$ sudo yum install ncurses-devel
ou
$ sudo dnf install ncurses-devel


Debian, Ubuntu, Linux Mint, Pop OS:

$ sudo apt install libncurses5-dev libncursesw5-dev


Après l'installation de NCurses, le programme se compile avec

$ make

Vous pouvez également utiliser l'éxécutable fourni sous le nom de 

$ ./quiz

vous pouvez utiliser les arguments :
 -h afficher l'aide
 -r afficher les règles
 -s afficher les créateurs
 -a définir le nombre de réponses [2 - 4]
 -q définir le nombre de questions [4 - 10]
