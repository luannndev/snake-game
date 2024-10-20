#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct Snake {
    int x[100], y[100];
    int length;
     int direction;
} Snake;

typedef struct Apple {
    int x, y;
} Apple;

void init_game(Snake* snake, Apple* apple) {
    snake->length = 1;
    snake->x[0] = WIDTH / 2;
    snake->y[0] = HEIGHT / 2;
    snake->direction = KEY_RIGHT;

    apple->x = rand() % WIDTH;
    apple->y = rand() % HEIGHT;
}

void draw(Snake* snake, Apple* apple) {
    clear();
    mvprintw(apple->y, apple->x, "A");

    for (int i = 0; i < snake->length; i++) {
        mvprintw(snake->y[i], snake->x[i], "O");
    }

    refresh();
}

void input(Snake* snake) {
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            if (snake->direction != KEY_DOWN) snake->direction = KEY_UP;
            break;
        case KEY_DOWN:
            if (snake->direction != KEY_UP) snake->direction = KEY_DOWN;
            break;
        case KEY_LEFT:
            if (snake->direction != KEY_RIGHT) snake->direction = KEY_LEFT;
            break;
        case KEY_RIGHT:
            if (snake->direction != KEY_LEFT) snake->direction = KEY_RIGHT;
            break;
    }
}

void logic(Snake* snake, Apple* apple) {
    int prev_x = snake->x[0];
    int prev_y = snake->y[0];
    int prev2_x, prev2_y;

    switch (snake->direction) {
        case KEY_UP:
            snake->y[0]--;
            break;
        case KEY_DOWN:
            snake->y[0]++;
            break;
        case KEY_LEFT:
            snake->x[0]--;
            break;
        case KEY_RIGHT:
            snake->x[0]++;
            break;
    }

    if (snake->x[0] == apple->x && snake->y[0] == apple->y) {
        snake->length++;
        apple->x = rand() % WIDTH;
        apple->y = rand() % HEIGHT;
    }

    for (int i = 1; i < snake->length; i++) {
        prev2_x = snake->x[i];
        prev2_y = snake->y[i];
        snake->x[i] = prev_x;
        snake->y[i] = prev_y;
        prev_x = prev2_x;
        prev_y = prev2_y;
    }
}

int main() {
    Snake snake;
    Apple apple;

    srand(time(NULL));
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    init_game(&snake, &apple);

    while (1) {
        draw(&snake, &apple);
        input(&snake);
        logic(&snake, &apple);
        usleep(100000);
    }

    endwin();
    return 0;
}
