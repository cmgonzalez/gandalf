/*
	This file is part of Gandalf.

	Pietro Bros is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Pietro Bros is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Gandalf.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

extern void          player_init(unsigned char f_lin, unsigned char f_col, unsigned char f_tile);
extern unsigned char player_collision(void);
extern void          player_pick_mushroom();
extern unsigned char player_check_input(void);
extern unsigned char player_action_fire();
extern unsigned char player_action_jump();
extern unsigned char player_move(void);
extern void          player_turn(void);
extern unsigned char player_move_input(void);
extern unsigned char player_hit_platform(void);
extern void          player_score_add(unsigned int f_score) __z88dk_fastcall;
extern void          player_hit_platform_clear(void);
extern void          player_check_floor(void);
extern void          player_pick_item(void);
extern void          player_anim_stairs(void);
extern void          player_tile( unsigned char f_tile, unsigned char f_inc );
extern void          player_check_stairs(unsigned char f_inc) __z88dk_fastcall;
extern void          player_check_stairs_vertical(signed char f_inc) __z88dk_fastcall;
extern void          player_fire(void);
extern unsigned char player_move_jump(void);
extern void          player_open_door(unsigned int f_index, unsigned char f_tile);
extern void          player_damage(unsigned char f_val) __z88dk_fastcall;
extern void          player_lost_life();
extern void          player_gasta_brick();
extern void          player_fix_walk_lin();


#endif
