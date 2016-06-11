#include <genesis.h>
#include "utils.h"
#include "game_shufflepuck.h"
#include "resources.h"
/*
	Player game object
*/
shuffle_racket player;

void player_setPosition(fix32 x,fix32 z)
{
	player.pos_x = x;
	player.pos_z = z;
	player.target_pos_x = x;
	player.target_pos_z = z;
}

void player_reset(void)
{
	player.pos_x = player.initial_pox_x;
	player.pos_z = player.initial_pox_z;
	player.prev_pos_x = player.pos_x;
	player.prev_pos_z = player.pos_z;
	player.width = FIX32(2.0);
	player.length = FIX32(0.5);
	player.max_racket_speed = FIX32(50.0); 
	player.racket_speed = FIX32(50.0);
}

void player_setControler(fix32 joy_x, fix32 joy_z)
{
	// joy_x = fix32Clamp(joy_x, FIX32(-1.0), FIX32(1.0));
	// joy_y = fix32Clamp(joy_y, FIX32(-1.0), FIX32(1.0));

	player.target_pos_x = fix32Add(player.target_pos_x, joy_x);
	player.target_pos_z = fix32Add(player.target_pos_z, joy_z);
	// RangeAdjust(x, 0.0, 1.0, board.board_width * -0.5 + (width * 0.5), board.board_width * 0.5 - (width * 0.5))
	// RangeAdjust(y, 0.0, 0.5, board.board_length * 0.5 - (length * 0.5), board.board_length * 0.35 - (length * 0.5))

	player.target_pos_x = fix32Clamp(player.target_pos_x, FIX32(board_width * -0.5), FIX32(board_width * 0.5));
	player.target_pos_z = fix32Clamp(player.target_pos_z, FIX32(board_length * 0.35), FIX32(board_length * 0.5));
}

void player_update(fix32 dt)
{
	player.prev_pos_x = player.pos_x;
	player.prev_pos_z = player.pos_z;

	player.pos_x = fix32Add(player.pos_x, fix32Mul(fix32Sub(player.target_pos_x, player.pos_x), fix32Mul(dt, player.racket_speed)));
	player.pos_z = fix32Add(player.pos_z, fix32Mul(fix32Sub(player.target_pos_z, player.pos_z), fix32Mul(dt, player.racket_speed)));

	player.velocity_x = fix32Sub(player.pos_x, player.prev_pos_x);
	player.velocity_z = fix32Sub(player.pos_z, player.prev_pos_z);
}