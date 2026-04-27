#include <iostream>
#include "champions_trophy.h"  // Assuming your Team class is in this file
using namespace std;

// ANSI color codes for terminal output
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

float points = 0;

void printTestResult(const string& testName, bool passed, float testPoints = 1) {
    if (passed) {
        cout << GREEN << "[PASSED] " << RESET << testName << endl;
        points += testPoints;
    } else {
        cout << RED << "[FAILED] " << RESET << testName << endl;
    }

}

void testTeamClass() {
    cout << "Running Team class tests...\n";

    // 1. Team Initialization
    Team warriors("Warriors", {"Alice", "Bob", "Charlie", "David"});

    printTestResult("1.1 Team Name Retrieval", warriors.getName() == "Warriors", 0.5);
    
    // 2. Player Presence Tests
    printTestResult("2.1 Player Exists - Alice", warriors.hasPlayer("Alice"),0.5);
    printTestResult("2.2 Player Exists - Charlie", warriors.hasPlayer("Charlie"),0.5);
    printTestResult("2.3 Player Does Not Exist - Eve", !warriors.hasPlayer("Eve"),0.5);

    // 3. Initial Win & Match Count
    printTestResult("3.1 Initial Wins and Matches", warriors.getWins() == 0 && warriors.getTotalMatches() == 0, 2);

    // 4. Updating Matches & Wins
    warriors.addMatch();
    warriors.addMatch();
    warriors.addWin();  // 1 win out of 2 matches

    printTestResult("4.1 Wins After Update", warriors.getWins() == 1);
    printTestResult("4.2 Matches After Update", warriors.getTotalMatches() == 2);
    printTestResult("4.3 Win Percentage After Update", warriors.getWinPercentage() == 50);

    // 5. Further Updates
    warriors.addMatch();
    warriors.addWin();  // 2 wins out of 3 matches

    printTestResult("5.1 Wins After Second Update", warriors.getWins() == 2);
    printTestResult("5.2 Matches After Second Update", warriors.getTotalMatches() == 3);
    printTestResult("5.3 Win Percentage After Second Update", warriors.getWinPercentage() == 66);

    cout << "\nTeam Class tests completed. Take a short break :)\n";
    cout << YELLOW << "Points: " << points << " / 10" << RESET << endl;
}

void testMatchClass() {
    cout << "\nRunning Match class tests...\n";

    // 1. Match Initialization (3 marks)
    Team tigers("Tigers", {"Alice", "Bob", "Charlie"});
    Team lions("Lions", {"David", "Eve", "Frank"});
    Match match(tigers, lions, 2024);

    printTestResult("1.1 Match Year Initialization", match.getYear() == 2024, 1);
    printTestResult("1.2 Player Stats Initially Empty", match.getPlayerStats().empty(), 2);

    // 2. Adding Player Performances (4 marks)
    match.addPerformance("Alice", 50, 1);
    match.addPerformance("Bob", 30, 0);
    match.addPerformance("Charlie", 20, 2);
    match.addPerformance("David", 60, 0);
    match.addPerformance("Eve", 10, 1);
    match.addPerformance("Frank", 5, 3);

    auto stats = match.getPlayerStats();
    printTestResult("2.1 Correct Runs Recorded for Alice", stats["Alice"].first == 50, 1);
    printTestResult("2.2 Correct Wickets Recorded for Frank", stats["Frank"].second == 3, 1);
    printTestResult("2.3 All Players' Stats Exist", stats.size() == 6, 2);

    // 3. Determining the Winner (5 marks)
    match.setWinner();
    string winner = match.getWinner();
    
    printTestResult("3.1 Winner is Determined", !winner.empty(), 2);
    printTestResult("3.2 Winning Team Correct (Tigers Expected)", winner == "Tigers", 3); // Tigers: 50+30+20 = 100, Lions: 60+10+5 = 75

    // 4. Edge Cases for Winner Selection (4 marks)
    Match tieMatch(tigers, lions, 2025);
    tieMatch.addPerformance("Alice", 50, 0);
    tieMatch.addPerformance("Bob", 20, 1);
    tieMatch.addPerformance("Charlie", 30, 2);
    tieMatch.addPerformance("David", 60, 1);
    tieMatch.addPerformance("Eve", 30, 2);
    tieMatch.addPerformance("Frank", 10, 0);
    tieMatch.setWinner();

    printTestResult("4.1 Handles Tie Case Correctly", !tieMatch.getWinner().empty(), 2);
    printTestResult("4.2 Correct Winner (Lions Expected)", tieMatch.getWinner() == "Lions", 2); // Lions: 60+30+10 = 100, Tigers: 50+20+30 = 100 (first team wins tie)

    // 5. Multiple Matches and Different Years (4 marks)
    Match olderMatch(lions, tigers, 2018);
    printTestResult("5.1 Older Match Has Correct Year", olderMatch.getYear() == 2018, 2);
    
    olderMatch.addPerformance("Charlie", 40, 2);
    olderMatch.addPerformance("David", 20, 1);
    printTestResult("5.2 Older Match Records Player Stats", olderMatch.getPlayerStats().size() == 2, 2);

    cout << "\nMatch class tests completed.\n";
    cout << YELLOW << "Points: " << points << " / 30" << RESET << endl;
}

void testCricketDatabase() {
    cout << "\nRunning CricketDatabase tests...\n\n";

    CricketDatabase db;
    CricketDatabase db2;  
    
    // 1. Adding Teams & Matches (15 marks)
    db.addTeam("Warriors", {"Alice", "Bob"});
    db.addTeam("Strikers", {"Charlie", "David"});
    //db2.processFile("data.txt");  

    db2.addTeam("Warriors", {"Alice", "Bob"});
    db2.addTeam("Strikers", {"Charlie", "David"});

    printTestResult("1.1 Teams Added Successfully",db.getTeams().size() == db2.getTeams().size(),5); // No direct getter for teams

    map<string, pair<int, int>> match1Stats = {
        {"Alice", {40, 2}}, {"Bob", {30, 0}},
        {"Charlie", {50, 1}}, {"David", {20, 2}}
    };
    db.addMatch("Warriors", "Strikers", 2024, match1Stats);
    // remove this line
    db2.addMatch("Warriors", "Strikers", 2024, match1Stats);
    printTestResult("1.2 Match Successfully Added",  db.getMatches().size() == db2.getMatches().size(), 5);  // No direct getter for matches
    printTestResult("1.3 Players Performance Updated", db.getPlayerPerformances().size() == db2.getPlayerPerformances().size(), 5);  // Assumes stats are correctly stored

    // 2. Adding Match Performance (15 marks)
    db.addMatchPerformance("Alice", 20, 1);
    db.addMatchPerformance("David", 15, 0);

    db2.addMatchPerformance("Alice", 20, 1);
    db2.addMatchPerformance("David", 15, 0);
    
    auto playerPerformances = db.getPlayerPerformances();
    for(auto p: playerPerformances){
        if(p.first == "Alice"){
            printTestResult("2.1 Performance Updated for Alice", p.second.size() == 2, 5);
        }
        if(p.first == "David"){
            printTestResult("2.2 Performance Updated for David", p.second.size() == 2, 5);
        }
    }

    printTestResult("2.3 Handles Non-Existent Players Gracefully (Individually)", db.addMatchPerformance("chakla", 10, 2) == 0, 5); // Assumes no crash if invalid player

    // 3. Querying Matches (10 marks)
    vector<Match> matches2024 = db.getMatchesByYear(2024);
    printTestResult("3.1 Retrieves Matches for Given Year", !matches2024.empty(), 5);
    printTestResult("3.2 Correct Year Retrieved", matches2024[0].getYear() == 2024, 5);

    // 4. Finding Players Based on Performance (20 marks)
    //CricketDatabase db3;
    //db3.processFile("test_data.txt");
    // this will be in hidden test case
    string bestBowler = db.findBestBowler();

    printTestResult("4.1 Best Bowler Found", !bestBowler.empty(), 5);

    string mostConsistent = db.findMostConsistentPlayer();
    printTestResult("4.2 Most Consistent Player Found", !mostConsistent.empty(), 5);

    string topRunScorer = db.findMostRunsScorer();
    printTestResult("4.3 Most Runs Scorer Found", !topRunScorer.empty(), 5);

    vector<string> topBatters = db.getTopNBatters(2);
    printTestResult("4.4 Retrieves Top Batters", topBatters.size() == 2, 5);
    for(auto p: topBatters){
        cout << p << endl;
    }

    // 5. Finding Best Teams (10 marks)
    string mostWins = db.findTeamWithMostWins();
    printTestResult("5.1 Team with Most Wins Found", !mostWins.empty(), 5);

    string bestWinRate = db.findTeamWithBestWinPercentage();
    printTestResult("5.2 Team with Best Win Percentage Found", !bestWinRate.empty(), 5);

    // 6. Testing getTopBowlers (5 marks)
    vector<string> topBowlers = db.getTopNBowlers(2);
    printTestResult("6.1 Retrieves Top Bowlers", topBowlers.size() == 2, 5);
    for(auto p: topBowlers){
        cout << p << endl;
    }

    // 7. Testing getClosestMatches (5 marks)
    vector<Match> closestMatches = db.getClosestMatches();
    printTestResult("7.1 Retrieves Closest Matches", closestMatches.size() == 1, 5); 

    cout << YELLOW << "\nPoints: " << points << " / 110" << RESET << endl;
}

int main() {
    testTeamClass();
    testMatchClass();
    testCricketDatabase();
    
    if(points == 110){
        cout << "All tests passed! Great job!\n";
    }
    return 0;
}
