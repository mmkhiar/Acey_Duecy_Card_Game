#include "GameStats.h"                                  // GameStats class

#include <algorithm>                                    // Algorithm Library
#include <iomanip>                                      // Format Library

GameStats::GameStats(){                           
    stats["wins"]=0;                                                    
    stats["losses"]=0;                          
    stats["rounds"]=0;
    stats["skips"]=0;
}

void GameStats::logRound(bool won, int bet, const array<Card,3> &hand) {        

    stats["rounds"]++;
    if (won) stats["wins"]++;
    else stats["losses"]++;

    betHistory.push_back(bet);

    highBets.push(bet);

    roundHistory.push_back(won ? 1:0);

    for (int i=0; i<3; i++) {
        int rank=hand[i].getRank();

        ranksSeen.insert(rank);         
        rankCount[rank]++;                      

        recentRanks.push_front(rank);   
        lastDrawn.push(hand[i]);        
    }

    // Queue message
    if (won) messages.push("Win");
    else messages.push("Loss");

    if (messages.size()>5) {
        messages.pop();
    }
}

void GameStats::logSkip(const array<Card,3> &hand) {

    stats["rounds"]++;
    stats["skips"]++;

    roundHistory.push_back(0);

    for (int i=0; i<3; i++) {
        int rank=hand[i].getRank();

        ranksSeen.insert(rank);
        rankCount[rank]++;

        recentRanks.push_front(rank);
        lastDrawn.push(hand[i]);
    }

    messages.push("Skip");

    if (messages.size()>5) {
        messages.pop();
    }
}

void GameStats::printStats() {

    cout<<"\n==============================  GAME STATS  =====================================\n";

    cout<<"\nBASIC RESULTS\n";
    cout<<"--------------------------------------------------------------------------------\n";
    for (auto it=stats.begin(); it!=stats.end(); ++it) {
        cout<<left<<setw(12)<<it->first<<": "<<it->second<<endl;
    }

    cout<<"\nBET SUMMARY\n";
    cout<<"--------------------------------------------------------------------------------\n";

    cout<<left<<setw(22)<<"Bet History"<<": ";
    for_each(betHistory.begin(), betHistory.end(), [](int b){
        cout<<b<<" ";
    });
    cout<<endl;

    int zeroBets=count(betHistory.begin(), betHistory.end(), 0);
    cout<<left<<setw(22)<<"Zero Bets"<<": "<<zeroBets<<endl;

    list<int> cleanBets=betHistory;
    cleanBets.remove(0);

    list<int> sortedBets=betHistory;
    sortedBets.sort();

    cout<<left<<setw(22)<<"Sorted Bets"<<": ";
    for (int b:sortedBets) {
        cout<<b<<" ";
    }
    cout<<endl;

    if (!cleanBets.empty()) {
        cout<<left<<setw(22)<<"Min Bet"<<": "<<*min_element(cleanBets.begin(), cleanBets.end())<<endl;
        cout<<left<<setw(22)<<"Max Bet"<<": "<<*max_element(cleanBets.begin(), cleanBets.end())<<endl;
    } else {
        cout<<left<<setw(22)<<"Min Bet"<<": N/A"<<endl;
        cout<<left<<setw(22)<<"Max Bet"<<": N/A"<<endl;
    }

    cout<<left<<setw(22)<<"Clean Bets No Zeroes"<<": ";
    for (int b:cleanBets) {
        cout<<b<<" ";
    }
    cout<<endl;

    cout<<"\nCARD TRACKING\n";
    cout<<"--------------------------------------------------------------------------------\n";

    cout<<left<<setw(22)<<"Unique Ranks Seen"<<": ";
    for (int r:ranksSeen) {
        cout<<Card::rankToFace(r)<<" ";
    }
    cout<<endl;

    map<int,int> freqMap(rankCount.begin(), rankCount.end());

    cout<<left<<setw(22)<<"Rank Frequencies"<<": ";
    for (auto &p:freqMap) {
        cout<<"["<<Card::rankToFace(p.first)<<"="<<p.second<<"] ";
    }
    cout<<endl;

    cout<<"\nRECENT ACTIVITY\n";
    cout<<"--------------------------------------------------------------------------------\n";

    queue<string> tempQ=messages;
    cout<<left<<setw(22)<<"Recent Results"<<": ";
    while (!tempQ.empty()) {
        cout<<tempQ.front()<<" ";
        tempQ.pop();
    }
    cout<<endl;

    priority_queue<int> tempPQ=highBets;
    cout<<left<<setw(22)<<"Top Bets"<<": ";
    while (!tempPQ.empty()) {
        cout<<tempPQ.top()<<" ";
        tempPQ.pop();
    }
    cout<<endl;

    stack<Card> tempStack=lastDrawn;
    cout<<left<<setw(22)<<"Last Drawn Cards"<<": ";
    int cnt=0;
    while (!tempStack.empty() && cnt<5) {
        cout<<tempStack.top()<<" | ";
        tempStack.pop();
        cnt++;
    }
    cout<<endl;

    cout<<"================================================================================\n";
}