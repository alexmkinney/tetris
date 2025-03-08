#ifndef RENDER_H
#define RENDER_H

bool init();
void draw_block(int x, int y, SDL_Color color);
void render_text(const char* text, int x, int y);
void render();
void cleanup_renderer();

#endif
