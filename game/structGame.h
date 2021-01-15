/*===================================================================
*
*    oO data structure for T2R Oo
*
*====================================================================
*
* File : structGame.h
* Date : 10/01/2021
* Author : Royer Jérémy
* 
*====================================================================
*
* Contains data structures to make the game clearer
*
*==================================================================*/

#ifndef __STRUCTGAME_H__
#define __STRUCTGAME_H__

#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"

typedef struct{
	int city1, city2;
	int length;
	t_color color1,color2;
	int taken;
}t_track;

typedef struct{
	int nbCities;
	int nbTracks;
	int* arrayTracks;
}t_gameBoard;

typedef struct{
	int nbWagons;
	int nbCards;
	t_color initCards[4];
	int cards[10]; /* number of cards of each color, there are 10 colors */
	int nbObjectives;
	t_objective objectives[20];
}t_player;

typedef struct{
	char gameName[50];
	int player;
	t_color faceUp[5];
	t_gameBoard gameBoard;
	t_player players[2];
	t_track Tracks[50];
}t_game;

#endif