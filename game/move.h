/*===================================================================
*
*    oO User's move T2R Oo
*
*====================================================================
*
* File : move.h
* Date : 11/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Header of move.c
*
*==================================================================*/

#ifndef __MOVE_H__
#define __MOVE_H__

void askMove(t_move* move);

t_return_code playOurMove(t_move* move, t_color* lastCard);

int needReplay(t_move* move, t_color lastCard);

#endif