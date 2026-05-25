#include "FunMode.h"
using namespace std;

bool FunMode::usesBet() const {
    return false;
}

void FunMode::settleRound(Player &ply, int betAmt, bool won) {
    (void)ply;
    (void)betAmt;
    (void)won;
}