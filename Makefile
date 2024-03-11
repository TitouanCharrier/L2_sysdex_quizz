#généré en partie par chat-gpt
# Nom du programme final
PROG_NAME = quiz

# Compilateur
CC = gcc

#Option de liens
LDFLAGS = -lncurses

# Options de compilation
CFLAGS = -g -Wall -Wextra -std=gnu99

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
	$(CC) $(OBJS) -o $(PROG_NAME) $(LDFLAGS)
	rm -f debug.log ./pipefolder/pipe_res ./pipefolder/pipe_print ./pipefolder/pipe_state

ext:
	gcc finalV.c -o finalV
	gcc finalD.c -o finalD


# Nettoyage des fichiers intermédiaires et de l'exécutable
clean:
	rm -f $(OBJS) $(PROG_NAME)


