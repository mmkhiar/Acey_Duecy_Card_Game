#ifndef GAMESTATS_H
#define GAMESTATS_H

#include <map>                  
#include <set>
#include <unordered_map>
#include <array>

#include <list>                 
#include <forward_list>
#include <queue>
#include <stack>
#include <string>
#include <iostream>

#include "Card.h"

using namespace std;

class GameStats {
private:
    // Associative containers
    map<string,int> stats;                  // wins, losses, rounds
    set<int> ranksSeen;                     // unique ranks
    unordered_map<int,int> rankCount;       // frequency of ranks

    // Sequence containers
    list<int> betHistory;                   // list replacement
    list<int> roundHistory;                 // list replacement
    forward_list<int> recentRanks;          // forward_list replacement

    // Container adaptors
    stack<Card> lastDrawn;                  // stack replacement
    queue<string> messages;                 // queue replacement
    priority_queue<int> highBets;           // priority_queue replacement

public:
    GameStats();

    void logRound(bool won, int bet, const array<Card,3> &hand);
    void printStats();
    void logSkip(const array<Card,3> &hand);
};

#endif

