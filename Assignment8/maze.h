#ifndef __MAZE_H
#define __MAZE_H

#include <vector>
#include <string>
#include <Windows.h>

enum MazeType
{
    MAZE_EMPTY,
    MAZE_WALL,
    MAZE_START,
    MAZE_END,
    MAZE_TIMER,//Special tile that substract time from timer
    MAZE_ERROR
};

class CMazeTile
{
private:
    unsigned char m_symbol;//Character to display on screen
    unsigned char m_colour;
    MazeType m_type;
    HANDLE m_hStdout;
public:
    bool setTile(MazeType t);
    bool setTileFromCharacter(unsigned char c);
    CMazeTile();
    void showTile();
    MazeType getType(){ return m_type; }
};

class CMaze
{
private:
    int m_rows;
    int m_cols;
    std::vector<std::vector<CMazeTile>> m_theMaze;
public:
    std::vector<std::vector<CMazeTile>> getMaze(){ return m_theMaze; };
    bool loadMaze(std::string file);
    void showMaze();
    void setTileType(COORD pos, MazeType t);
    COORD getStartCoord();
    int getRows(){ return m_rows; };
    int getCols(){ return m_cols; };
};

#endif