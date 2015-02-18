// Specification for Table Class 

#ifndef TABLE_H
#define TABLE_H
#include <string>
#include "Card.h"
#include "Deck.h"
#include "Player.h"

using namespace std;

class Table {

private:
	Card * tableCards;
	int pot;
	int highBet;
	int numBets;

public:
	Table();


	
	void setCards(Deck deck, int i);
	Card & getCards(int i);
	void printCards(int i);
	void setPot(int bet);
	int getPot();
	void setHighBet(int bet);
	int getHighBet();
	void setNumBets();
	int getNumBets();
	void resetNumBets();
	void clearTable();
	void printTable();

}; 

#endif

