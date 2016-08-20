#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct card {
	char rank[15];
	char suit[15];
	int value;
};
typedef struct card Card;

struct deck {
	Card deck[52];
	int top;
};
typedef struct deck Deck;

struct player {
	char name[15];
	Card hand[12];
	int count;
	double cash;
	double bet;
};
typedef struct player Player;

const double MINIUMUM_BET = 10;
const double STARTING_CASH = 1000;

char * read_name() { 
	char name[15];
	printf("What is your name? ");
	gets_s(name, 15);
	return name;
}

void print_card(Card c, Player p, char * hand_type, int is_hold_card) {
	if (hand_type == "p")  {
		if (p.count == 0)
			printf("Your first card is the %s of %s\n", c.rank, c.suit);
		else if (p.count == 1)
			printf("Your second card is the %s of %s\n", c.rank, c.suit);
		else
			printf("Your new card is the %s of %s\n", c.rank, c.suit);
	}
	else {
		if (is_hold_card)
			printf("Dealer's hold card is the %s of %s\n", c.rank, c.suit);
		else 
			printf("Dealer showing the %s of %s\n", c.rank, c.suit);
	}
}

void place_bet(Player * p) {
printf("You have $%lf. How much would you like to bet? ", p->cash);
	scanf_s("%lf%*c", &p->bet);
	if (p->bet > p->cash)
		p->bet = p->cash;
	if (p->bet < MINIUMUM_BET)
		p->bet = MINIUMUM_BET;
}

int is_ace(Card c) {
	if (strcmp(c.rank, "Ace") == 0)
		return 1;
	else
		return 0;
}

int determine_ace_value(int hand) {
	if ((hand + 11) > 21)
		return 1;
	else 
		return 11;
}

int hand_value(Player p) {
	int i, value = 0;
	for (i = 0; i < p.count; i++)
		value += p.hand[i].value;
	return value;
}

void deal_card(Player * p, Deck *  d, int display_message) { 
	Card c;
	c = d->deck[d->top++];
	if (is_ace(c))
		c.value = determine_ace_value(hand_value(*p));
	p->hand[p->count] = c;
	if (display_message) {
		if (display_message == 1)
			print_card(c, *p, "p", 0);
		else
			print_card(c, *p, "d", 0);
	}
	p->count++;
}

void deal(Deck * game_deck, Player * p, Player * d) {
	deal_card(p, game_deck, 1);
	deal_card(d, game_deck, 0);
	deal_card(p, game_deck, 1);
	deal_card(d, game_deck, 2);
}

void determine_winner(int player, int dealer, Player * p) {
	if (player > 21) {
			printf("You've busted with %d\n", player);
			p->cash -= p->bet;
		}
		else if (player > dealer || dealer > 21) {
			printf("You have %d. Dealer has %d. You win!\n", player, dealer);
			p->cash += p->bet;
		}
		else {
			printf("You have %d. Dealer has %d. You lose.\n", player, dealer);
			p->cash -= p->bet;
		}
}

void hit(Player * p, Deck * deck) {
	char answer[4] = "yes";
	while (stricmp(answer, "yes") == 0 && hand_value(*p) < 22 || stricmp(answer, "y") == 0 && hand_value(*p) < 22) {
			printf("You have %d. Would you like a hit? ", hand_value(*p));
			gets_s(answer, 4);
			if (stricmp(answer, "yes") == 0 || stricmp(answer, "y") == 0)
				deal_card(p, deck, 1);
	}
}
	
void dealer_play(Player * d, Player p, Deck * deck) {
	if (is_busted(p))
		return;
	else {
	print_card(d->hand[0], *d, "d", 1);
	while (hand_value(*d) < 17) 
		deal_card(d, deck, 2);
	}
}

int is_busted(Player p) {
	if (hand_value(p) > 21)
		return 1;
	else 
		return 0;
}
			
void print_deck(Card deck[], int card_amount) {

	int i;
	for (i = 0; i < card_amount; i++)
		printf("The value of the %s of %s is %d\n", deck[i].rank, deck[i].suit, deck[i].value);
}

void end_of_game_message(Player p) {
	if (p.cash <= 0)
		printf("Sorry you're out of cash\n");
	else
		printf("Thank you for playing. You ended up with $%lf\n", p.cash);
}

void build_deck(Card deck[52]) {
	char ranks[13][6] = {
		"Ace", "King", "Queen", "Jack", "Ten", "Nine", "Eight", "Seven", "Six", "Five", "Four", "Three", "Two"
	};

char suits[4][9] = { "Clubs", "Diamonds", "Hearts", "Spades" };


int values[13] = { 11, 10, 10, 10, 10, 9, 8, 7, 6, 5, 4, 3, 2 };

	int i;
	for (i = 0; i < 52; i++) {
    	strcpy_s(deck[i].rank, 6, ranks[i % 13]);
		strcpy_s(deck[i].suit, 9, suits[i % 4]);
		deck[i].value = values[i % 13];
	}
}

void shuffle(Card deck[52]) {
	int i, random;
	Card temp;
	time_t t;
	srand((unsigned) (&t));
	for (i = 0; i < 52; i++) {
		random = rand() % 52;
		temp = deck[i];
		deck[i] = deck[random];
		deck[random] = temp;
	}
}
