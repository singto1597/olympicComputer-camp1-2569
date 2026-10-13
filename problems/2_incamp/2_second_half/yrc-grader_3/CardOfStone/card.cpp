#include <vector>

#include "card.h"

namespace stone {

card::card() {}
card::card(rank new_rank, suit new_suit) {
    m_rank = new_rank;
    m_suit = new_suit;
}
card::~card() {}

void card::add(card &next_card) {
    // code here
    return;
}

int answer(std::vector<card> &cards) {
    // code here
    return 0;
}

}  // namespace stone
