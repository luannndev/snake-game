#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 10
#define MAX_LENGTH 100
#define NUM_OBSTACLES 10

typedef struct Snake {
    int x[MAX_LENGTH], y[MAX_LENGTH];
    int length;
    int direction;
} Snake;

typedef struct Apple {
    int x, y;
} Apple;

typedef struct Obstacle {
    int x, y;
} Obstacle;

void init_game(Snake* snake, Apple* apple, Obstacle* obstacles, int* score) {
    snake->length = 1;
    snake->x[0] = WIDTH / 2;
    snake->y[0] = HEIGHT / 2;
    snake->direction = KEY_RIGHT;

    apple->x = rand() % WIDTH;
    apple->y = rand() % HEIGHT;

    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstacles[i].x = rand() % WIDTH;
        obstacles[i].y = rand() % HEIGHT;
    }

    *score = 0;
}

void draw(Snake* snake, Apple* apple, Obstacle* obstacles, int score, bool gamePaused) {
    clear();

    mvprintw(apple->y, apple->x, "A");

    for (int i = 0; i < snake->length; i++) {
        mvprintw(snake->y[i], snake->x[i], "O");
    }

    for (int i = 0; i < NUM_OBSTACLES; i++) {
        mvprintw(obstacles[i].y, obstacles[i].x, "X");
    }

    mvprintw(0, 0, "Score: %d", score);

    if (gamePaused) {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "Game Paused");
    }

    refresh();
}

void input(Snake* snake, bool* gamePaused) {
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
        case 'p':
            *gamePaused = !*gamePaused;
            break;
        case 'q':
            endwin();
            exit(0);
    }
}

int is_collision(Snake* snake, Obstacle* obstacles) {
    if (snake->x[0] < 0 || snake->x[0] >= WIDTH || snake->y[0] < 0 || snake->y[0] >= HEIGHT) {
        return 1;
    }

    for (int i = 1; i < snake->length; i++) {
        if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]) {
            return 1;
        }
    }

    for (int i = 0; i < NUM_OBSTACLES; i++) {
        if (snake->x[0] == obstacles[i].x && snake->y[0] == obstacles[i].y) {
            return 1;
        }
    }

    return 0;
}

void logic(Snake* snake, Apple* apple, Obstacle* obstacles, int* score) {
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

    if (is_collision(snake, obstacles)) {
        endwin();
        printf("Game Over! Score: %d\n", *score);
        exit(0);
    }

    if (snake->x[0] == apple->x && snake->y[0] == apple->y) {
        snake->length++;
        (*score)++;
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
    Obstacle obstacles[NUM_OBSTACLES];
    int score;
    bool gamePaused = false;

    srand(time(NULL));
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    init_game(&snake, &apple, obstacles, &score);

    while (1) {
        draw(&snake, &apple, obstacles, score, gamePaused);
        input(&snake, &gamePaused);
        if (!gamePaused) {
            logic(&snake, &apple, obstacles, &score);
            usleep(100000);
        }
    }

    endwin();
    return 0;
}