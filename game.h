#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define CELL_SIZE 20
#define MAX_WINDOW_SIZE 400

#define NUM_ROBOTS 4
#define NUM_TARGETS 18
#define MAX_ROUNDS 10
#define NUM_WALLS 40

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position position;
    int score;
    int moves;
    char name[50];
} Player;

typedef struct {
    Position position;
    char display_char;
} Robot;

typedef struct {
    Position position;
    char display_char;
} Target;

typedef struct {
    Position position;
} Wall;

typedef struct {
    Player players[NUM_ROBOTS];
    Robot robots[NUM_ROBOTS];
    Target targets[NUM_TARGETS];
    Wall walls[NUM_WALLS];
    char **grid;
    int grid_size;
    int num_targets;
    int num_players;
} Game;

void clear_grid(Game *game);
void display_grid(Game *game);
void initialize_game(Game *game, int num_players, char player_names[][50]);
void update_score_and_targets(Game* game, int player_index, Position* position);
void move_robot(Game* game, int player_index, int robot_index, Position new_position);
void get_direction(Position* position, char* direction);
void player_move(Game* game, int player_index, int robot_index, char* direction);
void play_round(Game *game, int round);
void display_winner(Game *game);
void free_game(Game *game);

#endif

