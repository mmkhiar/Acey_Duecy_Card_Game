Description:		There are many variations of the Acey Duecy game but in essence, 2 cards are drawn face up and the 
*					player has to bet if the third card will be in between those two cards.
*					The name Acey Deucy comes from the best possible two cards for the player, a two and an Ace! 
*					There is a good chance the player will win UNLESS the third card is an Ace or a two!!
*                   That is a 11 over 13 chance (84.6% chance). A little over 4/5 win rate.
*/

//System Libraries
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "GameTypes.h"
#include "SaveSystem.h"
#include <memory>
#include "GameMode.h"
#include "FunMode.h"
#include "BetMode.h"
#include "GameStats.h"

#include <iostream>                         // Input/Output Library
#include <string>                           // String Library
#include <cstdlib>                          // Random Library
#include <cmath>                            // Math Library
#include <iomanip>                          // Format Library
#include <fstream>                          // File Library                         
#include <limits>                           // Limit Library
#include <cstring>                          // String Library   
#include <cctype>                           // Character Library
#include <cstdint>                          // Integer Library
#include <algorithm>                        // Algorithm Library
#include <array>                            // Array Library

using namespace std;


void Intro();
char Instructions(const string &playername);
string name();
void chooseGameMode(const string &Name, bool saved, bool &BetFlag, float &Blnce);
bool isYes(char c);
void toUpperStr(char s[]);
bool askAgain(const string &Name, char &YorN);
bool parseBetInput(const string &input, int &bet);



int main(int argv, char **argc){
	
    //Declare Variables
        char    YorN,                           // Yes or No for Game   (Global to game)
                RYorN;                          // Yes or No for Round  (Local to game logic)
        bool    BetFlag=false,                  // Does the player want to bet or not?
                ply_flg=true,                   // Does the player want to play after running out of money?
                saved =false,                   // Was the game loaded?
                earlyExit=false;                // Exit game loop
        string  Name,                           // Name of Player
                Fcard01,                        // Face rank for card 1
                Fcard02,                        // Face rank for card 2
                Fcard03;                        // Face rank for card 3
        float   Blnce=0;                        // Starting Balance
        int     betAmnt=0,                      // Amount to wager per hand
                card01=0,                       // Card 1
                card02=0,                       // Card 2
                card03=0;                       // Card 3
        Deck    deck;
        
        array<Card,3> hand;                     // Array of Cards
        Player ply;                             // dynamic allocation
        ply.setBal(Blnce);                      // set balance
        ply.setName("");                        // empty C-string
        Mode mode=MODE_FUN;                     // Game Mode
        SaveSystem save;                        // Save System
        unique_ptr<GameMode> modeObj;           // dynamic allocation
        GameStats stats;                        // Game Statistics


	//Initialize Values
    Intro();

        // Open File?
        cout<<"============================== GAME START ======================================"<<endl;
        cout<<"1) Load saved game\n";
        cout<<"2) New game\n";
        cout<<"3) Clear a save slot\n";
        cout<<"Choose: "<<endl;
        cout<<"================================================================================"<<endl;

        int opt;
        cin>>opt;
        cout<<endl;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opt=2;
        }

        if (opt==1) {
            if (save.loadMenu(ply, mode)) {
                saved=true;
                Name=ply.getName();
                Blnce=ply.getBal();
                BetFlag=(mode==MODE_BET);
            } else {
                saved=false;
                Name=name();
                ply.setName(Name);
                ply.setBal(0);
                Blnce=0;
                mode=MODE_FUN;
                BetFlag=false;
            }
        }
        else if (opt==3) {
            save.clearMenu();      
            Name=name();
            ply.setName(Name);
            ply.setBal(0);
            Blnce=0;
            mode=MODE_FUN;
            BetFlag=false;
        }
        else {
            Name=name();
            ply.setName(Name);
            ply.setBal(0);
            Blnce=0;
            mode=MODE_FUN;
            BetFlag=false;
        }

        char shout[30];
        strncpy(shout, ply.getName().c_str(), sizeof(shout)-1);
        shout[sizeof(shout)-1]='\0';
        toUpperStr(shout);
        cout<<"Let's GO, "<<shout<<"!"<<endl<<endl;


        //Instructions
        cout<<"Would you like to read the instructions on how to play? (y/n)"<<endl<<endl;
        cin>>YorN;
        cout<<endl;
        if (YorN!='Y' && YorN!='y' && YorN!='N' && YorN!='n'){                          // If player chooses something outside of 'y' or 'n'
            do {                                                                            // Do loop to keep on track
            cout<<Name<<", Please choose a 'y' for Yes or a 'n' for a No."<<endl<<endl;
            cout<<"Would you like to read the instructions on how to play? (y/n)"<<endl;
            cout<<endl;
            cin>>YorN;
            cout<<endl;
            }while (YorN!='Y' && YorN!='y' && YorN!='N' && YorN!='n');                  // Do loop ends when a proper input is given
        }
        else if (isYes(YorN)){
            char response;
            response=Instructions(Name);
            if(response=='n'||response=='N'){
                return 0;
            }
        }
        // Betting setting
do{
    
    chooseGameMode(Name, saved, BetFlag, Blnce);
    ply.setBal(Blnce);
    mode=BetFlag ? MODE_BET : MODE_FUN;

    if (mode==MODE_BET) modeObj=make_unique<BetMode>();
    else modeObj=make_unique<FunMode>();


    // Game intro
    cout<<"================================================================================"<<endl;
    cout<<endl;
    cout<<"\t\t\tOkay! Let's get started!"<<endl<<endl;
    cout<<"================================================================================"<<endl;
    cout<<endl;

    // This is where the game loop begins.
    do{        
        if (deck.size()<3) {
            deck.reset();
            deck.shuffleDeck();
        }

        hand[0]=deck.draw();
        hand[1]=deck.draw();
        hand[2]=deck.draw();

        if (hand[1]<hand[0]) {
            swap(hand[0], hand[1]);
        }

        card01=hand[0].getRank();
        card02=hand[1].getRank();
        card03=hand[2].getRank();

        // Face cards
        Fcard01=hand[0].face();
        Fcard02=hand[1].face();
        Fcard03=hand[2].face();
        
        // Betting logic
        betAmnt=0;

        // Only BET mode asks for bet
            betAmnt=0;

            if (modeObj->usesBet()) {
                cout<<"Enter your bet amount: (end to quit)"<<endl<<endl;
                cout<<"$ ";
                string betIn;
                cin>>betIn;

                int tempBet;

                if (!parseBetInput(betIn, tempBet)) {

                    cout<<"\nWould you like to save your game for later? (y/n)\n\n";
                    char saveChoice;
                    cin>>saveChoice;

                    if (isYes(saveChoice)) {
                        ply.setBal(Blnce);
                        ply.setName(Name);
                        save.saveMenu(ply, mode);
                    }

                    cout<<"\nThanks for playing "<<Name<<"!";
                    if (modeObj->usesBet()) cout<<" Your ending balance is $"<<Blnce<<".";
                    cout<<" Have a good one!\n"<<endl;

                    earlyExit=true;
                    break;
                }

                bool validNumber = true;
                for (char c : betIn) {
                    if (!isdigit(c)) {
                        validNumber = false;
                        break;
                    }
                }

                if (!validNumber) {
                    cout<<"Invalid input. Enter a number or 'q' to quit.\n";
                    continue;
                }

                betAmnt = tempBet;

                if (betAmnt>Blnce) {
                    do {
                        cout<<endl;
                        cout<<"Hold your horses "<<Name<<"! Your bet amount exceeds your balance!"<<endl;
                        cout<<"Please place a bet less than your account size of $"<<Blnce<<" (end to quit):"<<endl<<endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        string betIn;
                        cin>>betIn;

                        int tempBet;

                        if (!parseBetInput(betIn, tempBet)) {

                            cout<<"\nWould you like to save your game for later? (y/n)\n\n";
                            char saveChoice;
                            cin>>saveChoice;

                            if (isYes(saveChoice)) {
                                ply.setBal(Blnce);
                                ply.setName(Name);
                                save.saveMenu(ply, mode);
                            }

                            cout<<"\nThanks for playing "<<Name<<"!";
                            if (modeObj->usesBet()) cout<<" Your ending balance is $"<<Blnce<<".";
                            cout<<" Have a good one!\n"<<endl;

                            earlyExit=true;
                            break;
                        }   

                        bool validNumber = true;
                        for (char c : betIn) {
                            if (!isdigit(c)) {
                                validNumber = false;
                                break;
                            }
                        }

                        if (!validNumber) {
                            cout<<"Invalid input. Enter a number or 'q' to quit.\n";
                            continue;
                        }

                        betAmnt = tempBet;

                        if (cin.fail()) {
                            cout<<endl;
                            cout<<"\nThanks for playing "<<Name<<"! Your ending balance is $"<<Blnce<<". Have a good one!\n"<<endl;
                            return 0;
                        }
                    } while (betAmnt>Blnce);
                }

            }


        //Card Design
            
            //  Two cards side by side
            cout<<"\t\t┌─────┐\t\t\t\t┌─────┐"<<endl;                                   // Top Section of Card01 and Card02
            for (int i=0; i<3; i++){                                                        // Middle Section of Card01 and Card02
                if (i==1) {                                                                 // Middle of the card should have the string displayed.                                                             
                    if(Fcard01=="10"){                                                      // If a 10 is presented, it interferes with the width of the cards
                        cout<<"\t\t│ "<<Fcard01<<"  │\t\t\t\t│  "<<Fcard02<<"  │"<<endl;
                        }
                        else if(Fcard02=="10"){                                                 
                        cout<<"\t\t│ "<<left<<setw(2)<<Fcard01<<"  │\t\t\t\t│ "<<Fcard02<<"  │"<<endl;
                        }
                        else{                                                                   
                        cout<<"\t\t│  "<<Fcard01<<"  │\t\t\t\t│  "<<Fcard02<<"  │"<<endl;
                        }
                    }
                else{                                                                        // Else, a wall should be built
                    cout<<"\t\t│     │\t\t\t\t│     │"<<endl;
                }
            }
            cout<<"\t\t└─────┘\t\t\t\t└─────┘"<<endl;                                   // Bottom Section of Card01 and Card02

            cout<<"\t       Card 1| "<<Fcard01<<"\t\t      Card 2| "<<Fcard02<<"\t"<<endl<<endl;
            cout<<"Will the third card be between card #1 ("<<Fcard01<<") and card #2 ("<<Fcard02<<")? (y/n)"<<endl; 
            cout<<"(end to quit)"<<endl<<endl;
            cin>>RYorN;           // A new Yes or No local variable is implemented here to not intefere with global Yes or No variable that controls if player wants to play
            cout<<endl;

        if (RYorN=='Y'||RYorN=='y'){                                                      // Player thinks Card03 will fall between Card01 and Card02
            
            //  Solo Card
            cout<<"\t\t\t\t┌─────┐"<<endl;                                              // Top of solo card03
            for (int i=0; i<3; i++){
                if (i==1){                                                                  // Display string in the center of the solo card
                    if(Fcard03=="10"){
                        cout<<"\t\t\t\t│ "<<Fcard03<<"  │"<<endl;                           // If string is a 10
                    }
                    else{
                        cout<<"\t\t\t\t│ "<<right<<setw(2)<<Fcard03<<"  │"<<endl;           // If string is not a 10
                    }
                }
                else{
                    cout<<"\t\t\t\t│     │"<<endl;
                    }
                }
            
            cout<<"\t\t\t\t└─────┘"<<endl;                                                         // Bottom of the card
            cout<<"\t\t\t\tCard 3| "<<Fcard03<<endl;                                                   // Text of what the card is
            cout<<"\t\t\t\t("<<hand[2]<<")"<<endl;                                   // Description of the card
            cout<<endl;
            
            bool won=(card01<card03 && card03<card02);
            stats.logRound(won, betAmnt, hand);

            modeObj->settleRound(ply, betAmnt, won);
            Blnce=ply.getBal();

        // Game Calculations and Comparisons
                if (won) {
                    cout<<"You win!"<<endl<<endl;
                    if (modeObj->usesBet()) {
                        cout<<"You win $"<<betAmnt<<". Your new balance is $"<<Blnce<<"."<<endl;
                    }
                    ply_flg=askAgain(Name, YorN);
                    if (!ply_flg) YorN='n';
                }
                else {
                    // Lose reason text
                    if (hand[0].getRank()==hand[2].getRank()) {
                        cout<<"You lose! The third card hit the lower boundry! Sad."<<endl<<endl;
                    }
                    else if (hand[1].getRank()==hand[2].getRank()) {
                        cout<<"You lose! The third card hit the upper boundry! Sad."<<endl<<endl;
                    }
                    else {
                        cout<<"You lose! The third card is NOT between "<<Fcard01<<" and "<<Fcard02<<"."<<endl<<endl;
                    }
                    
                    if (modeObj->usesBet()) {
                        cout<<"You lose $"<<betAmnt<<". Your new balance is $"<<Blnce<<"."<<endl<<endl;
                    }

                    if (Blnce>0) {
                        ply_flg=askAgain(Name, YorN);
                        if (!ply_flg) YorN='n';
                    } else {
                        cout<<"Sorry bud, your balance is now $0. Super sad=("<<endl;
                        cout<<"\nThanks for playing "<<Name<<"!!"<<endl<<endl;
                        cout<<"Would you like to play again from the start "<<Name<<"? (y/n)"<<endl;
                        cout<<"(end to quit)"<<endl<<endl;
                        cin>>YorN;
                        if (isYes(YorN)){
                            ply_flg=true;
                            YorN='N';   // restart mode selection
                        } else {
                            ply_flg=false;
                        }
                    }
                }
        }
        else if (RYorN=='N'||RYorN=='n'){                                                 // Player DOES NOT thinks Card03 will fall between Card01 and Card02
            stats.logSkip(hand);
            //  Solo Card                                                                   // Card design repeats from above
            cout<<"\t\t\t\t┌─────┐"<<endl;
            for (int i=0; i<3; i++){
                if (i==1) {                                                                 // Display string in the center of the solo card
                    if(Fcard03=="10"){
                        cout<<"\t\t\t\t│ "<<Fcard03<<"  │"<<endl;                           // If string is a 10
                    }
                    else{
                        cout<<"\t\t\t\t│ "<<right<<setw(2)<<Fcard03<<"  │"<<endl;           // If string is not a 10
                    }
                }
                else{
                cout<<"\t\t\t\t│     │"<<endl;
                }
            }
            cout<<"\t\t\t\t└─────┘"<<endl;
            cout<<"\t\t\t\tCard 3| "<<Fcard03<<endl;
            cout<<"\t\t\t\t("<<hand[2]<<")"<<endl;

                if (card01<card03 && card03<card02){                                        // If player opts to skip round BUT the Card03 does end up betweeen Card01 and Card02
                    cout<<"The third card IS BETWEEN the first two! You missed!"<<endl;
                        if (modeObj->usesBet()){
                            cout<<"You win nothing! Your balance is $"<<Blnce<<"."<<endl;
                        }
                    cout<<"Would you like to play again "<<Name<<"? (y/n)"<<endl;
                    cout<<"(end to quit)"<<endl<<endl;
                    cin>>YorN;
                    cout<<endl;
                    if (isYes(YorN)){
                        ply_flg=true;
                    }else{
                        ply_flg=false;
                    };   
                }
                else {                                                                      // If player opts to skip round AND the Card03 does NOT end up betweeen Card01 and Card02
                    cout<<"Nice save!"<<endl<<endl;
                        if (modeObj->usesBet()){
                            cout<<"Your balance is $"<<Blnce<<"."<<endl<<endl;
                        }
                    cout<<"Would you like to play again "<<Name<<"? (y/n)"<<endl;
                    cout<<"(end to quit)"<<endl<<endl;
                    cin>>YorN;
                    cout<<endl;
                    if (isYes(YorN)){
                        ply_flg=true;
                    }else{
                        ply_flg=false;
                    };  
                };
            }
        else{
            ply_flg=false;
        }                                                                                   // This Do loop repeats as long as the global Yes or No stays as a Yes.
    }while(isYes(YorN));                                                         // As soon as player does not want to play anymore, or the balance goes below zero, the game ends.
}while(ply_flg==true);

    cout<<"Would you like to save your game for later? (y/n)"<<endl<<endl;
    cin>>YorN;
    cout<<endl;

    if(isYes(YorN)){
        ply.setBal(Blnce);
        ply.setName(Name);

        save.saveMenu(ply, mode);
    }

    stats.printStats();
    cout<<"\n\nThanks for playing "<<Name<<"!";
    if (modeObj->usesBet()) cout<<" Your ending balance is $"<<Blnce<<".";
    cout<<" Have a good one!\n"<<endl;                                          

	                                                                                                         //Exit the program
	return 0;                                                                                                // Program end
}	


void Intro(){
    //Intro
    cout<<"================================================================================"<<endl;             // Borders
    cout<<"================================================================================"<<endl;
    cout<<"\t\t\tWelcome to Acey Deucy!!\t\t\t\t"<<endl;                                                        // Game name
    cout<<"================================================================================"<<endl;
    cout<<"================================================================================"<<endl;
    cout<<endl;
    cout<<"In Acey-Deucey, the only guarantee is someone yelling ‘No way!’ at least once!"<<endl<<endl<<endl;   //Silly catch phrase
}

string name() {
    string name;
    cout<<"What is your name?"<<endl<<endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore any leftover newline in the input buffer
    getline(cin, name);
    cout<<endl;
    if (!name.empty()) {
        name[0]=toupper(name[0]);  // Capitalize the first letter
    }
    cout<<"Hi "<<name<<"!!"<<endl<<endl;
    return name;
}

char Instructions(const string &playername){                                                    // If player wants instructions on how to play Acey Deucy
    char    response;
            cout<<endl;
            cout<<"================================================================================"<<endl;
            cout<<endl<<"\t\t\tINSTRUCTIONS"<<endl<<endl;
            cout<<"The dealer draws two random cards and places them face up."<<endl;
            cout<<"You then decide how much to bet based on whether you think the third card will"<<endl;
            cout<<"land between the two drawn cards."<<endl<<endl;
            cout<<"If the card lands between the two cards, you win!"<<endl;
            cout<<"If the card falls outside one of the two cards OR matches one of the two cards,"<<endl;
            cout<<"you lose!"<<endl<<endl;
            cout<<"Remember A is high in this game!!"<<endl<<endl;
            cout<<"The game continues until you run out of money or choose to quit!!"<<endl<<endl<<endl;
            cout<<"Fortune favors the fearless "<<playername<<"!!"<<endl<<endl;

            //Ready to play?
            cout<<"================================================================================"<<endl;
            cout<<endl<<"Would you like to play "<<playername<<"? (y/n)"<<endl;
            cout<<endl;
            cin>>response;
            cout<<endl;

        if (tolower(response)!='y' && tolower(response)!='n'){                              // If player chooses something outside of 'y' or 'n'
            do {                                                                                // Do loop to keep on track
                cout<<playername<<", Please choose a 'y' for Yes or a 'n' for a No."<<endl<<endl;
                cout<<endl<<"Would you like to play "<<playername<<"? (y/n)"<<endl;
                cout<<endl;
                cin>>response;
                cout<<endl;
            }while (tolower(response)!='y' && tolower(response)!='n');}                     // Do loop ends when a proper input is given
        else if (tolower(response)=='n'){                                                       // Allows player to exit without starting
                cout<<"No worries! Have a good one!\n"<<playername<<"!"<<endl;
                cout<<"================================================================================"<<endl;
            }
            return response;
        }


void chooseGameMode(const string &Name, bool saved, bool &BetFlag, float &Blnce){
        char YorN;
        cout<<"================================================================================"<<endl;
        cout<<endl;
        cout<<"\t\t\tGreat! Let's get started "<<Name<<"!"<<endl;
        cout<<endl;
        cout<<"================================================================================"<<endl<<endl;
        cout<<"Would you like to bet or play for fun? ('b' for 'bet' / 'f' for 'Play for Fun')"<<endl;
        // There are two game modes so far. A "Betting mode" or a "Play for fun mode"
        // The betting mode allows player to place a bet while the play for fun mode allows the player to play without worrying about balance and wagers
        cout<<endl;
        cin>>YorN;
        cout<<endl;
        if (YorN=='b'||YorN=='B'){
            BetFlag=true;
            if (saved!=true){
                BetFlag=true;                                                               // There is a betting flag that when true activates the betting and wagers
                cout<<endl;
                cout<<"What is the starting balance?";                                      // This is where we input a starting balance to add and subtract from
                cout<<endl<<endl;
                cin>>Blnce;                                                                     
                
                while(cin.fail()||Blnce<0){
                    cout<<Name<<" please enter a valid positive number for your starting balance."<<endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin>>Blnce;
                }              
                cout<<endl;
                cout<<endl<<Name<<", you have a starting balance of $"<<fixed<<setprecision(0)<<Blnce<<'.'<<endl;         // This is where we re-affirm what the starting balance is
                cout<<endl;
            }
        }else{  
            BetFlag=false;                                                                  // Here is where the betting flag is false. This will not activate the betting mode.   
            cout<<endl;
        }
        }

bool isYes(char c){
    return (c=='y'||c=='Y');
}

void toUpperStr(char s[]){
    for (int i=0; s[i] !='\0'; ++i) {
        unsigned char ch=static_cast<unsigned char>(s[i]);
        s[i]=static_cast<char>(toupper(ch));
    }
}

bool askAgain(const string &Name, char &YorN){
    cout<<"Would you like to play again "<<Name<<"? (y/n)"<<endl<<endl;
    cin>>YorN;
    cout<<endl;
    return isYes(YorN);
}

bool parseBetInput(const string &input, int &bet){

    // Check quit commands
    if (input=="q" || input=="Q" ||
        input=="quit" || input=="QUIT" ||
        input=="exit" || input=="EXIT" ||
        input=="end" || input=="END") {
        return false;  // signal to quit
    }

    for (int i=0; i<input.size(); i++){
        if (!isdigit(input[i])) {
            return true; // invalid, but not quit
        }
    }

    bet = stoi(input);
    return true;
}
