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
					break;
				case WHITE:
					player->cards[WHITE]++;
					break;
				case BLUE:
					player->cards[BLUE]++;
					break;
				case YELLOW:
					player->cards[YELLOW]++;
					break;
				case ORANGE:
					player->cards[ORANGE]++;
					break;
				case BLACK:
					player->cards[BLACK]++;
					break;
				case RED:
					player->cards[RED]++;
					break;
				case GREEN:
					player->cards[GREEN]++;
					break;
				case MULTICOLOR:
					player->cards[MULTICOLOR]++;
					break;
			}
		}

		for(int i=0; i<20; i++){
			player->objectives[i].city1 = 0;
			player->objectives[i].city2 = 0;
			player->objectives[i].score = 0;
		}
	}
}

void initTracks(t_track Tracks[50], int* arrayTracks, int nbTracks){
	for(int i=0; i<nbTracks; i++){
		Tracks[i].city1 = arrayTracks[5*i];
		Tracks[i].city2 = arrayTracks[5*i+1];
		Tracks[i].length = arrayTracks[5*i+2];
		Tracks[i].color1 = arrayTracks[5*i+3];
		Tracks[i].color2 = arrayTracks[5*i+4];
		Tracks[i].taken = -1;
	}
}

void createGame(t_game* game){
	/* Necessary to connect to server */
	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	char* name = "South_bot";
	/* Necessary to wait for a game and get map */
	char* gameType = "TRAINING PLAY_RANDOM timeout=10000 map=USA";

	connectToServer(serverName, port, name);
	waitForT2RGame(gameType, game->gameName, &game->gameBoard.nbCities, &game->gameBoard.nbTracks);

	game->gameBoard.arrayTracks = malloc(5*game->gameBoard.nbTracks*sizeof(int));
	game->player = getMap(game->gameBoard.arrayTracks, game->faceUp, game->players[0].initCards);

	initPlayer(&game->players[0],0); /* Us */
	initPlayer(&game->players[1],1); /* Opponent */
	initTracks(game->Tracks,game->gameBoard.arrayTracks, game->gameBoard.nbTracks);
}
