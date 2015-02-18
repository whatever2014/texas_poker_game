// Specification for Player Class 

#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Card.h"
#include "Deck.h"
#include "Table.h"

using namespace std;

class Player {

private:
	string name;
	int chips;
	int bet;
	Card * cards;
	Card * hand;
	bool playing;
	bool smallBlind;
	bool bigBlind;
	int score;
	int highBet;

public:
	Player();
	Player(string name);
	void setName(string name);
	string getName();
	void setChips(int pot);
	int getChips();
	void setBet(Table * table, int bet);
	void setHighBet(int bet);
	int getHighBet();
	bool isPlaying();
	void setPlaying(bool playing);
	void setSmallBlind(bool smallBlind);
	void setBigBlind(bool bigBlind);
	bool isSmallBlind();
	bool isBigBlind();
	void setCards(Deck deck, int i);
	void getCards();
	int getCardSuit(int i);
	int getCardValue(int i);
	void deleteCards();
	void getHand();
	void setHand(Table * table);
	void setScore();
	int getScore();
	void makeHand(int index, int suit, int value);
	void sortHand();

}; 

#endif

