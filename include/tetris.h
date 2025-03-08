#ifndef TETRIS_H
#define TETRIS_H

#define BLOCK_SIZE 30
#define WIDTH 10
#define HEIGHT 20
#define SCREEN_WIDTH (WIDTH * BLOCK_SIZE)
#define SCREEN_HEIGHT (HEIGHT * BLOCK_SIZE)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern TTF_Font* font;
extern SDL_Color text_color;

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

#endif
