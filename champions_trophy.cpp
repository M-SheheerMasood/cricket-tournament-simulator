#include "champions_trophy.h"
using namespace std;

Team::Team()
{
    name = "";
    wins = 0;
    totalMatches = 0;
    players = {};
}

Team::Team(string name, vector<string> playerNames)
{
    this->name = name;
    wins = 0;
    totalMatches = 0;
    players = playerNames;
}

void Team::addWin()
{
    wins++;
}

void Team::addMatch()
{
    totalMatches++;
}

bool Team::hasPlayer(const string &player) const
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i] == player)
        {
            return true;
        }
    }
    return false;
}

const vector<string> &Team::getPlayers() const
{
    return players;
}

int Team::getWinPercentage() const
{
    double win = wins;
    double match = totalMatches;
    if (match == 0)
    {
        return 0;
    }
    return (win / match * 100);
}

string Team::getName() const
{
    return name;
}

int Team::getWins() const
{
    return wins;
}

int Team::getTotalMatches() const
{
    return totalMatches;
}

Match::Match(Team t1, Team t2, int y)
{
    team1 = t1;
    team2 = t2;
    if (y < 0)
    {
        y = 0;
    }
    year = y;
    winner = nullptr;
    playerStats.clear();
}

void Match::addPerformance(string player, int runs, int wickets)
{
    if (runs < 0 || wickets < 0)
    {
        return;
    }
    auto it = playerStats.find(player);
    if (it != playerStats.end())
    {
        it->second.first += runs;
        it->second.second += wickets;
    }
    else
    {
        playerStats.insert({player, make_pair(runs, wickets)});
    }
}

const map<string, pair<int, int>> &Match::getPlayerStats() const
{
    return playerStats;
}

int Match::getYear() const
{
    return year;
}

void Match::setWinner()
{
    int total_runs_team1 = 0;
    int total_runs_team2 = 0;
    for (auto p_team1 : team1.getPlayers())
    {
        for (auto stats : playerStats)
        {
            if (p_team1 == stats.first)
            {
                total_runs_team1 += stats.second.first;
                break;
            }
        }
    }
    for (auto p_team2 : team2.getPlayers())
    {
        for (auto stats : playerStats)
        {
            if (p_team2 == stats.first)
            {
                total_runs_team2 += stats.second.first;
                break;
            }
        }
    }
    if (total_runs_team1 > total_runs_team2)
    {
        winner = &team1;
    }
    else
    {
        winner = &team2;
    }
}

string Match::getWinner() const
{
    if (winner == nullptr)
    {
        return "";
    }
    return winner->getName();
}

const Team &Match::getTeam1() const
{
    return team1;
}

const Team &Match::getTeam2() const
{
    return team2;
}

void CricketDatabase::addTeam(string name, vector<string> players)
{
    if(name == "")
    {
        return;
    }
    if (teams.find(name) == teams.end())
    {
        Team new_team(name, players);
        teams.insert({name, new_team});
    }
}

bool CricketDatabase::addMatch(string team1, string team2, int year, const map<string, pair<int, int>> &performances)
{
    if (teams.find(team1) != teams.end() && teams.find(team2) != teams.end())
    {
        Match new_match(teams.find(team1)->second, teams.find(team2)->second, year);
        for (auto player : performances)
        {
            new_match.addPerformance(player.first, player.second.first, player.second.second);
            auto it = playerPerformances.find(player.first);
            if (it != playerPerformances.end())
            {
                playerPerformances[player.first].push_back(make_pair(player.second.first, player.second.second));
            }
            else
            {
                playerPerformances.insert({player.first, vector<pair<int, int>>()});
                playerPerformances[player.first].push_back(make_pair(player.second.first, player.second.second));
            }
        }
        new_match.setWinner();
        string winner = new_match.getWinner();
        if(winner == team1)
        {
            teams[team1].addWin();
            teams[team1].addMatch();
            teams[team2].addMatch();
        }
        else if (winner == team2)
        {
            teams[team2].addWin();
            teams[team2].addMatch();
            teams[team1].addMatch();
        }
        matches.push_back(new_match);
        return true;
    }
    return false;
}

bool CricketDatabase::addMatchPerformance(string player, int runs, int wickets)
{
    if (playerPerformances.find(player) != playerPerformances.end() && runs >= 0 && wickets >= 0)
    {
        playerPerformances.find(player)->second.push_back(make_pair(runs, wickets));
        return true;
    }
    return false;
}

vector<Match> CricketDatabase::getMatchesByYear(int year) const
{
    if (year > 0)
    {
        vector<Match> matches_by_year;
        for (auto match : matches)
        {
            if (match.getYear() == year)
            {
                matches_by_year.push_back(match);
            }
        }
        return matches_by_year;
    }
    return vector<Match>();
}

string CricketDatabase::findBestBowler() const
{
    string bowler = "";
    int max_wickets = 0;
    int total_wickets;
    for (auto player : playerPerformances)
    {
        total_wickets = 0;
        for (auto wickets : player.second)
        {
            total_wickets += wickets.second;
        }
        if (total_wickets > max_wickets)
        {
            max_wickets = total_wickets;
            bowler = player.first;
        }
    }
    return bowler;
}

string CricketDatabase::findMostConsistentPlayer() const
{
    string consistent = "";
    double max_consistancy = 0;
    double best_c;
    double run;
    double wicket;
    for (auto players : playerPerformances)
    {
        best_c = 0;
        run = 0;
        wicket = 0;
        for (auto c : players.second)
        {
            run += c.first;
            wicket += c.second;
        }
        best_c = run + (25 * wicket);
        if (best_c > max_consistancy)
        {
            max_consistancy = best_c;
            consistent = players.first;
        }
    }
    return consistent;
}

string CricketDatabase::findMostRunsScorer() const
{
    int max_runs = 0;
    string player = "";
    for (auto players : playerPerformances)
    {
        int total_runs = 0;
        for (auto run : players.second)
        {
            total_runs += run.first;
        }
        if (total_runs > max_runs)
        {
            max_runs = total_runs;
            player = players.first;
        }
    }
    return player;
}

string CricketDatabase::findTeamWithMostWins() const
{
    string team = "";
    int max_wins = -1;
    for (auto x : teams)
    {
        if (x.second.getWins() > max_wins)
        {
            team = x.first;
            max_wins = x.second.getWins();
        }
    }
    return team;
}

string CricketDatabase::findTeamWithBestWinPercentage() const
{
    string team = "";
    int max_win = -1;
    for (auto x : teams)
    {
        if (x.second.getWinPercentage() > max_win)
        {
            team = x.first;
            max_win = x.second.getWinPercentage();
        }
    }
    return team;
}

vector<string> CricketDatabase::getTopNBatters(int N) const
{
    if (N <= 0)
    {
        return vector<string>();
    }
    if (N > playerPerformances.size())
    {
        N = playerPerformances.size();
    }
    map<string, vector<pair<int, int>>> copy = playerPerformances;
    vector<string> ret;
    for (int i = 0; i < N; i++)
    {
        int max_runs = 0;
        string player = "";
        for (auto players : copy)
        {
            int total_runs = 0;
            for (auto run : players.second)
            {
                total_runs += run.first;
            }
            if (total_runs > max_runs)
            {
                max_runs = total_runs;
                player = players.first;
            }
        }
        if (player != "")
        {
            ret.push_back(player);
            copy.erase(copy.find(player));
        }
    }
    return ret;
}

vector<string> CricketDatabase::getTopNBowlers(int N) const
{
    if (N <= 0)
    {
        return vector<string>();
    }
    if (N > playerPerformances.size())
    {
        N = playerPerformances.size();
    }
    map<string, vector<pair<int, int>>> copy = playerPerformances;
    vector<string> ret;
    for (int i = 0; i < N; i++)
    {
        int max_wickets = 0;
        string player = "";
        for (auto players : copy)
        {
            int total_wickets = 0;
            for (auto wicket : players.second)
            {
                total_wickets += wicket.second;
            }
            if (total_wickets > max_wickets)
            {
                max_wickets = total_wickets;
                player = players.first;
            }
        }
        if (player != "")
        {
            ret.push_back(player);
            copy.erase(copy.find(player));
        }
    }
    return ret;
}

vector<Match> CricketDatabase::getClosestMatches() const
{
    if(matches.empty())
    {
        return vector<Match>();
    }
    vector<Match> v = matches;

    sort(v.begin(), v.end(), [](const Match &a, const Match &b)
         {
        int a1_runs = 0;
        int a2_runs = 0;
        for (const string player : a.getTeam1().getPlayers())
        {
            auto stats = a.getPlayerStats();
            if (stats.find(player) != stats.end())
            {
                a1_runs += stats.at(player).first;
            }
        }
        for (string player : a.getTeam2().getPlayers())
        {
            auto stats = a.getPlayerStats();
            if (stats.find(player) != stats.end())
            {
                a2_runs += stats.at(player).first;
            }
        }
        int a_diff = abs(a1_runs - a2_runs);
        int b1_runs = 0;
        int b2_runs = 0;
        for (const string player : b.getTeam1().getPlayers())
        {
            auto stats = b.getPlayerStats();
            if (stats.find(player) != stats.end())
            {
                b1_runs += stats.at(player).first;
            }
        }
        for (const string player : b.getTeam2().getPlayers())
        {
            auto stats = b.getPlayerStats();
            if (stats.find(player) != stats.end())
            {
                b2_runs += stats.at(player).first;
            }
        }
        int b_diff = abs(b1_runs - b2_runs);
        return a_diff < b_diff; });
    return v;
}

// getters
map<string, Team> CricketDatabase::getTeams() const
{
    return teams;
}
vector<Match> CricketDatabase::getMatches() const
{
    return matches;
}
map<string, vector<pair<int, int>>> CricketDatabase::getPlayerPerformances() const
{
    return playerPerformances;
}