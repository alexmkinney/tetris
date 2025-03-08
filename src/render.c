#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "tetris.h"
#include "tetromino.h"
#include "font_data.h"
#include "render.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font = NULL;
SDL_Color text_color = {255, 255, 255, 255};

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

    if (game_over) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlay);

        int text_w, text_h, x_position, y_position;
        const int line_spacing = 50;

        const char game_over_text[10] = "Game Over";
        TTF_SizeText(font, game_over_text, &text_w, &text_h);
        x_position = (SCREEN_WIDTH - text_w)/2;
        y_position = (SCREEN_HEIGHT/2) - line_spacing;

        render_text(game_over_text, x_position, y_position);

        TTF_SizeText(font, score_text, &text_w, &text_h);
        x_position = (SCREEN_WIDTH - text_w)/2;
        y_position = SCREEN_HEIGHT/2;

        render_text(score_text, x_position, y_position);
    }

    SDL_RenderPresent(renderer);
}

void cleanup_renderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
}
