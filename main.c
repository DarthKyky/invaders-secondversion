#include "game.h"
#include "world.h"
#include <stdlib.h>


int main(int argc, char** argv){
    title();
    display_menu();
    start_world(init_game,game_event,free);
    return 0;
}