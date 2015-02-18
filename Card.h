// Card.h
// Specification for Card class

#ifndef CARD_H
#define CARD_H

#include <string>

using namespace std;

class Card

{
private:
	string suit;
	string value;
	int suitIndex;
	int valueIndex;

public:
	Card(); 
	string getSuit();
	string getValue();
	int getSuitIndex();
	int getValueIndex();
	void setSuit(int suitIndex);
	void setValue(int valueIndex);
	void setSuitIndex(int suitIndex);
	void setValueIndex(int valueIndex);
};

#endif