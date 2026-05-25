#include "Player.h"

int Player::pCnt=0;

int Player::getCnt() {
    return pCnt;
}

Player::Player() : name(""), balance(0.0f) {
    ++pCnt;
}

Player::Player(const string& nm, float bal) : name(nm), balance(bal) {
    ++pCnt;
}

Player::Player(const Player& other) : name(other.name), balance(other.balance) {
    ++pCnt;
}

Player::~Player() {
    --pCnt;
}

void Player::setName(const string& nm) {
    name=nm;
}

void Player::setBal(float bal) {
    balance=bal;
}

void Player::addBal(float amt) {
    balance+=amt;
}

void Player::subBal(float amt) {
    balance-=amt;
}

void Player::copyNameTo(char out[], int max) const {
    if (max <= 0) return;
    strncpy(out, name.c_str(), max-1);
    out[max-1]='\0';
}

void Player::loadNameFrom(const char in[]) {
    if (in) name=in;
    else name="";
}

Player& Player::operator=(const Player& other) {
    if (this!=&other) {
        name=other.name;
        balance=other.balance;
    }
    return *this;
}