/***************************************************************************************
Title:    	Assignment 8
Course:   	GAM1545 Programming Fundamentals 1
Section:  	4
Author:   	Alvaro Chavez Mixco
Date:     	2014/12/03
Description: In this final assignment you will create a simple “semi-real-time” maze
game that displays and plays in the console window.
*****************************************************************************************/

#include "maze.h"
#include "player.h"
#include "functions.h"
#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

int g_ticks = 0;//Ticks elapse
int g_tickResolution = 100; //in msecs
bool g_isThreadRunning = false;

//Code given on class, no function header.
void handleTimer()
{
    g_ticks++;//Increases ticks.
}

//Code given on class, no function header.
unsigned int __stdcall TimerThread(void *p_thread_data)
{
    HANDLE event_handle = CreateEvent(NULL, FALSE, FALSE, "my_handle");

    while (g_isThreadRunning == true)//Do while the thread is running
    {

        switch (WaitForSingleObject(event_handle, g_tickResolution))
        {
        case WAIT_TIMEOUT:
            handleTimer();
            break;
        }

    }//Note there is a empty line after and before each loop and switch to prevent confusion with the brackets.

    return 0;//Kills timer
}

int main()
{
    int playerChoice;
    int mazeNumber;
    CONSOLE_CURSOR_INFO info;

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdout, &info);

    //Create thread.
    unsigned int thread_id = 0;
    g_isThreadRunning = true;//Runs the timer thread
    _beginthreadex(NULL, 0, TimerThread, NULL, 0, &thread_id);

    do
    {
        system("color 3F");
        playerChoice = showMenu();//Shows the menu and lets the player choose what to do

        switch (playerChoice)
        {
        case 1:
            mazeNumber = chooseMaze();
            if (mazeNumber != 0)//if mazeNumber==0 it goes back to main menu, else it runs the game.
            {
                //Turns cursor off
                info.bVisible = false;
                SetConsoleCursorInfo(hStdout, &info);

                playGame(mazeNumber);//Loads playGame function according to the maze chosen by the player.

                //Turns cursor on
                info.bVisible = true;
                SetConsoleCursorInfo(hStdout, &info);
            }
            break;
        case 2:
            showHelpMenu();
            break;
        case 3:
            do//Loops so that after user views a leaderboard user goes back to chooseMaze (leaderboad) menu instead of main menu.
            {
                system("color E1");
                mazeNumber = chooseMaze();
                if (mazeNumber != 0)//ShowLeaderboard if player doesn't decides to go back to main menu.
                {
                    showLeaderboard(mazeNumber);
                }
            } while (mazeNumber != 0);
            break;
        case 0:
            //Message for quitting the program.
            system("cls");
            cout << "Quitting the game..." << endl;
            cout << endl;
            system("pause");
            break;
        }

    } while (playerChoice != 0);

    g_isThreadRunning = false;//Finish the timer thread
    return 0;
}