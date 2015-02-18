/*
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Table.h"

using namespace std;

int small_blind = 25;
int big_blind = 50;

int human;

/** Prints a scoreboard of the all players and their chip counts.
	If a player folds or is out of the game, just name and chip count are 
	shown. Otherwise, the cards are shown as well.
	If watching, all cards are shown. */
void ScoreBoard(Player * players, int size) {
	for (int i = 0; i < size; i++) {
		if (i == human || human == -1) {
			cout << players[i].getName() << endl;
			cout << players[i].getChips() << endl;
			players[i].getCards();
			cout << endl;
		}
		else {
			cout << players[i].getName() << endl;
			cout << players[i].getChips() << endl;
			cout << endl;
		}
	}
}

/** Prints a scoreboard of the all players and their chip counts.
	If a player folds or is out of the game, just name and chip count are 
	shown. Otherwise, the cards are shown as well.
	If watching, all cards are shown. */
void showdownScoreboard(Player * players, int size) {
	for (int i = 0; i < size; i++) {
		if (players[i].isPlaying() || human == -1) {
			cout << players[i].getName() << endl;
			cout << players[i].getChips() << endl;
			players[i].getCards();
			cout << endl;
		}
		else {
			cout << players[i].getName() << endl;
			cout << players[i].getChips() << endl;
			cout << endl;
		}
	}
}

/** Moves the dealer button one player forward.
	Moves the small blind one player forward from the dealer.
	Moves the big blind one player forward from the small blind.
	In all cases, if the next player is out of the game, increment until a
	player in the game is found. If any of the three index values reach the end
	of the players array, index % numPlayers returns the index to the beginning
	of the players array. */
void moveDealerButton(Player * players, int & dealer, int & smallBlind, 
		int & bigBlind, int numPlayers, int & playFirst) {
	
	dealer++;
	if (dealer >= numPlayers) {
		dealer %= numPlayers;
	}
	while (!(players[dealer].isPlaying())) {
		dealer++;
		if (dealer >= numPlayers) {
			dealer %= numPlayers;
		}
	}
	
	smallBlind = dealer + 1;
	if (smallBlind >= numPlayers) {
		smallBlind %= numPlayers;
	}
	while (!(players[smallBlind].isPlaying())) {
		smallBlind++;
		if (smallBlind >= numPlayers) {
			smallBlind %= numPlayers;
		}
	}
	players[smallBlind].setSmallBlind(true);

	playFirst = smallBlind;

	bigBlind = smallBlind + 1;
	if (bigBlind >= numPlayers) {
		bigBlind %= numPlayers;
	}
	while(!(players[bigBlind].isPlaying())) {
		bigBlind++;
		if (bigBlind >= numPlayers) {
			bigBlind %= numPlayers;
		}
	}
	players[bigBlind].setBigBlind(true);
}

/** Resets player attributes to the base state after a hand is complete */
void resetHand(Player * players, Deck * deck, int numPlayers) {
	deck->shuffleDeck();
	for (int i = 0; i < numPlayers; i++) {
		players[i].setPlaying(true);
		players[i].setHighBet(0);
		players[i].deleteCards();
	}
}

/** Used at the end of a hand to see how many players remain and sets
	player's isPlaying() attribute to false if no chips. */
int playersRemaining(Player * players, int numPlayers) {
	int players_remaining = 0;
	for (int i = 0; i < numPlayers; i++) {
		if (players[i].getChips() > 0) {
			players_remaining++;
		}
		else if (players[i].getChips() <= 0) {
			players[i].setPlaying(false);
		}
	}
	return players_remaining;
}

/** Used to see if a player is still playing the hand */
int playingRound(Player * players, int numPlayers) {
	int playing = 0;
	for (int i = 0; i < numPlayers; i++) {
		if (players[i].isPlaying()) {
			playing ++;
		}
	}
	return playing;
}

/** Finds the lowest chip count remaining in the game. This amount
	is the maximum amount that can be bet, putting the low chip count
	all in. 
	The first for loop finds the first player remaining in the game and sets
	that chip count as the maximum. The next for loop finds the lowest chip
	count at the table. */
int setMaxBet(Player * players, int numPlayers) {
	int max_bet;
	for (int i = 0; i < numPlayers; i++) {
		if (players[i].isPlaying()) {
			max_bet = players[i].getChips();
			break;
		}
	}
	for (int i = 1; i < numPlayers; i++) {
		if (players[i].isPlaying() && players[i].getChips() < max_bet) {
			max_bet = players[i].getChips();
			cout << "Min bet is " << players[i].getName() << ": $" << 
				players[i].getChips() << endl;
		}
	}
	return max_bet;
}

/** Plays the flop cards on the table */
void flop(Table table, Deck deck) {
	for (int i = 0; i < 3; i++) {
		table.setCards(deck, i);
	}
}

/** Plays the turn card on the table */
void turn(Table table, Deck deck) {
	table.setCards(deck, 3);
}

/** Plays the river card on the table */
void river(Table table, Deck deck) {
	table.setCards(deck, 4);
}

/** Selecting the check option is essentially skipping a turn */
void checkTurn(Player * players, int i) {
	players[i].setPlaying(true);
}

/** This function sets a players bet, increases the number of bets on the
	table, and increases the high bet on the table.
	The setBet() function does the following:
    1. removes the amount of the bet from the player's chip count
    2. places the same bet amount into the pot
    3. increases the player's personal high bet for the hand
        this keeps track so a player cannot bet more than their chip count*/
void betTurn(Table * table, Player * players, int bet, int i) {
	players[i].setBet(table, bet);
	table->setNumBets();
	table->setHighBet(bet);
}

/** To call a bet, the player needs to match the high bet on the table. This is
	done by increasing the player's bet by the difference between the high bet
	on the table and the amount the player has already bet.*/
void callTurn(Table * table, Player * players, int i) {
	players[i].setBet(table, table->getHighBet() - players[i].getHighBet());
}

/** Similar to the callTurn function above, if a player is going to raise a 
	bet, the player must first call the current high bet, which is the 
	difference between the high bet on the table and what the player has
	already bet. Then, the player increases the high bet on the table by the
	allowed bet amount.*/
void raiseTurn(Table * table, Player * players, int bet, int i) {
	players[i].setBet(table, table->getHighBet() - players[i].getHighBet() + 
		bet);
	table->setNumBets();
	table->setHighBet(bet);
}

/** Folding results in player.isPlaying() set to false, and then the game moves
	on to the next player. */
void foldTurn(Player * players, int i) {
	players[i].setPlaying(false);
}

/** Determines what is considered a "good hand." Currently shown as having
	matching cards or suits or at least one ace. */
bool goodHand(Player * players, int index) {
	return (players[index].getCardSuit(1) == players[index].getCardSuit(2) ||
			players[index].getCardValue(1) == players[index].getCardValue(2) ||
			players[index].getCardValue(1) == 12 ||
			players[index].getCardValue(2) == 12);
}

/** Determines what is considered a "bad hand." Currently shown as having
	a 2-7 hand, or cards differing in value by 5 or more and having 
	non-matching suits and no aces. */
bool badHand(Player * players, int index) {
	return ((players[index].getCardValue(1) == 0 && 
			 players[index].getCardValue(2) == 5) || 
			(players[index].getCardValue(1) == 5 && 
			 players[index].getCardValue(2) == 0) ||
			(abs(players[index].getCardValue(1) - 
				players[index].getCardValue(2)) >= 5 &&
				(players[index].getCardSuit(1) != 
				 players[index].getCardSuit(2)) &&
				(players[index].getCardValue(1) != 12 &&
				 players[index].getCardValue(2) != 12)));
}

/** Returns true based on a probability given as the parameter. If p = 0.6, 
	function will return true 60% of the time. */
bool prob_true(double p){
    return rand()/(RAND_MAX+1.0) < p;
}

/** The takeTurn function provides options for the player to select based on 
	game situations. This function occurs if other required actions do not
	occur in the playRound function below.
	Due to its size, see comments throughout the function. */
void takeTurn(Table * table, Player * players, int index, int numPlayers,
	int bet, int max_bet) {
	
	int decision = 0;

	/** If placing a bet will result in the high bet on the table being larger
		than the maximum allowed bet (which is the small chip count at the 
		table), then the bet amount is changed to be whatever will put the
		low chip count all in. */
	if (table->getHighBet() + bet > max_bet) {
		bet = max_bet - table->getHighBet();
	}

	/** This will skip the round if a player is put all in. If there are no
		bets on the table and the maximum allowed bet is zero, then there
		can be no bets during the roung (since a player is all in), and this
		essentially ends the hand and goes to the showdown. */
	if (table->getNumBets() == 0 && bet == 0) {
		players[index].setPlaying(true);
	}

	/** If there are no bets on the table, then the player has options to
		check, bet, or fold. */
	else if (table->getNumBets() == 0 && table->getHighBet() + bet <= 
		max_bet) {
		/** Artificial intelligence for non-human players. */
		if (index != human) {
			if (goodHand(players, index)) {
				betTurn(table, players, bet, index);
				cout << players[index].getName() << " bets!" << endl << endl;
			}
			else {
				checkTurn(players, index);
				cout << players[index].getName() << " checks!" << endl << endl;
			}
		}
		else {
		while (!cin || decision > 4 || decision < 1) {
			cout << players[index].getName() << 
				"! What do you want to do? Bet = " << bet << endl;
			cout << "Pot: " << table->getPot() << "   Max Bet: " << max_bet <<
				"   High Bet: " << table->getHighBet() 
				<< "  Your high bet: " << players[index].getHighBet() << endl;
			cout << "1. Check" << endl;
			cout << "2. Bet" << endl;
			cout << "3. Fold" << endl;
			cin >> decision;
			cin.clear();
			cin.ignore(1000, '\n');
		}

		if (decision == 1) {
			checkTurn(players, index);
			// players[index].setPlaying(true);
		}

		else if (decision == 2) {
			betTurn(table, players, bet, index);
			// players[index].setBet(table, bet);
			// table->setNumBets();
			// table->setHighBet(bet);
		}

		else if (decision == 3) {
			foldTurn(players, index);
			// players[index].setPlaying(false);
		}

		else {
			cout << "ERROR with takeTurn function" << endl;
			exit(0);
		}
		}
	}

	/** If there is money in the pot, someone has bet, so the player can no 
		longer check, but can only call, raise, or fold. The player can raise
		as long as the bet limit for the round has not been met. */
	else if (table->getPot() > 0 && table->getNumBets() <4 && 
		table->getHighBet() + bet <= max_bet && bet != 0) {
		/** Artificial intelligence for non-human players. */
		if (index != human) {
			if (goodHand(players, index)) {
				raiseTurn(table, players, bet, index);
				cout << players[index].getName() << " raises!" << endl << endl;
			}
			else if (badHand(players, index)) {
				foldTurn(players, index);
				cout << players[index].getName() << " folds!" << endl << endl;
			}
			else {
				if (prob_true(0.2)) {
					raiseTurn(table, players, bet, index);
					cout << players[index].getName() << " raises!" << endl << endl;
				}
				else if (prob_true(0.75)) {
					callTurn(table, players, index);
					cout << players[index].getName() << " calls!" << endl << endl;
				}
				else {
					foldTurn(players, index);
					cout << players[index].getName() << " folds!" << endl << endl;
				}
			}
		}
		else {
		while (!cin || decision > 3 || decision < 1) {
			cout << players[index].getName() 
				<< "! What do you want to do? Bet = " << bet << endl;
			cout << "Pot: " << table->getPot() << "   Max Bet: " << max_bet <<
				"   High Bet: " << table->getHighBet() 
				<< "  Your high bet: " << players[index].getHighBet() << endl;
			cout << "1. Call" << endl;
			cout << "2. Raise" << endl;
			cout << "3. Fold" << endl;
			cin >> decision;
			cin.clear();
			cin.ignore(1000, '\n');
		}

		if (decision == 1) {
			callTurn(table, players, index);
			// players[index].setBet(table, table->getHighBet() - 
			// 	layers[index].getHighBet());
		}

		else if (decision == 2) {
			raiseTurn(table, players, bet, index);
			// players[index].setBet(table, table->getHighBet() - 
			// 	players[index].getHighBet() + bet);
			// table->setNumBets();
			// table->setHighBet(bet);
		}

		else {
			foldTurn(players, index);
			// players[index].setPlaying(false);
		}
		}
	}

	/** If none of the above situations can be met, then the maximum bets for
		the round have been met, or a player has been put all in, so the only
		options are to call or fold. */
	else {
		/** Artificial intelligence for non-human players. */
		if (index != human) {
			if (goodHand(players, index)) {
				callTurn(table, players, index);
				cout << players[index].getName() << " calls!" << endl << endl;
			}
			else if (badHand(players, index)) {
				foldTurn(players, index);
				cout << players[index].getName() << " folds!" << endl << endl;
			}
			else {
				if (prob_true(0.6)) {
					callTurn(table, players, index);
					cout << players[index].getName() << " calls!" << endl << endl;
				}
				else {
					foldTurn(players, index);
					cout << players[index].getName() << " folds!" << endl << endl;
				}
			}
		}
		else{
		while (!cin || decision > 2 || decision < 1) {
			cout << players[index].getName() << 
				"! What do you want to do? Bet = " << bet << endl;
			cout << "Pot: " << table->getPot() << "   Max Bet: " << max_bet <<
				"   High Bet: " << table->getHighBet() 
				<< "  Your high bet: " << players[index].getHighBet() << endl;
			cout << "1. Call" << endl;
			cout << "2. Fold" << endl;
			cin >> decision;
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (decision == 1) {
			callTurn(table, players, index);
			// players[index].setBet(table, table->getHighBet() - 
			// 	players[index].getHighBet());
		}
		else {
			foldTurn(players, index);
			// players[index].setPlaying(false);
		}
		}
	}
}

/** playRound controls the gameplay for one ROUND (one round of four inside a 
	hand) of betting. To allow the round to play until only one player is left
	or all bets are equal, two for loops are inside a while loop. The two loops
	run from the player after the dealer to the end of the player array, and 
	the second loop runs from the beginning of the player array to the player
	after the dealer. */
void playRound(Table * table, Player * players, int numPlayers, int playFirst,
	int bet, int max_bet) {
	
	/** Clear the number of bets on the table back to zero. Four bets caps
		betting for the round. */
	table->resetNumBets();
		for (int i = 0; i < numPlayers; i++) {
			players[i].setHighBet(0);
		}

	/** Before the round starts, completeRound set to false. After both for
		loops complete once, completeRound is set to true. The logic here
		is that a round may not end until all players have had at least one
		action and all bets are equal. Once every player has had a turn, the 
		completeRound boolean is set to true, allowing the round to end once
		bets are equal. */
	bool completeRound = false;

	while (true) {
		/** Note that the two loops are identical, so only the first loop will
			have comments. */
		for (int i = playFirst; i < numPlayers; i++) {

			/** Count the number of players currently playing the round. If
				only one, then all the rest have folded. */
			int playing_round = playingRound(players, numPlayers);
			if (playing_round == 1) {
				cout << "Only one left!" << endl << endl << endl;
				return;
			}

			/** If a player has folded or is out of chips, the player will be
				skipped. */
			if (!(players[i].isPlaying())) {
				continue;
			}

			/** The small blind player as selected with the moveDealerButton
				function above plays the small blind amount, increments the 
				number of bets on the table, and then sets the small blind
				attribute to false.*/
			else if (players[i].isSmallBlind()) {
				players[i].setBet(table, small_blind);
				table->setNumBets();
				players[i].setSmallBlind(false);
			}

			/** Same idea as described above for small blind. */
			else if (players[i].isBigBlind()) {
				if (max_bet < big_blind) {
					big_blind = max_bet;
				}
				players[i].setBet(table, big_blind);
				table->setHighBet(big_blind);
				players[i].setBigBlind(false);
			}

			/** When at least one round has been played and all players have
				the same bet, the round is complete. */
			else if (completeRound && players[i].getHighBet() == 
				table->getHighBet()) {
				cout << "Equal bids let's move on!" << endl << endl << endl;
				return;
			}
			else {

				/** If none of the above situations occur, then the player must
					choose which option to take for the round as described in
					takeTurn above. */
				takeTurn(table, players, i, numPlayers, bet, max_bet);
			}
		}
		for (int i = 0; i < playFirst; i++) {
			int playing_round = playingRound(players, numPlayers);
				if (playing_round == 1) {
					cout << "Only one left!" << endl << endl << endl;
					return;
				}
			if (!(players[i].isPlaying())) {
				continue;
			}
			else if (players[i].isSmallBlind()) {
				players[i].setBet(table, small_blind);
				table->setNumBets();
				players[i].setSmallBlind(false);
			}
			else if (players[i].isBigBlind()) {
				if (max_bet < big_blind) {
					big_blind = max_bet;
				}
				players[i].setBet(table, big_blind);
				table->setHighBet(big_blind);
				players[i].setBigBlind(false);
			}
			else if (completeRound && players[i].getHighBet() == 
				table->getHighBet()) {
				cout << "Equal bids let's move on!" << endl;
				return;
			}
			else {
				takeTurn(table, players, i, numPlayers, bet, max_bet);
			}
		}
		completeRound = true;
	}
}

/** Describes how a winner is selected and money is paid out to the one or more
	winners. */
void determineWinner(Table * table, Player * players, int numPlayers) {
	int playing_round = playingRound(players, numPlayers);
	int pot = table->getPot();
	int high_score = 0;
	int win_counter = 0;

	/** If there is only one player left playing the round, the player wins
		by default and has the value of the pot added to the chip count. */
	if (playing_round == 1) {
		for (int i = 0; i < numPlayers; i++) {
			if (players[i].isPlaying()) {
				cout << players[i].getName() << " wins!" << endl << endl;
				players[i].setChips(pot);
			}
		}
	}
		/** If multiple players remain at the end of betting, the showdown
			occurs*/
	else {

		/** For all players remaining at the showdown, the two private cards
			and five community cards are placed in the hand array and sorted
			by card value from high to low. Then the scoring is performed as
			described in setScore() in Player class. */
		for (int i = 0; i < numPlayers; i++) {
			if (players[i].isPlaying()) {
				players[i].setHand(table);
				players[i].setScore();
			}
		}

		/** Need a place to begin comparing high scores, so increment through
			the player array, and call the score of the first player remaining
			the high sccore. */
		for (int i = 0; i < numPlayers; i++) {
			if (players[i].isPlaying() && 
				players[i].getScore() > high_score) {
					high_score = players[i].getScore();
					// win_counter++;
			}
		}
		/** Increment through the player array, and if a player remains and has
			the same score as the high score, increment a counter. */
		for (int i = 0; i < numPlayers; i++) {
			if (players[i].isPlaying() && 
				players[i].getScore() == high_score) {
					// high_score = players[i].getScore();
					win_counter++;
			}
		}
		/** Increment through the player array, and if a player remains and has
			the same score as the high score, they won at least a share of the 
			pot. Divide the pot by the number of winners. */
		for (int i = 0; i < numPlayers; i++) {
			if (players[i].isPlaying() &&
				players[i].getScore() == high_score) {
				cout << players[i].getName() << " wins!" << endl << endl;
				players[i].setChips(pot / win_counter);
			}
		}
	}
	/** Prints out the hand and the score of the player to console. */
	for (int i = 0; i < numPlayers; i++) {
		if (players[i].isPlaying()) {
			players[i].getHand();
		}
	}
}

int main() {
	/** Initialize variables */
	int numPlayers = 4;
	int playersLeft = 0;
	string name;
	string pause;

	/** only one dock and table need to be instantiated. */
	Deck deck = Deck();
	Table table = Table();

	/** Indices that increment after each hand as described in the 
		moveDealerButton function. */
	int dealer = -1;
	int smallBlind = dealer + 1;
	int bigBlind = smallBlind + 1;
	int playFirst = dealer + 1;

	int bet = 50;

	cout << "Hello, let's play poker!" << endl;

	// cout << "Let's print out the cards..." << endl;
	// deck.printDeck();
	// cout << endl << "Neato! Now let's shuffle the deck..." << endl;

	/** Shuffle the deck. */
	deck.shuffleDeck();
	// deck.printDeck();

	/** Select number of players for game. This can, and may need to be,
		changed to a set number of computer players with one human. */
	// while (!cin || (numPlayers > 10 || numPlayers <= 1)) {
	// 	cout << "How many players? Choose 2-10 please: ";
	// 	cin >> numPlayers;
	// 	cin.clear();
	// 	cin.ignore(1000, '\n');
	// }

	/** Make an array of Player objects. */
	Player * players = new Player[numPlayers];
	playersLeft = numPlayers;

	/** Name the players. */
	// for (int i = 0; i < numPlayers; i++) {
	// 	string name;
	// 	cout << "Please enter the name of Player " << i+1 << ": ";
	// 	getline(cin, name);
	// 	players[i] = Player(name);
	// }

	/** Sets up the game for either a human player or a simulation of 
		computer players. */
	int game_type;
	while (!cin || game_type > 2 || game_type < 1) {
		cout << "Playing or watching? " << endl;
		cout << "1. Playing" << endl;
		cout << "2. Watching" << endl;
		cin >> game_type;
		cin.clear();
		cin.ignore(1000, '\n');
	}

	if (game_type ==1) {
		human = 3;
		cout << "Please enter your name: ";
		getline(cin, name);
		players[human] = Player(name);
		players[0] = Player("Rick");
		players[1] = Player("Marty");
		players[2] = Player("Shannon");
	}

	else {
		human = -1;
		players[0] = Player("Rick");
		players[1] = Player("Marty");
		players[2] = Player("Shannon");
		players[3] = Player("Molly");
	}


	/** Prints the indices to console for error checking. */
	// cout << dealer << endl;
	// cout << smallBlind << endl;
	// cout << bigBlind << endl;
	// cout << playFirst << endl;


	/** Move the dealer button forward. */
	moveDealerButton(players, dealer, smallBlind, bigBlind, numPlayers, playFirst);

	/** Prints the indices to console for error checking. */
	// cout << dealer << endl;
	// cout << smallBlind << endl;
	// cout << bigBlind << endl;
	// cout << playFirst << endl;

	/** THIS WHILE LOOP CONTAINS THE ACTUAL GAMEPLAY 
		The game continues until there is only one player left at the table. */
	while (playersLeft > 1) {
		cout << "Press any key to continue...";
		cin.ignore();
		/** More print to console commands for error checking. */
		cout << "Dealer: " << players[dealer].getName() << endl;
		cout << "Small Blind: " << players[smallBlind].getName() << endl;
		cout << "Big Blind: " << players[bigBlind].getName() << endl;
		cout << "Playing First: " << players[playFirst].getName() << endl;

		/** If the player is playing, then deal cards around the table. */
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < numPlayers; i++) {
				if (players[i].isPlaying()){
					players[i].setCards(deck, j);
				}
			}
		} 

		bet = 50;

		/** Determine the small chip count at the table and make this the
			maximum bet amount. */
		int max_bet = setMaxBet(players, numPlayers);
		// if (bet + table.getHighBet() >= max_bet) {
		// 	bet = max_bet - table.getHighBet();
		// }

		/** Prints the name, score, and cards of the player to console. */
		ScoreBoard(players, numPlayers);


		/** Plays a pre-flop round. */
		playRound(&table, players, numPlayers, playFirst, bet, max_bet);
		
		max_bet = setMaxBet(players, numPlayers);
		// if (bet + table.getHighBet() >= max_bet) {
		// 	bet = max_bet - table.getHighBet();
		// }

		/** Plays the three flop cards to the table and prints them to 
			console. */
		flop(table, deck);
		table.printTable();	
		
		/** Plays a round after the three flop cards are played. */
		playRound(&table, players, numPlayers, playFirst, bet, max_bet);
		
		max_bet = setMaxBet(players, numPlayers);
		// if (bet + table.getHighBet() >= max_bet) {
		// 	bet = max_bet - table.getHighBet();
		// }

		/** Plays the turn card and prints the table. */
		turn(table, deck);
		table.printTable();

		/** The bet is doubled for the final two rounds. */
		bet *= 2;
		
		/** Plays a round after the turn card. */
		playRound(&table, players, numPlayers, playFirst, bet, max_bet);
		
		max_bet = setMaxBet(players, numPlayers);
		// if (bet + table.getHighBet() >= max_bet) {
		// 	bet = max_bet - table.getHighBet();
		// }

		/** Plays the river card and prints the table. */
		river(table, deck);
		table.printTable();	
		
		/** Plays the final round after all five cards are on the table. */
		playRound(&table, players, numPlayers, playFirst, bet, max_bet);

		table.printTable();	

		/** Determine the winner and print the scoreboard. */
		determineWinner(&table, players, numPlayers);
		showdownScoreboard(players, numPlayers);

		/** Resets the player attributes controlling the playing of a hand:
			setPlaying to true, reset the high bet to zero, and remove the
			private cards. */
		resetHand(players, &deck, numPlayers);

		/** Clear the table of cards, pot value, high bet, and number of 
			bets. */
		table.clearTable();

		/** Determines the number of players remaining in the game. */
		playersLeft = playersRemaining(players, numPlayers);

		/** Increments the dealer button one player forward, which in turn 
			moves the blinds and the player who plays first. */
		moveDealerButton(players, dealer, smallBlind, bigBlind, numPlayers, 
			playFirst);
	}

	/** When only one player is remaining in the game, the while loop breaks.
		If only one player is left in the game, one player has all the chips.
		Find the player who has the chips and select as the winner. */
	for (int i = 0; i < numPlayers; i++) {
		if (players[i].getChips() > 0) {
			cout << players[i].getName() << " WINS!!!" << endl << endl;
			players[i].setPlaying(false);
		}
	}

	ScoreBoard(players, numPlayers);

/** Used for testing scores, disregard for gameplay. 

	Player dergin = Player("Dergin Farklestein");
	dergin.makeHand(0, 0, 10);
	dergin.makeHand(1, 1, 8);
	dergin.makeHand(2, 3, 7);
	dergin.makeHand(3, 0, 6);
	dergin.makeHand(4, 1, 5);
	dergin.makeHand(5, 2, 4);
	dergin.makeHand(6, 0, 1);

	Player torple = Player("Torple Farklestein");
	torple.makeHand(0, 2, 2);
	torple.makeHand(1, 1, 4);
	torple.makeHand(2, 0, 1);
	torple.makeHand(3, 1, 4);
	torple.makeHand(4, 2, 4);
	torple.makeHand(5, 0, 8);
	torple.makeHand(6, 3, 4);

	// dergin.setHand();
	dergin.sortHand();
	dergin.setScore();
	dergin.getHand();
	// torple.setHand();
	torple.sortHand();
	torple.setScore();
	torple.getHand();*/

	return EXIT_SUCCESS;
}