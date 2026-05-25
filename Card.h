#include <iostream>
#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };

class Card {
private:
    int  rank;   
    Suit suit;   

public:
    
    Card();
    Card(int r, Suit s);

    inline int  getRank() const { return rank; }
    inline Suit getSuit() const { return suit; }

    inline void setRank(int r) { rank = r; }
    inline void setSuit(Suit s) { suit = s; }

    string face() const;
    static string rankToFace(int rank);
    const char* suitText() const;
    string desc() const;
    

    friend ostream& operator<<(ostream& os, const Card& c);
    bool operator<(const Card& other) const;
    bool operator==(const Card& other) const;

};

#endif
