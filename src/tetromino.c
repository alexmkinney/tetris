#include <SDL2/SDL.h>
#include "../include/tetromino.h"

// Tetromino shapes [type][rotation][y][x]
const int shapes[7][4][4][4] = {
    // I
    {{{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // Rotation 0
     {{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}}, // Rotation 1
     {{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}}, // ...
     {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}}, // ...
    // O
    {{{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}},
    // T
    {{{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,1,0,0}},
     {{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,1,0,0}},
     {{0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {0,1,0,0}}},
    // L
    {{{0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,1,0}},
     {{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {1,0,0,0}},
     {{0,1,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,0,0}}},
    // J
    {{{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,1,1,0}, {0,1,0,0}, {0,1,0,0}},
     {{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,0,1,0}},
     {{0,0,1,0}, {0,0,1,0}, {0,1,1,0}, {0,0,0,0}}},
    // S
    {{{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,1,0}},
     {{0,0,0,0}, {0,0,0,0}, {0,1,1,0}, {1,1,0,0}},
     {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}}},
    // Z
    {{{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
     {{0,0,0,0}, {0,0,1,0}, {0,1,1,0}, {0,1,0,0}},
     {{0,0,0,0}, {0,0,0,0}, {1,1,0,0}, {0,1,1,0}},
     {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}}
};

// Colors for tetrominoes (RGBA)
const SDL_Color colors[] = {
    {0, 255, 255, 255},   // I (cyan)
    {255, 255, 0, 255},   // O (yellow)
    {128, 0, 128, 255},   // T (purple)
    {255, 165, 0, 255},   // L (orange)
    {0, 0, 255, 255},     // J (blue)
    {0, 255, 0, 255},     // S (green)
    {255, 0, 0, 255}      // Z (red)
};
