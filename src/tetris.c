#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include "tetris.h"
#include "tetromino.h"
#include "render.h"
#include "game_logic.h"
#include "input.h"

int main(int argc, char* argv[]) {
    if (!init()) return 1;

    srand(time(0));
    spawn_piece();

    Uint64 last_update = SDL_GetTicks64();

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            get_input(&e);
        }

        // Auto-drop (500ms default)
        drop_piece(&last_update);

        render();
        SDL_Delay(16); // 60 fps
    }

    cleanup_renderer();
    SDL_Quit();
    return 0;
}
