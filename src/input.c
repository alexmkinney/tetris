#include <SDL2/SDL.h>
#include <stdbool.h>
#include "tetris.h"
#include "game_logic.h"
#include "input.h"

void get_input(SDL_Event* e) {
    if (e->type == SDL_QUIT) quit = true;

    if (!game_over && e->type == SDL_KEYDOWN) {
        Piece tmp = current;
        switch (e->key.keysym.sym) {
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
        else if (e->key.keysym.sym == SDLK_SPACE) {
            merge_piece();
            clear_lines();
            spawn_piece();
        }
    } else if (game_over && e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_ESCAPE: quit = true; break;
            case SDLK_RETURN: reset(); break;
        }
    }
}
