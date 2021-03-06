#include "genesis.h"
#include "easing_table.h"

#ifndef TRANSITION_HELPER
#define TRANSITION_HELPER

#define RSE_FRAMES(A) ((A*60)/framerate)

void RSE_turn_screen_to_white(void);
void RSE_turn_screen_to_black(void);
void RSE_pause(u16 frames);
void RSE_clearTileRowB(u16 row);
void RSE_clearTileRowA(u16 row);
void RSE_resetScrolling(void);
void RSE_clearAll(void);

#endif
