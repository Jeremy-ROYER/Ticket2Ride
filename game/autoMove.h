/*===================================================================
*
*    oO Automatic player move T2R Oo
*
*====================================================================
*
* File : autoMove.h
* Date : 15/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Header of autoMove.c
*
*==================================================================*/

#ifndef __AUTO_MOVE_H__
#define __AUTO_MOVE_H__

int distanceMini(int D[50], int Visit[50], int nbCities);

void findShortestWay(int src, int dest, int G[50][50], int D[50], int Prec[50], int nbCities);

int tracksToTake(int src, int dest, int Prec[50], t_game* game);

void chooseObj(t_game* game, t_move* move, t_move* lastMove);

t_move chooseDraw(t_game* game, int replay);

void chooseClaim(t_track track, t_move* move, int color, int nbLoco);

int chooseMove(t_game* game, t_move* move, t_move* lastMove, int replay, int firstTurn);

#endif