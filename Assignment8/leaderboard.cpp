#include "leaderboard.h"
#include <iostream>
#include <string>
#include <list>
#include <fstream>

using namespace std;

void CScoreItem::setScore(string name, int score, int numMoves)
{
    m_name = name;
    m_score = score;
    m_numMoves = numMoves;
}

void CScoreItem::getScore(string &name, int &score, int &numMoves)
{
    name = m_name;
    score = m_score;
    numMoves = m_numMoves;
}

//Code given on class, no function header.
bool CScoreItem::operator<(const CScoreItem &rhs)
{
    return(m_score < rhs.m_score);//Overloaded comparison operator.
}

list<CScoreItem>CLeaderboard::getList()
{
    return m_theList;
}

/**************************************************
CLeaderboard::addItem
Description: Adds the score to the leaderboard if the
score is high (low, in this case) enough, or if there are less
than the max number of entries.
Arguments:  string name-name of the user playing.
int score-The score obtained in the game by the user, in this
case the time .
int numMoves-the number of moves made by the user, this doesn't
influence the order of the leaderboard.
Return: bool-True if there is a new high score; false if
the high score wasn't achieved.
Date: 2014/11/28
Author: Alvaro Chavez Mixco
***************************************************/
bool CLeaderboard::addItem(string name, int score, int numMoves)
{
    CScoreItem item;
    //Lowest refers to the lowest one in the leaderboard not lowest time.

    if (m_theList.size() >= m_maxEntries)//If the leaderboard is full
    {
        CScoreItem lowestItem;
        lowestItem = m_theList.back();
        string lowestName;
        int lowestScore;
        int lowestMoves;

        lowestItem.getScore(lowestName, lowestScore, lowestMoves);

        if (score > lowestScore)//If the score actually makes it into the leaderboard
        {
            m_theList.pop_back();
            item.setScore(name, score, numMoves);
            m_theList.push_back(item);
            m_theList.sort();

            return true;
        }
    }
    else //If the leaderboard is not full
    {
        item.setScore(name, score, numMoves);
        m_theList.push_back(item);
        m_theList.sort();

        return true;
    }

    return false;
}

/**************************************************
CLeaderboard::checkScore
Description: Compares the score given to the method with
the lowest score in the list
Arguments:  int score-score obtained by the user.
Return: bool-True if there is a new high score; false if
the high score wasn't achieved.
Date: 2014/11/28
Author: Alvaro Chavez Mixco
***************************************************/
bool CLeaderboard::checkScore(int score)
{
    CScoreItem item;

    if (m_theList.size() >= m_maxEntries)
    {
        CScoreItem lowestItem;
        lowestItem = m_theList.back();
        string lowestName;
        int lowestScore;
        int lowestMoves;

        lowestItem.getScore(lowestName, lowestScore, lowestMoves);

        if (score > lowestScore)
        {
            return true;
        }
    }
    else
    {
        return true;
    }

    return false;
}

/**************************************************
CLeaderboard::saveLeaderboard
Description: Saves the current state of the leaderboard.
Arguments: Void
Return: bool-True if the leaderboard was saved; false if
the leaderboard wasn't saved
Date: 2014/11/13
Author: Alvaro Chavez Mixco
***************************************************/
bool CLeaderboard::saveLeaderboard()
{
    ofstream leaderboard;
    leaderboard.open(m_leaderboardName, ios::out);//Saves the score to the file, which name was previously given when the leaderboard was loaded.

    if (!leaderboard.is_open())//Checks if the file can be open
    {
        return false;
    }

    for (auto i : m_theList)//Iterates through the list
    {
        string name;
        int score;
        int moves;

        i.getScore(name, score, moves);

        leaderboard << name << "\n";
        leaderboard << score << "\n";
        leaderboard << moves << "\n";
    }

    leaderboard.close();

    return true;
}

/**************************************************
CLeaderboard::loadLeaderboard
Description: Loads the current state of the leaderboard.
Arguments: string leaderboardName- Name or number of which
leaderboard to load.
Return: bool-True if there leaderboard was loaded; false if
the leaderboard wasn't loaded
Date: 2014/11/13
Author: Alvaro Chavez Mixco
***************************************************/
bool CLeaderboard::loadLeaderboard(string leaderboardName)
{
    string name;
    int score;
    int moves;
    ifstream leaderboard(leaderboardName);
    m_leaderboardName = leaderboardName;//Saves the name of the leaderboard loaded

    if (!leaderboard.is_open())//Checks if the file can be opened
    {
        return false;
    }

    while (true)//Reads leaderboard
    {
        leaderboard >> name;
        if (leaderboard.fail() == true)//Breaks when there are no more scores to read.
        {
            break;
        }

        leaderboard >> score;
        if (leaderboard.fail() == true)
        {
            break;
        }

        leaderboard >> moves;
        if (leaderboard.fail() == true)
        {
            break;
        }

        addItem(name, score, moves);
    }

    leaderboard.close();

    return true;
}

/**************************************************
CLeaderboard (Costructor)
Description: Cleaderboard constructor that limits the
number of entries in hte leaderboard.
Arguments: int max-maximum numnber of entries in the
leaderboard.
Return:
Date: 2014/11/28
Author: Alvaro Chavez Mixco
***************************************************/
CLeaderboard::CLeaderboard(int max)
{
    m_maxEntries = max;
}