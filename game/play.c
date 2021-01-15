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

int main(){
	/* data structure necessary for the creation of the game */
	t_game game;
	createGame(&game);

	t_return_code returnCode = NORMAL_MOVE;
	t_move move;
	int replay = 0;
	t_color lastMove = NONE;
	
	while(returnCode == NORMAL_MOVE){
		printMap();

		/* Turn to player */
		if(game.player == 0){
			askMove(&move);
			replay = needReplay(&move, lastMove);
			returnCode = playOurMove(&move, &lastMove);
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

	free(game.gameBoard.arrayTracks);
	closeConnection();

	return EXIT_SUCCESS;
}