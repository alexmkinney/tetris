#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "tetris.h"
#include "tetromino.h"
#include "game_logic.h"

int playfield[HEIGHT][WIDTH] = {0};

Piece current;

int last_type = -1;

bool game_over = false;
bool quit = false;
int score = 0;
int drop_interval = 500;

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

    // Duplicate detection
    if (current.type == last_type) {
        current.type = rand() % 7;
    }
    last_type = current.type;

    current.rotation = 0;
    current.x = WIDTH / 2 - 2; // Middle of playfield
    current.y = 0;

    if (check_collision(current)) {
        printf("Game Over!\nScore: %d\n", score);
        game_over = true;
    }
}

void drop_piece(Uint64* last_update) {
    if (SDL_GetTicks64() - *last_update > drop_interval && !game_over) {
        *last_update = SDL_GetTicks64();
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
}

void reset() {
    game_over = false;
    score = 0;
    
    // Clear playfield
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            playfield[y][x] = 0;
        }
    }
}
