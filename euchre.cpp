// Project UID 1d9f47bfc76643019cfbf037641defe1
//euchre.cpp
#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <array>
#include <vector>
#include <sstream>
#include "Pack.h"
#include "Card.h"
#include "Player.h"

using namespace std;
void shuffleDeck(string shuffleOrNot,Pack pack);
void dealCards(int numCards,int dealer_idx, Player * Players[], Pack &pack);
int idx(int dealer_idx);
int mod_four(int num);
 Card leading_card(int &winner_idx,string trumpCard,Player *Players[],vector<Card>&cardVec);
 void play(Card card, string name);
 void march(int tricksOne, int tricksTwo,bool trumpsOne, bool trumpsTwo);
 void euchered(int tricksOne, int tricksTwo,bool trumpsOne, bool trumpsTwo);
int team_one_points=0;int team_two_points=0;
class Game{
private:
    vector<string> playerNames;
    Pack pack;
    vector<int> team_scores;
    vector<string> playerType;
    string name;
    string type;
    const string shuffleOrNot;
    Player * Players[4];
public:
bool teamOneTrumps=0;bool teamTwoTrumps=0;
int hand_index=0;
int dealer_index=0;
string trumpCard;
     Game(int argc, char*argv[], istream &input)
        :pack(input),shuffleOrNot(argv[2]){
            for(int i=0; i<4;++i){
                name=argv[(2*i)+4];
                playerNames.push_back(name);
                type=argv[(2*i)+5];
                playerType.push_back(type);
            }
            for(int i=0; i<4;++i){
                Players[i]=Player_factory(playerNames[i],playerType[i]);
            }
            if((strcmp(argv[5],"Simple")!=0&&strcmp(argv[5],"Human"))!=0||
        (strcmp(argv[7],"Simple")!=0&&strcmp(argv[7],"Human")!=0)||
        (strcmp(argv[9],"Simple")!=0&&strcmp(argv[9],"Human")!=0)||
        (strcmp(argv[11],"Simple")!=0&&strcmp(argv[11],"Human")!=0)){
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        }
    }

    ~Game(){
        for(int i=0; i<4;++i){
            delete Players[i];
        }
    }
    Card setupTable(){
        shuffleDeck(shuffleOrNot,pack);
         cout<<"Hand "<<hand_index<<endl;
        cout<<Players[dealer_index]->get_name()<<" deals"<<endl;
        dealCards(3,dealer_index,Players,pack);
        dealCards(2,dealer_index+1,Players,pack);
        dealCards(3,dealer_index+2,Players,pack);
        dealCards(2,dealer_index+3,Players,pack);
        dealCards(2,dealer_index,Players,pack);
        dealCards(3,dealer_index+1,Players,pack);
        dealCards(2,dealer_index+2,Players,pack);
        dealCards(3,dealer_index+3,Players,pack);
        Card upcard=pack.deal_one();
        cout<<upcard<<" turned up"<<endl;
        return upcard;
    }
    void makingTrump(Card &upcard){
        int round=1;bool trumpOrNot=false;
         while(round<=2){
            if(trumpOrNot) break;
            for(int i=1; i<=4;++i){
                int player = mod_four(i+dealer_index);
                string name=Players[player]->get_name();
                bool dealer=false;if(player==dealer_index) dealer=true;
                if(round==1){
                    if(Players[player]->make_trump(upcard,dealer,round,trumpCard)){
                        if(player==0||player==2) teamOneTrumps=1;
                        if(player==1||player==3) teamTwoTrumps=1;
                            cout<<name<<" orders up "<<trumpCard<<endl;
                            cout<<endl;
                        Players[dealer_index]->add_and_discard(upcard);
                        trumpOrNot=true;
                        break;
                    }else{
                        cout<<name<<" passes"<<endl;
                    }
                }else if(round==2){
                    if(Players[player]->make_trump(upcard,dealer,round,trumpCard)){
                            if(player==0||player==2) teamOneTrumps=1;
                            if(player==1||player==3) teamTwoTrumps=1;
                            cout<<name<<" orders up "<<trumpCard<<endl;
                            cout<<endl;
                            trumpOrNot=true;
                            break;
                        }else{
                            cout<<name<<" passes"<<endl;
                        }
                    if(i==4){
                        upcard.get_suit()=Suit_next(trumpCard);
                    }
                }
            }
            round++;
         }
    }
    void trickTaking(){
        int team_one_tricks=0;int team_two_tricks=0;
        int winner_idx=dealer_index;
        winner_idx=idx(winner_idx);
        int tricks=1;
        while(tricks<=5){
            vector<Card> cardVec;
            Card led_card=leading_card(winner_idx,trumpCard,Players,cardVec);
            for(int i=0; i<3;++i){
                int chosen_idx=mod_four(i+winner_idx+1);
                Card chosen_card=Players[chosen_idx]->
                                        play_card(led_card,trumpCard);
                cardVec.push_back(chosen_card);
                play(chosen_card,Players[chosen_idx]->get_name());
             }
            Card highCard=led_card;
            int highCardIdx=0;
            for(int i=0; i<4;++i){
                if(Card_less(highCard,cardVec[i],led_card,trumpCard)){
                    highCard=cardVec[i];
                    highCardIdx=i;
                }
                if(cardVec[i].is_right_bower(trumpCard)){
                    highCard=cardVec[i];
                    highCardIdx=i;
                    break;
                }
            }
            int victor_idx=mod_four(highCardIdx+winner_idx);
            cout<<Players[victor_idx]->get_name()<<" takes the trick"<<endl;
            cout<<endl;
            winner_idx=victor_idx;
            if(victor_idx==0||victor_idx==2) team_one_tricks++;
            if(victor_idx==1||victor_idx==3) team_two_tricks++;
            tricks++;
        }
        if(team_one_tricks>team_two_tricks){
            cout<<playerNames[0]<<" and "<<playerNames[2]<<" win the hand"<<endl;
        }else{
            cout<<playerNames[1]<<" and "<<playerNames[3]<<" win the hand"<<endl;
        }
        dealer_index++;dealer_index%=4;hand_index++;
        march(team_one_tricks,team_two_tricks,teamOneTrumps,teamTwoTrumps);
        euchered(team_one_tricks,team_two_tricks,teamOneTrumps,teamTwoTrumps);
    }
 void getWinner(){
        cout<<Players[0]->get_name()<<" and "<<Players[2]->get_name()<<" have "<<
        team_one_points<<" points"<<endl;
        cout<<Players[1]->get_name()<<" and "<<Players[3]->get_name()<<" have "<<
        team_two_points<<" points"<<endl;
        cout<<endl;
        hand_index++;
        pack.reset();

        if(team_one_points>team_two_points){
        cout<<Players[0]->get_name()<<" and "<<Players[2]->get_name()<<" win!"<<endl;
    }else if(team_two_points>team_one_points){
        cout<<Players[1]->get_name()<<" and "<<Players[3]->get_name()<<" win!"<<endl;
    }
 }
};
void shuffleDeck(string shuffle,Pack pack){
    if(shuffle=="Shuffle"){
        pack.shuffle();
    }
}

void dealCards(int numCards,int dealer_idx, Player * Players[], Pack &pack){
    int player_idx=0;
    if(dealer_idx<3){
        player_idx=dealer_idx+1;
    }
    if(dealer_idx==3) player_idx=0;
    if(dealer_idx>=4){
        player_idx=(dealer_idx%4)+1;
    }
    if(dealer_idx%4==3) player_idx=0;
    for(int i=0; i<numCards;++i){
        Players[player_idx]->add_card(pack.deal_one());
    }
     
}
// bool makeTrump(bool dealer,Card upcard,int round,Player *player,string &order_up_suit){
//     return player->make_trump(upcard,dealer,round,order_up_suit);
// }
int idx(int dealer_idx){
    int player_idx=0;
    if(dealer_idx==3) return 0;
    if(dealer_idx%4==3) return 0;
    if(dealer_idx>=4){
        player_idx=(dealer_idx%4)+1;
        return player_idx;
    }
    if(dealer_idx<3){
        return dealer_idx+1;
    }
    return dealer_idx+1;
}
void play(Card card, string name){
    cout<<card<<" played by "<<name<<endl;
}
int mod_four(int num){
    return num%4;
}
void march(int tricksOne, int tricksTwo,bool trumpsOne, bool trumpsTwo){
   if(trumpsOne){
       if(tricksOne==5){
           cout<<"March";
           team_one_points+=2;
       }else if(tricksOne==3||tricksOne==4){
           team_one_points++;
       }
   }
   if(trumpsTwo){
       if(tricksTwo==5){
           cout<<"March";
           team_two_points+=2;
       }else if(tricksTwo==3||tricksTwo==4){
           team_two_points++;
       }
   }
}
void euchered(int tricksOne, int tricksTwo,bool trumpsOne, bool trumpsTwo){
    if(!trumpsOne){
        if(tricksOne==3||tricksOne==4||tricksOne==5){
            cout<<"euchred!"<<endl;
            team_one_points+=2;
        }
    }
    if(!trumpsTwo){
        if(tricksTwo==3||tricksTwo==4||tricksTwo==5){
            cout<<"euchred!"<<endl;
            team_two_points+=2;
        }
    }
}
int commandline(int argc, char **argv){
    if((strcmp(argv[5],"Simple")!=0&&strcmp(argv[5],"Human"))!=0||
    (strcmp(argv[7],"Simple")!=0&&strcmp(argv[7],"Human")!=0)||
    (strcmp(argv[9],"Simple")!=0&&strcmp(argv[9],"Human")!=0)||
    (strcmp(argv[11],"Simple")!=0&&strcmp(argv[11],"Human")!=0)){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    }else{
        return 0;
    }
}
 Card leading_card(int &winner_idx,string trumpCard,Player *Players[],vector<Card>&cardVec){
            Card led_card = Players[winner_idx]->lead_card(trumpCard);
            cout<<led_card<<" led by "<<Players[winner_idx]->get_name()<<endl;
            cardVec.push_back(led_card);
            return led_card;
 }

int main(int argc, char *argv[])
{
    string filename = argv[1];
    int points_total = atoi(argv[3]);
    string type1 = "Simple";
    string type2 = "Human";
    if(argc!=12||(points_total<1||points_total>100)||((strcmp(argv[2],"shuffle")!=0
    &&(strcmp(argv[2],"noshuffle")!=0)))){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    }
    // commandline(argc,argv);
    ifstream fin;
    fin.open(filename);
    if(!fin.is_open()){
        cout << "Error opening " << filename << endl;
        return 1;
    }
    Game game(argc, argv, fin);
    // Pack Pack(fin);
    for(int i=0; i<argc;++i){
        cout<<argv[i]<<" ";
    }
    cout<<endl;
   
    while(team_one_points<points_total&&team_two_points<points_total){
        // int team_one_tricks = 0;
        // int team_two_tricks = 0;    
        Card upcard = game.setupTable();
        game.makingTrump(upcard);
        game.trickTaking();
        // march(team_one_tricks,team_two_tricks,teamOneTrumps,teamTwoTrumps);
        // euchered(team_one_tricks,team_two_tricks,teamOneTrumps,teamTwoTrumps);
    }
    game.getWinner();
}
    
