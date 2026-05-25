#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstdint>
#include <cstring>

#include "GameTypes.h"
#include "Player.h"

using namespace std;

class SaveSystem {
public:
    static const int MAX_SLOTS=9;
    static const int NAME_MAX =30;

private:
    struct SaveSlot {
        uint8_t used;
        char    name[NAME_MAX];
        uint8_t mode;
        float   bal;
    };

    const char* fileName;

    bool slotInit();
    bool slotRead(SaveSlot &rec, int idx);
    bool slotWrite(const SaveSlot &rec, int idx);

    bool clearSlot(int idx);
    void slotList();
    int  slotPick(const char *msg);
    const char* modeTxt(Mode m) const;

public:
    SaveSystem(const char* fname="player_game_info.dat");
    ~SaveSystem();

    bool loadMenu(Player &ply, Mode &mode);
    bool saveMenu(const Player &ply, Mode mode);

    bool clearMenu();
};


#endif
