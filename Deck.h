#ifndef DECK_H
#define DECK_H

#include <vector>
#include <random>
#include "Card.h"

using namespace std;

class Deck {
private:
    vector<Card> cards;
    mt19937 rng;

public:
    Deck();                 // builds and shuffles
    void reset();           // rebuilds full deck
    void shuffleDeck();     // shuffles current deck
    Card draw();            // draw top card (throws if empty)
    int  size() const;      // cards remaining (inline-ish)
};

#endif
