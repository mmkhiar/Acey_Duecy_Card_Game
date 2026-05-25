#include "SaveSystem.h"

SaveSystem::SaveSystem(const char* fname):fileName(fname) {
    slotInit();
}

SaveSystem::~SaveSystem() {
}

const char* SaveSystem::modeTxt(Mode m) const {
    return (m==MODE_BET)?"BET":"FUN";
}

bool SaveSystem::slotInit() {
    fstream fs(fileName, ios::in | ios::out | ios::binary);
    if (!fs) {
        ofstream create(fileName, ios::out | ios::binary);
        if (!create) return false;

        SaveSlot empty{};
        empty.used=0;
        for (int i=0; i<MAX_SLOTS; i++) {
            create.write(reinterpret_cast<const char*>(&empty), sizeof(SaveSlot));
            if (!create) return false;
        }
        return true;
    }

    fs.seekg(0, ios::end);
    streamoff bytes=fs.tellg();
    streamoff need =static_cast<streamoff>(MAX_SLOTS) * sizeof(SaveSlot);

    if (bytes<need) {
        fs.close();
        ofstream pad(fileName, ios::app | ios::binary);
        if (!pad) return false;

        SaveSlot empty{};
        empty.used=0;

        streamoff diff=need-bytes;
        int missing=static_cast<int>((diff+sizeof(SaveSlot)-1)/sizeof(SaveSlot));

        for (int i=0; i<missing; i++) {
            pad.write(reinterpret_cast<const char*>(&empty), sizeof(SaveSlot));
            if (!pad) return false;
        }
    }
    return true;
}

bool SaveSystem::slotRead(SaveSlot &rec, int idx) {
    if (idx<0||idx>=MAX_SLOTS) return false;
    ifstream fs(fileName, ios::in | ios::binary);
    if (!fs) return false;

    streampos pos=static_cast<streampos>(idx) * static_cast<streamoff>(sizeof(SaveSlot));
    fs.seekg(pos, ios::beg);
    if (!fs) return false;

    fs.read(reinterpret_cast<char*>(&rec), sizeof(SaveSlot));
    return static_cast<bool>(fs);
}

bool SaveSystem::slotWrite(const SaveSlot &rec, int idx) {
    if (idx<0||idx>=MAX_SLOTS) return false;
    fstream fs(fileName, ios::in | ios::out | ios::binary);
    if (!fs) return false;

    streampos pos=static_cast<streampos>(idx) * static_cast<streamoff>(sizeof(SaveSlot));
    fs.seekp(pos, ios::beg);
    if (!fs) return false;

    fs.write(reinterpret_cast<const char*>(&rec), sizeof(SaveSlot));
    return static_cast<bool>(fs);
}

void SaveSystem::slotList() {
    cout<<"==============================  SAVE SLOTS ======================================\n";
    for (int i=0; i<MAX_SLOTS; i++) {
        SaveSlot rec{};
        bool ok=slotRead(rec, i);
        cout<<(i+1)<<") ";

        if (!ok||rec.used==0) {
            cout<<"[EMPTY]\n";
        } else {
            Mode m=(rec.mode==(uint8_t)MODE_BET)?MODE_BET:MODE_FUN;
            cout<<rec.name<<" | $"<<fixed<<setprecision(0)<<rec.bal<<" | "<<modeTxt(m)<<"\n";
        }
    }
    cout<<"================================================================================\n\n";
}

int SaveSystem::slotPick(const char *msg) {
    int slot=0;
    while (true) {
        cout<<msg<<" (1-"<<MAX_SLOTS<<", 0 to cancel): ";
        cin>>slot;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Enter a number.\n";
            continue;
        }
        if (slot==0) return -1;
        if (slot>=1 && slot <= MAX_SLOTS) return slot-1; // convert to 0-based
        cout<<"Pick a slot between 1 and "<<MAX_SLOTS<<".\n";
    }
}

bool SaveSystem::loadMenu(Player &ply, Mode &mode) {
    if (!slotInit()) {
        cerr<<"Could not initialize save file.\n";
        return false;
    }

    slotList();
    int idx=slotPick("Load which slot?");
    if (idx<0) return false;

    SaveSlot rec{};
    if (!slotRead(rec, idx)||rec.used==0) {
        cout<<"That slot is empty.\n\n";
        return false;
    }

    ply.setName(rec.name);
    ply.setBal(rec.bal);
    mode=(rec.mode==(uint8_t)MODE_BET)?MODE_BET:MODE_FUN;

    cout<<"Loaded "<<ply.getName()
        <<" with $"<<fixed<<setprecision(0)<<ply.getBal()
        <<" ("<<modeTxt(mode)<<").\n\n";

    return true;
}

bool SaveSystem::clearSlot(int idx) {
    if (idx<0||idx>=MAX_SLOTS) return false;

    SaveSlot empty{};
    empty.used=0;
    empty.mode=0;
    empty.bal =0.0f;
    empty.name[0]='\0';

    return slotWrite(empty, idx);
}

bool SaveSystem::clearMenu() {
    if (!slotInit()) {
        cerr<<"Could not initialize save file.\n";
        return false;
    }

    slotList();
    int idx=slotPick("Clear which slot?");
    if (idx<0) return false;

    SaveSlot cur{};
    if (!slotRead(cur, idx)||cur.used==0) {
        cout<<"That slot is already empty.\n\n";
        return false;
    }

    cout<<"Clear slot "<<(idx+1)<<" ("<<cur.name<<")? (y/n): ";
    char yn;
    cin>>yn;
    cout<<endl;

    if (!(yn=='y'||yn=='Y')) {
        cout<<"Clear canceled.\n\n";
        return false;
    }

    if (!clearSlot(idx)) {
        cerr<<"Error clearing slot.\n\n";
        return false;
    }

    cout<<"Slot "<<(idx+1)<<" cleared.\n\n";
    return true;
}

bool SaveSystem::saveMenu(const Player &ply, Mode mode) {
    if (!slotInit()) {
        cerr<<"Could not initialize save file.\n";
        return false;
    }

    slotList();
    int idx=slotPick("Save to which slot?");
    if (idx<0) return false;


    SaveSlot cur{};
    if (slotRead(cur, idx) && cur.used==1) {
        cout<<"Slot "<<(idx+1)<<" currently has "<<cur.name
            <<". Overwrite? (y/n): ";
        char yn;
        cin>>yn;
        cout<<endl;
        if (!(yn=='y'||yn=='Y')) {
            cout<<"Save canceled.\n\n";
            return false;
        }
    }

    SaveSlot rec{};
    rec.used=1;
    strncpy(rec.name, ply.getName().c_str(), NAME_MAX-1);
    rec.name[NAME_MAX-1]='\0';
    rec.mode=(uint8_t)mode;
    rec.bal=ply.getBal();

    if (!slotWrite(rec, idx)) {
        cerr<<"Error writing save slot.\n\n";
        return false;
    }

    cout<<"Saved to slot "<<(idx+1)<<".\n\n";
    return true;
}
