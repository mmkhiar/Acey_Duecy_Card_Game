#ifndef BETMODE_H
#define BETMODE_H

#include "GameMode.h"

class BetMode : public GameMode {
public:
    bool usesBet() const override;
    void settleRound(Player &ply, int betAmt, bool won) override;
};

#endif
