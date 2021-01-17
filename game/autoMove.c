/*===================================================================
*
*    oO Automatic player move T2R Oo
*
*====================================================================
*
* File : autoMove.c
* Date : 15/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Allows our player to play automatically.
*
*==================================================================*/

#include <limits.h>

#include "TicketToRideAPI.h"
#include "structGame.h"
#include "move.h"
#include "autoMove.h"


/* 	D = the shortest distance between src and i
	Visit = indicates if the city has been visited */
int distanceMini(int D[50], int Visit[50], int nbCities){
	int min = INT_MAX;
	int indice_min;

	for(int i=0; i<nbCities; i++){
		if(Visit[i] == 0 && D[i] < min){
			min = D[i];
			indice_min = i;
		}
	}

	return indice_min;
}

/*  src = city of start
	dest = city of destination
	G = distance between two cities
	D = the shortest distance between src and i
	Prec = indicates from which city you have to come to go to i */
void findShortestWay(int src, int dest, int G[50][50], int D[50], int Prec[50], int nbCities){
	int Visit[50]; /* indicates if the city has been visited, 0 = FALSE and 1 = TRUE */
	int u = src;

	/* Initialization */
	for(int i=0; i<nbCities; i++){
		D[i] = INT_MAX;
		Visit[i] = 0; 
	}

	/* Starting from the source */
	D[src] = 0;

	/* Searching shortest way to go from src to dest */
	while(u != dest){
		/* Closest un-visited city can be found */
		u = distanceMini(D, Visit, nbCities);
		Visit[u] = 1;

		/* Updates each linked city */
		for(int i=0; i<nbCities; i++){
			if(Visit[i]==0 && G[u][i] != INT_MAX && (D[u] + G[u][i]) < D[i]){
				D[i] = D[u] + G[u][i];
				Prec[i] = u;
			}
		}
	}
}

int tracksToTake(int src, int dest, int Prec[50], t_game* game){
	int u = dest;
	int track;

	while(u != src){
		for(int i=0; i<game->gameBoard.nbTracks; i++){
			if((game->Tracks[i].city1==u && game->Tracks[i].city2==Prec[u])||(game->Tracks[i].city2==u && game->Tracks[i].city1==Prec[u])){
				track = i;
			}
		}

		if(game->Tracks[track].taken == -1){
			game->neededTracks[game->nbNeeded]= game->Tracks[track];
			game->nbNeeded++;
		}

		u = Prec[u];
	}
	return game->nbNeeded;
}

void chooseObj(t_game* game, t_move* move, t_move* lastMove){
	int D[50], Prec[50], lengthTrackObj[3], scoreObj[3], choose[3];

	for(int i=0; i<3; i++){
		int src = lastMove->drawObjectives.objectives[i].city1;
		int dest = lastMove->drawObjectives.objectives[i].city2;
		findShortestWay(src, dest, game->lengthTracks, D, Prec, game->gameBoard.nbCities);
		game->nbNeeded = tracksToTake(src, dest, Prec, game);
		lengthTrackObj[i] = D[dest];
		scoreObj[i] = lastMove->drawObjectives.objectives[i].score;
	}

	if(game->players[0].nbObjectives == 3){
		for(int i=0; i<3; i++){
			if(lastMove->drawObjectives.objectives[i].city1 == 35 || lastMove->drawObjectives.objectives[i].city2 == 35)
				choose[i] = 0;
			else
				choose[i] = 1;
		}

		if(choose[0]+choose[1]+choose[2]<2)
			choose[0] = choose[1] = choose[2] = 1;
	}

	else{
		for(int i=0; i<3; i++){
			if(lastMove->drawObjectives.objectives[i].city1 == 35 || lastMove->drawObjectives.objectives[i].city2 == 35)
				choose[i] = 0;
			else if(lengthTrackObj[i] < 15 && game->players[0].nbWagons > 20 && game->players[1].nbWagons > 20)
				choose[i] = 1;
			else if(lengthTrackObj[i] < 10 && game->players[0].nbWagons > 15 && game->players[1].nbWagons > 15)
				choose[i] = 1;
			else if(lengthTrackObj[i] < 6 && game->players[0].nbWagons > 10 && game->players[1].nbWagons > 10)
				choose[i] = 1;
			else
				choose[i] = 0;
		}

		if(choose[0]+choose[1]+choose[2] == 3){
			if(scoreObj[0]<scoreObj[1]){
				if(scoreObj[0]<scoreObj[2])
					choose[0] = 0;
				else
					choose[2] = 0;
			}
			else{
				if(scoreObj[1]<scoreObj[2])
					choose[1] = 0;
				else
					choose[2] = 0;
			}
		}

		else if(choose[0]+choose[1]+choose[2] == 0){
			if(scoreObj[0]<scoreObj[1]){
				if(scoreObj[0]<scoreObj[2])
					choose[0] = 1;
				else
					choose[2] = 1;
			}
			else{
				if(scoreObj[1]<scoreObj[2])
					choose[1] = 1;
				else
					choose[2] = 1;
			}
		}
	}

	for(int i=0; i<3; i++)
		move->chooseObjectives.chosen[i] = choose[i];
}

t_move chooseDraw(t_game* game, int replay){
	t_move retMove;

	for(int j=0; j<2; j++){
		if(game->players[0].cards[game->neededTracks[j].color1] >= game->players[0].cards[game->neededTracks[j].color2]){
			for(int i=0; i<5; i++){
				if( !(game->faceUp[i] == MULTICOLOR /*&& replay == 1*/) ){
					if(game->neededTracks[j].color1 == game->faceUp[i]){
						retMove.type = DRAW_CARD;
						retMove.drawCard.card = game->faceUp[i];
						return retMove;
					}
				}
			}
		}

		else{
			for(int i=0; i<5; i++){
				if( !(game->faceUp[i] == MULTICOLOR /*&& replay == 1*/) ){
					if(game->neededTracks[j].color2 == game->faceUp[i]){
						retMove.type = DRAW_CARD;
						retMove.drawCard.card = game->faceUp[i];
						return retMove;
					}
				}
			}
		}
	}

	if(game->players[0].cards[MULTICOLOR] < 2){
		for(int i=0; i<5; i++){
			if(game->faceUp[i] == MULTICOLOR && replay == 0){
				int colorMax = 0;
				for(int i=0; i<9; i++){
					if(game->players[0].cards[i] > colorMax)
						colorMax = game->players[0].cards[i];
				}
				if(colorMax > 3 && colorMax < 6){
					retMove.type = DRAW_CARD;
					retMove.drawCard.card = game->faceUp[i];
					return retMove;
				}
			}
		}
	}

	retMove.type = DRAW_BLIND_CARD;
	return retMove;
}

void chooseClaim(t_track track, t_move* move, int color, int nbLoco){
	move->type = CLAIM_ROUTE;
	move->claimRoute.city1 = track.city1;
	move->claimRoute.city2 = track.city2;
	move->claimRoute.color = color;
	move->claimRoute.nbLocomotives = nbLoco;
}

int chooseMove(t_game* game, t_move* move, t_move* lastMove, int replay, int firstTurn){
	int claim=0;
	int D[50], Prec[50];

	if(firstTurn == 1){
		move->type = DRAW_OBJECTIVES;
		firstTurn = 0;
	}

	else{
		if(replay == 1){
			if(lastMove->type == DRAW_OBJECTIVES){
				move->type = CHOOSE_OBJECTIVES;
				chooseObj(game, move, lastMove);
			}

			else if(lastMove->type == DRAW_BLIND_CARD || lastMove->type == DRAW_CARD){
				*move = chooseDraw(game, replay);
			}
		}

		else{
			game->nbNeeded = 0;

			for(int i=0; i<game->players[0].nbObjectives; i++){
				int src = game->players[0].objectives[i].city1;
				int dest = game->players[0].objectives[i].city2;
				findShortestWay(src, dest, game->lengthTracks, D, Prec, game->gameBoard.nbCities);
				game->nbNeeded = tracksToTake(src, dest, Prec, game);
			}

			if((game->nbNeeded == 1 && game->players[1].nbWagons > 15 && game->players[0].nbWagons > 15)|| (game->nbNeeded == 0 && game->players[1].nbWagons > 10 && game->players[0].nbWagons > 10)){
				move->type = DRAW_OBJECTIVES;
			}

			else{
				for(int i=0; i<game->nbNeeded; i++){
					if(claim == 0){
						if(game->neededTracks[i].color1 != MULTICOLOR && game->neededTracks[i].length <= game->players[0].nbWagons){
							if(game->neededTracks[i].length <= game->players[0].cards[game->neededTracks[i].color1]){
								chooseClaim(game->neededTracks[i], move, game->neededTracks[i].color1, 0);
								claim = 1;
								break;
							}

							else if(game->neededTracks[i].length <= game->players[0].cards[game->neededTracks[i].color2]){
								chooseClaim(game->neededTracks[i], move, game->neededTracks[i].color2, 0);
								claim = 1;
								break;
							}

							else if((game->neededTracks[i].length > 3 || game->players[0].cards[MULTICOLOR] > 3) && game->neededTracks[i].length <= (game->players[0].cards[game->neededTracks[i].color1] + game->players[0].cards[MULTICOLOR])){
								chooseClaim(game->neededTracks[i], move, game->neededTracks[i].color1, game->neededTracks[i].length-game->players[0].cards[game->neededTracks[i].color1]);
								claim = 1;
								break;
							}

							else if((game->neededTracks[i].length > 3 || game->players[0].cards[MULTICOLOR] > 3) && game->neededTracks[i].length <= (game->players[0].cards[game->neededTracks[i].color2] + game->players[0].cards[MULTICOLOR])){
								chooseClaim(game->neededTracks[i], move, game->neededTracks[i].color2, game->neededTracks[i].length-game->players[0].cards[game->neededTracks[i].color2]);
								claim = 1;
								break;
							}
						}

						else{
							for(int j=0; j<9; j++){
								if(game->neededTracks[i].length <= game->players[0].cards[j] && game->neededTracks[i].length <= game->players[0].nbWagons){
									chooseClaim(game->neededTracks[i], move, j, 0);
									claim = 1;
									break;
								}
								else if((game->neededTracks[i].length > 3 || game->players[0].cards[MULTICOLOR] > 3) && game->neededTracks[i].length <= (game->players[0].cards[j]+game->players[0].cards[MULTICOLOR]) && game->players[0].cards[j] > 0 && game->neededTracks[i].length <= game->players[0].nbWagons){
									chooseClaim(game->neededTracks[i], move, j, game->neededTracks[i].length-game->players[0].cards[j]);
									claim = 1;
									break;
								}
							}
						}
					}
				}

				if(claim == 0){
					if(game->players[0].nbWagons < 6 || (game->nbNeeded == 0 && game->players[0].nbWagons < 11)){
						for(int k=1; k<6; k++){
							for(int i=0; i<9; i++){
								if(game->players[0].cards[i] == k && game->neededTracks[0].color1 != i && game->neededTracks[0].color2 != i){
									for(int j=0; j<game->gameBoard.nbCities; j++){
										if(game->Tracks[j].length == k && game->Tracks[j].color1 == i && game->Tracks[j].taken == -1 && game->Tracks[j].length <= game->players[0].nbWagons){
											chooseClaim(game->Tracks[j], move, i, 0);
											claim = 1;
											break;
										}
									}
								}
							}
						}
					}
					
					else{
						for(int i=0; i<9; i++){
							if(game->players[0].cards[i] == 6 && game->neededTracks[0].color1 != i && game->neededTracks[0].color2 != i){
								for(int j=0; j<game->gameBoard.nbCities; j++){
									if(game->Tracks[j].length == 6 && game->Tracks[j].color1 == i && game->Tracks[j].taken == -1 && game->Tracks[j].length <= game->players[0].nbWagons){
										chooseClaim(game->Tracks[j], move, i, 0);
										claim = 1;
										break;
									}
								}
							}
						}
					}
				}

				if(claim == 0 && game->players[1].nbWagons < 3){
					int colorMax = 0;
					int indice_colorMax = 0;
					int withLoco, Locos;
					int color2e, indice_color2e;
					
					for(int i=0; i<9; i++){
						if(game->players[0].cards[i] > colorMax && game->players[0].nbWagons >= game->players[0].cards[i]){
							color2e = colorMax;
							colorMax = game->players[0].cards[i];
							indice_color2e = indice_colorMax;
							indice_colorMax = i;
						}
					}

					for(int j=0; j<game->gameBoard.nbCities; j++){
						if(game->Tracks[j].length == color2e && (game->Tracks[j].color1 == indice_color2e || game->Tracks[j].color1 == MULTICOLOR) && game->Tracks[j].taken == -1 && game->Tracks[j].length <= game->players[0].nbWagons){
							chooseClaim(game->Tracks[j], move, indice_color2e, 0);
							claim = 1;
							break;
							}
					}

					withLoco = color2e + game->players[0].cards[MULTICOLOR];
					if(withLoco > 6)
						withLoco = 6;

					for(int j=0; j<game->gameBoard.nbCities; j++){
						if(game->Tracks[j].length == withLoco && (game->Tracks[j].color1 == indice_colorMax || game->Tracks[j].color1 == MULTICOLOR) && game->Tracks[j].taken == -1 && game->Tracks[j].length <= game->players[0].nbWagons){
							Locos = game->Tracks[j].length - color2e;
							if( Locos < 0)
								Locos = 0;
							chooseClaim(game->Tracks[j], move, indice_color2e, Locos);
							claim = 1;
							break;
						}
					}

					for(int j=0; j<game->gameBoard.nbCities; j++){
						if(game->Tracks[j].length == colorMax && (game->Tracks[j].color1 == indice_colorMax || game->Tracks[j].color1 == MULTICOLOR) && game->Tracks[j].taken == -1 && game->Tracks[j].length <= game->players[0].nbWagons){
							chooseClaim(game->Tracks[j], move, indice_colorMax, 0);
							claim = 1;
							break;
						}
					}

					withLoco = colorMax + game->players[0].cards[MULTICOLOR];
					if(withLoco > 6)
						withLoco = 6;

					for(int j=0; j<game->gameBoard.nbCities; j++){
						if(game->Tracks[j].length == withLoco && (game->Tracks[j].color1 == indice_colorMax || game->Tracks[j].color1 == MULTICOLOR) && game->Tracks[j].taken == -1 && game->Tracks[j].length <= game->players[0].nbWagons){
							Locos = game->Tracks[j].length - colorMax;
							if( Locos < 0)
								Locos = 0;
							chooseClaim(game->Tracks[j], move, indice_colorMax, Locos);
							claim = 1;
							break;
						}
					}		
				}

				if(claim == 0)
					*move = chooseDraw(game, replay);
			}
		}
	}

	return firstTurn;
}