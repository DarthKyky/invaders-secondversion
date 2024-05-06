
#include "world.h"
#define bullets 120
#define MAX_ENEMIES 42
#define ENEMY_SPEED 0.1 

struct game {
    int player_x;
    int player_y;
    
};

struct enemy {
    int x;
    int y;
   	bool exist;
};


int game_event(struct event* event,void* game);

void* init_game();

void title();

void display_menu();

void center_player(int term_height, int term_width);

void draw_player(int x, int y);

void player_move(int x, int y, struct event* event, struct game* game);

void move_bullets(int num_enemies);

void fire(int player_x, int player_y);

void create_enemies();

void move_enemies(struct game* state);

void draw_enemy(int x, int y);

void draw_enemy_2(int x, int y);

void draw_enemy_3(int x, int y);

void delay(unsigned int milliseconds);
