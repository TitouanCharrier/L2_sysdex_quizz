
# Nom du programme final
PROG_NAME = main

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c99

# Listes des fichiers sources
SRCS = Father.c Son.c main.c

# Listes des fichiers objets générés
OBJS = $(SRCS:.c=.o)

# Cible par défaut
all: $(PROG_NAME)

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Création de l'exécutable
$(PROG_NAME): $(OBJS)
	$(CC) $(OBJS) -o $(PROG_NAME)

# Nettoyage des fichiers intermédiaires et de l'exécutable
clean:
	rm -f $(OBJS) $(PROG_NAME)
