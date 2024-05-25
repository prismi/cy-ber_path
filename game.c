#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "SDL2/SDL.h"

void clear_grid(Game *game) {
    for (int i = 0; i < game->grid_size; i++) {
        for (int j = 0; j < game->grid_size; j++) {
            game->grid[i][j] = ' ';
        }
    }
}

void display_grid(Game *game) {
    int window_size = game->grid_size * CELL_SIZE;
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_size, window_size, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i <= game->grid_size; i++) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, window_size);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, window_size, i * CELL_SIZE);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < NUM_ROBOTS; i++) {
        Position pos = game->robots[i].position;
        SDL_Rect rect = {pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < NUM_TARGETS; i++) {
        Position pos = game->targets[i].position;
        SDL_Rect rect = {pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < NUM_WALLS; i++) {
        Position pos = game->walls[i].position;
        SDL_Rect rect = {pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void initialize_game(Game *game, int num_players, char player_names[][50]) {
    game->grid_size = rand() % 6 + 15;

    game->grid = (char **)malloc(game->grid_size * sizeof(char *));
    for (int i = 0; i < game->grid_size; i++) {
        game->grid[i] = (char *)malloc(game->grid_size * sizeof(char));
    }

    clear_grid(game);

    game->num_targets = NUM_TARGETS;
    game->num_players = num_players;

    for (int i = 0; i < num_players; i++) {
        game->players[i].score = 0;
        game->players[i].moves = 0;
        strncpy(game->players[i].name, player_names[i], 50);
    }

    for (int i = 0; i < NUM_ROBOTS; i++) {
        do {
            game->robots[i].position.x = rand() % game->grid_size;
            game->robots[i].position.y = rand() % game->grid_size;
        } while (game->grid[game->robots[i].position.x][game->robots[i].position.y] != ' ');
        game->grid[game->robots[i].position.x][game->robots[i].position.y] = 'R';
        printf("Robot %d initialized at position (%d, %d)\n", i, game->robots[i].position.x, game->robots[i].position.y);
    }

    for (int i = 0; i < NUM_TARGETS; i++) {
        do {
            game->targets[i].position.x = rand() % game->grid_size;
            game->targets[i].position.y = rand() % game->grid_size;
        } while (game->grid[game->targets[i].position.x][game->targets[i].position.y] != ' ');
        game->grid[game->targets[i].position.x][game->targets[i].position.y] = 'T';
        printf("Target %d initialized at position (%d, %d)\n", i, game->targets[i].position.x, game->targets[i].position.y);
    }

    for (int i = 0; i < NUM_WALLS; i++) {
        do {
            game->walls[i].position.x = rand() % game->grid_size;
            game->walls[i].position.y = rand() % game->grid_size;
        } while (game->grid[game->walls[i].position.x][game->walls[i].position.y] != ' ');
        game->grid[game->walls[i].position.x][game->walls[i].position.y] = 'W';
    }
}

void update_score_and_targets(Game* game, int player_index, Position* position) {
    for (int j = 0; j < NUM_TARGETS; j++) {
        if (game->targets[j].position.x == position->x && game->targets[j].position.y == position->y) {
            game->players[player_index].score++;
            game->num_targets--;
            game->targets[j].position.x = -1;
            game->targets[j].position.y = -1;
            game->grid[position->x][position->y] = ' ';
            break;
        }
    }
}

int compare_moves(const void *a, const void *b) {
    return ((Player*)a)->moves - ((Player*)b)->moves;
}

void move_robot(Game* game, int player_index, int robot_index, Position new_position) {
    game->grid[game->robots[robot_index].position.x][game->robots[robot_index].position.y] = ' ';
    game->grid[new_position.x][new_position.y] = 'R';
    game->robots[robot_index].position = new_position;
    update_score_and_targets(game, player_index, &new_position);
}

void get_direction(Position* position, char* direction) {
    if (strcmp(direction, "up") == 0) {
        position->y--;
    } else if (strcmp(direction, "down") == 0) {
        position->y++;
    } else if (strcmp(direction, "left") == 0) {
        position->x--;
    } else if (strcmp(direction, "right") == 0) {
        position->x++;
    }
}

void player_move(Game* game, int player_index, int robot_index, char* direction) {
    Position new_position = game->robots[robot_index].position;
    get_direction(&new_position, direction);
    if (new_position.x >= 0 && new_position.x < game->grid_size && new_position.y >= 0 && new_position.y < game->grid_size && game->grid[new_position.x][new_position.y] == ' ') {
        move_robot(game, player_index, robot_index, new_position);
    } else {
        printf("Invalid move. Try again.\n");
    }
}

void play_round(Game *game, int round) {
    printf("\nRound %d:\n", round);
    for (int i = 0; i < game->num_players; i++) {
        printf("\n%s's turn:\n", game->players[i].name);
        for (int j = 0; j < NUM_ROBOTS; j++) {
            printf("Move robot %d: ", j);
            char direction[10];
            scanf("%s", direction);
            player_move(game, i, j, direction);
            game->players[i].moves++;
            if (game->num_targets == 0) {
                printf("All targets collected!\n");
                return;
            }
            display_grid(game);
        }
    }
}

void display_winner(Game *game) {
    qsort(game->players, game->num_players, sizeof(Player), compare_moves);
    printf("\n\nGame Over!\n");
    printf("Final Scores:\n");
    for (int i = 0; i < game->num_players; i++) {
        printf("%s: %d points\n", game->players[i].name, game->players[i].score);
    }
    printf("\nWinner: %s\n", game->players[0].name);
}

void free_game(Game *game) {
    for (int i = 0; i < game->grid_size; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
}

