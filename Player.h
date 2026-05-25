#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <cstring>
#include <iostream>

using namespace std;

class SaveSystem;  

class Player {
private:
    string name;              
    float  balance;           
    static int pCnt;          

public:
    Player();
    Player(const string& nm, float bal = 0.0f);
    ~Player();
    Player(const Player& other);
    Player& operator=(const Player& other);

    inline const string& getName() const { return name; }
    inline float getBal() const { return balance; }

    void setName(const string& nm);
    void setBal(float bal);

    void addBal(float amt);
    void subBal(float amt);

    void copyNameTo(char out[], int max) const;
    void loadNameFrom(const char in[]);


    static int getCnt();

    friend class SaveSystem;
};

#endif
