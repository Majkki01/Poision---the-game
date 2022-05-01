#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#define MAXVALUES 20 // max possible amount of values (o = 20)
#define MAXCARDS 129 // max possible amount of cards (g + k*o => 9 + 6*20 = 129)
#define MAXPLAYERS 10 // max possible amount of players (n = 6)
#define MAXCOLORS 6 // max amount of colors without green = k = 6
using namespace std;

enum color {
	green,
	blue,
	red,
	violet,
	yellow,
	white,
	black,
	not_set
};

typedef struct card {
	enum color cardcolor = not_set;
	int value = {};
};

typedef struct player {
	card P_CARDS[MAXCARDS];
	card F_CARDS[MAXCARDS];
};


void generate_deck(card CARDS[], int VAL[], int k, int g, int gv, int o) {
	//declaring all cards in game
	int val_i = 0;
	for (int i = 0; i < g + (k * o); i++) {
		if (i < g) {
			CARDS[i].cardcolor = green;
			CARDS[i].value = gv;
		}
		else if (i < g + o) {
			CARDS[i].cardcolor = blue;
			CARDS[i].value = VAL[val_i];
			val_i++;
			if (i == g + o - 1) val_i = 0;
		}
		else if (i < g + 2 * o) {
			CARDS[i].cardcolor = red;
			CARDS[i].value = VAL[val_i];
			val_i++;
			if (i == g + 2 * o - 1) val_i = 0;
		}
		else if (i < g + 3 * o) {
			CARDS[i].cardcolor = violet;
			CARDS[i].value = VAL[val_i];
			val_i++;
			if (i == g + 3 * o - 1) val_i = 0;
		}
		else if (i < g + 4 * o) {
			CARDS[i].cardcolor = yellow;
			CARDS[i].value = VAL[val_i];
			val_i++;
			if (i == g + 4 * o - 1) val_i = 0;
		}
		else if (i < g + 5 * o) {
			CARDS[i].cardcolor = white;
			CARDS[i].value = VAL[val_i];
			val_i++;
			if (i == g + 5 * o - 1) val_i = 0;
		}
		else if (i < g + 6 * o) {
			CARDS[i].cardcolor = black;
			CARDS[i].value = VAL[val_i];
			val_i++;
			if (i == g + 6 * o - 1) val_i = 0;
		}
	}
}

void print_deck(card CARDS[]) {
	//I want to print names of enum elements, so I create an array of names
	char color_names[][7] = { "green","blue","red","violet","yellow","white","black" };

	for (int i = 0; CARDS[i].value != 0 && i < MAXCARDS; i++) {
		cout << CARDS[i].value << " " << color_names[CARDS[i].cardcolor] << " ";
	}
}

void schuffle(card CARDS[], int size) {
	srand(time(NULL));
	for (int i = 1; i < size; i++) {
		swap(CARDS[size - i], CARDS[rand() % (size - i)]);
	}
}
void deal_deck(card CARDS[], player PLAYERS[], int c, int p) {
	schuffle(CARDS, c);
	for (int i = 0, counter_p = 0, counter_c = 0; i < c; i++) {
		PLAYERS[counter_p].P_CARDS[counter_c] = CARDS[i];
		if (counter_p == p - 1) {
			counter_p = 0;
			counter_c++;
		}
		else counter_p++;
	}
}
//c stands for amount of card and p stands for amount of players
//counter_p counts players who are supposed to get the card while dealing 
//counter_c counts number of card which a player is going to get 

void view_game(player PLAYERS[], card PILES[][MAXCARDS], int active, int n, int k, int e) {
	cout << "active player = " << active << endl;
	cout << "players number = " << n << endl;
	cout << "explosion threshold = " << e << endl;
	for (int i = 0; i < n; i++) {
		cout << i + 1 << " player hand cards: ";
		print_deck(PLAYERS[i].P_CARDS);
		cout << endl;
		cout << i + 1 << " player deck cards: ";
		print_deck(PLAYERS[i].F_CARDS);
		cout << endl;
	}
	for (int i = 0; i < k; i++) {
		cout << i + 1 << " pile cards: ";
		print_deck(PILES[i]);
		cout << endl;
	}
}

int check_green(player PLAYERS[], card PILES[][MAXCARDS], int n, int k) {
	int g = 0, gv, check = 0, wrong = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].P_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].P_CARDS[j].cardcolor == green) {
				g++;
				if (check == 0) {
					gv = PLAYERS[i].P_CARDS[j].value;
					check++;
				}
				if (gv != PLAYERS[i].P_CARDS[j].value) wrong++;
			}
		}
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].F_CARDS[j].cardcolor == green) {
				g++;
				if (check == 0) {
					gv = PLAYERS[i].F_CARDS[j].value;
					check++;
				}
				if (gv != PLAYERS[i].F_CARDS[j].value) wrong++;
			}
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; PILES[i][j].value != 0; j++) {
			if (PILES[i][j].cardcolor == green) {
				g++;
				if (check == 0) {
					gv = PILES[i][j].value;
					check++;
				}
				if (gv != PILES[i][j].value) wrong++;
			}
		}
	}

	if (g == 0) {
		cout << "Green cards does not exist" << endl;
		return 0;
	}
	else if (wrong == 0) {
		return 1;
	}
	else {
		cout << "Different green cards values occurred" << endl;
		return 2;
	}
}

int check_cards_num(player PLAYERS[], card PILES[][MAXCARDS], int n, int k) {
	int colors[6] = {};
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].P_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].P_CARDS[j].cardcolor == blue) {
				colors[0]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == red) {
				colors[1]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == violet) {
				colors[2]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == yellow) {
				colors[3]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == white) {
				colors[4]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == black) {
				colors[5]++;
			}
		}
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].F_CARDS[j].cardcolor == blue) {
				colors[0]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == red) {
				colors[1]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == violet) {
				colors[2]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == yellow) {
				colors[3]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == white) {
				colors[4]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == black) {
				colors[5]++;
			}
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; PILES[i][j].value != 0; j++) {
			if (PILES[i][j].cardcolor == blue) {
				colors[0]++;
			}
			else if (PILES[i][j].cardcolor == red) {
				colors[1]++;
			}
			else if (PILES[i][j].cardcolor == violet) {
				colors[2]++;
			}
			else if (PILES[i][j].cardcolor == yellow) {
				colors[3]++;
			}
			else if (PILES[i][j].cardcolor == white) {
				colors[4]++;
			}
			else if (PILES[i][j].cardcolor == black) {
				colors[5]++;
			}
		}
	}
	int diffrent = 0;
	for (int i = 0; i < k - 1; i++) {
		if (colors[i] != colors[i + 1]) diffrent++;
	}
	if (diffrent == 0) {
		return 1;
	}
	else {
		cout << "At least two colors with a different number of cards were found:" << endl;
		if (colors[0] != 0) cout << "blue cards are " << colors[0] << endl;
		if (colors[1] != 0) cout << "red cards are " << colors[1] << endl;
		if (colors[2] != 0) cout << "violet cards are " << colors[2] << endl;
		if (colors[3] != 0) cout << "yellow cards are " << colors[3] << endl;
		if (colors[4] != 0) cout << "white cards are " << colors[4] << endl;
		if (colors[5] != 0) cout << "black cards are " << colors[5] << endl;
		return 0;
	}
}

int check_cards_val(player PLAYERS[], card PILES[][MAXCARDS], int n, int k) {
	int VALUES[MAXCOLORS][MAXVALUES] = {};
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].P_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].P_CARDS[j].cardcolor == blue) {
				VALUES[0][PLAYERS[i].P_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == red) {
				VALUES[1][PLAYERS[i].P_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == violet) {
				VALUES[2][PLAYERS[i].P_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == yellow) {
				VALUES[3][PLAYERS[i].P_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == white) {
				VALUES[4][PLAYERS[i].P_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].P_CARDS[j].cardcolor == black) {
				VALUES[5][PLAYERS[i].P_CARDS[j].value - 1]++;
			}
		}
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].F_CARDS[j].cardcolor == blue) {
				VALUES[0][PLAYERS[i].F_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == red) {
				VALUES[1][PLAYERS[i].F_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == violet) {
				VALUES[2][PLAYERS[i].F_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == yellow) {
				VALUES[3][PLAYERS[i].F_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == white) {
				VALUES[4][PLAYERS[i].F_CARDS[j].value - 1]++;
			}
			else if (PLAYERS[i].F_CARDS[j].cardcolor == black) {
				VALUES[5][PLAYERS[i].F_CARDS[j].value - 1]++;
			}
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; PILES[i][j].value != 0; j++) {
			if (PILES[i][j].cardcolor == blue) {
				VALUES[0][PILES[i][j].value - 1]++;
			}
			else if (PILES[i][j].cardcolor == red) {
				VALUES[1][PILES[i][j].value - 1]++;
			}
			else if (PILES[i][j].cardcolor == violet) {
				VALUES[2][PILES[i][j].value - 1]++;
			}
			else if (PILES[i][j].cardcolor == yellow) {
				VALUES[3][PILES[i][j].value - 1]++;
			}
			else if (PILES[i][j].cardcolor == white) {
				VALUES[4][PILES[i][j].value - 1]++;
			}
			else if (PILES[i][j].cardcolor == black) {
				VALUES[5][PILES[i][j].value - 1]++;
			}
		}
	}
	int diffrent = 0;
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < MAXVALUES; j++) {
			if (VALUES[0][j] != VALUES[i][j]) diffrent++;
		}
	}
	if (diffrent == 0) {
		return 1;
	}
	else {
		char color_names[][7] = { "blue","red","violet","yellow","white","black" };
		cout << "The values of cards of all colors are not identical:" << endl;
		for (int i = 0; i < k; i++) {
			cout << color_names[i] << " cards values: ";
			for (int j = 0; j < MAXVALUES; j++) {
				for (int k = 0; k < VALUES[i][j]; k++) {
					cout << j + 1 << " ";
				}
			}
			cout << endl;
		}
		return 0;
	}
}

int state_check(card PILES[][MAXCARDS], int AMOUNT[MAXPLAYERS], int n, int k, int active, int e) {
	int wrong = 0, PILE_VAL[MAXCOLORS] = {}, check = 0;
	color first_col;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (AMOUNT[i] - AMOUNT[j] > 1 || AMOUNT[i] - AMOUNT[j] < -1) wrong++;
		}
		if (i < n - 1) {
			if (AMOUNT[i] < AMOUNT[i + 1] && i + 2 != active) wrong++;
		}
	}
	if (wrong != 0) cout << "The number of players cards on hand is wrong" << endl;

	for (int i = 0; i < k; i++) {
		for (int j = 0; PILES[i][j].value != 0; j++) {
			if (check == 0 && PILES[i][j].cardcolor != green) {
				first_col = PILES[i][j].cardcolor;
				check++;
			}
			if (PILES[i][j].cardcolor != first_col && PILES[i][j].cardcolor != green) {
				cout << "Two different colors were found on the " << i + 1 << " pile" << endl;
				wrong++;
			}
		}
		check = 0;
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; PILES[i][j].value != 0; j++) {
			PILE_VAL[i] += PILES[i][j].value;
		}
	}
	for (int i = 0; i < k; i++) {
		if (PILE_VAL[i] > e) {
			cout << "Pile number " << i + 1 << " should explode earlier" << endl;
			wrong++;
		}
	}
	if (wrong == 0) return 1;
	else return 0;
}

void move_cards(player PLAYERS[], card PILES[][MAXCARDS], int active, int pile) {
	for (int i = 0, j = 0; PILES[pile][i].value != 0; i++, j++) {
		while (PLAYERS[active - 1].F_CARDS[j].value != 0)j++;
		PLAYERS[active - 1].F_CARDS[j] = PILES[pile][i];
		PILES[pile][i].value = 0;
		PILES[pile][i].cardcolor = not_set;
	}
}

void cauldron_check(player PLAYERS[], card PILES[][MAXCARDS], int active, int k, int e) {
	int SUM[MAXCOLORS] = {};
	for (int i = 0; i < k; i++) {
		for (int j = 0; PILES[i][j].value != 0; j++) {
			SUM[i] += PILES[i][j].value;
		}
		if (SUM[i] > e) {
			move_cards(PLAYERS, PILES, active, i);
			break;
		}
	}
}

void easy_move(player PLAYERS[], card PILES[][MAXCARDS], int* active, int n, int k, int e) {
	card move_card;
	int pile_number = -1, j;
	move_card.cardcolor = PLAYERS[*active - 1].P_CARDS[0].cardcolor;
	move_card.value = PLAYERS[*active - 1].P_CARDS[0].value;

	for (int i = 0; PLAYERS[*active - 1].P_CARDS[i].value != 0; i++) {
		PLAYERS[*active - 1].P_CARDS[i] = PLAYERS[*active - 1].P_CARDS[i + 1];
	}
	for (int i = 0; i < k; i++) {
		if (move_card.cardcolor != green) {
			for (j = 0; PILES[i][j].value != 0; j++) {
				if (PILES[i][j].cardcolor == move_card.cardcolor) {
					pile_number = i;
					break;
				}
				else if (PILES[i][j].cardcolor != green) break;
			}
		}
		else {
			pile_number = 0;
			break;
		}
		if (pile_number != -1) break;
	}
	if (pile_number == -1) {
		for (int i = 0; i < k; i++) {
			for (int j = 0; PILES[i][j].value == 0 || PILES[i][j].cardcolor == green; j++) {
				if (PILES[i][j].value == 0) {
					pile_number = i;
					break;
				}
			}
			if (pile_number != -1) break;
		}
	}
	for (int i = 0; i < MAXCARDS; i++) {
		if (PILES[pile_number][i].value == 0) {
			PILES[pile_number][i] = move_card;
			break;
		}
	}
	cauldron_check(PLAYERS, PILES, *active, k, e);
	if (*active != n) (*active)++;
	else *active = 1;
}

void direct_move(player PLAYERS[], card PILES[][MAXCARDS], card card, int pile, int* active, int n, int k, int e) {
	for (int i = 0; PLAYERS[*active - 1].P_CARDS[i].value != 0; i++) {
		if (PLAYERS[*active - 1].P_CARDS[i].value == card.value && PLAYERS[*active - 1].P_CARDS[i].cardcolor == card.cardcolor) {
			for (int j = i; PLAYERS[*active - 1].P_CARDS[j].value != 0; j++) {
				PLAYERS[*active - 1].P_CARDS[j] = PLAYERS[*active - 1].P_CARDS[j + 1];
			}
			break;
		}
	}
	for (int i = 0; i < MAXCARDS; i++) {
		if (PILES[pile][i].value == 0) {
			PILES[pile][i] = card;
			break;
		}
	}
	cauldron_check(PLAYERS, PILES, *active, k, e);
	if (*active != n) (*active)++;
	else *active = 1;
}

int sum_in_pile_check(card PILES[][MAXCARDS], int pile_index, int move_card) {
	int sum = 0;
	for (int i = 0; PILES[pile_index][i].value != 0; i++) {
		sum += PILES[pile_index][i].value;
	}
	sum += move_card;
	return sum;
}

void highest_value_move(player PLAYERS[], card PILES[][MAXCARDS], int* active, int n, int k, int e) {
	card move_card[MAXCOLORS + 1];
	int index = 1, not_unique = 0;
	move_card[0] = PLAYERS[*active - 1].P_CARDS[0];
	for (int i = 1; PLAYERS[*active - 1].P_CARDS[i].value != 0; i++) {
		if (PLAYERS[*active - 1].P_CARDS[i].value > move_card[0].value) {
			move_card[0] = PLAYERS[*active - 1].P_CARDS[i];
		}
	}
	for (int i = 1; PLAYERS[*active - 1].P_CARDS[i].value != 0; i++) {
		for (int j = 0; j < k + 1; j++) {
			if (PLAYERS[*active - 1].P_CARDS[i].cardcolor == move_card[j].cardcolor) {
				not_unique++;
			}
		}
		if (PLAYERS[*active - 1].P_CARDS[i].value == move_card[0].value && not_unique == 0) {
			move_card[index] = PLAYERS[*active - 1].P_CARDS[i];
			index++;
		}
		not_unique = 0;
	}
	int SUMS[MAXCOLORS + 1] = {}, move_card_pile_i[MAXCOLORS + 1] = {};
	index = 0;
	int check = 0;
	for (int i = 0; move_card[i].value != 0 && i < k + 1; i++) {
		if (move_card[i].cardcolor != green) {
			for (int j = 0; j < k; j++) {
				if (PILES[j][0].cardcolor == move_card[i].cardcolor) {
					SUMS[index] = sum_in_pile_check(PILES, j, move_card[i].value);
					move_card_pile_i[index] = j;
					index++;
					check++;
				}
			}
			if (check == 0) {
				for (int j = 0; j < k; j++) {
					if (PILES[j][0].value == 0) {
						SUMS[index] = move_card[i].value;
						move_card_pile_i[index] = j;
						index++;
						check = 0;
						break;
					}
				}
			}
		}
		else {
			int lowest = sum_in_pile_check(PILES, 0, move_card[i].value);
			for (int j = 1; j < k; j++) {
				if (sum_in_pile_check(PILES, j, move_card[i].value) < lowest) {
					lowest = sum_in_pile_check(PILES, j, move_card[i].value);
					move_card_pile_i[index] = j;
				}
			}
			SUMS[index] = lowest;
			index++;
		}
	}
	card final_card;
	final_card = move_card[0];
	int final_i = 0;

	for (int i = 1; i < k; i++) {
		if (SUMS[i] < SUMS[final_i] && SUMS[i] != 0) {
			final_card = move_card[i];
			final_i = i;
		}
	}
	direct_move(PLAYERS, PILES, final_card, move_card_pile_i[final_i], &*active, n, k, e);
}

void lowest_value_move(player PLAYERS[], card PILES[][MAXCARDS], int* active, int n, int k, int e) {
	card move_card[MAXCOLORS + 1];
	int index = 1, not_unique = 0;
	move_card[0] = PLAYERS[*active - 1].P_CARDS[0];
	for (int i = 1; PLAYERS[*active - 1].P_CARDS[i].value != 0; i++) {
		if (PLAYERS[*active - 1].P_CARDS[i].value < move_card[0].value) {
			move_card[0] = PLAYERS[*active - 1].P_CARDS[i];
		}
	}
	for (int i = 1; PLAYERS[*active - 1].P_CARDS[i].value != 0; i++) {
		for (int j = 0; j < k + 1; j++) {
			if (PLAYERS[*active - 1].P_CARDS[i].cardcolor == move_card[j].cardcolor) {
				not_unique++;
			}
		}
		if (PLAYERS[*active - 1].P_CARDS[i].value == move_card[0].value && not_unique == 0) {
			move_card[index] = PLAYERS[*active - 1].P_CARDS[i];
			index++;
		}
		not_unique = 0;
	}
	int SUMS[MAXCOLORS + 1] = {}, move_card_pile_i[MAXCOLORS + 1] = {};
	index = 0;
	int check = 0;
	for (int i = 0; move_card[i].value != 0 && i < k + 1; i++) {
		if (move_card[i].cardcolor != green) {
			for (int j = 0; j < k; j++) {
				if (PILES[j][0].cardcolor == move_card[i].cardcolor) {
					SUMS[index] = sum_in_pile_check(PILES, j, move_card[i].value);
					move_card_pile_i[index] = j;
					index++;
					check++;
				}
			}
			if (check == 0) {
				for (int j = 0; j < k; j++) {
					if (PILES[j][0].value == 0) {
						SUMS[index] = move_card[i].value;
						move_card_pile_i[index] = j;
						index++;
						check = 0;
						break;
					}
				}
			}
		}
		else {
			int lowest = sum_in_pile_check(PILES, 0, move_card[i].value);
			for (int j = 1; j < k; j++) {
				if (sum_in_pile_check(PILES, j, move_card[i].value) < lowest) {
					lowest = sum_in_pile_check(PILES, j, move_card[i].value);
					move_card_pile_i[index] = j;
				}
			}
			SUMS[index] = lowest;
			index++;
		}
	}
	card final_card;
	int final_i = 0;
	for (int i = 0; i < k; i++) {
		if (SUMS[i] > e) SUMS[i] = 0;
	}
	for (int i = 0; i < k; i++) {
		if (SUMS[i] != 0) {
			final_card = move_card[i];
			final_i = i;
			break;
		}
	}
	for (int i = 1; i < k; i++) {
		if (SUMS[i] < SUMS[final_i] && SUMS[i] != 0) {
			final_card = move_card[i];
			final_i = i;
		}
	}
	if (final_card.value != 0) direct_move(PLAYERS, PILES, final_card, move_card_pile_i[final_i], &*active, n, k, e);
	else(highest_value_move(PLAYERS, PILES, &*active, n, k, e));
}

void load_game(player PLAYERS[], card PILES[][MAXCARDS], int* active, int* n, int* k, int* e, int AMOUNT[MAXPLAYERS]) {
	int j = 0, counter = 0, x = 0, char_c = 0, COL_CHECK[6] = {};
	char a[10], b[10], c[10], d[10], LINE[999], color[10] = {};
	memset(color, 'NULL', sizeof(color));
	cin >> a >> b >> c >> *active;
	cin >> a >> b >> c >> *n;
	cin >> a >> b >> c >> *e;
	for (int i = 0; i < *n; i++) {
		cin >> a >> b >> c >> d;
		fgets(LINE, sizeof(LINE), stdin);
		while (LINE[j] != '\0') {
			if ((LINE[j] >= '1' && LINE[j] <= '9') && (LINE[j - 1] == ' ') && (LINE[j + 1] == ' ')) {
				counter++;
				PLAYERS[i].P_CARDS[x].value = LINE[j] - 48;
			}
			else if (LINE[j] >= '1' && LINE[j] <= '9' && LINE[j + 1] != ' ' && LINE[j - 1] == ' ') {
				counter++;
				PLAYERS[i].P_CARDS[x].value = (10 * (LINE[j] - 48)) + (LINE[j + 1] - 48);
				j++;
			}
			else if (LINE[j] != ' ') {
				while (LINE[j] >= 'a' && LINE[j] <= 'z') {
					color[char_c] = LINE[j];
					j++;
					char_c++;
				}
				if (color[0] == 'g') PLAYERS[i].P_CARDS[x].cardcolor = green;
				else if (color[0] == 'b' && color[2] == 'u') {
					PLAYERS[i].P_CARDS[x].cardcolor = blue;
					COL_CHECK[0] = 1;
				}
				else if (color[0] == 'r') {
					PLAYERS[i].P_CARDS[x].cardcolor = red;
					COL_CHECK[1] = 1;
				}
				else if (color[0] == 'v') {
					PLAYERS[i].P_CARDS[x].cardcolor = violet;
					COL_CHECK[2] = 1;
				}
				else if (color[0] == 'y') {
					PLAYERS[i].P_CARDS[x].cardcolor = yellow;
					COL_CHECK[3] = 1;
				}
				else if (color[0] == 'w') {
					PLAYERS[i].P_CARDS[x].cardcolor = white;
					COL_CHECK[4] = 1;
				}
				else if (color[0] == 'b' && color[2] == 'a') {
					PLAYERS[i].P_CARDS[x].cardcolor = black;
					COL_CHECK[5] = 1;
				}
				memset(color, 'NULL', sizeof(color));
				char_c = 0;
				x++;
			}
			j++;
		}
		AMOUNT[i] = counter;
		counter = 0;
		j = 0;
		x = 0;
		cin >> a >> b >> c >> d;
		memset(LINE, 'NULL', sizeof(LINE));
		fgets(LINE, sizeof(LINE), stdin);
		while (LINE[j] != '\0') {
			if ((LINE[j] >= '1' && LINE[j] <= '9') && (LINE[j - 1] == ' ') && (LINE[j + 1] == ' ')) {
				PLAYERS[i].F_CARDS[x].value = LINE[j] - 48;
			}
			else if (LINE[j] >= '1' && LINE[j] <= '9' && LINE[j + 1] != ' ' && LINE[j - 1] == ' ') {
				PLAYERS[i].F_CARDS[x].value = (10 * (LINE[j] - 48)) + (LINE[j + 1] - 48);
				j++;
			}
			else if (LINE[j] != ' ') {
				while (LINE[j] >= 'a' && LINE[j] <= 'z') {
					color[char_c] = LINE[j];
					j++;
					char_c++;
				}
				if (color[0] == 'g') PLAYERS[i].F_CARDS[x].cardcolor = green;
				else if (color[0] == 'b' && color[2] == 'u') {
					PLAYERS[i].F_CARDS[x].cardcolor = blue;
					COL_CHECK[0] = 1;
				}
				else if (color[0] == 'r') {
					PLAYERS[i].F_CARDS[x].cardcolor = red;
					COL_CHECK[1] = 1;
				}
				else if (color[0] == 'v') {
					PLAYERS[i].F_CARDS[x].cardcolor = violet;
					COL_CHECK[2] = 1;
				}
				else if (color[0] == 'y') {
					PLAYERS[i].F_CARDS[x].cardcolor = yellow;
					COL_CHECK[3] = 1;
				}
				else if (color[0] == 'w') {
					PLAYERS[i].F_CARDS[x].cardcolor = white;
					COL_CHECK[4] = 1;
				}
				else if (color[0] == 'b' && color[2] == 'a') {
					PLAYERS[i].F_CARDS[x].cardcolor = black;
					COL_CHECK[5] = 1;
				}
				memset(color, 'NULL', sizeof(color));
				char_c = 0;
				x++;
			}
			j++;
		}
		j = 0;
		x = 0;
	}

	int num;
	*k = 0;
	for (int i = 0; i < 6; i++) {
		*k += COL_CHECK[i];
	}
	for (int i = 0; i < *k; i++) {
		cin >> num >> a >> b;
		memset(LINE, 'NULL', sizeof(LINE));
		fgets(LINE, sizeof(LINE), stdin);
		while (LINE[j] != '\0') {
			if ((LINE[j] >= '1' && LINE[j] <= '9') && (LINE[j - 1] == ' ') && (LINE[j + 1] == ' ')) {
				PILES[i][x].value = LINE[j] - 48;
			}
			else if (LINE[j] >= '1' && LINE[j] <= '9' && LINE[j + 1] != ' ' && LINE[j - 1] == ' ') {
				PILES[i][x].value = (10 * (LINE[j] - 48)) + (LINE[j + 1] - 48);
				j++;
			}
			else if (LINE[j] != ' ') {
				while (LINE[j] >= 'a' && LINE[j] <= 'z') {
					color[char_c] = LINE[j];
					j++;
					char_c++;
				}
				if (color[0] == 'g') PILES[i][x].cardcolor = green;
				else if (color[0] == 'b' && color[2] == 'u') PILES[i][x].cardcolor = blue;
				else if (color[0] == 'r') PILES[i][x].cardcolor = red;
				else if (color[0] == 'v') PILES[i][x].cardcolor = violet;
				else if (color[0] == 'y') PILES[i][x].cardcolor = yellow;
				else if (color[0] == 'w') PILES[i][x].cardcolor = white;
				else if (color[0] == 'b' && color[2] == 'a') PILES[i][x].cardcolor = black;
				memset(color, 'NULL', sizeof(color));
				char_c = 0;
				x++;
			}
			j++;
		}
		j = 1;
		x = 0;
	}
}

void end_of_round(player PLAYERS[MAXPLAYERS], int POINTS[MAXPLAYERS], int n, int k) {
	int COL_AMOUNTS[MAXPLAYERS][MAXCOLORS] = {}, RESISTANCE[MAXCOLORS] = {}, HIGHEST[MAXCOLORS] = {};
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].F_CARDS[j].cardcolor != green) {
				COL_AMOUNTS[i][PLAYERS[i].F_CARDS[j].cardcolor - 1]++;
			}
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < n; j++) {
			if (COL_AMOUNTS[j][i] > HIGHEST[i]) {
				HIGHEST[i] = COL_AMOUNTS[j][i];
				RESISTANCE[i] = j;
			}
		}
		for (int j = 0; j < n; j++) {
			if (COL_AMOUNTS[j][i] == HIGHEST[i] && RESISTANCE[i] != j) {
				RESISTANCE[i] = -1;
				break;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			if (RESISTANCE[PLAYERS[i].F_CARDS[j].cardcolor - 1] != i || PLAYERS[i].F_CARDS[j].cardcolor == green) {
				if (PLAYERS[i].F_CARDS[j].cardcolor == green) POINTS[i] += 2;
				else POINTS[i]++;
			}
		}
	}
	if (RESISTANCE[0] != -1) cout << "Na kolor blue odporny jest gracz " << RESISTANCE[0] + 1 << endl;
	if (RESISTANCE[1] != -1) cout << "Na kolor red odporny jest gracz " << RESISTANCE[1] + 1 << endl;
	if (RESISTANCE[2] != -1) cout << "Na kolor violet odporny jest gracz " << RESISTANCE[2] + 1 << endl;
	if (RESISTANCE[3] != -1) cout << "Na kolor yellow odporny jest gracz " << RESISTANCE[3] + 1 << endl;
	if (RESISTANCE[4] != -1) cout << "Na kolor white odporny jest gracz " << RESISTANCE[4] + 1 << endl;
	if (RESISTANCE[5] != -1) cout << "Na kolor black odporny jest gracz " << RESISTANCE[5] + 1 << endl;
	for (int i = 0; i < n; i++) {
		cout << "Wynik gracza " << i + 1 << " = " << POINTS[i] << endl;
	}
}

void end_of_round_without_printing(player PLAYERS[MAXPLAYERS], int POINTS[MAXPLAYERS], int n, int k) {
	int COL_AMOUNTS[MAXPLAYERS][MAXCOLORS] = {}, RESISTANCE[MAXCOLORS] = {}, HIGHEST[MAXCOLORS] = {};
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			if (PLAYERS[i].F_CARDS[j].cardcolor != green) {
				COL_AMOUNTS[i][PLAYERS[i].F_CARDS[j].cardcolor - 1]++;
			}
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < n; j++) {
			if (COL_AMOUNTS[j][i] > HIGHEST[i]) {
				HIGHEST[i] = COL_AMOUNTS[j][i];
				RESISTANCE[i] = j;
			}
		}
		for (int j = 0; j < n; j++) {
			if (COL_AMOUNTS[j][i] == HIGHEST[i] && RESISTANCE[i] != j) {
				RESISTANCE[i] = -1;
				break;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			if (RESISTANCE[PLAYERS[i].F_CARDS[j].cardcolor - 1] != i || PLAYERS[i].F_CARDS[j].cardcolor == green) {
				if (PLAYERS[i].F_CARDS[j].cardcolor == green) POINTS[i] += 2;
				else POINTS[i]++;
			}
		}
	}
}

int end_round_check(player PLAYERS[MAXPLAYERS], int n) {
	int check = 0;
	for (int i = 0; i < n; i++) {
		if (PLAYERS[i].P_CARDS[0].value != 0) check++;
	}
	if (check == 0) return 1;
	else return 0;
}

void reset_game(player PLAYERS[], card PILES[][MAXCARDS], card CARDS[], int n, int k, int* all_cards) {
	*all_cards = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].P_CARDS[j].value != 0; j++) {
			CARDS[*all_cards] = PLAYERS[i].P_CARDS[j];
			PLAYERS[i].P_CARDS[j].value = 0;
			PLAYERS[i].P_CARDS[j].cardcolor = not_set;
			(*all_cards)++;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; PLAYERS[i].F_CARDS[j].value != 0; j++) {
			CARDS[*all_cards] = PLAYERS[i].F_CARDS[j];
			PLAYERS[i].F_CARDS[j].value = 0;
			PLAYERS[i].F_CARDS[j].cardcolor = not_set;
			(*all_cards)++;
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; PILES[i][j].value != 0; j++) {
			CARDS[*all_cards] = PILES[i][j];
			PILES[i][j].value = 0;
			PILES[i][j].cardcolor = not_set;
			(*all_cards)++;
		}
	}
}

int main() {

	int n, k, g, gv, o, e, active, VAL[MAXVALUES] = {}, all_cards = 0, option, gamemode, AMOUNT[MAXPLAYERS] = {}, POINTS[MAXPLAYERS] = {}, r;
	float win_a = 0, win_b = 0, draw = 0;
	card CARDS[MAXCARDS], PILES[MAXPLAYERS][MAXCARDS]; //table of all cards in game
	player PLAYERS[MAXPLAYERS]; // table of players


	cout << "POISON, THE GAME" << endl;
	cout << "----------------" << endl;
	cout << "MAIN MENU" << endl;
	cout << "---------" << endl;
	cout << "1.New game" << endl;
	cout << "2.Load game" << endl;
	cout << "3.Exit" << endl;
	cout << "----------------" << endl;
	cout << "Choose option: " << endl;
	cin >> option;

	switch (option) {
	case 1:
		cout << "Enter parameters of the game: (with spaces between them)" << endl;
		cout << "----------------" << endl;
		cout << "( players number / amount of colors without green / amount of posion cards / value of poison cards / amount of card for every color without green / explosion threshold)" << endl;
		cout << "----------------" << endl;
		cin >> n >> k >> g >> gv >> o >> e;
		all_cards = g + (k * o);
		active = 1;
		//getting all values of cards in game
		cout << "Enter all cards values for each color:" << endl;
		for (int i = 0; i < o; i++) {
			cin >> VAL[i];
		}

		generate_deck(CARDS, VAL, k, g, gv, o);
		deal_deck(CARDS, PLAYERS, all_cards, n);
		break;
	case 2:
		cout << "Enter game state:" << endl;
		load_game(PLAYERS, PILES, &active, &n, &k, &e, AMOUNT);
		while (check_green(PLAYERS, PILES, n, k) != 1 || check_cards_num(PLAYERS, PILES, n, k) != 1 || check_cards_val(PLAYERS, PILES, n, k) != 1 || state_check(PILES, AMOUNT, n, k, active, e) != 1) {
			cout << "The state of the game is incorrect. Try to load the game again" << endl;
			memset(AMOUNT, 0, sizeof(AMOUNT));
			all_cards = 0;
			reset_game(PLAYERS, PILES, CARDS, n, k, &all_cards);
			load_game(PLAYERS, PILES, &active, &n, &k, &e, AMOUNT);
		}
		cout << "Save has been loaded correcly" << endl;
		break;
	case 3:
		return 0;
		break;
	}
	cout << "----------" << endl;
	cout << "GAME MODES" << endl;
	cout << "1. Simple game with easy moves only" << endl;
	cout << "2. Game with lowest values moves approach for the 1st player" << endl;
	cout << "----------------" << endl;
	cout << "Choose gamemode: " << endl;
	cin >> gamemode;
	cout << "----------------" << endl;
	cout << "How many rounds do you want to play?" << endl;
	cin >> r;
	cout << "----------------" << endl;
	cout << "Current state of the game:" << endl;
	view_game(PLAYERS, PILES, active, n, k, e);
	cout << "----------------" << endl;
	switch (gamemode) {
	case 1:
		for (int i = 0; i < r; i++) {
			cout << "--------" << endl;
			cout << "ROUND #" << i + 1 << endl;
			cout << "--------" << endl;
			view_game(PLAYERS, PILES, active, n, k, e);
			cout << "----------------" << endl;
			Sleep(4000);
			while (end_round_check(PLAYERS, n) == 0) {
				easy_move(PLAYERS, PILES, &active, n, k, e);
				view_game(PLAYERS, PILES, active, n, k, e);
				cout << "----------------" << endl;
				Sleep(2000);
			}
			end_of_round(PLAYERS, POINTS, n, k);
			all_cards = 0;
			reset_game(PLAYERS, PILES, CARDS, n, k, &all_cards);
			deal_deck(CARDS, PLAYERS, all_cards, n);
			active = 1;
		}
		break;
	case 2:
		for (int i = 0; i < r; i++) {
			cout << "--------" << endl;
			cout << "ROUND #" << i + 1 << endl;
			cout << "--------" << endl;
			view_game(PLAYERS, PILES, active, n, k, e);
			cout << "----------------" << endl;
			Sleep(4000);
			while (end_round_check(PLAYERS, n) == 0) {
				if (active == 1) lowest_value_move(PLAYERS, PILES, &active, n, k, e);
				else easy_move(PLAYERS, PILES, &active, n, k, e);
				view_game(PLAYERS, PILES, active, n, k, e);
				cout << "----------------" << endl;
				Sleep(2000);
			}
			end_of_round(PLAYERS, POINTS, n, k);
			all_cards = 0;
			reset_game(PLAYERS, PILES, CARDS, n, k, &all_cards);
			deal_deck(CARDS, PLAYERS, all_cards, n);
			active = 1;
		}
		break;
	}

	int winner = POINTS[0], winner_i = 1;
	for (int i = 1; i < n; i++) {
		if (POINTS[i] < winner) {
			winner = POINTS[i];
			winner_i = i + 1;
		}
	}
	cout << "THE WINNER IS PLAYER #" << winner_i << " with " << winner << " points " << endl;
	cout << "-------------------------------------" << endl;

	cout << "TEST OF EFFECTIVENESS" << endl;
	cout << "Comparing two players which one of whom is chosing the lowest value (or the highest in some cases) and the second one is making only simple moves" << endl;
	srand(time(NULL));
	for (int i = 0; i < 400; i++) {
		for (int j = 0; j < MAXVALUES; j++) {
			VAL[j] = 0;
		}
		for (int j = 0; j < MAXCARDS; j++) {
			CARDS[j].cardcolor = not_set;
			CARDS[j].value = 0;
		}
		for (int j = 0; j < MAXPLAYERS; j++) {
			POINTS[j] = 0;
		}
		active = 1;
		n = 2;
		k = rand() % 6 + 1;
		g = rand() % 9 + 1;
		gv = rand() % 10 + 1;
		o = rand() % 20 + 1;
		e = rand() % 38 + 13;
		all_cards = g + (k * o);
		for (int j = 0; j < o; j++) {
			VAL[j] = rand() % 9 + 1;
		}
		generate_deck(CARDS, VAL, k, g, gv, o);
		deal_deck(CARDS, PLAYERS, all_cards, n);
		while (end_round_check(PLAYERS, n) == 0) {
			if (active == 1) lowest_value_move(PLAYERS, PILES, &active, n, k, e);
			else easy_move(PLAYERS, PILES, &active, n, k, e);
		}
		end_of_round_without_printing(PLAYERS, POINTS, n, k);
		if (POINTS[0] < POINTS[1]) win_a++;
		else if (POINTS[0] > POINTS[1]) win_b++;
		else draw++;
		reset_game(PLAYERS, PILES, CARDS, n, k, &all_cards);
	}
	cout << "Player 1 wins " << win_a / 4 << " % of the games" << endl;
	cout << "Player 2 wins " << win_b / 4 << " % of the games" << endl;
	cout << draw / 4 << " % of the games are drawed" << endl;

	return 0;
}