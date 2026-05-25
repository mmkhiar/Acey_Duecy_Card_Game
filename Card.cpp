#include "Card.h"

Card::Card() : rank(2), suit(CLUBS) {}

Card::Card(int r, Suit s) : rank(r), suit(s) {}

string Card::face() const {
    return rankToFace(rank);
}

const char* Card::suitText() const {
    switch (suit) {
        case CLUBS: return "Clubs";
        case DIAMONDS: return "Diamonds";
        case HEARTS: return "Hearts";
        case SPADES:return "Spades";
        default: return "Unknown";
    }
}


string Card::desc() const {
    return face()+" of "+string(suitText());
}

ostream& operator<<(ostream& os, const Card& c) {
    os<<c.face()<<" of "<<c.suitText();
    return os;
}

bool Card::operator<(const Card& other) const {
    return rank<other.rank;
}

bool Card::operator==(const Card& other) const {
    return rank==other.rank;
}
string Card::rankToFace(int rank){
    switch(rank){
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        case 14: return "A";
        default: return to_string(rank);
    }
}