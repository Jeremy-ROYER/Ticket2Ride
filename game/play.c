/*===================================================================
*
*    oO Fonction main T2R Oo
*
*====================================================================
*
* File : play.c
* Date : 30/11/2020
* Author : Royer Jérémy
* 
*====================================================================
*
* Enables to launch the game and play
*
*==================================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "TicketToRideAPI.h"
#include "structGame.h"
#include "createGame.h"
#include "move.h"
#include "update.h"
#include "autoMove.h"

int main(){
	/* For Tournament */
	/* Necessary to connect to server */
	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	char* name = "South_bot";
	/* Necessary to wait for a game and get map */
	connectToServer(serverName, port, name);
	
	/* For Tournament, infinity loop */
	/* Just remove it if you play simple game (1v1, one time) */
	while(1){
		/* data structure necessary for the creation of the game */
		t_game game;
		createGame(&game);

		t_return_code returnCode = NORMAL_MOVE;
		t_move move, lastMove;
		int replay = 0;
		t_color lastCard = NONE;

		int firstTurn = 1;
		
		while(returnCode == NORMAL_MOVE){ /* For single play (1v1) : returnCode == NORMAL_MOVE */
			printMap();

			/* Turn to player */
			if(game.player == 0){
				//askMove(&move);
				lastMove = move;
				firstTurn = chooseMove(&game, &move, &lastMove, replay, firstTurn);
				replay = needReplay(&move, lastCard);
				returnCode = playOurMove(&move, &lastCard);
			}

			/* Turn to opponent */
			else{
				returnCode = getMove(&move, &replay);
			}

			/* Update data of game if the game is not finish */
			if(returnCode !=  WINNING_MOVE && returnCode != LOOSING_MOVE)
				updateGame(&game, &move);

			/* the other player will play */
			if(returnCode == NORMAL_MOVE && !replay){
				game.player = !game.player;
			}
		}

		/* End of game message  */
		if( (returnCode ==  WINNING_MOVE && game.player == 0) || (returnCode == LOOSING_MOVE && game.player == 1) )
			printf("You won, well done !\n");
		else
			printf("The opponent won, unluck.. \n");

		free(game.gameBoard.arrayTracks);
	}
	closeConnection();

	//free(game.gameBoard.arrayTracks);

	return EXIT_SUCCESS;
}