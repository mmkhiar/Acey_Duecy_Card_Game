#ifndef FUNMODE_H
#define FUNMODE_H

#include "GameMode.h"

class FunMode : public GameMode {
public:
    bool usesBet() const override;
    void settleRound(Player &ply, int betAmt, bool won) override;
};

#endif
