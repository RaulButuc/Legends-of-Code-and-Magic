/**
  *  Legends of Code and Magic - https://www.codingame.com/ide/puzzle/legends-of-code-magic
  *
  *  Code is a bit messy due to the "all-in-one-file" and "class precedence" rules.
  * 
  *  WIP (Work-in-Progress) - Final version will use a Monte-Carlo (or better) search/simulation approach. 
  * 
  *  @author Raul Butuc
  *  @version 1.0.0
  */

#include <iostream>
#include <cmath>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>

#define self first
#define enemy second

class Player {
  
    public: 
        Player() { }
      
        Player(int _health, int _mana, int _deck, int _rune, int _draw) :
            health(_health), mana(_mana), deck(_deck), rune(_rune), draw(_draw) { }
      
        auto getHealth() const -> int {
            return this->health;
        }
       
        auto getMana() const -> int {
            return this->mana;
        }
       
        auto getDeck() const -> int {
            return this->deck;
        }
        
        auto getRune() const -> int {
            return this->rune;
        }
       
        auto getDraw() const -> int {
            return this->draw;
        }
      
    private:
        int health, mana, deck, rune, draw;
    
};

typedef std::pair<Player, Player> Players;

class Card {

    public:
        Card() { }

        Card(int _number, int _instanceId, int _location, int _type, int _cost, int _attack,
             int _defense, std::string _ability, int _myHealthChange, int _opponentHealthChange,
             int _draw) : 
             number(_number), instanceId(_instanceId), location(_location), type(_type), cost(_cost),
             attack(_attack), defense(_defense), ability(_ability), myHealthChange(_myHealthChange),
             opponentHealthChange(_opponentHealthChange), draw(_draw) { }

        auto getNumber() const -> int {
            return this->number;
        }

        auto getInstanceId() const -> int {
            return this->instanceId;
        }

        auto getLocation() const -> int {
            return this->location;
        }

        auto getType() const -> int {
            return this->type;
        }

        auto getCost() const -> int {
            return this->cost;
        }

        auto getAttack() const -> int {
            return this->attack;
        }

        auto getDefense() const -> int {
            return this->defense;
        }

        auto getAbility() const -> std::string {
            return this->ability;
        }

        auto getMyHealthChanges() const -> int {
            return this->myHealthChange;
        }

        auto getOpponentHealthChange() const -> int {
            return this->opponentHealthChange;
        }

        auto getDraw() const -> int {
            return this->draw;
        }

        auto costAverageFn() const -> int {
            return (2 * (3 * this->attack + this->defense) / 4 ) / std::pow(this->cost, 2);
        }

    private:
        int number, instanceId, location, type, cost, attack, defense;
        std::string ability;
        int myHealthChange, opponentHealthChange, draw;

};

class State {
    
    public:
        State() { }
    
        State(int _opponentHand, int _opponentActions, std::vector<std::string> _opponentCards,
              int _cardCount, std::vector<Card> _cards) :
            opponentHand(_opponentHand), opponentActions(_opponentActions),
            opponentCards(_opponentCards), cardCount(_cardCount), cards(_cards) { }
        
        auto getOpponentHand() const -> int {
            return this->opponentHand;
        }
        
        auto getOpponentActions() const -> int {
            return this->opponentActions;
        }
        
        auto getOpponentCards() const -> std::vector<std::string> {
            return this->opponentCards;
        }
        
        auto getCardCount() const -> int {
            return this->cardCount;
        }
        
        auto getCards() const -> std::vector<Card> {
            return this->cards;
        }
    
    private:
        int opponentHand, opponentActions;
        std::vector<std::string> opponentCards;
        int cardCount;
        std::vector<Card> cards;
    
};

class Game {
    
    public:
        Game() { }
        
        static int currentTurn;
        static bool running;
        static Players players;
        static State state;
        
        static auto readDataForCurrentTurn() -> void {
            currentTurn++;
            int health, mana, deck, rune, draw;
            std::cin >> health >> mana >> deck >> rune >> draw; std::cin.ignore();
            players.self = Player(health, mana, deck, rune, draw);
            std::cin >> health >> mana >> deck >> rune >> draw; std::cin.ignore();
            players.enemy = Player(health, mana, deck, rune, draw);
            int opponentHand, opponentActions;
            std::vector<std::string> opponentCards;
            std::cin >> opponentHand >> opponentActions; std::cin.ignore();
            for (int i = 0; i < opponentActions; ++i) {
                std::string opponentCard;
                getline(std::cin, opponentCard);
                opponentCards.push_back(opponentCard);
            }
            int cardCount;
            std::vector<Card> cards;
            std::cin >> cardCount; std::cin.ignore();
            for (int i = 0; i < cardCount; ++i) {
                int cardNumber, instanceId, location, cardType, cost, attack, defense;
                std::string abilities;
                int myHealthChange, opponentHealthChange, cardDraw;
                std::cin >> cardNumber >> instanceId >> location >> cardType >> cost
                         >> attack >> defense >> abilities >> myHealthChange
                         >> opponentHealthChange >> cardDraw; std::cin.ignore();
                cards.push_back(Card(cardNumber, instanceId, location, cardType, cost, attack, 
                                     defense, abilities, myHealthChange, opponentHealthChange, draw));                
            }
            state = State(opponentHand, opponentActions, opponentCards, cardCount, cards);
        }
        
        static auto isDraftingPhase() -> bool {
            return currentTurn <= 30;
        }
    
};

class Action {
    
    private:
        static auto summonCards(int _freeSlots, int _manaLevel, std::vector<Card> _cards)
                -> std::vector<std::string> {
            // TODO: improve the logic for summoning now that cards have abilities
            std::vector<std::string> summons;

            if (_freeSlots > 0) {
                std::sort(_cards.begin(), _cards.end(),
                          [](const Card &_card_A, const Card &_card_B) -> bool {
                              return _card_A.costAverageFn() > _card_B.costAverageFn();
                          });

                for (auto _card : _cards) {
                    int manaCost = _card.getCost();
                    if (_manaLevel >= manaCost) {
                        summons.push_back("SUMMON " + std::to_string(_card.getInstanceId()));
                        _manaLevel -= manaCost;
                        _freeSlots--;
                    }
                }
            }

            return summons;
        }

        static auto attackCards(std::vector<Card> _cards) -> std::vector<std::string> {
            // TODO: improve the logic for attacking now that cards have abilities
            std::vector<std::string> attacks;
            
            if (_cards.size() > 0) {
                for (auto _card : _cards) {
                    attacks.push_back("ATTACK " + std::to_string(_card.getInstanceId()) + " -1");
                }
            }

            return attacks;
        }

        static auto performActions(std::vector<std::string> _summons,
                                   std::vector<std::string> _attacks) -> void {
            int numSummons = _summons.size();
            int numAttacks = _attacks.size();

            if (numSummons == 0 && numAttacks == 0) {
                std::cout << "PASS" << "\n";
            }
            else if (numSummons > 0 && numAttacks == 0) {
                Action::formatPrint(_summons);
                std::cout << "\n";
            }
            else if (numSummons == 0 && numAttacks > 0) {
                Action::formatPrint(_attacks);
                std::cout << "\n";
            }
            else {
                Action::formatPrint(_summons);
                std::cout << ";";
                Action::formatPrint(_attacks);
                std::cout << "\n";
            }
        }

        static auto formatPrint(std::vector<std::string> _actions) -> void {
            auto sep = ";";
            auto delim = "";
            for (const auto &action : _actions) {
                std::cout << delim << action;
                delim = sep;
            }
        }

    public:
        static auto draftingStrategy() -> void {
            std::vector<Card> cards = Game::state.getCards();
            int _pos = 0;
            Card _card = cards[_pos];
            
            for (auto pos = 1; pos < cards.size(); ++pos) {
                if (_card.costAverageFn() < cards[pos].costAverageFn()) {
                    _card = cards[pos];
                    _pos = pos;
                }
            }

            std::cout << "PICK " << _pos << "\n";
        }

        static auto gameplayStrategy() -> void {
            int numCardsInDeck = Game::players.self.getDeck();
            int manaLevel = Game::players.self.getMana();
            int numCards = Game::state.getCardCount();
            std::vector<Card> cards, cardsInHand, cardsOnBoard;
            cards = Game::state.getCards();

            for (auto card : cards) {
                int location = card.getLocation();
                switch (location) {
                    case 0:
                        cardsInHand.push_back(card); 
                        break;

                    case 1:
                        cardsOnBoard.push_back(card);
                        break;

                    case -1:
                        // Add funcitonality for cardsOnEnemyBoard 
                        break;

                    default:
                        break;
                }
            }

            int freeSlots = 6 - cardsOnBoard.size();
            std::vector<std::string> summons = Action::summonCards(freeSlots, manaLevel, cardsInHand);
            std::vector<std::string> attacks = Action::attackCards(cardsOnBoard);

            Action::performActions(summons, attacks);
        }
    
};

int Game::currentTurn = 0;
bool Game::running = true;
Players Game::players = Players(Player(), Player());
State Game::state = State();

auto main() -> int {
    while (Game::running) {
        Game::readDataForCurrentTurn();
        if (Game::isDraftingPhase()) {
            Action::draftingStrategy();
        }
        else {
            Action::gameplayStrategy();
        }
    }
}