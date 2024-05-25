#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
    srand(time(NULL));

    int num_players;
    printf("Enter the number of players: ");
    scanf("%d", &num_players);

    char player_names[num_players][50];
    for (int i = 0; i < num_players; i++) {
        printf("Enter name for player %d: ", i + 1);
        scanf("%s", player_names[i]);
    }

    Game game;
    initialize_game(&game, num_players, player_names);

    for (int round = 1; round <= MAX_ROUNDS && game.num_targets > 0; round++) {
        play_round(&game, round);
    }

    display_winner(&game);
    free_game(&game);

    return 0;
}

