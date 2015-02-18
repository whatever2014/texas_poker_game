// Card.cpp
// Implementation for Card

#include "Card.h"
#include "Deck.h"

#include <iostream>
#include <string>

using namespace std;


Card::Card() {
	this->suit = " ";
	this->value = " ";
	this->suitIndex = 0;
	this->valueIndex = 0;
}


void Card::setSuit(int suitIndex) {
	switch (suitIndex) {
		case 0: suit = "S"; break;
		case 1: suit = "H"; break;
		case 2: suit = "D"; break;
		case 3: suit = "C"; break;
	}
}

void Card::setValue(int valueIndex) {
	switch (valueIndex) {
		case 0: value = "2"; break;
		case 1: value = "3"; break;
		case 2: value = "4"; break;
		case 3: value = "5"; break;
		case 4: value = "6"; break;
		case 5: value = "7"; break;
		case 6: value = "8"; break;
		case 7: value = "9"; break;
		case 8: value = "10"; break;
		case 9: value = "J"; break;
		case 10: value = "Q"; break;
		case 11: value = "K"; break;
		case 12: value = "A"; break;
	}
}

void Card::setSuitIndex(int suitIndex) {
	this->suitIndex = suitIndex;
}

void Card::setValueIndex(int valueIndex) {
	this->valueIndex = valueIndex;
}

string Card::getSuit() {
	return suit;
}

string Card::getValue() {
	return value;
}

int Card::getSuitIndex() {
	return suitIndex;
}

int Card::getValueIndex() {
	return valueIndex;
}

