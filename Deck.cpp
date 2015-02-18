// Deck.cpp
// Implementation for Deck

#include "Deck.h"
#include "Card.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int const NUMBER_SUITS = 4;
int const NUMBER_CARDS = 13;
int const DECK = NUMBER_CARDS * NUMBER_SUITS;
int topCard;

/** The deck object is an array of all 52 cards in a deck */
Deck::Deck() {
	card = new Card[DECK];
	for (int i = 0; i < NUMBER_SUITS; i++) {
		int cardIndex = 0;
		for (int j = 0; j < NUMBER_CARDS; j++) {
			cardIndex = i * NUMBER_CARDS + j;
			card[cardIndex].setSuit(i);
			card[cardIndex].setSuitIndex(i);
			card[cardIndex].setValue(j);
			card[cardIndex].setValueIndex(j); 
		}
	}
}

/** Shuffling the deck between hands resets the topCard to the top of the deck
	and randomly orders the deck. A for loop moves every card from the first to
	the end of the deck to a random location in the deck. */
void Deck::shuffleDeck() {
	topCard = DECK - 1;
	srand(time(NULL));
	for (int i = 0; i < DECK; i++) {
		int j = rand() % DECK;
		Card temp;
		temp = card[i];
		card[i] = card[j];
		card[j] = temp;
	}
}

/** Returns a reference to a card in the deck and decrements the topCard */
Card & Deck::dealCard() {
	return card[topCard--];
}

/** Prints the deck to console for testing */
void Deck::printDeck() {
	for (int i = 0; i < DECK; i++) {
		int index = i + 1;
		cout << setw(2) << index << ". " << card[i].getSuit() << " " << 
		setw(2) << card[i].getValue() //<< endl
		<< " " << card[i].getValueIndex() << endl;
	}
}