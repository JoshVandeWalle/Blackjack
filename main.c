#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blackjack.h"

int main() {
	Deck game_deck;
	Player p, d;
	char answer[4] = "yes";
	p.cash = STARTING_CASH;
	build_deck(game_deck.deck);
	while (stricmp(answer, "yes") == 0 && p.cash >= MINIUMUM_BET || stricmp(answer, "y") == 0 && p.cash >= MINIUMUM_BET) {
		place_bet(&p);
		shuffle(game_deck.deck);
		p.count = d.count = game_deck.top = 0;
		deal(&game_deck, &p, &d);
		hit(&p, &game_deck);
		dealer_play(&d, p, &game_deck);
		determine_winner(hand_value(p), hand_value(d), &p);
		printf("\nWould you like to continue playing? ");
		gets_s(answer, 4);
		system("cls");
	}
	end_of_game_message(p);
	system("pause");
	return 0;
}
