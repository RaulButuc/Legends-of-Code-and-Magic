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
#include <map>
#include <cmath>
#include <climits>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>

#define self first
#define enemy second


// Based on prior simulations we can get a priority list to use while in the draft phase
std::map<int, int> cardPriorityById;
std::vector<int> cardPriority {
    68, 7, 65, 49, 116, 69, 151, 48, 53, 51, 44, 67, 29, 139, 84, 18, 158, 28, 64, 80, 33, 85,
    32, 147, 103, 37, 54, 52, 50, 99, 23, 87, 66, 81, 148, 88, 150, 121, 82, 95, 115, 133, 152,
    19, 109, 157, 105, 3, 75, 96, 114, 9, 106, 144, 129, 17, 111, 128, 12, 11, 145, 15, 21, 8,
    134, 155, 141, 70, 90, 135, 104, 41, 112, 61, 5, 97, 26, 34, 73, 6, 36, 86, 77, 83, 13, 89,
    79, 93, 149, 59, 159, 74, 94, 38, 98, 126, 39, 30, 137, 100, 62, 122, 22, 72, 118, 1, 47, 71,
    4, 91, 27, 56, 119, 101, 45, 16, 146, 58, 120, 142, 127, 25, 108, 132, 40, 14, 76, 125, 102,
    131, 123, 2, 35, 130, 107, 43, 63, 31, 138, 124, 154, 78, 46, 24, 10, 136, 113, 60, 57, 92,
    117, 42, 55, 153, 20, 156, 143, 110, 160, 140
};

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
             opponentHealthChange(_opponentHealthChange), draw(_draw) {
            canUse = hasAbility('C') ? true : false;
        }

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

        auto getCanUse() const -> bool {
            return this->canUse;
        }

        auto setCanUse(const bool &_canUse) -> bool {
            this->canUse = _canUse;
        }

        auto costAverageFn() -> int {
            switch (this->type) {
                case 0:
                    return (20 * this->attack + 10 * this->defense) / std::pow(this->cost, 2)
                            + extrasFn();

                case 1:
                    return 25 * (this->attack + this->defense) / std::pow(this->cost, 2);

                case 2:
                    return -25 * (this->attack + this->defense) / std::pow(this->cost, 2);

                case 3:
                    return -50 * this->defense / std::pow(this->cost, 2);

                default:
                    break;
            }
            return 0;
        }

        auto guardianFn() -> int {
            return hasAbility('G') && this->type == 0 ?
                    (5 * this->attack + 45 * this->defense) / std::pow(this->cost, 2) + extrasFn() : 0;
        }

        auto extrasFn() -> int {
            return hasAbility('C') || hasAbility('D') || hasAbility('L') || hasAbility('W') ? 
                    5 : 2 * (this->myHealthChange - this->opponentHealthChange + this->draw);
        }

        

        auto hasAbility(char _abilityType) -> bool {
            return this->ability.find(_abilityType);
        }

    private:
        bool canUse;
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
        static auto summonCards(int _freeSlots, int _manaLevel, std::vector<Card> _cards,
                std::vector<Card> &_cardsOnBoard, std::vector<Card> _cardsOnEnemyBoard) -> std::vector<std::string> {
            std::vector<std::string> summons;

            if (_freeSlots > 0) {
                std::sort(_cards.begin(), _cards.end(),
                          [](Card _card_A, Card _card_B) -> bool {
                              return _card_A.costAverageFn() > _card_B.costAverageFn() ||
                                     _card_A.guardianFn() > _card_B.guardianFn();
                          });

                for (auto _card : _cards) {
                    int manaCost = _card.getCost();
                    if (_manaLevel >= manaCost) {
                        int _target = -1;
                        switch (_card.getType()) {
                            case 0:
                                summons.push_back("SUMMON " + std::to_string(_card.getInstanceId()));
                                _manaLevel -= manaCost;
                                _freeSlots--;
                                if (_card.hasAbility('C')) {
                                    _cardsOnBoard.push_back(_card);
                                }
                                break;

                            case 1:
                                _target = _cardsOnBoard.begin() != _cardsOnBoard.end() ? _cardsOnBoard.begin()->getInstanceId() : -1;
                                if (_target >= 0) {
                                    summons.push_back("USE " + std::to_string(_card.getInstanceId()) +
                                                      " " + std::to_string(_target));
                                    _manaLevel -= manaCost;
                                }
                                break;

                            case 2:
                                _target = _cardsOnEnemyBoard.rbegin() != _cardsOnEnemyBoard.rend() ? _cardsOnEnemyBoard.rbegin()->getInstanceId() : -1;
                                if (_target >= 0) {
                                    summons.push_back("USE " + std::to_string(_card.getInstanceId()) +
                                                      " " + std::to_string(_target));
                                    _manaLevel -= manaCost;
                                }
                                break;

                            case 3:
                                summons.push_back("USE " + std::to_string(_card.getInstanceId()) +
                                                  " " + std::to_string(_target));
                                _manaLevel -= manaCost;
                                break;

                            default:
                                break;

                        }
                    }
                }
            }

            return summons;
        }

        /**
         *   To be replaced by a Monte-Carlo search strategy after Gold league.
         **/
        static auto attackCards(std::vector<Card> _myCards, std::vector<Card> _enemyCards)
                -> std::vector<std::string> {
            std::vector<std::string> attacks;
            std::vector<Card> enemyGuards;
            if (_myCards.size() > 0) {
                if (_enemyCards.size() > 0) {                    
                    for (auto _card : _enemyCards) {
                        if (_card.hasAbility('G')) {
                            enemyGuards.push_back(_card);
                        }
                    }
                }
                
                for (auto _guard : enemyGuards) {
                    int index = 0;
                    int defense = _guard.getDefense();
                    int guardId = _guard.getInstanceId();
                    while (defense > 0 && index < _myCards.size()) {
                        auto card = _myCards[index];
                        if (card.getCanUse()) {
                            int attack = card.getAttack();
                            attacks.push_back("ATTACK " + std::to_string(card.getInstanceId()) + 
                                              " " + std::to_string(guardId));
                            defense -= attack;
                            index++;
                            card.setCanUse(false);
                        }
                    }
                }

                for (auto _card : _myCards) {
                    if (_card.getCanUse()) { 
                        attacks.push_back("ATTACK " + std::to_string(_card.getInstanceId()) + " -1");
                    }
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
            std::cout << "PICK " << std::min(cardPriorityById[cards.at(2).getNumber()],
                cardPriorityById[cards.at(0).getNumber()] < cardPriorityById[cards.at(1).getNumber()] ? 
                0 : 1) <<  "\n";
        }

        static auto gameplayStrategy() -> void {
            int numCardsInDeck = Game::players.self.getDeck();
            int manaLevel = Game::players.self.getMana();
            int numCards = Game::state.getCardCount();
            std::vector<Card> cards, cardsInHand, cardsOnBoard, cardsOnEnemyBoard;
            cards = Game::state.getCards();

            for (const auto &card : cards) {
                int location = card.getLocation();
                switch (location) {
                    case 0:
                        cardsInHand.push_back(card); 
                        break;

                    case 1:
                        cardsOnBoard.push_back(card);
                        break;

                    case -1:
                        cardsOnEnemyBoard.push_back(card);
                        break;

                    default:
                        break;
                }
            }

            int freeSlots = 6 - cardsOnBoard.size();
            std::vector<std::string> summons = Action::summonCards(freeSlots, manaLevel, cardsInHand, cardsOnBoard, cardsOnEnemyBoard);
            std::vector<std::string> attacks = Action::attackCards(cardsOnBoard, cardsOnEnemyBoard);

            Action::performActions(summons, attacks);
        }
    
};

int Game::currentTurn = 0;
bool Game::running = true;
Players Game::players = Players(Player(), Player());
State Game::state = State();

auto main() -> int {
    for (int id = 0; id < cardPriority.size(); ++id) {
        cardPriorityById.insert(std::make_pair(cardPriority.at(id), id));
    }
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