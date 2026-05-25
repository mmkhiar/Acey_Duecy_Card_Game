#include "BetMode.h"
#include <iomanip>
using namespace std;

bool BetMode::usesBet() const {
    return true;
}

void BetMode::settleRound(Player &ply, int betAmt, bool won) {
    lastWin=won;
    float bal=ply.getBal();
    if (won) bal+=betAmt;
    else bal-=betAmt;
    ply.setBal(bal);
}
