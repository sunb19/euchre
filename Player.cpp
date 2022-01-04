// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <array>
#include <vector>
#include <sstream>
#include "Player.h"
#include <algorithm>

using namespace std;



class SimplePlayer : public Player
{
    string name;
    vector<Card> deck;
    public:
    SimplePlayer(const string &name_in)
        : name(name_in) {}

    //EFFECTS returns player's name
    virtual const string &get_name() const
    {
        return name;
    }
    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    virtual void add_card(const Card &c)
    {
        assert(deck.size() < MAX_HAND_SIZE);
        deck.push_back(c);
    }
    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    virtual bool make_trump(const Card &upcard, bool is_dealer,
                            int round, std::string &order_up_suit) const
    {
        string upcard_suit = upcard.get_suit();
        int num_facecards = 0;
        int num_facecards2 = 0;
        if (round == 1)
        {
            for (auto &i : deck)
            {
                if (i.is_face() && i.is_trump(upcard_suit))
                {
                    num_facecards++;
                }
            }
            if (num_facecards >= 2)
            {
                order_up_suit = upcard_suit;
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (round == 2)
        {
            string next_suit = Suit_next(upcard_suit);
            for (auto &i : deck)
            {
                if (i.is_face() && i.is_trump(next_suit))
                {
                    num_facecards2++;
                }
            }
            if (num_facecards2 >= 1)
            {
                order_up_suit = next_suit;
                return true;
            }
            if (is_dealer)
            {
                order_up_suit = next_suit;
                return true;
            }
            return false;
        }
        return false;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard)
    {
        int index = 0;
        int size = deck.size();
        deck.push_back(upcard);
        Card weak_card = deck[0];
        for (int i = 0; i < size; i++)
        {
            if (Card_less(deck[i], weak_card, upcard.get_suit()))
            {
                index = i;
                weak_card = deck[i];
            }
        }
        deck.erase(deck.begin() + index);
    }

    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(const std::string &trump)
    {assert(deck.size() >= 1);
        Card weakest_card = deck[0];
        Card highest_trump = deck[0];
        int weakest_card_idx=0;
        int highest_trump_idx=0;
        int numTrumpCards = 0;
        for (auto &i : deck)
        {
            if (i.is_trump(trump)){
                ++numTrumpCards;
            }
        }
        if (numTrumpCards != deck.size()){
            for (int i=0; i<static_cast<int>(deck.size());++i){
                if (!deck[i].is_trump(trump)&&deck[i]>weakest_card){
                        weakest_card = deck[i];
                        weakest_card_idx=i;
                }
                }
                deck.erase(deck.begin()+weakest_card_idx);
                return weakest_card;
        }else{
            for(int i=0; i<static_cast<int>(deck.size());++i){
                if(Card_less(highest_trump,deck[i],trump)){
                    highest_trump=deck[i];
                    highest_trump_idx=i;
                }
            }
            deck.erase(deck.begin()+highest_trump_idx);
            return highest_trump;
        }
        return weakest_card;
    }

    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, const std::string &trump){
        assert(deck.size()>=1);
        Card lowest_card = deck[0];
        int lowest_idx = 0;
        int led_suit_nums=0;
        string ledSuit=led_card.get_suit(trump);
        for(int i=0; i<static_cast<int>(deck.size());++i){
            string deckSuit=deck[i].get_suit(trump);
            if(led_suit_nums==0){
                if(deckSuit==ledSuit){
                    led_suit_nums++;
                    lowest_card=deck[i];
                    lowest_idx=i;
                }else{
                    if(Card_less(deck[i],lowest_card,led_card,trump)){
                        lowest_card=deck[i];
                        lowest_idx=i;
                   
                    }
                }
                }else if(led_suit_nums>0){
                    if(deckSuit==ledSuit){
                        if(Card_less(lowest_card,deck[i],led_card,trump)){
                            lowest_card=deck[i];
                            lowest_idx=i;
                        }
                    }
                }
        }
            deck.erase(deck.begin()+lowest_idx);
            return lowest_card;    
    }
};


class HumanPlayer : public Player
{
    vector<Card> deck;
    string name;
    public:
    HumanPlayer(const string &name_in)
        : name(name_in) {}

    //EFFECTS returns player's name
    virtual const string &get_name() const
    {
        return name;
    }
    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c)
    {
        assert(deck.size() < MAX_HAND_SIZE);
        deck.push_back(c);
    };
 
    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, std::string &order_up_suit) const {
        vector<Card> copyHand = deck;
        sort(copyHand.begin(),copyHand.end());
        for(int i=0; i<static_cast<int>(copyHand.size()); ++i){
            cout<<"Human player "<<name<<"'s hand: ["<<i<<"] "<<copyHand[i]<<endl;
        }
        string pass;
        string upcard_suit = upcard.get_suit();
        cout<<"Human player "<<name<<", please enter a suit, or \"pass\":"<<endl;
        cin>>pass;
        if(round==1){
            if(pass=="pass"){
                // cout<<name<<" passes"<<endl;
                return false;
            }else if(pass==upcard_suit){
                // cout<<name<<" orders up "<<upcard_suit<<endl;
                order_up_suit=upcard_suit;
                return true;
            }
        }else if(round==2){
            if(pass=="pass"){
                // cout<<name<<" passes"<<endl;
                            return false;
            }else if(pass!=upcard_suit){
                // cout<<name<<" orders up "<<pass<<endl;
                order_up_suit=pass;
                return true;
            }
        }
        return false;
    };

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard){
        assert(deck.size()>=1);
        sort(deck.begin(),deck.end());
        vector<Card> copyHand = deck;
        for(int i=0; i<static_cast<int>(copyHand.size()); ++i){
            cout<<"Human player "<<name<<"'s hand: ["<<i<<"] "<<copyHand[i]<<endl;
        }
        int discard_upcard;
        cout<<"Discard upcard: [-1]"<<endl;
        cout<<"Human player "<<name<<", please select a card to discard:"<<endl;
        cin>>discard_upcard;
        if(discard_upcard>-1){
            deck.push_back(upcard);
            deck.erase(deck.begin()+discard_upcard);
        }
        
    };

    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(const std::string &trump){
        assert(deck.size()>=1);
        int lead_card_index;
        sort(deck.begin(),deck.end());
        vector<Card> copyHand = deck;
        for(int i=0; i<copyHand.size(); ++i){
            cout<<"Human player "<<name<<"'s hand: ["<<i<<"] "<<copyHand[i]<<endl;
        }
        cout<<"Human player "<<name<<", please select a card:"<<endl;
        cin>>lead_card_index;
        Card lead_card_choice = deck[lead_card_index];
        // cout<<lead_card_choice<<" led by "<<name<<endl;
        deck.erase(deck.begin()+lead_card_index);
        return lead_card_choice;

    }

    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, const std::string &trump){
        assert(deck.size()>=1);
        int played_card_index;
        sort(deck.begin(),deck.end());
        vector<Card> copyHand = deck;
        for(int i=0; i<copyHand.size(); ++i){
            cout<<"Human player "<<name<<"'s hand: ["<<i<<"] "<<copyHand[i]<<endl;
        }
        cout<<"Human player "<<name<<", please select a card:"<<endl;
        cin>>played_card_index;
        Card played_card_choice = copyHand[played_card_index];
        // cout<<played_card_choice<<" played by "<<name<<endl;
        deck.erase(deck.begin()+played_card_index);
        return played_card_choice;
    }

    
};//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player *Player_factory(const std::string &name, const std::string &strategy)
{

    if (strategy == "Simple")
    {
        return new SimplePlayer(name);
    }
    else if (strategy == "Human")
    {
        return new HumanPlayer(name);
    }
    return nullptr;
}

//EFFECTS: Prints player's name to os
std::ostream &operator<<(std::ostream &os, const Player &p)
{
    os << p.get_name();
    return os;
}