#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include "tetris.h"
#include "tetromino.h"
#include "render.h"
#include "game_logic.h"

int main(int argc, char* argv[]) {
    if (!init()) return 1;

    srand(time(0));
    spawn_piece();

    Uint64 last_update = SDL_GetTicks64();

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;

            if (!game_over) {
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
            } else if (game_over && e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: quit = true; break;
                    case SDLK_RETURN: reset(); break;
                }
            }
        }

        // Auto-drop (500ms default)
        if (SDL_GetTicks64() - last_update > drop_interval && !game_over) {
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

    cleanup_renderer();
    SDL_Quit();
    return 0;
}
