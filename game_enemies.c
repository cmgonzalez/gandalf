/*
	This file is part of Pietro Bros.

	Pietro Bros is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Pietro Bros is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Pietro Bros.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include "nirvana+.h"
#include "game.h"
#include "game_enemies.h"
#include "game_ay.h"
#include "game_engine.h"
#include "game_player.h"
#include "game_sprite.h"
#include "game_sound.h"
#include "game_zx.h"
#include "macros.h"





unsigned char enemy_collision(void) {
	if ( BIT_CHK(state[sprite]  , STAT_HIT)  ) return 0;
	if ( BIT_CHK(state[sprite]  , STAT_KILL)  ) return 0;
	if ( BIT_CHK(state[sprite]  , STAT_FALL)  ) return 0;
	if ( BIT_CHK(state[sprite]  , STAT_JUMP)  ) return 0;
	if ( BIT_CHK(state_a[sprite], STAT_TURN)  ) return 0;

	for (enemies = 0; enemies < SPR_P2 ; ++enemies ) {
		if ( class[enemies] == 0 || class[enemies] > FIGHTERFLY ) continue;
		if ( enemies == sprite ) continue;

		if ( BIT_CHK(state[enemies], STAT_KILL)  ) continue;
		if ( BIT_CHK(state[enemies], STAT_FALL) ) continue;
		if ( spr_collision_check( sprite, enemies, 0 ) ) {

			//TURN OTHER ENEMY (enemies)
			if ( !BIT_CHK(state[enemies], STAT_HIT) &&
				 BIT_CHK(s_state, STAT_DIRL) != BIT_CHK(state[enemies], STAT_DIRL) &&
				 BIT_CHK(s_state, STAT_DIRR) != BIT_CHK(state[enemies], STAT_DIRR) &&
				 class[sprite] != COIN_1
				) {
				enemy_collision_turn_dir(enemies);
			}

			//TURN CURRENT ENEMY (sprite)
			state[sprite] = s_state;
			enemy_collision_turn_dir(sprite);
			s_state = state[sprite];
			spr_timer[sprite] = zx_clock();
			return 1;
		}
	}
	BIT_CLR(state_a[sprite], STAT_PUSH);
	return 0;
}

void enemy_collision_turn_dir( unsigned char f_sprite) __z88dk_fastcall {
	if ( !BIT_CHK(state_a[f_sprite], STAT_PUSH) ) {
		BIT_FLP(state[f_sprite], STAT_DIRR);
		BIT_FLP(state[f_sprite], STAT_DIRL);
		BIT_SET(state_a[f_sprite], STAT_PUSH);
		if ( class[f_sprite] <= SIDESTEPPER_MAGENTA ) {
			//ONLY SHELLCREEPERS TURNING ANIMATION
			BIT_SET(state_a[f_sprite], STAT_TURN);
			spr_timer[f_sprite] = zx_clock();
			tile[f_sprite] = spr_tile(f_sprite);
			colint[f_sprite] = 0;
		}
	}
}

void enemy_hit(void){

}

void enemy_flip_change_dir( unsigned char f_keep ) __z88dk_fastcall {

  unsigned char x_player;
	unsigned char x_enemy;

	if (BIT_CHK(state[enemies], STAT_DIRR)) {
		BIT_SET(state_a[enemies], STAT_LDIRR);
		BIT_CLR(state_a[enemies], STAT_LDIRL);
	} else {
		BIT_SET(state_a[enemies], STAT_LDIRR);
		BIT_CLR(state_a[enemies], STAT_LDIRL);
	}

  x_player = spr_calc_hor(sprite);
	x_enemy = spr_calc_hor(enemies);

	if (x_player  > x_enemy ) {
		BIT_SET(state[enemies], STAT_DIRL);
		BIT_CLR(state[enemies], STAT_DIRR);
	} else {
		if (col[sprite] < col[enemies]) {
			BIT_SET(state[enemies], STAT_DIRR);
			BIT_CLR(state[enemies], STAT_DIRL);
		} else {
			//col[sprite] == col[enemies]
			if (!f_keep) {
				BIT_CLR(state[enemies], STAT_DIRL);
				BIT_CLR(state[enemies], STAT_DIRR);
			}
		}
	}
	colint[enemies] = 0;
}

void enemy_flip(unsigned char f_tile) __z88dk_fastcall {

	spr_timer[enemies] = zx_clock();
	BIT_SET(state[enemies], STAT_JUMP);
	BIT_CLR(state[enemies], STAT_FALL);
	BIT_FLP(state[enemies], STAT_HIT);
	jump_lin[enemies] =  lin[enemies];
  colint[enemies] = 0;

	if (BIT_CHK(state[enemies], STAT_HIT)) {
		//Normal
		ay_fx_play(ay_effect_02);
		enemy_flip_change_dir(0);

		tile[enemies] = f_tile + 6;
		player_score_add(1);
	} else {
		//Flipped
		if (BIT_CHK(state_a[enemies], STAT_LDIRR)) {
			BIT_SET(state[enemies], STAT_DIRR);
			BIT_CLR(state[enemies], STAT_DIRL);
		} else {
			BIT_SET(state[enemies], STAT_DIRR);
			BIT_CLR(state[enemies], STAT_DIRL);
		}
		tile[enemies] = spr_tile(enemies);
	}
}

void enemy_flip_sidestepper(unsigned char f_tile) __z88dk_fastcall {
	if ( BIT_CHK(state[enemies], STAT_ANGRY) ) {
		enemy_flip(f_tile);
	} else {
		enemy_flip_change_dir(1);
		BIT_FLP(state[enemies], STAT_JUMP);
		BIT_CLR(state[enemies], STAT_FALL);
		BIT_SET(state[enemies], STAT_ANGRY);
		jump_lin[enemies] =  lin[enemies];
		tile[enemies] = spr_tile(enemies);
	}
}

void enemy_turn(void){
	for ( sprite = 0; sprite < 6 ; ++sprite ) {
		if ( class[sprite] > 0 && spr_chktime(&sprite) ) {
			s_lin0  = lin[sprite];
			s_col0  = col[sprite];
			s_tile0 = tile[sprite] + colint[sprite];
			s_state = state[sprite];
			last_time[sprite] = zx_clock();
			//KILLED ENEMY
				if ( BIT_CHK(s_state, STAT_KILL) && !BIT_CHK(s_state, STAT_JUMP) ) {
							spr_killed(sprite);
							continue;
				}
			//FLIPPED ENEMY
			if ( BIT_CHK(s_state,STAT_HIT) && !BIT_CHK(s_state,STAT_FALL) && !BIT_CHK(s_state,STAT_JUMP) ) {
				sprite_speed_alt[sprite] = ENEMY_KILLED_SPEED; /* Flipped enemy */
				enemy_standard_hit();
				++colint[sprite];
				if (colint[sprite] > 2) colint[sprite] = 0;
				spr_redraw();
				continue;
			}

			enemy_standard();
			spr_redraw();
			state[sprite] = s_state;
		}
	}
}

void enemy_standard(void){
	//SHELLCREEPERS || SIDESTEPPERS
		if ( BIT_CHK(s_state, STAT_JUMP) ) {
			//JUMPING
			sprite_speed_alt[sprite] = ENEMY_JUMP_SPEED;
			spr_move_horizontal();
			spr_move_up();
			//MAX HIT JUMP
			if ( jump_lin[sprite] - lin[sprite] >= ENEMIES_MAXJUMP ) {
				//enemy_ugly_fix();
				spr_set_fall();
			}
		} else {
			//WALKING OR FALLING
			enemy_walk();
	 		//JUMP BEFORE ENTER PIPES
			if ( lin[sprite] == GAME_LIN_FLOOR && ( col[sprite] < 5 || col[sprite] > 25) ) {
				BIT_SET(s_state, STAT_JUMP);
			}
		}
}

void enemy_slipice(void){
}

void enemy_fireball_red(void){
}

void enemy_standard_hit(void) {

}

void enemy_evolve(unsigned char f_enemy) __z88dk_fastcall{
	switch (class[f_enemy]) {
		case SHELLCREEPER_GREEN:
      enemy_upgrade(f_enemy, SHELLCREEPER_RED, TILE_SHELLCREEPER_RED);
			break;
		case SHELLCREEPER_RED:
  		enemy_upgrade(f_enemy, SHELLCREEPER_BLUE, TILE_SHELLCREEPER_BLUE);
	  	break;
		case SIDESTEPPER_RED:
      enemy_upgrade(f_enemy, SIDESTEPPER_GREEN, TILE_SIDESTEPPER_GREEN);
			break;
		case SIDESTEPPER_GREEN:
  		enemy_upgrade(f_enemy, SIDESTEPPER_MAGENTA, TILE_SIDESTEPPER_MAGENTA);
	  	break;
	}
}

void enemy_upgrade(unsigned char f_enemy, unsigned char f_class, unsigned char f_tile){
	tile[f_enemy] = f_tile + 6;
  class[f_enemy] = f_class;
}

void enemy_walk(void){
	if (BIT_CHK(s_state, STAT_JUMP) == 0 && BIT_CHK(s_state, STAT_FALL) == 0) {
		if ( (col[sprite] & 1) && !BIT_CHK(s_state, STAT_HIT) ) {
			//index_d = 0;
			tmp = 0;
		} else {
			tmp = game_check_maze(spr_calc_index(lin[sprite] + 16,col[sprite]));
		}
	} else {
		tmp = game_check_maze( spr_calc_index( lin[sprite] + 16 , col[sprite] ) );
	}

	if ( tmp ) {
		//FALLING
		if ( !BIT_CHK(s_state, STAT_JUMP) ) {
			BIT_SET(s_state, STAT_FALL);
		}
		if (jump_lin[sprite] == 0) {
			jump_lin[sprite] = lin[sprite];
		}
		if ( BIT_CHK(s_state, STAT_FALL) ) {
			spr_move_down();
		} else {
			spr_move_up();
		}
		sprite_speed_alt[sprite] = ENEMY_FALL_SPEED;
		tmp = 1;
		tmp0 = game_check_maze( spr_calc_index( lin[sprite]+16, col[sprite] + 1 ) );
		tmp1 = game_check_maze( spr_calc_index( lin[sprite]+16, col[sprite] - 1 ) );
		tmp = !tmp0 && !tmp1;
		if ( tmp && ( lin[sprite] - jump_lin[sprite] <= 8 ) ) spr_move_horizontal();
	} else {
		//OVER PLATFORM
		if ( BIT_CHK(s_state, STAT_FALL) ) {
			BIT_CLR(s_state, STAT_FALL);
			jump_lin[sprite] = 0;
			sprite_speed_alt[sprite] = 0;
		}
		if ( !BIT_CHK(state_a[sprite], STAT_TURN) ) {
			if ( (loop_count & 3) == 0 ) enemy_collision();
			spr_move_horizontal();
		} else {
			spr_timer[sprite] = zx_clock();
			if (colint[sprite] < 2) {
				++colint[sprite];
			} else {
				BIT_CLR(state_a[sprite], STAT_TURN);
				tile[sprite] = spr_tile(sprite);
			}
		}
	}
}

void enemy_init(unsigned char f_sprite,unsigned  char f_lin,unsigned  char f_col,unsigned  char f_class,unsigned  char f_dir) {
	++spr_count;
	class[f_sprite] = f_class;
	lin[f_sprite]  = f_lin;
	col[f_sprite]  = f_col;
	tmp = 0;
	state[f_sprite] = 0;
	state_a[f_sprite] = 0;
	jump_lin[f_sprite] = 0;

	if (f_dir == DIR_RIGHT){
		BIT_SET(state[f_sprite], STAT_DIRR);
	}
	if (f_dir == DIR_LEFT){
		BIT_SET(state[f_sprite], STAT_DIRL);
	}
	colint[f_sprite] = 0;
	tile[f_sprite] = spr_tile(f_sprite);
	last_time[f_sprite] = 0;
	spr_timer[f_sprite] = zx_clock();
	sprite_speed_alt[f_sprite] = 0;
}

void enemy_kill(unsigned char f_sprite) __z88dk_fastcall {
	tmp = f_sprite; //DUMMY
	sound_kill();
}
