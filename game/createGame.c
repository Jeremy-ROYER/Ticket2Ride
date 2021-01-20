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

#include <limits.h>
#include "TicketToRideAPI.h"
#include "structGame.h"


void initPlayer(t_player* player, int playerNum){
	player->nbWagons = 45;
	player->nbCards = 4;
	player->nbObjectives = 0;

	/* If playerNum=0 it means it's us */
	if(playerNum == 0){
		for(int i=0; i<10; i++)
			player->cards[i] = 0;

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

void initTracks(t_track Tracks[50], int lengthTracks[50][50], t_gameBoard* gameBoard){
	for(int i=0; i<gameBoard->nbTracks; i++){
		Tracks[i].city1 = gameBoard->arrayTracks[5*i];
		Tracks[i].city2 = gameBoard->arrayTracks[5*i+1];
		Tracks[i].length = gameBoard->arrayTracks[5*i+2];
		Tracks[i].color1 = gameBoard->arrayTracks[5*i+3];
		Tracks[i].color2 = gameBoard->arrayTracks[5*i+4];
		Tracks[i].taken = -1;
	}

	for(int i=0; i<gameBoard->nbCities; i++){
		for(int j=0; j<gameBoard->nbCities; j++){
			lengthTracks[i][j] = INT_MAX;
			for(int k=0; k<gameBoard->nbTracks; k++){
				if( (Tracks[k].city1 == i && Tracks[k].city2 == j) || (Tracks[k].city1 == j && Tracks[k].city2 == i) )
					lengthTracks[i][j] = Tracks[k].length;
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
	//char* gameType = "TOURNAMENT TEST"; /* TRAINING NICE_BOT timeout=10000 map=USA */
	//connectToServer(serverName, port, name);

	char* gameType = "TOURNAMENT EISE3"; /* "TRAINING NICE_BOT timeout=10000 map=USA" for training alone ; "" to play against one player one time ; "TOURNAMENT <name>" to join and play in tournament */
	waitForT2RGame(gameType, game->gameName, &game->gameBoard.nbCities, &game->gameBoard.nbTracks);

	game->gameBoard.arrayTracks = malloc(5*game->gameBoard.nbTracks*sizeof(int));
	game->player = getMap(game->gameBoard.arrayTracks, game->faceUp, game->players[0].initCards);

	game->nbNeeded = 0;
	initPlayer(&game->players[0],0); /* Us */
	initPlayer(&game->players[1],1); /* Opponent */
	initTracks(game->Tracks, game->lengthTracks, &game->gameBoard);
}
