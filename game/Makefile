#--------------------------------------------------------------------
#
#File : Makefile
#Date : 30/11/2020
#Author : ROYER Jérémy
#
#--------------------------------------------------------------------
#--------------------------------------------------------------------

# options de compilation
CC = gcc
CCFLAGS = -Wall
LIBS = 
LIBSDIR = 
PATHAPI = ../API/

# fichiers du projet
SRC = clientAPI.c TicketToRideAPI.c createGame.c move.c update.c play.c
OBJ = $(SRC:.c=.o)
EXEC = run.out


# règle initiale
all: $(EXEC)

# dépendance des .h
clientAPI.o: $(PATHAPI)clientAPI.h
TicketToRideAPI.o: $(PATHAPI)clientAPI.h $(PATHAPI)TicketToRideAPI.h
createGame.o: $(PATHAPI)TicketToRideAPI.h structGame.h createGame.h
move.o: $(PATHAPI)TicketToRideAPI.h move.h
update.o: $(PATHAPI)TicketToRideAPI.h structGame.h update.h
play.o: $(PATHAPI)TicketToRideAPI.h $(PATHAPI)clientAPI.h structGame.h createGame.h move.h update.h

# règles de compilation
clientAPI.o: $(PATHAPI)clientAPI.c
	$(CC) $(CCFLAGS) -o $@ -c $(PATHAPI)$<

TicketToRideAPI.o: $(PATHAPI)TicketToRideAPI.c
	$(CC) $(CCFLAGS) -o $@ -c $(PATHAPI)$<

createGame.o: createGame.c
	$(CC) $(CCFLAGS) -o $@ -c $< -I$(PATHAPI)

move.o: move.c
	$(CC) $(CCFLAGS) -o $@ -c $< -I$(PATHAPI)

update.o: update.c
	$(CC) $(CCFLAGS) -o $@ -c $< -I$(PATHAPI)

play.o: play.c
	$(CC) $(CCFLAGS) -o $@ -c $< -I$(PATHAPI)
	
# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS) $(LIBSDIR)

# règles supplémentaires
clean:
	rm -f *.o
mproper:
	rm -f $(EXEC) *.o