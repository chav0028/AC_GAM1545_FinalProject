#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include "maze.h"
#include <Windows.h>
#include <string>//Used for blinkingColors();

void updateUI(CMaze maze, int numMoves);
void blinkingColors(std::string blinkingText);
int showMenu();
bool checkTimer(COORD position, CMaze &maze);
int chooseMaze();
unsigned char getKeyEventProc(KEY_EVENT_RECORD ker);
void playGame(int mazeNumber);
void showHelpMenu();
void showHelp();
void showControls();
void showLeaderboard(int mazeNumber);

#endif