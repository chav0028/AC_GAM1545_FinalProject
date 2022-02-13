#ifndef __LEADERBOARD_H
#define __LEADERBOARD_H

#include <string>
#include <list>

class CScoreItem
{
private:
    std::string m_name;
    int m_score;
    int m_numMoves;
public:
    void setScore(std::string name, int score, int numMoves);
    void getScore(std::string &name, int &score, int &numMoves);
    bool operator<(const CScoreItem &rhs);
};

class CLeaderboard
{
private:
    std::list<CScoreItem>m_theList;
    unsigned int m_maxEntries;
    std::string m_leaderboardName;//Name according to the maze the leaderboard belongs to.
public:
    std::list<CScoreItem>getList();
    bool addItem(std::string name, int score, int numMoves);
    bool checkScore(int score);
    bool saveLeaderboard();
    bool loadLeaderboard(std::string leaderboardName);
    CLeaderboard(int max);
};

#endif