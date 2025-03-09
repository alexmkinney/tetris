#ifndef RENDER_H
#define RENDER_H

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern TTF_Font* font;
extern SDL_Color text_color;

bool init();
void draw_block(int x, int y, SDL_Color color);
void render_text(const char* text, int x, int y);
void render();
void cleanup_renderer();

#endif
