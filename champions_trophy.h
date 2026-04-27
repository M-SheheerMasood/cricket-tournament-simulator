#ifndef CRICKET_DATABASE_H
#define CRICKET_DATABASE_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

class Team {
private:
    string name;
    int wins;
    int totalMatches;
    vector<string> players;

public:
    Team();
    Team(string name, vector<string> playerNames);
    void addWin();
    void addMatch();
    bool hasPlayer(const string& player) const;
    const vector<string>& getPlayers() const;
    int getWinPercentage() const;
    string getName() const;
    int getWins() const;
    int getTotalMatches() const;
};

class Match {
private:
    Team team1, team2;
    int year;
    map<string, pair<int, int>> playerStats;
    Team* winner;

public:
    Match(Team t1, Team t2, int y);
    void addPerformance(string player, int runs, int wickets);
    const map<string, pair<int, int>>& getPlayerStats() const;
    int getYear() const;
    void setWinner();
    string getWinner() const;
    const Team& getTeam1() const;
    const Team& getTeam2() const;
};

class CricketDatabase {
private:
    vector<Match> matches;
    map<string, vector<pair<int, int>>> playerPerformances;
    map<string, Team> teams;

public:
    void addTeam(string name, vector<string> players);
    bool addMatch(string team1, string team2, int year, const map<string, pair<int, int>>& performances);
    bool addMatchPerformance(string player, int runs, int wickets);
    vector<Match> getMatchesByYear(int year) const;
    string findBestBowler() const;
    string findMostConsistentPlayer() const;
    string findMostRunsScorer() const;
    string findTeamWithMostWins() const;
    string findTeamWithBestWinPercentage() const;
    vector<string> getTopNBatters(int N) const;
    vector<string> getTopNBowlers(int N) const;
    vector<Match> getClosestMatches() const;
    void processFile(string filename);

    map<string, Team> getTeams() const;
    vector<Match> getMatches() const;
    map<string, vector<pair<int, int>>> getPlayerPerformances() const;
};

#endif
