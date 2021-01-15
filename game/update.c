/*===================================================================
*
*    oO Update game T2R Oo
*
*====================================================================
*
* File : update.c
* Date : 11/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Allows to update the game data after a player's move.
*
*==================================================================*/

#include "TicketToRideAPI.h"
#include "structGame.h"
#include "update.h"

void updateObjectives(t_player* player, int numPlayer, t_move* move, t_typeMove typeMove){
	if(typeMove == DRAW_OBJECTIVES){
		if(numPlayer == 0){
			for(int i=0; i<3; i++)
				player->objectives[i+player->nbObjectives] = move->drawObjectives.objectives[i];
			player->nbObjectives = player->nbObjectives + 3;
		}
	}

	else if(typeMove == CHOOSE_OBJECTIVES){
		if(numPlayer == 0){
			for(int i=0; i<5; i=i+2){
				if(!move->chooseObjectives.chosen[i]){
					player->objectives[player->nbObjectives+i-3].city1 = 0;
					player->objectives[player->nbObjectives+i-3].city2 = 0;
					player->objectives[player->nbObjectives+i-3].score = 0;
					for(int j=(player->nbObjectives+i-3); j<player->nbObjectives ; j++)
						player->objectives[j] = player->objectives[j+1];
					player->nbObjectives--;
				}
			}
		}
		else{
			for(int i=0; i<3; i++){
				if(!move->chooseObjectives.chosen[i])
					player->nbObjectives++;
			}
		}
	}
}

void updateBlindCard(t_player* player, int numPlayer, t_move* move){
	if(numPlayer == 1)
		player->nbCards++;
	else{
		player->nbCards++;
		player->cards[move->drawBlindCard.card]++;
	}
}

void updateCard(t_game* game, t_move* move){
	game->players[game->player].nbCards++;
	for(int i=0; i<5; i++)
		game->faceUp[i] = move->drawCard.faceUp[i];
}

void updateRoute(t_game* game, t_move* move){
	for(int i=0; i<(game->gameBoard.nbTracks); i++){
		if(game->Tracks[i].city1 == move->claimRoute.city1 && game->Tracks[i].city2 == move->claimRoute.city2){
			game->players[game->player].nbCards = game->players[game->player].nbCards - game->Tracks[i].length;
			game->players[game->player].nbWagons = game->players[game->player].nbWagons - game->Tracks[i].length;
			if(game->player == 0){
				game->Tracks[i].taken = 0;
				game->players[0].cards[move->claimRoute.color] = game->players[0].cards[move->claimRoute.color] - (game->Tracks[i].length - move->claimRoute.nbLocomotives);
				game->players[0].cards[MULTICOLOR] = game->players[0].cards[MULTICOLOR] - move->claimRoute.nbLocomotives;
			}
			else
				game->Tracks[i].taken = 1;
		}
	}
}

void updateGame(t_game* game, t_move* move){
	switch(move->type){
		case CLAIM_ROUTE :
			updateRoute(game, move);
			break;
		case DRAW_BLIND_CARD :
			updateBlindCard(&game->players[game->player], game->player, move);
			break;
		case DRAW_CARD :
			updateCard(game, move);
			break;
		case DRAW_OBJECTIVES :
		case CHOOSE_OBJECTIVES :
			updateObjectives(&game->players[game->player], game->player, move, move->type);
			break;
	}
}