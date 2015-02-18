// Deck.h
// Specification for Deck class

#ifndef Deck_H
#define Deck_H

#include <string>
#include "Card.h"

using namespace std;

class Deck

{
private:
	Card * card;

public:
	Deck();
	void shuffleDeck();
	Card & dealCard();
	void printDeck();

};

#endif