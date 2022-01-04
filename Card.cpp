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
#include "Card.h"
using namespace std;
// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char *const Card::RANK_TWO;
constexpr const char *const Card::RANK_THREE;
constexpr const char *const Card::RANK_FOUR;
constexpr const char *const Card::RANK_FIVE;
constexpr const char *const Card::RANK_SIX;
constexpr const char *const Card::RANK_SEVEN;
constexpr const char *const Card::RANK_EIGHT;
constexpr const char *const Card::RANK_NINE;
constexpr const char *const Card::RANK_TEN;
constexpr const char *const Card::RANK_JACK;
constexpr const char *const Card::RANK_QUEEN;
constexpr const char *const Card::RANK_KING;
constexpr const char *const Card::RANK_ACE;

constexpr const char *const Card::SUIT_SPADES;
constexpr const char *const Card::SUIT_HEARTS;
constexpr const char *const Card::SUIT_CLUBS;
constexpr const char *const Card::SUIT_DIAMONDS;

// add your code below

//EFFECTS Initializes Card to the Two of Spades
Card::Card()
    : rank(RANK_TWO), suit(SUIT_SPADES)
{
}

//REQUIRES rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
//  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
//  suit is one of "Spades", "Hearts", "Clubs", "Diamonds"
//EFFECTS Initializes Card to specified rank and suit
Card::Card(const std::string &rank_in, const std::string &suit_in)
    : rank(rank_in), suit(suit_in)
{
    assert(rank_in == "Two" || rank_in == "Three" || 
            rank_in == "Four" || rank_in == "Five" || 
            rank_in == "Six" ||
            rank_in == "Seven" || rank_in == "Eight" ||
            rank_in == "Nine" || rank_in == "Ten" ||
            rank_in == "Jack" || rank_in == "Queen" ||
            rank_in == "King" || rank_in == "Ace");
            
    assert(suit_in == "Spades" || suit_in == "Hearts" ||
           suit_in == "Clubs" || suit_in == "Diamonds");
}
//EFFECTS Returns the rank
std::string Card::get_rank() const
{
    return rank;
}
//EFFECTS Returns the suit.  Does not consider trump.
std::string Card::get_suit() const
{
    return suit;
}
//REQUIRES trump is a valid suit
//EFFECTS Returns the suit
//HINT: the left bower is the trump suit!
std::string Card::get_suit(const std::string &trump) const
{
    assert(trump == SUIT_CLUBS || trump == SUIT_DIAMONDS ||
           trump == SUIT_SPADES || trump == SUIT_HEARTS);
    if (is_left_bower(trump))
    {
        return trump;
    }
    else
    {
        return suit;
    }
}
//EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
bool Card::is_face() const
{
    if (rank == RANK_JACK || rank == RANK_QUEEN)
    {
        return true;
    }
    if (rank == RANK_KING || rank == RANK_ACE)
    {
        return true;
    }
    return false;
}
//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the trump suit
bool Card::is_right_bower(const std::string &trump) const
{
    assert(trump == SUIT_CLUBS || trump == SUIT_DIAMONDS ||
           trump == SUIT_SPADES || trump == SUIT_HEARTS);
    if (suit == trump && rank == RANK_JACK)
    {
        return true;
    }
    return false;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the next suit
bool Card::is_left_bower(const std::string &trump) const
{
    assert(trump == SUIT_CLUBS || trump == SUIT_DIAMONDS ||
           trump == SUIT_SPADES || trump == SUIT_HEARTS);
    string next_suit = Suit_next(trump);
    if (rank == RANK_JACK)
    {
        return suit == next_suit;
    }
    return false;
}
//REQUIRES trump is a valid suit
//EFFECTS Returns true if the card is a trump card.  All cards of the trump
// suit are trump cards.  The left bower is also a trump card.
bool Card::is_trump(const std::string &trump) const
{
    assert(trump == SUIT_CLUBS || trump == SUIT_DIAMONDS ||
           trump == SUIT_SPADES || trump == SUIT_HEARTS);
    return trump == suit || is_left_bower(trump);
}

//EFFECTS Returns true if lhs is lower value than rhs.
//  Does not consider trump.
bool operator<(const Card &lhs, const Card &rhs)
{
    int lhs_idx = 0;
    int rhs_idx = 0;
    for (int i = 0; i < NUM_RANKS; ++i)
    {
        if (RANK_NAMES_BY_WEIGHT[i] == lhs.get_rank())
        {
            lhs_idx = i;
        }
        if (RANK_NAMES_BY_WEIGHT[i] == rhs.get_rank())
        {
            rhs_idx = i;
        }
    }
    if (lhs_idx != rhs_idx)
    {
        return lhs_idx < rhs_idx;
    }
    // else
    // {
    //     return false;
    // }

    int lhs_suit_idx = 0;
    int rhs_suit_idx = 0;
    for (int i = 0; i < NUM_SUITS; ++i)
    {
        if (SUIT_NAMES_BY_WEIGHT[i] == lhs.get_suit())
        {
            lhs_suit_idx = i;
        }
        if (SUIT_NAMES_BY_WEIGHT[i] == rhs.get_suit())
        {
            rhs_suit_idx = i;
        }
    }
    if (lhs_suit_idx != rhs_suit_idx)
    {
        return lhs_suit_idx < rhs_suit_idx;
    }
    // else
    // {
    //     return false;
    // }

    return false;
}

//EFFECTS Returns true if lhs is lower value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator<=(const Card &lhs, const Card &rhs)
{
    return lhs < rhs || lhs == rhs;
}

//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs)
{
    return !(lhs <= rhs);
}

//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator>=(const Card &lhs, const Card &rhs)
{
    return !(lhs < rhs) || lhs == rhs;
}

//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs)
{
    return lhs.get_rank() == rhs.get_rank() &&
           lhs.get_suit() == rhs.get_suit();
}

//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs)
{
    return !(lhs == rhs);
}

//REQUIRES suit is a valid suit
//EFFECTS returns the next suit, which is the suit of the same color
std::string Suit_next(const std::string &suit)
{

    if (suit == "Spades")
    {
        return "Clubs";
    }
    else if (suit == "Clubs")
    {
        return "Spades";
    }
    else if (suit == "Hearts")
    {
        return "Diamonds";
    }
    else
    {
        return "Hearts";
    }
}

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const std::string &trump)
{
    assert(trump == Card::SUIT_CLUBS || trump == Card::SUIT_DIAMONDS ||
           trump == Card::SUIT_SPADES || trump == Card::SUIT_HEARTS);

    //compare if both are trump
    if (a.is_trump(trump) && (b.is_trump(trump)))
    {
        if (b.is_right_bower(trump))
            return true;
        if (a.is_right_bower(trump))
            return false;
        if (!a.is_right_bower(trump) && b.is_left_bower(trump))
        {
            return true;
        }

        if (a.is_left_bower(trump) && !b.is_right_bower(trump))
        {
            return false;
        }
        return a < b;
    }

    //compare if neither are trump
    if (!a.is_trump(trump) && (!b.is_trump(trump)))
    {
        return a < b;
    }

    //compare a is trump b is not trump
    if (a.is_trump(trump) && (!b.is_trump(trump)))
    {
        return false;
    }

    //compare a is not trump b is trump
    if (!a.is_trump(trump) && (b.is_trump(trump)))
    {
        return true;
    }

    return false;

    //a not trump b trump
    //a trump b not trump
    //a trump b trump use left bower and right bower
    //a not trump b not trump compare ranks and suits
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card,
               const std::string &trump)
{
    assert(trump == Card::SUIT_CLUBS || trump == Card::SUIT_DIAMONDS ||
           trump == Card::SUIT_SPADES || trump == Card::SUIT_HEARTS);
    /*
    1)Both trump
    2)1 trump?
    3)Both leading
        3)compare ranks
    4)One is leading
    5)neither leading (regular checks)

    */
    string led_suit = led_card.get_suit();
    //case 1,2
    if ((a.get_suit() != led_suit && b.get_suit() != led_suit) || led_suit == trump)
    {
        Card_less(a, b, trump);
    }
    //case 3
    if (a.get_suit() == trump && b.get_suit() != trump)
        return false;
    if (a.get_suit() != trump && b.get_suit() == trump)
        return true;
    //case 4
    if (a.get_suit() == led_suit && b.get_suit() == led_suit)
    {
        if (a.is_left_bower(trump))
        {
            return false;
        }
        else if (b.is_left_bower(trump))
        {
            return true;
        }
        else
        {
            return a < b;
        }
    }
    //case 5
    if (a.get_suit() != trump && a.get_suit() != led_suit &&
        b.get_suit() != trump && b.get_suit() != led_suit)
    {
        return a < b;
    }
    return 0;

    //case 1: if neither a nor b are led suits, use only trump and call function above
    //case 2: if trump and led suit are same
    //case 3: one of them is trump, the other are led or not trump
    //case 4: both of them are led
    //case 5: a b are not trump nor led
    //case 6:
}

// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec (see the appendices) before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
