#include "Deck.h"
#include <stdexcept>
#include <algorithm>

Deck::Deck() : rng(random_device{}()) {
    reset();
    shuffleDeck();
}

void Deck::reset() {
    cards.clear();
    cards.reserve(52);

    for (int s = CLUBS; s <= SPADES; ++s) {
        for (int r = 2; r <= 14; ++r) {          // 2..14 (A=14)
            cards.push_back(Card(r, static_cast<Suit>(s)));
        }
    }
}

void Deck::shuffleDeck() {
    shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::draw() {
    if (cards.empty()) {
        throw runtime_error("Deck is empty.");
    }
    Card top = cards.back();
    cards.pop_back();
    return top;
}

int Deck::size() const {
    return static_cast<int>(cards.size());
}
