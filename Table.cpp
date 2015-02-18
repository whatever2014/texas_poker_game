// implementation for Player class

#include <iostream>
#include <string>

#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Table.h"

using namespace std;


Table::Table() {
	tableCards = new Card[5];
	pot = 0;
	highBet = 0;
	numBets = 0;
}

void Table::setCards(Deck deck, int i) {
	tableCards[i] = deck.dealCard();
}

Card & Table::getCards(int i) {
	return tableCards[i];
}

void Table::printCards(int i) {
	cout << tableCards[i].getValue() << " of " << tableCards[i].getSuit() << endl;
}

void Table::setPot(int bet) {
	pot += bet;
}

int Table::getPot() {
	return pot;
}

void Table::setHighBet(int bet) {
	highBet += bet;
}

int Table::getHighBet() {
	return highBet;
}

void Table::setNumBets() {
	numBets++;
}

int Table::getNumBets() {
	return numBets;
}

void Table::resetNumBets() {
	numBets = 0;
	highBet = 0;
}

void Table::clearTable() {
	tableCards = new Card[5];
	pot = 0;
	highBet = 0;
	numBets = 0;
}

void Table::printTable() {
	for (int i = 0; i < 5; i++) {
		cout << tableCards[i].getValue() << " of " << tableCards[i].getSuit() << "   ";
	}
	cout << "Pot: $" << pot << endl;
	cout << endl;
}