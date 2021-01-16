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
	/* data structure necessary for the creation of the game */
	t_game game;
	createGame(&game);

	t_return_code returnCode = NORMAL_MOVE;
	t_move move, lastMove;
	int replay = 0;
	t_color lastCard = NONE;

	int firstTurn = 1;
	
	while(returnCode == NORMAL_MOVE){
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

		updateGame(&game, &move);

		/* the other player will play */
		if(returnCode == NORMAL_MOVE && !replay){
			game.player = !game.player;
		}

	}

	/* End of game message  */
	if( (returnCode ==  WINNING_MOVE && game.player == 0) || (returnCode == LOOSING_MOVE && game.player == 1) )
		printf("You won, bravo!\n");
	else
		printf("The opponent won, unluck.. \n");

	closeConnection();

	free(game.gameBoard.arrayTracks);

	return EXIT_SUCCESS;
}