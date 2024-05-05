#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "game.h"
#include "world.h"

struct enemy enemies[MAX_ENEMIES];
int max_bullets = bullets;
int bullet_width = 1;
int bullet_height = 1;
int bullet_x[bullets]; // Масив для зберігання позицій X снарядів
int bullet_y[bullets]; // Масив для зберігання позицій Y снарядів
int num_bullets = 0; // Поточна кількість снарядів

void* init_game() {
    // Allocate memory for the state
    struct game* game = calloc(1, sizeof(struct game));
    int x;
    int y;
    //int hp = 3;
    getmaxyx(stdscr, y, x);
    game->player_x = x / 2;
    game->player_y = y - 2;
    
    return game;
}

int game_event(struct event* event, void* game) {
    struct game* state = game;
    display_menu();
    player_move(state->player_x, state->player_y, event, game);
    free(game);
    return 0;
}

void title(){
    initscr();
    noecho();
    mvprintw(3, 1, " ________  ________  ________  ________  _______           ___  ________   ___      ___ ________  ________  _______   ________  ________      ");
    mvprintw(4, 1, "|\\   ____\\|\\   __  \\|\\   __  \\|\\   ____\\|\\  ___ \\         |\\  \\|\\   ___  \\|\\  \\    /  /|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\|\\   ____\\     ");
    mvprintw(5, 1, "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\___|\\ \\   __/|        \\ \\  \\ \\  \\\\ \\  \\ \\  \\  /  / | \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\ \\  \\ \\  \\___|_    ");
    mvprintw(6, 1, " \\ \\_____  \\ \\   __  \\ \\   __  \\ \\  \\    \\ \\  \\_|/__       \\ \\  \\ \\  \\\\ \\  \\ \\  \\/  / / \\ \\   __  \\ \\  \\  \\ \\ \\  \\_|/_\\ \\   _  _\\ \\_____  \\   ");
    mvprintw(7, 1, "  \\|____|\\  \\ \\  \\_\\  \\ \\  \\ \\  \\ \\  \\____\\ \\  \\_|\\ \\       \\ \\  \\ \\  \\\\ \\  \\ \\    / /   \\ \\  \\ \\  \\ \\  \\__\\ \\ \\  \\_|\\ \\ \\  \\ \\  \\|____|\\  \\  ");
    mvprintw(8, 1, "    ____\\_\\  \\ \\__\\ \\__\\ \\__\\ \\__\\ \\_______\\ \\_______\\       \\ \\__\\ \\__\\\\ \\__\\ \\__/ /     \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ ___\\_\\  \\ ");
    mvprintw(9, 1, "   |\\_________\\|__|\\|__|\\|__|\\|__|\\|_______|\\|_______|        \\|__|\\|__| \\|__|\\|__|/       \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|\\_________\\");
    mvprintw(10, 1, "   \\|_________|                                                                                                                   \\|_________|");
    mvprintw(LINES-1, COLS/2 - 14, "Press any key to continue...");
    getch();
    erase();
    system("clear");
    refresh();
    //endwin();
}
    
void display_menu() {
    int height;
    int width;
    getmaxyx(stdscr, height, width);
    int rule_width = 80;
    int menu_height = 7;
    int menu_width = 40;
    int rule_x = (width - rule_width) / 2;
    int start_y = (height - menu_height) / 2;
    int start_x = (width - menu_width) / 2;
    WINDOW *rule_win = newwin(menu_height, rule_width, start_y, rule_x);
    WINDOW *menu_win = newwin(menu_height, menu_width, start_y, start_x);
    box(menu_win, 0, 0);

    keypad(menu_win, true);
    refresh();
    wrefresh(menu_win);

    //int choice = 0;
    int highlight = 1;
    int c;
    bool selected = false;        
    int first = 0;
    while (!selected) {
        mvprintw(3, 1, " ________  ________  ________  ________  _______           ___  ________   ___      ___ ________  ________  _______   ________  ________      ");
        mvprintw(4, 1, "|\\   ____\\|\\   __  \\|\\   __  \\|\\   ____\\|\\  ___ \\         |\\  \\|\\   ___  \\|\\  \\    /  /|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\|\\   ____\\     ");
        mvprintw(5, 1, "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\___|\\ \\   __/|        \\ \\  \\ \\  \\\\ \\  \\ \\  \\  /  / | \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\ \\  \\ \\  \\___|_    ");
        mvprintw(6, 1, " \\ \\_____  \\ \\   __  \\ \\   __  \\ \\  \\    \\ \\  \\_|/__       \\ \\  \\ \\  \\\\ \\  \\ \\  \\/  / / \\ \\   __  \\ \\  \\  \\ \\ \\  \\_|/_\\ \\   _  _\\ \\_____  \\   ");
        mvprintw(7, 1, "  \\|____|\\  \\ \\  \\_\\  \\ \\  \\ \\  \\ \\  \\____\\ \\  \\_|\\ \\       \\ \\  \\ \\  \\\\ \\  \\ \\    / /   \\ \\  \\ \\  \\ \\  \\__\\ \\ \\  \\_|\\ \\ \\  \\ \\  \\|____|\\  \\  ");
        mvprintw(8, 1, "    ____\\_\\  \\ \\__\\ \\__\\ \\__\\ \\__\\ \\_______\\ \\_______\\       \\ \\__\\ \\__\\\\ \\__\\ \\__/ /     \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ ___\\_\\  \\ ");
        mvprintw(9, 1, "   |\\_________\\|__|\\|__|\\|__|\\|__|\\|_______|\\|_______|        \\|__|\\|__| \\|__|\\|__|/       \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|\\_________\\");
        mvprintw(10, 1, "   \\|_________|                                                                                                                   \\|_________|");
        
        int x = start_x+1;
        int y = start_y;

        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(4, COLOR_BLACK, COLOR_GREEN);

        attron(COLOR_PAIR(2));
        mvprintw(y, x, "Main Menu");
        mvprintw(y+1, x+2, "Choose variant:");
        attroff(COLOR_PAIR(2));

        if (highlight == 1) {
            attron(COLOR_PAIR(1));
            mvprintw(y + 3, x+17, "Play");
            attroff(COLOR_PAIR(1));
        } 
        else{
            mvprintw(y + 3, x+16, "Play");
        }

        if (highlight == 2) {
            attron(COLOR_PAIR(1));
            mvprintw(y + 4, x+17, "Controls");
            attroff(COLOR_PAIR(1));
        } 
        else{
            mvprintw(y + 4, x+16, "Controls");
        }

        if (highlight == 3) {
            attron(COLOR_PAIR(1));
            mvprintw(y + 5, x+17, "Rules");
            attroff(COLOR_PAIR(1));
        } 
        else{
            mvprintw(y + 5, x+16, "Rules");
        }
        if(first == 0){
            mvprintw(LINES - 10, COLS/2-8, "Press any key...");
            first++;
        }
        refresh();
        c = wgetch(menu_win);
        refresh();
        mvprintw(LINES - 10, COLS/2-8, "                                    ");
        refresh();
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = 3;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == 3)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // ENTER key
                if (highlight == 1) {
                    selected = true;
                    clear();
                    refresh();
                    refresh();
                    clear_screen();
                    refresh();
                    break;
                }
                if(highlight == 2){
                    clear();
                    box(menu_win, 0, 0);
                    refresh();
                    mvprintw(3, 1, " ________  ________  ________  ________  _______           ___  ________   ___      ___ ________  ________  _______   ________  ________      ");
                    mvprintw(4, 1, "|\\   ____\\|\\   __  \\|\\   __  \\|\\   ____\\|\\  ___ \\         |\\  \\|\\   ___  \\|\\  \\    /  /|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\|\\   ____\\     ");
                    mvprintw(5, 1, "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\___|\\ \\   __/|        \\ \\  \\ \\  \\\\ \\  \\ \\  \\  /  / | \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\ \\  \\ \\  \\___|_    ");
                    mvprintw(6, 1, " \\ \\_____  \\ \\   __  \\ \\   __  \\ \\  \\    \\ \\  \\_|/__       \\ \\  \\ \\  \\\\ \\  \\ \\  \\/  / / \\ \\   __  \\ \\  \\  \\ \\ \\  \\_|/_\\ \\   _  _\\ \\_____  \\   ");
                    mvprintw(7, 1, "  \\|____|\\  \\ \\  \\_\\  \\ \\  \\ \\  \\ \\  \\____\\ \\  \\_|\\ \\       \\ \\  \\ \\  \\\\ \\  \\ \\    / /   \\ \\  \\ \\  \\ \\  \\__\\ \\ \\  \\_|\\ \\ \\  \\ \\  \\|____|\\  \\  ");
                    mvprintw(8, 1, "    ____\\_\\  \\ \\__\\ \\__\\ \\__\\ \\__\\ \\_______\\ \\_______\\       \\ \\__\\ \\__\\\\ \\__\\ \\__/ /     \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ ___\\_\\  \\ ");
                    mvprintw(9, 1, "   |\\_________\\|__|\\|__|\\|__|\\|__|\\|_______|\\|_______|        \\|__|\\|__| \\|__|\\|__|/       \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|\\_________\\");
                    mvprintw(10, 1, "   \\|_________|                                                                                                                   \\|_________|");
                    box(menu_win, 0, 0);
                    mvprintw(y, COLS/2-5, "Controls:");
                    mvprintw(y+2, COLS/2 - 15, "Left arrow - player goes left");
                    mvprintw(y+3, COLS/2 - 16, "Right arrow - player goes right");
                    mvprintw(y+4, COLS/2 - 8, "Arrow up - fire");
                    mvprintw(y+5, COLS/2 - 15, "Q - Instant exit from the game");
                    wrefresh(menu_win);
                    refresh();
                    getch();
                    clear_screen();
                    break;
                }
                if(highlight == 3){
                                        clear();
                    box(menu_win, 0, 0);
                    refresh();
                    mvprintw(3, 1, " ________  ________  ________  ________  _______           ___  ________   ___      ___ ________  ________  _______   ________  ________      ");
                    mvprintw(4, 1, "|\\   ____\\|\\   __  \\|\\   __  \\|\\   ____\\|\\  ___ \\         |\\  \\|\\   ___  \\|\\  \\    /  /|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\|\\   ____\\     ");
                    mvprintw(5, 1, "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\___|\\ \\   __/|        \\ \\  \\ \\  \\\\ \\  \\ \\  \\  /  / | \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\ \\  \\ \\  \\___|_    ");
                    mvprintw(6, 1, " \\ \\_____  \\ \\   __  \\ \\   __  \\ \\  \\    \\ \\  \\_|/__       \\ \\  \\ \\  \\\\ \\  \\ \\  \\/  / / \\ \\   __  \\ \\  \\  \\ \\ \\  \\_|/_\\ \\   _  _\\ \\_____  \\   ");
                    mvprintw(7, 1, "  \\|____|\\  \\ \\  \\_\\  \\ \\  \\ \\  \\ \\  \\____\\ \\  \\_|\\ \\       \\ \\  \\ \\  \\\\ \\  \\ \\    / /   \\ \\  \\ \\  \\ \\  \\__\\ \\ \\  \\_|\\ \\ \\  \\ \\  \\|____|\\  \\  ");
                    mvprintw(8, 1, "    ____\\_\\  \\ \\__\\ \\__\\ \\__\\ \\__\\ \\_______\\ \\_______\\       \\ \\__\\ \\__\\\\ \\__\\ \\__/ /     \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ ___\\_\\  \\ ");
                    mvprintw(9, 1, "   |\\_________\\|__|\\|__|\\|__|\\|__|\\|_______|\\|_______|        \\|__|\\|__| \\|__|\\|__|/       \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|\\_________\\");
                    mvprintw(10, 1, "   \\|_________|                                                                                                                   \\|_________|");
                    box(rule_win, 0, 0);
                    mvprintw(y, COLS/2-3, "RULES:");
                    mvprintw(y+2, COLS/2 - 39, "The task is to destroy 42 enemies, for which you have 120 rounds of ammunition.");
                    mvprintw(y+3, COLS/2 - 17, "If you run out of ammo, you lose.");
                    mvprintw(y+4, COLS/2 - 31, "If the enemies are at the same height as the player, you lose.");
                    mvprintw(y+5, COLS/2 - 4, "Q - quit");
                    wrefresh(rule_win);
                    refresh();
                    getch();
                    clear_screen();
                    break;
                }
                break;
            default:
            break;  
        }
        if(selected){
            break;
        }
        wclear(menu_win);
        box(menu_win, 0, 0);
        wrefresh(menu_win);
    }
}

void draw_player(int x, int y) {
    set_color_cell('I', x, y, COLOR_BLUE,COLOR_BLACK);
    set_color_cell('/', x-1, y, COLOR_BLUE,COLOR_BLACK);
    set_color_cell('\\', x+1, y, COLOR_BLUE,COLOR_BLACK);
    set_color_cell('/', x-2, y+1, COLOR_BLUE,COLOR_BLACK);
    set_color_cell('_', x-1, y+1, COLOR_BLUE,COLOR_BLACK);
    set_color_cell('|', x, y+1, COLOR_BLUE,COLOR_BLACK);
    set_color_cell('_', x+1, y+1, COLOR_BLUE,COLOR_BLACK);
    set_color_cell('\\', x+2, y+1, COLOR_BLUE,COLOR_BLACK);
    mvprintw(0, 0,"Bullets:%d", max_bullets);
}

void player_move(int x, int y, struct event* event, struct game* state) {
    noecho();
    draw_player(x, y);
    nodelay(stdscr, TRUE);
    int input;
    create_enemies();
    bool shoot = false;
    while (1) {
        erase();
        int num_enemies = MAX_ENEMIES;
        move_bullets(num_enemies);
        draw_player(x, y);
        move_enemies(state);
        refresh();
        input = getch(); // Get user input
        if(max_bullets == 0){
            mvprintw(LINES/3, COLS/2-25, "__   _______ _   _   _     _____ _____ _____ ");
            mvprintw(LINES/3+1, COLS/2-25, "\\ \\ / /  _  | | | | | |   |  _  /  ___|  ___| ");
            mvprintw(LINES/3+2, COLS/2-25, " \\ V /| | | | | | | | |   | | | \\ `--.| |__  ");
            mvprintw(LINES/3+3, COLS/2-25, "  \\ / | | | | | | | | |   | | | |`--. \\  __| ");
            mvprintw(LINES/3+4, COLS/2-25, "  | | \\ \\_/ / |_| | | |___\\ \\_/ /\\__/ / |___ ");
            mvprintw(LINES/3+5, COLS/2-25, "  \\_/  \\___/ \\___/  \\_____/\\___/\\____/\\____/ ");
            mvprintw(LINES/3+7, COLS/2-23, "You have no ammo left... Press Q to quit");
            int end;
            while ((end = getch()) != 'Q' && end != 'q') {
            }
            clear_screen();
            endwin();
            exit(0);
        }
        if (input != ERR) { // Check if input is available
            if (input == KEY_LEFT) {
                x--;
                refresh();
            } 
            else if (input == KEY_RIGHT) {
                x++;
                refresh();
            } 
            else if (input == KEY_UP && !shoot) {
                fire(x, y);
                move_bullets(num_enemies);
                refresh();
                shoot = true;
            }
             else if (input == 'q' || input == 'Q') {
                mvprintw(LINES/3, COLS/2-25, "______  ___  _____  _____   _____ _   _ _____ _____");
                mvprintw(LINES/3+1, COLS/2-25, "| ___ \\/ _ \\|  __ \\|  ___| |  _  | | | |_   _|_   _|");
                mvprintw(LINES/3+2, COLS/2-25, "| |_/ / /_\\ \\ |  \\/| |__   | | | | | | | | |   | |");
                mvprintw(LINES/3+3, COLS/2-25, "|    /|  _  | |  _ | |__|  | | | | | | | | |   | |");
                mvprintw(LINES/3+4, COLS/2-25, "| |\\ \\| | | | |_| \\| |___  \\ \\/  / |_| |_| |_  | |  ");
                mvprintw(LINES/3+5, COLS/2-25, "\\_| \\_\\_| |_/\\____/\\____/   \\_/\\_\\\\___/ \\___/  \\_/");
                mvprintw(LINES/3+7, COLS/2-14, "Press Q, ro confirm RAGE QUIT");
                int end;
                while ((end = getch()) != 'Q' && end != 'q') {


                }
                clear_screen();
                endwin();
                exit(0);
            }
        }
        else{
            shoot = false;
        }

        move_bullets(num_enemies);
        delay(30);
    }
}

void fire(int player_x, int player_y) {
    // Додати новий снаряд до списку
    bullet_x[num_bullets] = player_x; // Початкова позиція снаряду поруч з гравцем
    bullet_y[num_bullets] = player_y-1; // Змінено на -1, якщо снаряд має летіти вгору
    num_bullets++;
    max_bullets--;  
}

void move_bullets(int num_enemies) {
    for (int i = 0; i < num_bullets; i++) {
        bullet_y[i]--; // Рухаємо кожен снаряд вгору
        init_pair(8, COLOR_RED, COLOR_BLACK);
        init_pair(9, COLOR_YELLOW, COLOR_BLACK);
        // Перевіряємо зіткнення кожного снаряду з кожним ворогом
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (enemies[j].exist && bullet_x[i] >= enemies[j].x && bullet_x[i] <= enemies[j].x + 2 &&
                bullet_y[i] >= enemies[j].y && bullet_y[i] <= enemies[j].y + 2) {
                //Якщо снаряд потрапив у ворога, видаляємо ворога зі списку
                attron(COLOR_PAIR(8));
                mvprintw(enemies[j].y, enemies[j].x, "\\|/");
                attron(COLOR_PAIR(9));
                mvprintw(enemies[j].y + 1, enemies[j].x, "=+=");
                attroff(COLOR_PAIR(9));
                attron(COLOR_PAIR(8));
                mvprintw(enemies[j].y + 2, enemies[j].x, "/|\\");
                attroff(COLOR_PAIR(8));
                delay(50);
                refresh();
                enemies[j].exist = false;
                // Видаляємо кулю, яка потрапила у ворога
                bullet_x[i] = -1;
                bullet_y[i] = -1;
                break;
            }
        }
        // Якщо снаряд вийшов за межі екрану, видаляємо його
        if (bullet_y[i] <= 0) {
            bullet_x[i] = -1;
            bullet_y[i] = -1;
        }
        // Відображаємо снаряд на екрані
        if (bullet_y[i] > 0) {
            init_pair(1, COLOR_GREEN, COLOR_BLACK);
            attron(COLOR_PAIR(1));
            mvprintw(bullet_y[i], bullet_x[i], "^");
            attroff(COLOR_PAIR(1));
            refresh();
        }
    }
}

void create_enemies() {
    int start_x = 5; // Початкова позиція x першого ворога
    int start_y = 5; // Початкова позиція y першого ряду ворогів
    int row_gap = 4; // Відстань між рядами ворогів
    int col_gap = 8; // Відстань між ворогами в ряду
    int enemy_x = start_x;
    int enemy_y = start_y;
    int num_enemies = 0;
    for (int row = 0; row < 3; row++) { // Три ряди ворогів
        for (int col = 0; col < MAX_ENEMIES / 3; col++) { // В кожному ряду max_enemies / 3 ворогів
            // Додати дані про ворога в масив
            enemies[num_enemies].x = enemy_x;
            enemies[num_enemies].y = enemy_y;
            enemies[num_enemies].exist = true;
            // Відмальовування ворога
            if(row == 0){
                draw_enemy(enemy_x, enemy_y);
            }
            else if(row == 1){
                draw_enemy_2(enemy_x, enemy_y);
            }
            else if(row == 2){
                draw_enemy_3(enemy_x, enemy_y);
            }
            // Оновлення позиції ворога
            enemy_x += col_gap;
            num_enemies++;
        }
        // Оновлення позиції ворога для наступного ряду
        enemy_x = start_x;
        enemy_y += row_gap;
    }
    refresh(); // Оновити екран
}

void move_enemies(struct game* state) {
    static int direction = 1; // Напрямок руху ворогів: 1 - вправо, -1 - вліво
    int live_enemies = 0; // Змінна для відслідковування кількості живих ворогів
    // Зміщуємо координати X кожного ворога на значення, визначене напрямком руху
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x += direction;
    }
    // Перевіряємо, чи вороги досягли межі екрану
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if ((enemies[i].x >= COLS - 4 || enemies[i].x <= 0) && enemies[i].exist) {
            // Якщо так, змінюємо напрямок руху на протилежний
            direction *= -1;
            // Зміщуємо всіх ворогів вниз, щоб вони не накладалися один на одного
            for (int j = 0; j < MAX_ENEMIES; j++) {
                enemies[j].y += 2;
            }
            // Після зміни напрямку руху ворогів, вихід із циклу
            break;
        }
    }
    // Оновлюємо позиції ворогів на екрані та підраховуємо кількість живих ворогів
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].exist) {
            if (i >= 0 && i < 14) {
                draw_enemy(enemies[i].x, enemies[i].y);
            }
            else if (i >= 14 && i < 28) {
                draw_enemy_2(enemies[i].x, enemies[i].y);
            } 
            else if (i >= 28 && i < MAX_ENEMIES) {
                draw_enemy_3(enemies[i].x, enemies[i].y);
            }
            live_enemies++; // Збільшуємо лічильник живих ворогів
        } 
        else{
            // Якщо ворог був знищений, очистимо його позицію на екрані
            mvprintw(enemies[i].y, enemies[i].x, "   ");
            mvprintw(enemies[i].y + 1, enemies[i].x, "   ");
            mvprintw(enemies[i].y + 2, enemies[i].x, "   ");
        }
    }

    // Перевіряємо, чи всі вороги мертві
    if (live_enemies == 0) {
        int end;
        mvprintw(LINES/3, COLS/2-25, "__   _______ _   _   _    _  _____ _   _ ");
        mvprintw(LINES/3+1, COLS/2-25, "\\ \\ / /  _  | | | | | |  | ||  _  | \\ | |");
        mvprintw(LINES/3+2, COLS/2-25, " \\ V /| | | | | | | | |  | || | | |  \\| |");
        mvprintw(LINES/3+3, COLS/2-25, "  \\ / | | | | | | | | |/\\| || | | | . ` |");
        mvprintw(LINES/3+4, COLS/2-25, "  | | \\ \\_/ / |_| | \\  /\\  /\\ \\_/ / |\\  |");
        mvprintw(LINES/3+5, COLS/2-25, "  \\_/  \\___/ \\___/   \\/  \\/  \\___/\\_| \\_/");
        mvprintw(LINES / 2, COLS / 2 - 25, "Congratulations! You've defeated all Aliens!");
        mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Press Q to quit");
        refresh();
        while ((end = getch()) != 'Q' && end != 'q') {
        }
        clear_screen();
        endwin();
        exit(0);
    }
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if(enemies[i].y == state->player_y - 4){
            mvprintw(LINES/3, COLS/2-25, "__   _______ _   _   _     _____ _____ _____ ");
            mvprintw(LINES/3+1, COLS/2-25, "\\ \\ / /  _ \\| | | | | |   |  _  /  ___|  ___| ");
            mvprintw(LINES/3+2, COLS/2-25, " \\ V /| | | | | | | | |   | | | \\ `--.| |  ");
            mvprintw(LINES/3+3, COLS/2-25, "  \\ / | | | | | | | | |   | | | |`--. \\  | ");
            mvprintw(LINES/3+4, COLS/2-25, "  | | \\ \\_/ / |_| | | |___\\ \\_/ /\\__/ / |___ ");
            mvprintw(LINES/3+5, COLS/2-25, "  \\_/  \\___/ \\___/  \\_____/\\___/\\____/\\____/ ");
            mvprintw(LINES/2, COLS/2-23, "You were caught by the Aliens! Game Over!");
            mvprintw(LINES/2+1, COLS/2-10, "Press Q to quit");
            int end;
            while ((end = getch()) != 'Q' && end != 'q') {
            }
            clear_screen();
            endwin();
            exit(0);    
        }
    }
    refresh();
}

void draw_enemy(int x, int y){
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    mvprintw(y, x, "/-\\");
    mvprintw(y + 1, x, "|0|");
    mvprintw(y + 2, x, "\\-/");
    attroff(COLOR_PAIR(4));
    refresh();
}

void draw_enemy_2(int x, int y){
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(5));
    mvprintw(y, x, "(_)");
    mvprintw(y + 1, x, "(=)");
    mvprintw(y + 2, x, "\\_/ ");
    attroff(COLOR_PAIR(5));
}

void draw_enemy_3(int x, int y){
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(6));
    mvprintw(y, x, "\\^/");
    mvprintw(y + 1, x, "}*{");
    mvprintw(y + 2, x, "{&}");
    attroff(COLOR_PAIR(6));
}

void delay(unsigned int milliseconds) {
    unsigned int ms = milliseconds * 1000; // Перетворення мілісекунд у мікросекунди
    clock_t start_time = clock();

    while (clock() < start_time + ms) {
        // Порожній цикл для затримки
    }
}

