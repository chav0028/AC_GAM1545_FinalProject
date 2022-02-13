#include "player.h"
#include "mycolours.h"
#include <Windows.h>
#include <iostream>

using namespace std;

void CPlayer::setPos(int x, int y)//Overloaded setPos function, assignment required.
{
    m_pos.X = x;
    m_pos.Y = y;
}

void CPlayer::setPos(COORD pos)
{
    m_pos = pos;
}

void CPlayer::getPos(int &x, int &y)//Overloaded getPos function, done in class.
{
    m_pos.X = x;
    m_pos.Y = y;
}

COORD CPlayer::getPos()
{
    return m_pos;
}

/**************************************************
CPlayer::showPlayer
Description: Displays the player according to the char
assigned to it.
Arguments:Void
Return:Void
Date: 2014/11/05
Author:Alvaro Chavez Mixco
***************************************************/
void CPlayer::showPlayer()
{
    //Displays the char that represents the place on the curso.
    SetConsoleCursorPosition(m_hStdout, m_pos);//Sets position.
    SetConsoleTextAttribute(m_hStdout, m_colour);//Sets color
    cout << m_symbol;//Display

}

/**************************************************
CPlayer::CPlayer
Description: Constructor that requires a char to
represent the player with.
Arguments:unsigned char- the char which will represent
the player.
Return:
Date: 2014/11/05
Author:Alvaro Chavez Mixco
***************************************************/
CPlayer::CPlayer(unsigned char c)
{
    m_symbol = c;//Char that will represent the cahracter
    m_pos.X = 0;
    m_pos.Y = 0;
    m_colour = setColour(WHITE, BLACK);
    m_hStdin = GetStdHandle(STD_INPUT_HANDLE);
    m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
}