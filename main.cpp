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

class State {
    
    public:
        State() { }
    
        State(int _opponentHand, int _opponentActions, std::vector<std::string> _opponentCards,
              int _cardCount, std::vector<int> _cardNumbers, std::vector<int> _instanceIds,
              std::vector<int> _locations, std::vector<int> _cardTypes, std::vector<int> _costs,
              std::vector<int> _attacks, std::vector<int> _defenses,
              std::vector<std::string> _abilities, std::vector<int> _myHealthChanges,
              std::vector<int> _opponentHealthChanges, std::vector<int> _cardDraws) :
            opponentHand(_opponentHand), opponentActions(_opponentActions),
            opponentCards(_opponentCards), cardCount(_cardCount), cardNumbers(_cardNumbers),
            instanceIds(_instanceIds), locations(_locations), cardTypes(_cardTypes), costs(_costs),
            attacks(_attacks), defenses(_defenses), myHealthChanges(_myHealthChanges), 
            opponentHealthChanges(_opponentHealthChanges), cardDraws(_cardDraws) { }
        
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
        
        auto getCardNumbers() const -> std::vector<int> {
            return this->cardNumbers;
        }
        
        auto getInstanceIds() const -> std::vector<int> {
            return this->instanceIds;
        }
        
        auto getLocations() const -> std::vector<int> {
            return this->locations;
        }
        
        auto getCardTypes() const -> std::vector<int> {
            return this->cardTypes;
        }
        
        auto getCosts() const -> std::vector<int> {
            return this->costs;
        }
        
        auto getAttacks() const -> std::vector<int> {
            return this->attacks;
        }
        
        auto getDefenses() const -> std::vector<int> {
            return this->defenses;
        }
        
        auto getAbilities() const -> std::vector<std::string> {
            return this->abilities;
        }
        
        auto getMyHealthChanges() const -> std::vector<int> {
            return this->myHealthChanges;
        }
        
        auto getOpponentHealthChanges() const -> std::vector<int> {
            return this->opponentHealthChanges;
        }
        
        auto getCardDraws() const -> std::vector<int> {
            return this->cardDraws;
        }
    
    private:
        int opponentHand, opponentActions;
        std::vector<std::string> opponentCards;
        int cardCount;
        std::vector<int> cardNumbers, instanceIds, locations, cardTypes, costs, attacks, defenses;
        std::vector<std::string> abilities;
        std::vector<int> myHealthChanges, opponentHealthChanges, cardDraws;
    
};

class Action {
    
    public:
        static std::string PICK;
        static std::string PASS;
    
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
            std::vector<int> cardNumbers, instanceIds, locations, cardTypes, costs, attacks, defenses;
            std::vector<std::string> abilities;
            std::vector<int> myHealthChanges, opponentHealthChanges, cardDraws;
            std::cin >> cardCount; std::cin.ignore();
            for (int i = 0; i < cardCount; ++i) {
                int _cardNumber, _instanceId, _location, _cardType, _cost, _attack, _defense;
                std::string _abilities;
                int _myHealthChange, _opponentHealthChange, _cardDraw;
                std::cin >> _cardNumber >> _instanceId >> _location >> _cardType >> _cost
                         >> _attack >> _defense >> _abilities >> _myHealthChange
                         >> _opponentHealthChange >> _cardDraw; std::cin.ignore();
                cardNumbers.push_back(_cardNumber);
                instanceIds.push_back(_instanceId);
                locations.push_back(_location);
                cardTypes.push_back(_cardType);
                costs.push_back(_cost);
                attacks.push_back(_attack);
                defenses.push_back(_defense);
                abilities.push_back(_abilities);
                myHealthChanges.push_back(_myHealthChange);
                opponentHealthChanges.push_back(_opponentHealthChange);
                cardDraws.push_back(_cardDraw);
                
            }
            state = State(opponentHand, opponentActions, opponentCards, cardCount, cardNumbers,
                          instanceIds, locations, cardTypes, costs, attacks, defenses, abilities,
                          myHealthChanges, opponentHealthChanges, cardDraws);
        }
        
        static auto isDraftingPhase() -> bool {
            return players.self.getMana() == 0 && players.enemy.getMana() == 0;
        }
    
};

int Game::currentTurn = 0;
bool Game::running = true;
Players Game::players = Players(Player(), Player());
State Game::state = State();
std::string Action::PICK = "PICK 1";
std::string Action::PASS = "PASS";

Game game;

auto main() -> int {
    while (Game::running) {
        Game::readDataForCurrentTurn();
        if (Game::isDraftingPhase()) {
            std::cout << Action::PICK << std::endl;
        }
        else {
            std::cout << Action::PASS << std::endl;
        }
    }
}