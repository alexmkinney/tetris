#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

bool check_collision(Piece p);
void merge_piece();
int clear_lines();
void spawn_piece();
void drop_piece(Uint64* last_update);
void reset();

#endif
