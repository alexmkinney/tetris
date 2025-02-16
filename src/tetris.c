#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/font_data.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "../include/tetromino.h"

#define BLOCK_SIZE 30
#define WIDTH 10
#define HEIGHT 20
#define SCREEN_WIDTH (WIDTH * BLOCK_SIZE)
#define SCREEN_HEIGHT (HEIGHT * BLOCK_SIZE)

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static TTF_Font* font = NULL;
static SDL_Color text_color = {255, 255, 255, 255};

static int playfield[HEIGHT][WIDTH] = {0};

typedef struct {
    int x, y;
    int type;
    int rotation;
} Piece;

Piece current;

bool quit = false;
int score = 0;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return false;
    }

    SDL_RWops* font_rw = SDL_RWFromConstMem(minimal_ttf, minimal_ttf_len);
    font = TTF_OpenFontRW(font_rw, 1, 24);  // '1' = auto-close RWops
    if (!font) {
        printf("Failed to load embedded font: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void draw_block(int x, int y, SDL_Color color) {
    SDL_Rect rect = {
        x * BLOCK_SIZE,
        y * BLOCK_SIZE,
        BLOCK_SIZE - 1,
        BLOCK_SIZE - 1
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void render_text(const char* text, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = surface->w,
        .h = surface->h
    };
    
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw playfield
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (playfield[y][x]) {
                draw_block(x, y, colors[playfield[y][x] - 1]);
            }
        }
    }

    // Draw current piece
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[current.type][current.rotation][y][x]) {
                int px = current.x + x;
                int py = current.y + y;
                if (py >= 0) {
                    draw_block(px, py, colors[current.type]);
                }
            }
        }
    }

    // Render score
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);
    render_text(score_text, 10, 10);  // Position at top-left

    SDL_RenderPresent(renderer);
}

// Returns true if collision is detected and false if the position is valid
bool check_collision(Piece p) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[p.type][p.rotation][y][x]) { // Check if the cell in the current shape's grid is 1 (skips if cell is 0)
                // Absolute x and y values for each cell of the tetromino on the grid
                int px = p.x + x;
                int py = p.y + y;

                // Check boundaries
                if (px < 0 || px >= WIDTH || py >= HEIGHT) return true;
                // Check playfield collisions
                if (py >= 0 && playfield[py][px]) return true;
            }
        }
    }

    return false;
}

// Called upon collision with another piece, "locks in" piece into playfield
void merge_piece() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[current.type][current.rotation][y][x]) { // See check_collision()
                int px = current.x + x;
                int py = current.y + y;

                if (py >= 0) {
                    // Mark the playfield cell with piece type, preserves color
                    playfield[py][px] = current.type + 1; // Offset by 1 because "0" represents empty space
                }
            }
        }
    }
}

// Clears full lines, returns number of lines cleared
int clear_lines() {
    int lines_cleared = 0;

    for (int y = HEIGHT - 1; y >= 0; y--) { // Check each line from bottom up
        bool full = true;

        for (int x = 0; x < WIDTH; x++) {
            if (!playfield[y][x]) { // Check for any holes in the line
                full = false;
                break;
            }
        }

        if (full) {
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < WIDTH; x++) {
                    playfield[ny][x] = playfield[ny - 1][x]; // Shift all lines down
                }
            }

            for (int x = 0; x < WIDTH; x++) playfield[0][x] = 0; // Clear top row
            lines_cleared++;
            y++; // Since lines shifted, increment to the next line
        }
    }

    // Scoring
    const int points[] = {0, 100, 300, 500, 800};  // Single, Double, Triple, Tetris
    score += points[lines_cleared];

    return lines_cleared;
}

void spawn_piece() {
    current.type = rand() % 7;
    current.rotation = 0;
    current.x = WIDTH / 2 - 2; // Middle of playfield
    current.y = 0;

    if (check_collision(current)) {
        printf("Game Over!\nScore: %d\n", score);
        quit = true;
    }
}

int main(int argc, char* argv[]) {
    if (!init()) return 1;

    srand(time(0));
    spawn_piece();

    Uint64 last_update = SDL_GetTicks64();

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;

            if (e.type == SDL_KEYDOWN) {
                Piece tmp = current;
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:  tmp.x--; break;
                    case SDLK_RIGHT: tmp.x++; break;
                    case SDLK_DOWN:  tmp.y++; break;
                    case SDLK_UP:    tmp.rotation = (tmp.rotation + 1) % 4; break;
                    case SDLK_SPACE:
                        while (!check_collision(tmp)) {
                            current = tmp;
                            tmp.y++;
                        }
                        break;
                    case SDLK_ESCAPE: quit = true; break;
                }
                if (!check_collision(tmp)) current = tmp;
                else if (e.key.keysym.sym == SDLK_SPACE) {
                    merge_piece();
                    clear_lines();
                    spawn_piece();
                }
            }
        }

        // Auto-drop every 500ms
        if (SDL_GetTicks64() - last_update > 500) {
            last_update = SDL_GetTicks64();
            Piece tmp = current;
            tmp.y++; // Try to drop piece one unit, but don't commit before checking collision

            if (check_collision(tmp)) {
                merge_piece();
                clear_lines();
                spawn_piece();
            } else {
                current = tmp; // If no collisions, current piece is allowed to drop
            }
        }

        render();
        SDL_Delay(16); // 60 fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}
