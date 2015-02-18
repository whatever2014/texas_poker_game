// implementation for Player class

#include <iostream>
#include <string>

#include "Player.h"
#include "Card.h"
#include "Deck.h"

using namespace std;


Player::Player() {
	this->name = "Default name";
	this->chips = 1000;
	this->cards = new Card[2];
	this->hand = new Card[7];
	this->playing = true;
	this->smallBlind = false;
	this->bigBlind = false;
    this->highBet = 0;
	this->score = 0;
}

Player::Player(string name) {
	this->name = name;
	this->chips = 1000;
	this->cards = new Card[2];
	this->hand = new Card[7];
	this->playing = true;
	this->smallBlind = false;
	this->bigBlind = false;
    this->highBet = 0;
	this->score = 0;
}

void Player::setName(string name) {
	this->name = name;
}

string Player::getName() {
	return name;
}

/** Take the chips from the pot if the player is the winner
    and adds the value of the pot to the player's chips*/
void Player::setChips(int pot) {
    chips += pot;
}

int Player::getChips() {
	return chips;
}

/** When a player places a bet, this function does the following:
    1. removes the amount of the bet from the player's chip count
    2. places the same bet amount into the pot
    3. increases the player's personal high bet for the hand
        this keeps track so a player cannot bet more than their chip count*/
void Player::setBet(Table * table, int bet) {
	chips -= bet;
	table->setPot(bet);
    highBet += bet;
}

// Not certain if this is being used since setBet above makes this redundant
void Player::setHighBet(int bet) {
    highBet = bet;
}

int Player::getHighBet(){
	return highBet;
}

bool Player::isPlaying() {
	return playing;
}

void Player::setPlaying(bool playing) {
    this->playing = playing;
}

void Player::setSmallBlind(bool smallBlind){
	this->smallBlind = smallBlind;
}

void Player::setBigBlind(bool bigBlind) {
	this->bigBlind = bigBlind;
}

bool Player::isSmallBlind() {
	return smallBlind;
}

bool Player::isBigBlind() {
	return bigBlind;
}

/** Takes the top card from the deck and puts it in the player's private
    hand cards*/
void Player::setCards(Deck deck, int i) {
	cards[i] = deck.dealCard();
}

/** Prints cards out to console*/
void Player::getCards() {
    cout << cards[0].getValue() << " of " << cards[0].getSuit() << endl;
    cout << cards[1].getValue() << " of " << cards[1].getSuit() << endl;
}

int Player::getCardSuit(int i) {
    return cards[i].getSuitIndex();
}

int Player::getCardValue(int i) {
    return cards[i].getValueIndex();
}

/** Reassigns player's private hand cards to blank card objects */
void Player::deleteCards() {
    cards = new Card[2];
    hand = new Card[7];
}

/** Prints to console the player's hand, consisting of the two private cards 
    and the five community cards on the table -> see below for function
    to get the cards to the hand */
void Player::getHand() {
    cout << hand[0].getValue() << " of " << hand[0].getSuit() << "   ";    
    cout << hand[1].getValue() << " of " << hand[1].getSuit() << "   ";    
    cout << hand[2].getValue() << " of " << hand[2].getSuit() << "   ";    
    cout << hand[3].getValue() << " of " << hand[3].getSuit() << "   ";    
    cout << hand[4].getValue() << " of " << hand[4].getSuit() << "   ";    
    cout << hand[5].getValue() << " of " << hand[5].getSuit() << "   ";    
	cout << hand[6].getValue() << " of " << hand[6].getSuit() << "   ";
    cout << score << "   " << name << endl;    
}

/** Takes the two private cards and the five community table cards and places
    them in the player's hand. The seven cards are sorted high to low with 
    exchange sort. Cards must be sorted high to low before scoring.*/
void Player::setHand(Table * table) {
    //sorting the cards by its value for hand array: use the bubble sort
    // ascending order
    
    for (int i = 0; i < 5; ++i) {
        hand[i] = table->getCards(i);
    }
    
    for (int i = 0; i < 2; i++) {
        hand[i+5] = cards[i];
    }

    for (int j = 1; j < 7; j++) {
        for (int i = 0; i < 7 - j; i++) {
            if (hand[i].getValueIndex() < hand[i+1].getValueIndex()) {
                Card temp = hand[i];
                hand[i] = hand[i+1];
                hand[i+1] = temp;
            }
        }
    }
}
    
/** Determines the score from the seven cards. See below for explanations. 
    Note that the individual highcard values are set to -1 because the integer
    value for a 2 card is 0. */
void Player::setScore() {

    int const SUITS = 4;

    bool straightFlush = false;
    bool four = false;
    bool fullHouse = false;
    bool flush = false;
    bool straight = false;
    bool three = false;
    bool twoPairs = false;
    bool pairs = false;
    int highCard_straightFlush = -1;
    int highCard_flush = -1;
    int highCard_straight = -1;
    int highCard_four = -1;
    int highCard_three = -1;
    int highCard_twoPair = -1;
    int highCard_pair = -1;
    
    score = 0;

    /** If nothing else, highcard is the first, and therefore largest value,
        card in the hand */
    int highCard = hand[0].getValueIndex();

    /** FLUSH
        Flush is determined by sorting through the cards in the hand searching
        for cards by suit. If a match is found, the first card value is stored
        as the highcard. If at least five of the seven cards in the hand are
        the same suit, flush is true. */
    for (int i = 0; i < SUITS;  i++) {
        int k = 0;
        for (int j = 0; j < 7; j++) {
            if (i == hand[j].getSuitIndex()) {
                if (highCard_flush < 0) {
                    highCard_flush = hand[j].getValueIndex();
                }
                k++;
            }
            if (k >= 5) {
                flush = true;
            }
        }
    }
    
    /** STRAIGHT
        Starts searching beginning at the first card in the hand. If the
        next card in the array has a value one less than the card before,
        add one to the counter. If the cards are the same value, skip over
        with continue. Once a card is more than one less than the one before,
        the inner loop breaks. If the counter is greater or equal to four,
        there are at least five consecutive numbers decreasing by one, 
        which is a straight. This then causes the outer loop to break,
        conserving the high card value.

        If a straight is not found, the outer loop moves one index value to 
        the right (ex. hand[0] to hand[1]. If this fails, the index moves 
        once more to the third element in the hand array. If this fails there
        is no straight since there would be at most four cards decreasing from
        the fourth element. */
    for (int j = 0; j < 3; j++) {
        if (straight) {
            break;
        }
        int k = 0;
        for (int i = j; i < 6; i++){
            if ((hand[i].getValueIndex() == hand[i+1].getValueIndex()+1)) {
                k++;
            }
            else if (hand[i].getValueIndex() == hand[i+1].getValueIndex()) {
                continue;
            }
            else {
                break;
            }
            if (k >= 4) {
                straight = true;
                highCard_straight = hand[j].getValueIndex();
                break;
            }
        }
    }    

    /** STRAIGHT FLUSH
        Very similar to the straight algorithm above, sorts through starting
        at the first index and moving up to and including the third index
        if needed. Cards must decrease by one and be the same suit to increment
        the counter. */
    for (int j = 0; j < 3; j++) {
        if (straightFlush) {
            break;
        }
        int k = 0;
        for (int i = j; i < 6; i++){
            if (i + j < 7 && (hand[j+i].getValueIndex() == 
                hand[j+i+1].getValueIndex()+1) &&
                hand[i].getSuitIndex() == hand[i+1].getSuitIndex()) {
                k++;
            }
            else {
                break;
            }
            if (k >= 4) {
                straightFlush = true;
                highCard_straightFlush = hand[j].getValueIndex();
                break;
            }
        }
    }    

    /** FULL HOUSE, FOUR OF A KIND, THREE OF A KIND, TWO PAIR, AND PAIRS
        Since the cards are sorted high to low, any cards of the same value
        will be found in order in the hand. The loop starts at the first index
        and continues to the sixth index, since the while loop looks one index
        forward. The while loop counts how many times two cards in sequnce have
        the same value. The determination for four of a kind and three of a 
        kind are straighforward, but two of a kind requires another level of 
        checking. Because a full house is a three of a kind and a pair, the 
        second else if statement below only states that pair is true if the
        value of the pair does not equal the value of the three of a kind. This
        occurs if there is a three of a kind, then when the index increments,
        the second and third cards of the three of a kind will result in pairs
        being true without the extra determination. Also, if there is a 
        previous pair, keep the highcard value of the larger pair.*/
    for (int i = 0; i < 6; i++) {
        int k = 0;
        
        while (k + i < 6 && hand[k+i].getValueIndex() == 
            hand[k+i+1].getValueIndex()) {
            k++;
        }
        if (k == 3){
            four = true;
            highCard_four = hand[i].getValueIndex();
        }
        else if (k == 2) {
            three = true;
            highCard_three = hand[i].getValueIndex();
        }
        else if (k == 1 && highCard_three != hand[i].getValueIndex()) {
            if (pairs) {
                twoPairs = true;
                highCard_twoPair = highCard_pair;
            }
            else {
                pairs = true;
                highCard_pair = hand[i].getValueIndex();
            }
        }
        if (three && pairs){
            fullHouse = true;
        }
    }        
    
    /** Based on the boolean values determined above,
        the score of the best hand for the player is calculated below */
    if (straightFlush) {
        highCard = highCard_straightFlush;
        score = 180  + highCard;
    }
    
    else if (four){
        highCard = highCard_four;
        score = 160 + highCard;
    }
    
    else if (fullHouse) {
        highCard = highCard_three;
        score = 140 + highCard;
    }
    
    else if (flush) {
        highCard = highCard_flush;
        score = 120 + highCard;
    }
    
    else if (straight) {
        highCard = highCard_straight;
        score = 100 + highCard;
    }
    
    else if (three) {
        highCard = highCard_three;
        score = 80 + highCard;
    }
    
    else if  (twoPairs) {
        highCard = highCard_twoPair;
        score = 60 + highCard;
    }
    
    else if (pairs) {
        highCard = highCard_pair;
        score = 40 + highCard;
    }
    
    else {
        score = highCard;
    }
}

int Player::getScore() {
    return score;
}

/** Used only for testing purposes. Sets the value of a hand card to
    a desired value*/
void Player::makeHand(int index, int suit, int value) {
    hand[index].setSuit(suit);
    hand[index].setSuitIndex(suit);
    hand[index].setValue(value);
    hand[index].setValueIndex(value);
}

/** Used only for testing purposes. Sorts the hand high to low */
void Player::sortHand() {
    for (int j = 1; j < 7; j++) {
        for (int i = 0; i < 7 - j; i++) {
            if (hand[i].getValueIndex() < hand[i+1].getValueIndex()) {
                Card  temp = Card();
                temp = hand[i];
                hand[i] = hand[i+1];
                hand[i+1] = temp;
            }
        }
    }
}
