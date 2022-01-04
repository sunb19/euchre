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
#include "Pack.h"

using namespace std;

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on.
// NOTE: The standard order is the same as that in pack.in.
// NOTE: Do NOT use pack.in in your implementation of this function
Pack::Pack()
    : next(0)
{
   
    int ind=0;
    for (int i = 0; i < NUM_SUITS; ++i)
    {
        for (int j = 7; j < NUM_RANKS; ++j)
        {
            cards[ind++] = Card(RANK_NAMES_BY_WEIGHT[j], SUIT_NAMES_BY_WEIGHT[i]);
        }
    }
}

// REQUIRES: pack_input contains a representation of a Pack in the
//           format required by the project specification
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
Pack::Pack(std::istream &pack_input)
{
    next = 0;
    int i=0;
    string garbage;
    string suit;
    string rank;
    while (pack_input >> rank >> garbage >> suit)
    {
        cards[i++] = Card(rank, suit);
    }
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one()
{
    next++;
    return cards[next - 1];
}
// EFFECTS: Resets next index to first card in the Pack
void Pack::reset()
{
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times. See
//          https://en.wikipedia.org/wiki/In_shuffle.

void Pack::shuffle()
{
    reset();
    int counter = 0;
    Card copy_arr1[12];
    Card copy_arr2[12];
    Card output_arr[24];
    while (counter++ < 7)
    {
        for (int i = 0; i < PACK_SIZE; ++i)
        {
            if (i < 12)
            {
                copy_arr1[i] = cards[i];
            }
            else
            {
                copy_arr2[i - 12] = cards[i];
            }
        }
        int a = 0;
        int b = 0;
        while (a < PACK_SIZE && b < PACK_SIZE / 2)
        {
            output_arr[a] = copy_arr2[b];
            ++a;
            output_arr[a] = copy_arr1[b];
            ++a;
            ++b;
        }
        for (int k = 0; k < PACK_SIZE; ++k)
        {
            cards[k] = output_arr[k];
        }
    }
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const
{
    return next >= 24;
}
