/*===================================================================
*
*    oO Create game T2R Oo
*
*====================================================================
*
* File : createGame.h
* Date : 10/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Header of createGame.c
*
*==================================================================*/

#ifndef __CREATEGAME_H__
#define __CREATEGAME_H__

void initPlayer(t_player* player, int playerNum);

void initTracks(t_track Tracks[50], int lengthTracks[50][50], t_gameBoard* gameBoard);

void createGame(t_game* game);
	
#endif