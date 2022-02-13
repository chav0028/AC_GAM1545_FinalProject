#include "functions.h"
#include "maze.h"
#include "player.h"
#include "leaderboard.h"
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <string>

using namespace std;

extern int g_ticks;//Extern, compiler looks for variables in external (another file in the solution) file
extern int g_tickResolution;

/**************************************************
updateUI
Description: Updates and displays the user interface
(timer).
Arguments:  Cmaze maze-Gets the maze so that it can know
the size of it and display the UI properly at the bottom
of it.
int numMoves-The number of moves made by the player.
It also makes use of global variables g_ticks and g_tickResolution
for the display of the timer.
Return: Void
Date: 2014/11/21
Author: Alvaro Chavez Mixco
***************************************************/
void updateUI(CMaze maze, int numMoves)//Displays user interface
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;

    pos.X = 0;
    pos.Y = maze.getRows() + 1;//Value depends on size of maze.
    SetConsoleCursorPosition(hConsole, pos);
    int secs = (g_ticks*g_tickResolution) / 1000; //Pass msecs to secs.
    cout << "Time:" << secs;
    cout << "  Moves: " << numMoves;
    cout << "  Press ""Esc"" to quit the game.";
}

/**************************************************
blinkingColors
Description: Makes a word have blinking effects and
displays it multiple times.
Arguments:  string with the text that will blink.
Return: Void
Date: 2014/11/28
Author: Alvaro Chavez Mixco
***************************************************/
void blinkingColors(string blinkingText)
{
    int count = 0;

    while (count < 2)//Make text appear multiple times with colors blinking.
    {
        cout << blinkingText << endl;
        system("color CA");
        Sleep(500);
        cout << blinkingText << endl;
        system("color C5");
        cout << blinkingText << endl;
        Sleep(500);
        cout << blinkingText << endl;
        system("color CE");
        cout << blinkingText << endl;
        Sleep(500);
        cout << blinkingText << endl;
        system("color CB");
        cout << blinkingText << endl;
        count++;
    }

}

/**************************************************
showMenu
Description: Displays the main menu for the Maze game.
Arguments:  Void
Return: Integer of the choice made by the user.
Date: 2014/11/17
Author: Alvaro Chavez Mixco
***************************************************/
int showMenu()
{
    int playerChoice;

    do
    {
        system("cls");

        //Display menu
        cout << "\t\t   Maze" << endl;
        cout << "\t\tMain Menu" << endl;
        cout << "1. Play Game" << endl;
        cout << "2. Help & Options" << endl;
        cout << "3. Leaderboards" << endl;
        cout << "0. Exit Game" << endl;
        cin >> playerChoice;

        if (playerChoice < 0 || playerChoice > 3) //Invalid option message
        {
            cout << endl;
            PlaySound(TEXT("invalidOption.wav"), NULL, SND_FILENAME | SND_NODEFAULT);
            cout << endl;
            cout << "Invalid choice" << endl;
            cout << endl;
            system("pause");
        }
    } while (playerChoice < 0 || playerChoice > 3);//Check if the user choice is valid

    if (playerChoice == 0)
    {
        PlaySound(TEXT("quit.wav"), NULL, SND_FILENAME | SND_NODEFAULT); //Special sound if user exits the game.
    }

    return playerChoice;
}

/**************************************************
checkTimer
Description: Checks if the tile the player is trying to
move is the special tile timer and if it is, substracts 3 seconds
from timer
Arguments:  COORD position-The position the player is trying
to move to.
CMaze maze-The maze the player is in.
Return: bool-false if it isn't the timer tile, true if
it is.
Date: 2014/11/28
Author:Alvaro Chavez Mixco
***************************************************/
bool checkTimer(COORD position, CMaze &maze)
{

    if (maze.getMaze()[position.Y][position.X].getType() == MAZE_TIMER)//Checks if the tile the player is trying to move to is the timer special tile.
    {
        g_ticks = g_ticks - 30;//Substract 3 seconds. Time may be a negative number.
        maze.setTileType(position, MAZE_EMPTY);//Resets tile to empty after the "Stopwatch" (Timer tile) has been "picked".
        mciSendString("play stopwatch.wav", 0, 0, 0);//Sound for picking object

        return true;
    }

    return false;
}

/**************************************************
chooseMaze
Description: Let's the player choose which maze to play.
Arguments: Void
Return: int of the number of the number of maze chosen..
Date: 2014/11/17
Author:Alvaro Chavez Mixco
***************************************************/
int chooseMaze()
{
    int playerChoice;

    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
    do
    {
        system("cls");
        //Displays options
        cout << "Choose a maze" << endl;
        cout << "1.Easy" << endl;
        cout << "2.Medium" << endl;
        cout << "3.Hard" << endl;
        cout << "0.Go back to Main Menu" << endl;
        cin >> playerChoice;
        if (playerChoice != 1 && playerChoice != 2 && playerChoice != 3 && playerChoice != 0)//Error message for invalid options.
        {
            cout << endl;
            cout << "Invalid choice" << endl;
            cout << endl;
            PlaySound(TEXT("invalidOption.wav"), NULL, SND_FILENAME | SND_NODEFAULT);//Doesn't execute next line of code instantly (ASYNC) so that the player knows that he made an invalid choice.           
            system("pause");
        }

    } while (playerChoice != 1 && playerChoice != 2 && playerChoice != 3 && playerChoice != 0);

    //Sound placed outside loop so that it only plays when a valid choice is made.
    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT);//Doesn't use ASYNC to prevent conflict with other sounds

    return playerChoice;
}

/**************************************************
getKeyEventProc
Description: Checks if event record is a key event
and tells what key was pressed.
Arguments: KEY_EVENT_RECORD ker-A key event (when the user
presses a key in the keyboard)
Return: Unsigned char that represents the direction the
user is trying to move to.
Date: 2014/11/14
Author:Alvaro Chavez Mixco
***************************************************/
unsigned char getKeyEventProc(KEY_EVENT_RECORD ker)
{

    if (ker.bKeyDown)//Returns the "value" of the keys pressed (Either arrow keys or WASD), or Escape to quit,
    {
        if (ker.wVirtualKeyCode == VK_LEFT || ker.wVirtualKeyCode == 0x41)//Sets left arrow key AND "A" key as left.
        {
            return VK_LEFT;
        }
        else if (ker.wVirtualKeyCode == VK_RIGHT || ker.wVirtualKeyCode == 0x44)
        {
            return VK_RIGHT;
        }
        else if (ker.wVirtualKeyCode == VK_UP || ker.wVirtualKeyCode == 0x57)
        {
            return VK_UP;
        }
        else if (ker.wVirtualKeyCode == VK_DOWN || ker.wVirtualKeyCode == 0x53)
        {
            return VK_DOWN;
        }
        else if (ker.wVirtualKeyCode == VK_ESCAPE || ker.wVirtualKeyCode == 0x1B)
        {
            return VK_ESCAPE;
        }
    }

    return 0;
}

/**************************************************
playGame
Description: Maze game logic.
Arguments: int mazeNumber-The number (difficulty) of
the maze chosen by the user.
Return: Void
Date: 2014/11/17
Author:Alvaro Chavez Mixco
***************************************************/
void playGame(int mazeNumber)
{
    CMaze maze;
    CPlayer player(1);
    COORD startPos;
    INPUT_RECORD irInBuf[128];
    DWORD cNumRead;
    CLeaderboard lb(5);
    bool resultSave;
    int numMoves = 0;
    bool winGame = false;
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)//Error checking
    {
        cout << "Error: GetStdHandle" << endl;
        return;
    }
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)//Error checking
    {
        cout << "Error: GetStdHandle" << endl;
        return;
    }

    //Loads and displays the game.
    system("cls");

    switch (mazeNumber)//Loads the maze and its corresponding leaderboard.
    {
    case 1:
        maze.loadMaze("maze1.txt");//Easy
        lb.loadLeaderboard("leaderboard1.txt");
        PlaySound(TEXT("maze1Music"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);//Looping background music
        break;
    case 2:
        maze.loadMaze("maze2.txt");//Medium
        lb.loadLeaderboard("leaderboard2.txt");
        PlaySound(TEXT("maze2Music"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
        break;
    case 3:
        maze.loadMaze("maze3.txt");//Hard
        lb.loadLeaderboard("leaderboard3.txt");
        PlaySound(TEXT("maze3Music"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
        break;
    }

    maze.showMaze();//Mazes are different size, but fit in default windows, no need to resize screen.

    //Sets the player start position
    startPos = maze.getStartCoord();
    player.setPos(startPos);
    player.showPlayer();

    g_ticks = 0;//Resets timer to 0.

    //Game loop   
    while (winGame != true)
    {

        //Handle Input
        PeekConsoleInput(hStdin, irInBuf, 1, &cNumRead);//Check if there is an event
        if (cNumRead == 0)
        {
            updateUI(maze, numMoves);
            continue;
        }

        if (!ReadConsoleInput(hStdin, irInBuf, 1, &cNumRead))
        {
            cout << "Error:ReadConsoleInput" << endl;
            return;
        }

        if (cNumRead > 0)
        {
            if (irInBuf[0].EventType == KEY_EVENT)//Checks for keyboard events
            {
                COORD coord = player.getPos();
                unsigned char key;
                key = getKeyEventProc(irInBuf[0].Event.KeyEvent);

                if (key != 0)//Used to prevent "multiple" key readings, from virtual keys and normal keys (Problems with movement count and timer being count multiple times).
                {
                    //Player movement if correct keyboard pressed
                    if (key == VK_LEFT)
                    {
                        coord.X--;
                    }
                    else if (key == VK_RIGHT)
                    {
                        coord.X++;
                    }
                    else if (key == VK_UP)
                    {
                        coord.Y--;
                    }
                    else if (key == VK_DOWN)
                    {
                        coord.Y++;
                    }
                    else if (key == VK_ESCAPE)//Quit game
                    {
                        PlaySound(NULL, 0, 0);;//Stop background Music
                        PlaySound(TEXT("quit.wav"), NULL, SND_FILENAME | SND_NODEFAULT); //Sound of user quitting the game.
                        break;
                    }

                    //Error checking so that player doesn't move outside the screen.
                    if (coord.X < 0)
                    {
                        coord.X = 0;
                    }
                    if (coord.Y < 0)
                    {
                        coord.Y = 0;
                    }

                    //Checks if the tile the player is trying to move to is a wall 
                    if (maze.getMaze()[coord.Y][coord.X].getType() != MAZE_WALL)//If the player is not trying to walk through a wall
                    {
                        //Erase current position
                        SetConsoleCursorPosition(hStdout, player.getPos());
                        maze.getMaze()[player.getPos().Y][player.getPos().X].showTile();//Displays Tile

                        checkTimer(coord, maze);//Check if the player moves to special tile timer (Substract 3 seconds from timer).

                        //Updates to new position
                        player.setPos(coord);
                        player.showPlayer();
                        ++numMoves;//Add moves to move counter, doesn't count for "moving" against a wall.
                    }
                    //Checks if the tile the player is trying to move to is the end of the maze
                    if (maze.getMaze()[coord.Y][coord.X].getType() == MAZE_END)//Finish game when you reach the end of the maze.
                    {
                        winGame = true;//Exits game loop.
                    }

                }//End of if (key != 0) (End of => used to know which closing bracket belongs to what)
            }//End of if (irInBuf[0].EventType == KEY_EVENT)
        }//End of if (cNumRead > 0)

        updateUI(maze, numMoves);//Updates the timer display.

    }//End of while loop

    if (winGame == true)//Display special screen (and leaderboard if applicable) when user finish the maze.
    {

        int score = (g_ticks*g_tickResolution) / 1000;

        PlaySound(NULL, 0, 0);/*Stop background music, it is stopped explicitly (instead of
                                                just playing another sound directly) to be safe it stopped.*/

        //Displays victory screen with end of game stats
        system("cls");
        system("color CE");
        PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_NODEFAULT);
        cout << "YOU WIN!" << endl;
        cout << "Time: " << score << " Seconds" << endl;
        cout << "Moves: " << numMoves << endl;
        system("pause");

        if (lb.checkScore(score) == true) // Special screen if user gets high score
        {
            string highScoreText = "NEW HIGHSCORE";
            string userName;

            system("cls");
            //Display "special effects" for high score.
            PlaySound(TEXT("highScore.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
            blinkingColors(highScoreText);
            blinkingColors(highScoreText);
            system("color CE");
            cout << "New High Score: " << score << " seconds" << endl;
            cout << "Moves: " << numMoves << endl;
            cout << "Enter your username." << endl;
            cin >> userName;

            lb.addItem(userName, score, numMoves);//Adds score to the high score if applicable

            resultSave = lb.saveLeaderboard();//Saves the high Score

            if (resultSave == false)//Error saving highscore file
            {
                cout << "Error saving High Score" << endl;
            }

            system("cls");

            system("color E1");
            showLeaderboard(mazeNumber);//After making the high score, takes the user to the leaderboard of the maze they completed.
        }
    }//End of if (winGame == true)

}

/**************************************************
showHelpMenu
Description: Displays the options for the submenu of
Helps & Options.
Arguments: Void
Return: Void
Date: 2014/11/17
Author: Alvaro Chavez Mixco
***************************************************/
void showHelpMenu()
{
    int choice;

    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
    do
    {
        system("color F0");
        system("cls");

        //Display menu.
        cout << "\t\t\tHelp & Options" << endl;
        cout << endl;
        cout << "1. How to play" << endl;
        cout << "2. Controls" << endl;
        cout << "0. Back to main menu" << endl;
        cin >> choice;

        if (choice != 0)
        {

            switch (choice)//Processes the user's choice
            {
            case 1:
                showHelp();
                system("cls");
                break;
            case 2:
                showControls();
                system("cls");
                break;
            default:
                cout << endl;
                PlaySound(TEXT("invalidOption.wav"), NULL, SND_FILENAME | SND_NODEFAULT);
                cout << "Invalid choice." << endl;
                cout << endl;
                system("pause");
                break;
            }

        }

    } while (choice != 0);//Quits the submenu if user chooses 0.

    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT);//Sound if user goes back.
}

/**************************************************
showHelp
Description: Shows the instructions for the maze game.
Arguments:  Void
Return: Void
Date: 2014/11/17
Author: Alvaro Chavez Mixco
***************************************************/
void showHelp()
{
    system("cls");

    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
    //Display menu
    cout << "\t\t\tHow to play" << endl;
    cout << endl;
    cout << "- Reach the end of the maze in the fastest time possible." << endl;
    cout << "- Your moves are being counted, so don't waste them." << endl;//Moves don't affect high score order
    cout << "- The fastest time makes it into the leaderboard" << endl;
    cout << endl;
    cout << (unsigned char)1 << " - Player character." << endl;//The characters are what appear normally in the game
    cout << (unsigned char)16 << " - Start point of the maze." << endl;
    cout << (unsigned char)215 << " - End of the maze." << endl;
    cout << (unsigned char)232 << " - Stopwatch. Substract 3 seconds from the timer." << endl;
    cout << endl;

    system("pause");
    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);//Sound when users goes back
}

/**************************************************
showControls
Description: Shows the controls for the maze game.
Arguments:  Void
Return: Void
Date: 2014/11/21
Author: Alvaro Chavez Mixco
***************************************************/
void showControls()
{
    system("cls");

    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);//Button sound
    //Display menu
    cout << "\t\t\tControls" << endl;
    cout << endl;
    cout << "Up arrow/W - Move up" << endl;
    cout << "Down arrow/S - Move down" << endl;
    cout << "Left arrow/A - Move left" << endl;
    cout << "Right arrow/D - Move right" << endl;
    cout << "ESC - Exit the game" << endl;
    cout << endl;

    system("pause");
    PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
}

/**************************************************
showLeaderboard
Description: Read the current leaderboard from a file
and show it.
Arguments:  int mazeNumber-The number of maze for which the
respective leaderboard will be loaded.
Return: Void
Date: 2014/12/03
Author: Alvaro Chavez Mixco
***************************************************/
void showLeaderboard(int mazeNumber)
{
    bool resultLoad = false;
    CLeaderboard lb(5);

    system("cls");

    //Display Leaderboards
    cout << "\t\t\tLeaderboard" << endl;

    switch (mazeNumber)//Loads the leaderboard according to the player choice.
    {
    case 1:
        resultLoad = lb.loadLeaderboard("leaderboard1.txt");//Easy
        cout << "\t\t\t    Easy" << endl;
        break;
    case 2:
        resultLoad = lb.loadLeaderboard("leaderboard2.txt");//Medium
        cout << "\t\t\t   Medium" << endl;
        break;
    case 3:
        resultLoad = lb.loadLeaderboard("leaderboard3.txt");//Hard
        cout << "\t\t\t    Hard" << endl;
        break;
    }

    if (resultLoad == false)//Checks if there is a leaderboard
    {
        cout << endl;
        cout << "There is no highscore yet." << endl;
        cout << endl;
        system("pause");
    }
    else
    {

        for (auto i : lb.getList())//Iterates through the leaderboard
        {
            string name;
            int time;
            int moves;

            i.getScore(name, time, moves);

            cout << "Name: " << name << endl;
            cout << "Time: " << time << " seconds" << endl;
            cout << "Moves:" << moves << endl;
            cout << "________________" << endl;
        }

        system("pause");
        PlaySound(TEXT("blipSelect.wav"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);//Button sound
    }

}