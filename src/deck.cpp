#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include "deck.hpp"
Deck::Deck(std::string name, std::vector<Card> cards) : name(name), cards(cards) {}
Deck::Deck(std::string name) : name(name) {}
Deck::Deck() {}
void Deck::add_card(Card card)
{
    cards.push_back(card);
}
void Deck::print()
{
    for (auto card : cards)
    {
        card.print();
    }
}
void Deck::load(std::string filename)
{
    // load file and get name of deck
    std::ifstream file;
    file.open(filename);
    name = filename.substr(0, filename.find_last_of("."));
    assert(file);

    std::string buf;
    TokenKind cur_tok = TokenKind::TERM;
    std::string cur_line;
    Card card_buf = Card("", "", false, false);
    while (std::getline(file, cur_line))
    {
        switch (cur_tok)
        {
        case TokenKind::TERM:
            if (cur_line[0] == '\t')
            {
                if (card_buf.term[0] == '\n')
                {
                    card_buf.term = card_buf.term.substr(1);
                }
                cur_tok = TokenKind::DEFINITION;
                card_buf.definition += cur_line.substr(1);
                break;
            }
            else if (cur_line[0] == '*')
            {
                card_buf.starred = true;
                cur_line = cur_line.substr(1);
            }
            card_buf.term += "\n" + cur_line;
            break;
        case TokenKind::DEFINITION:
            if (cur_line[0] != '\t')
            {
                cur_tok = TokenKind::TERM;
                add_card(card_buf);
                card_buf = Card("", "", false, false);
                if (cur_line[0] == '*')
                {
                    card_buf.starred = true;
                    cur_line = cur_line.substr(1);
                }
                card_buf.term += cur_line;
                break;
            }
            card_buf.definition += "\n" + cur_line.substr(1);
            break;
        }
    }
    add_card(card_buf);
    file.close();
}
void Deck::write(std::string filename)
{
    std::ofstream file;
    file.open(filename);
    assert(file);
    for (auto card : cards)
    {
        file << (card.starred ? "*" : "") << card.term << std::endl;
        std::stringstream ss(card.definition);
        std::string buf;
        while (std::getline(ss, buf, '\n'))
        {
            file << "\t" << buf << std::endl;
        }
    }
}
