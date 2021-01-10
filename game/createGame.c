/*===================================================================
*
*    oO Create game T2R Oo
*
*====================================================================
*
* File : createGame.c
* Date : 10/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Allows to create the game (server, board, ...)
*
*==================================================================*/

#include "TicketToRideAPI.h"
#include "structGame.h"


void initPlayer(t_player* player, int playerNum){
	player->nbWagons = 45;
	player->nbCards = 4;
	player->nbObjectives = 0;

	/* If playerNum=0 it means it's us */
	if(playerNum == 0){
		for(int i=0; i<4; i++){
			switch(player->initCards[i]){
				case PURPLE:
					player->cards[PURPLE]++;
				case WHITE:
					player->cards[WHITE]++;
				case BLUE:
					player->cards[BLUE]++;
				case YELLOW:
					player->cards[YELLOW]++;
				case ORANGE:
					player->cards[ORANGE]++;
				case BLACK:
					player->cards[BLACK]++;
				case RED:
					player->cards[RED]++;
				case GREEN:
					player->cards[GREEN]++;
				case MULTICOLOR:
					player->cards[MULTICOLOR]++;
			}
		}
	}
}

void createGame(t_game* game){
	/* Necessary to connect to server */
	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	char* name = "South_bot";
	/* Necessary to wait for a game and get map */
	char* gameType = "TRAINING DO_NOTHING timeout=10000 map=USA";

	connectToServer(serverName, port, name);
	waitForT2RGame(gameType, game->gameName, &game->gameBoard.nbCities, &game->gameBoard.nbTracks);

	game->gameBoard.arrayTracks = malloc(5*game->gameBoard.nbTracks*sizeof(int));
	game->player = getMap(game->gameBoard.arrayTracks, game->faceUp, game->players[0].initCards);

	initPlayer(&game->players[0],0); /* Us */
	initPlayer(&game->players[1],1); /* Opponent */	
}
