#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "Player.h"

class GameMode {
protected:
    bool lastWin;   

public:
    virtual ~GameMode(){}
    virtual bool usesBet()const = 0;
    virtual void settleRound(Player &ply, int betAmt, bool won) = 0;
};

#endif
