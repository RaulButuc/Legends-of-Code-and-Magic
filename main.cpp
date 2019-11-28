/**
  *  Legends of Code and Magic - https://www.codingame.com/ide/puzzle/legends-of-code-magic
  *
  *  @author Raul Butuc
  *  @version 1.0.0
  */

#include <iostream>
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
            players.first = Player(health, mana, deck, rune, draw);
            std::cin >> health >> mana >> deck >> rune >> draw; std::cin.ignore();
            players.second = Player(health, mana, deck, rune, draw);
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
            return players.self.getMana() == 0 && players.enemy.getMana() == 0;
        }
    
};

class Action {
    
    public:
        static auto draftingStrategy() -> void {
            if (Game::state.getCardCount() != 3) {
                std::cerr << "This is NOT the drafting phase. This is the battle phase!";
                return;
            }
            
            Card bestChoice;
            std::vector<Card> cards = Game::state.getCards();

            for (auto card : cards) {
                std::cerr << "card #" << card.getNumber() << " " << card.getCost() << "\n";
            }
        }

        static auto gameplayStrategy() -> void {
            // TODO: choose between aggressive or defensive based on a deterministic approach
        }

        static auto aggressiveStrategy() -> void {
            // TODO: maximize the damage output towards the enemy player (ignore enemy cards)
        }

        static auto defensiveStrategy() -> void {
            // TODO: minimize the damage taken from the enemy player's cards
        }
    
};

int Game::currentTurn = 0;
bool Game::running = true;
Players Game::players = Players(Player(), Player());
State Game::state = State();

Game game;

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