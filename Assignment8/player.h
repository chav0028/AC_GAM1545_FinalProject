#ifndef __PLAYER_H
#define __PLAYER_H

#include <Windows.h>

class CPlayer
{
private:
    COORD m_pos;
    unsigned char m_colour;
    unsigned char m_symbol;
    HANDLE m_hStdin;
    HANDLE m_hStdout;

public:
    void setPos(int x, int y);
    void setPos(COORD pos);
    void getPos(int &x, int &y);
    COORD getPos();
    void showPlayer();
    CPlayer(unsigned char c);
};

#endif