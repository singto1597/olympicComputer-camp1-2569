#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "card.h"

// clang-format off
inline stone::rank parse_rank(const std::string &str) {
    static const std::unordered_map<std::string, stone::rank> rank_map = {
        {"ace",     stone::rank::ace},      {"two",     stone::rank::two},      {"three",   stone::rank::three},
        {"four",    stone::rank::four},     {"five",    stone::rank::five},     {"six",     stone::rank::six},
        {"seven",   stone::rank::seven},    {"eight",   stone::rank::eight},    {"nine",    stone::rank::nine},
        {"ten",     stone::rank::ten},      {"jack",    stone::rank::jack},     {"queen",   stone::rank::queen},
        {"king",    stone::rank::king}};

    auto it = rank_map.find(str);
    if (it != rank_map.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid rank: " + str);
}

inline stone::suit parse_suit(const std::string &str) {
    static const std::unordered_map<std::string, stone::suit> suit_map = {
        {"clubs",       stone::suit::clubs},
        {"diamonds",    stone::suit::diamonds},
        {"hearts",      stone::suit::hearts},
        {"spades",      stone::suit::spades}};

    auto it = suit_map.find(str);
    if (it != suit_map.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid suit: " + str);
}
// clang-format on

int main(int argc, char *argv[]) {
    std::cin.tie(nullptr)->sync_with_stdio(false);

    int q;
    std::cin >> q;
    for (int qCount = 0; qCount < q; ++qCount) {
        int n;
        std::cin >> n;
        std::vector<stone::card> cards(n);

        for (int i = 0; i < n; ++i) {
            std::string rank;
            std::string suit;
            std::cin >> rank >> suit;

            cards[i] = stone::card(parse_rank(rank), parse_suit(suit));
        }

        int k;
        std::cin >> k;

        for (int i = 0; i < k; ++i) {
            int curr;
            int next;
            std::cin >> curr >> next;
            cards[curr].add(cards[next]);
        }
        std::cout << stone::answer(cards) << '\n';
    }
    return 0;
}
