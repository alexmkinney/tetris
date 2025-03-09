#ifndef TETRIS_H
#define TETRIS_H

#define BLOCK_SIZE 30
#define WIDTH 10
#define HEIGHT 20
#define SCREEN_WIDTH (WIDTH * BLOCK_SIZE)
#define SCREEN_HEIGHT (HEIGHT * BLOCK_SIZE)

extern int playfield[HEIGHT][WIDTH];

typedef struct {
    int x, y;
    int type;
    int rotation;
} Piece;

extern Piece current;

extern int last_type;

extern bool game_over;
extern bool quit;
extern int score;
extern int drop_interval;

#endif
