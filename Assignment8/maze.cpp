#include "maze.h"
#include "mycolours.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>

using namespace std;

/**************************************************
CMazeTile::setTile
Description: Sets the characteristics for each different
type of tile in the maze.
Arguments:MazeType-The type of tile.
Return:bool
Date: 2014/11/07
Author:Alvaro Chavez Mixco
***************************************************/
bool CMazeTile::setTile(MazeType t)
{
    bool result = true;

    switch (t)
    {
    case MAZE_WALL:
        m_symbol = (unsigned char)219;
        m_type = t;
        m_colour = setColour(LIGHT_GRAY, BLACK);
        break;
    case MAZE_EMPTY:
        m_symbol = ' ';
        m_type = t;
        m_colour = setColour(BLACK, BLACK);
        break;
    case MAZE_START:
        m_symbol = (unsigned char)16;
        m_type = t;
        m_colour = setColour(LIGHT_GREEN, BLACK);
        break;
    case MAZE_END:
        m_symbol = (unsigned char)215;
        m_type = t;
        m_colour = setColour(LIGHT_RED, BLACK);
        break;
    case MAZE_TIMER:
        m_symbol = (unsigned char)232;
        m_type = t;
        m_colour = setColour(LIGHT_CYAN, BLACK);
        break;
    default:
        m_type = MAZE_ERROR;
        m_symbol = 'E';
        m_colour = setColour(RED, RED);//Noticeable color to find error
        result = false;
        break;
    }

    return result;
}

/**************************************************
CMazeTile::setTileFromCharacter
Description: Assigns a type for the tile according to
its representation in the maze text file.
Arguments:unsigned char C-the chars that appear in the
text file of the "maze".
Return:bool-true if operation successful, false-if it isn't
Date: 2014/11/05
Author:Alvaro Chavez Mixco
***************************************************/
bool CMazeTile::setTileFromCharacter(unsigned char c)
{
    bool result = true;

    switch (c)
    {
    case 'x'://checks for both "types(caps)" of letter in case of a typo in the maze design.
    case 'X':
        setTile(MAZE_WALL);
        break;
    case ' ':
        setTile(MAZE_EMPTY);
        break;
    case 's':
    case 'S':
        setTile(MAZE_START);
        break;
    case 't':
    case 'T':
        setTile(MAZE_TIMER);
        break;
    case 'f':
    case 'F':
        setTile(MAZE_END);
        break;
    default:
        setTile(MAZE_ERROR);
        result = false;
        break;
    }

    return result;
}

/**************************************************
CMazeTile::CMazeTile
Description: Cosntructor that creates a handle and sets
the color of the maze tile.
Arguments:void
Return:
Date: 2014/11/05
Author:Alvaro Chavez Mixco
***************************************************/
CMazeTile::CMazeTile()

{
    m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);//Creates handle
    m_colour = setColour(LIGHT_GRAY, BLACK);
}

/**************************************************
CMazeTile::showTile
Description: Displays a single tile of the maze.
Arguments:void
Return: Void
Date: 2014/11/05
Author:Alvaro Chavez Mixco
***************************************************/
void CMazeTile::showTile()
{
    SetConsoleTextAttribute(m_hStdout, m_colour);
    cout << (unsigned char)m_symbol;
}

/**************************************************
CMaze::loadMaze
Description: Reads the "map" file for the maze and
loads it.
Arguments:string file-name of the file that is to be read.
Return:bool-true if operation successful, false-if it isn't
Date: 2014/11/07
Author:Alvaro Chavez Mixco
***************************************************/
bool CMaze::loadMaze(string file)//Reads the maze file.
{
    ifstream inFile;
    int row;
    int col;
    string line;

    inFile.open(file);
    if (!inFile.is_open())//Error message if file can't be opened.
    {
        cout << "Error Reading file" << endl;
        return false;
    }

    inFile >> row;
    inFile >> col;

    inFile.ignore(10000, '\n');

    for (int r = 0; r < row; r++)//Creates the structure of the maze, line by line.
    {
        getline(inFile, line, '\n');

        if (inFile.fail())//Error message
        {
            cout << "Error in row: " << r << " expected " << row << " rows, but stopped." << endl;
        }

        if (line.size() != col)// Error message
        {
            cout << "Error in row: " << r << " expected " << col << " chars, but found " << line.size() << endl;
        }

        vector<CMazeTile>rowVect;

        for (int i = 0; i < line.size(); i++)//Set characters for each tile.
        {
            CMazeTile tile;

            tile.setTileFromCharacter(line[i]);

            if (tile.setTileFromCharacter(line[i]) == false)//Error message
            {
                cout << "Error character." << line[i] << endl;
            }
            rowVect.push_back(tile);//Adds tile to row.
        }
        m_theMaze.push_back(rowVect);//Adds line to the maze.
    }

    m_rows = row;
    m_cols = col;

    return true;
}

/**************************************************
CMaze::showMaze
Description: Displays the maze on the screen.
Arguments:Void
Return:Void
Date: 2014/11/07
Author:Alvaro Chavez Mixco
***************************************************/
void CMaze::showMaze()
{

    for (int r = 0; r < m_rows; r++)//For loop for rows.
    {

        for (int c = 0; c < m_cols; c++)//For loop for columns
        {
            m_theMaze[r][c].showTile();
        }
        cout << endl;
    }

}

/**************************************************
CMaze::setTileType
Description: Sets the type of a tile.
Arguments:COORD pos-Position of tile being changed.
MazeType t-The type of maze to which the tile will be changed.
Return:Void
Date: 2014/11/12
Author:Alvaro Chavez Mixco
***************************************************/
void CMaze::setTileType(COORD pos, MazeType t)
{
    m_theMaze[pos.Y][pos.X].setTile(t);
}

/**************************************************
CMaze::getStartCoord
Description: Gets the starting coordinates for the player.
Arguments:Void
Return:COORD-The coordinates of the starting position.
Date: 2014/11/12
Author:Alvaro Chavez Mixco
***************************************************/
COORD CMaze::getStartCoord()
{
    COORD startPos;
    startPos.X = -1;//Starts wtih a bad value for error checking.
    startPos.Y = -1;

    for (int r = 0; r < m_rows; r++)
    {

        for (int c = 0; c < m_cols; c++)
        {
            if (m_theMaze[r][c].getType() == MAZE_START)//Gets the coordinates for the start position.
            {
                startPos.X = c;
                startPos.Y = r;
                return startPos;
            }
        }
    }

    return startPos;
}