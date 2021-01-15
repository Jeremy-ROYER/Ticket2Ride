/*===================================================================
*
*    oO Update game T2R Oo
*
*====================================================================
*
* File : update.h
* Date : 11/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Header of update.c
*
*==================================================================*/

#ifndef __UPDATE_H__
#define __UPDATE_H__

void updateObjectives(t_player* player, int numPlayer, t_move* move, t_typeMove typeMove);

void updateBlindCard(t_player* player, int numPlayer, t_move* move);

void updateCard(t_game* game, t_move* move);

void updateRoute(t_game* game, t_move* move);

void updateGame(t_game* game, t_move* move);

#endif