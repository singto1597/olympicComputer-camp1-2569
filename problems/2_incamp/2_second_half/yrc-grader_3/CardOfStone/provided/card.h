#include <vector>

namespace stone {

enum class suit { clubs, diamonds, hearts, spades };
enum class rank { ace, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king };

class card {
public:
    suit m_suit;
    rank m_rank;
    card *m_next = nullptr;

    card();
    card(rank new_rank, suit new_suit);
    ~card();

    void add(card &next_card);
};

int answer(std::vector<card> &cards);

};  // namespace stone
