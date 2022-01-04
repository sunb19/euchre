// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(Card::RANK_NINE, first.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, first.get_suit());
}

TEST(test_pack_default_ctor2){
//   ASSERT_EQUAL(cards[0], Card(Card::RANK_NINE, Card::SUIT_SPADES));
}
TEST(test_pack_reset)
{
  Pack pack;
  pack.deal_one();
  pack.shuffle();
  pack.reset();
  Card first_card = pack.deal_one();
  ASSERT_EQUAL(first_card, Card(Card::RANK_KING, Card::SUIT_CLUBS));
}
TEST(test_pack_empty)
{
  Pack pack;
  pack.shuffle();
  for (int i = 0; i < 23; ++i)
  {
    pack.deal_one();
    ASSERT_FALSE(pack.empty());
  }
  pack.deal_one();
  ASSERT_TRUE(pack.empty());
}
TEST(test_pack_shuffle)
{
  Pack pack;
  pack.shuffle();
  Card card = Card();
    for(int i=0;i<12;++i){
        card = pack.deal_one();
    }
  ASSERT_EQUAL(card, Card(Card::RANK_QUEEN, Card::SUIT_SPADES));
}
TEST_MAIN()
