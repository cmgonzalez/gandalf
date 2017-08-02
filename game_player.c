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
#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>
#include "nirvana+.h"
#include "game.h"
#include "game_player.h"
#include "game_ay.h"
#include "game_enemies.h"
#include "game_engine.h"
#include "game_sprite.h"
#include "game_sound.h"
#include "game_zx.h"
#include "macros.h"


void player_init(unsigned char f_sprite, unsigned  char f_lin, unsigned  char f_col, unsigned  char f_tile) {
	//COMMON SPRITE VARIABLES
	class[f_sprite] = PLAYER;
	tile[f_sprite] = f_tile;
	lin[f_sprite]  = f_lin;//*SPRITELIN(f_sprite);
	col[f_sprite]  = f_col;//*SPRITECOL(f_sprite);
	colint[f_sprite] = 0;
	state[f_sprite] = 0;
	state_a[f_sprite] = 0;
	jump_lin[f_sprite] = f_lin;
	last_time[f_sprite] = zx_clock();
	sprite_speed_alt[f_sprite] = 0;

	BIT_SET(state_a[f_sprite], STAT_LOCK);
	//PLAYER ONLY VARIABLES
	if (f_sprite == SPR_P1) {
		index_player = 0;
		BIT_SET(state_a[f_sprite], STAT_LDIRR);
	} else {
		index_player = 1;
		BIT_SET(state_a[f_sprite], STAT_LDIRL);
	}
	hit_col[index_player] = 0;
	hit_lin[index_player] = 0;
	sliding[index_player] = 0;
	player_jump_c[index_player] = 0;
	NIRVANAP_spriteT(f_sprite, f_tile, f_lin, f_col);
}

void player_calc_slide(  ) {
  sliding[index_player] = PLAYER_SLIDE_NORMAL;
}

unsigned char player_check_input(void) {
	return dirs & IN_STICK_FIRE || dirs & IN_STICK_LEFT || dirs & IN_STICK_RIGHT;
}

unsigned char player_collision(void) {
	if ( class[sprite] == 0 ) return 0;
	if (BIT_CHK(state[sprite], STAT_HIT)) return 0;
	if (BIT_CHK(state[sprite], STAT_KILL)) return 0;
	hit_count = 0;
	for (enemies = 0; enemies < SPR_P2 ; ++enemies ) {
		if ( spr_collision_check(sprite, enemies, SPRITE_VCOL_MARGIN) ) {

			if ( BIT_CHK(state[enemies], STAT_HIT) ) {
				/* Kill the enemy */
				enemy_kill(enemies);
			} else {
				if (class[enemies] == COIN_1 || class[enemies] == COIN_2) {
					/* Get Coin */
					player_coin(enemies,80);
				} else {
					/* Player Killed */
					ay_fx_play(ay_effect_18);
					if (game_god_mode) {
						enemy_kill(enemies);
					} else {
						if (!game_inmune) player_kill();
					}

				}
			}
		}
	}

  //UPDATE SCORE OSD
	if ( hit_count > 0 && lin[sprite] > 24) {
		if (score_osd_col[index_player] != 0xFF) {
		  NIRVANAP_drawT(  TILE_EMPTY , score_osd_lin[index_player] , score_osd_col[index_player]  );
		}
		score_osd_col[index_player] = col[sprite];
		score_osd_lin[index_player] = lin[sprite] - 6;
		score_osd_update_time[index_player] = zx_clock();
    score_osd_tile[index_player] = TILE_800;
		if ( hit_count > 1) score_osd_tile[index_player] = TILE_NICE;
	  if ( hit_count > 6) score_osd_tile[index_player] = TILE_EXTRA;
	}
	return 0;
}

void player_kill(void) {
	if ( !BIT_CHK(state[sprite], STAT_KILL) ) {
		sound_hit_brick();
		BIT_SET(state[sprite], STAT_KILL);
		sprite_speed_alt[sprite] = SPRITE_FALL_SPEED;
		if ( BIT_CHK(state[sprite], STAT_DIRR) ) {
			tile[sprite] = TILE_P1_HITR;
		} else {
			tile[sprite] = TILE_P1_HITR + TILE_P1_LEN;
		}
		player_hit_brick_clear();
		NIRVANAP_spriteT(sprite, tile[sprite], lin[sprite], col[sprite]);
		spr_timer[sprite] = zx_clock();
		if (sprite == SPR_P1) {
			BIT_SET(state_a[sprite], STAT_LDIRL);
			BIT_CLR(state_a[sprite], STAT_LDIRR);
		} else {
			BIT_SET(state_a[sprite], STAT_LDIRR);
			BIT_CLR(state_a[sprite], STAT_LDIRL);
		}
	}
}

void player_restart(unsigned char f_sprite) __z88dk_fastcall {
	if( col[f_sprite] < 4) {
		spr_back_paint(0 + 15 * 32);
	}
	if( col[f_sprite] > 26) {
		spr_back_paint(26 + 15 * 32);
	}

	ay_fx_play(ay_effect_15);
	player_init( SPR_P1,0,14,TILE_P1_STANR);
	BIT_SET(state[f_sprite],STAT_HIT);
	BIT_SET(state_a[f_sprite],STAT_LOCK);
	spr_timer[f_sprite] = zx_clock();
	sprite_speed_alt[f_sprite] = SPRITE_RESTART_SPEED;

}

unsigned char player_lost_life(void){
	sound_hit_enemy();
	--game_lives[index_player];

	if (game_lives[index_player] == 0) {
		/* Player dies */
		if ( game_lives[0] == 0 && game_lives[1] == 0 ) {
			game_over = 1;
		}
		/* Do not restart player */
		return 0;
	}
	game_print_lives();
	return 1;
}



void player_turn(void) {
	if ( class[sprite] == PLAYER && game_lives[index_player] > 0) {
		if ( spr_chktime(&sprite)  ) {
			dirs = 0;
			dirs = (joyfunc1) (&k1);
			player_move();
			player_collision();
		}
	}
}

unsigned char player_move(void){

	/* Player initial Values */
	s_lin0 = lin[sprite];
	s_col0 = col[sprite];
	s_tile0 = tile[sprite] + colint[sprite];
	s_state = state[sprite];


	/* Killed Player */
	if ( BIT_CHK(s_state, STAT_KILL) ) {
		tile[sprite] = TILE_P1_KILL;
		if ( game_check_time(spr_timer[sprite], PLAYER_ANIM_HIT_TIME ) ) {
			spr_killed(sprite);
		}
		return 0;
	}
	/* Read player input */
	player_move_input();

	s_state = state[sprite];

	if ( !BIT_CHK(s_state, STAT_JUMP) && !BIT_CHK(s_state, STAT_FALL)) {
		/* Check if the player have floor, and set fall if not */
		player_check_floor();

	} else {
		/* Jumping or Falling */
		sprite_lin_inc_mul = 2;
		if (player_jump_c[index_player] > 3 ) sprite_lin_inc_mul = 1;
		if (player_jump_c[index_player] > 8 ) sprite_lin_inc_mul = 0;
		if (player_jump_c[index_player] > 12) sprite_lin_inc_mul = 1;
		if (player_jump_c[index_player] > 17) sprite_lin_inc_mul = 2;

		if ( BIT_CHK(s_state, STAT_JUMP) ) {
			/* Jump Handling */
			if ( player_jump_c[index_player] < PLAYER_JUMP_MAX ) {
				if ( !BIT_CHK(state_a[sprite], STAT_HITBRICK) ) {
					spr_move_up();
				}
			} else {
				spr_set_fall();
			}
		} else {
			/* Falling Handling */
			if ( spr_move_down() ) {
				BIT_CLR( s_state , STAT_FALL );
				BIT_CLR( state_a[sprite] , STAT_HITBRICK );
				player_jump_c[index_player] = 0;
				jump_lin[sprite] = 0;
				sliding[index_player] = 0;
				colint[sprite] = 0;
				tile[sprite] = spr_tile_dir(TILE_P1_STANR, sprite, 1);
				if ( BIT_CHK(s_state, STAT_DIRL) || BIT_CHK(s_state, STAT_DIRR) ) player_calc_slide();
			}
		}
		spr_move_horizontal();
		sprite_lin_inc_mul = 0;
		++player_jump_c[index_player];
	}
	/* Draw Player sprite */
	spr_redraw();

	/* Anim Restored hitted platforms */
	if ( game_check_time( spr_timer[sprite], PLAYER_HIT_BRICK_TIME ) ) {
		player_hit_brick_clear();
	}

	/* Pick Items */
  //player_pick_item();

	/* Store State variable */
	state[sprite] = s_state;
	return 0;
}

void player_pick_item( unsigned int f_index){
  unsigned char v0;

	v0 = scr_map[f_index];
	if ( v0 >= TILE_ITEM_S && v0 <= TILE_ITEM_E ) {
		//PICK ITEM
		scr_map[f_index] = TILE_EMPTY;
		NIRVANAP_drawT(TILE_EMPTY, f_index / 16, f_index % 16);
	}
}

unsigned char player_move_input(void) {



	if ( BIT_CHK(s_state, STAT_JUMP) || BIT_CHK(s_state, STAT_FALL) ) return 0;
	/* User have pressed valid input */
	if ( player_check_input() && ( sliding[index_player] == 0 || dirs & IN_STICK_FIRE ) ) {
		/* New jump */
		if ( dirs & IN_STICK_FIRE ) {
			if ( ay_is_playing() != AY_PLAYING_MUSIC ) ay_fx_play(ay_effect_03);
			sound_jump();
			if ( BIT_CHK( state[sprite] ,  STAT_DIRR ) ) {
				colint[sprite]=0;
			} else {
				colint[sprite]=2;
			}
			BIT_SET(s_state, STAT_JUMP);
			BIT_CLR(s_state, STAT_FALL);
			jump_lin[sprite] = lin[sprite];
			state[sprite] = s_state; /*TODO FIXME!*/
			tile[sprite] = spr_tile_dir(TILE_P1_JUMPR, sprite, TILE_P1_LEN);
			sprite_speed_alt[sprite] = PLAYER_JUMP_SPEED;
			player_jump_c[index_player] = 0;
			sliding[index_player] = 0;
			return 1;
		}

			/* Move Right */
			if ( dirs & IN_STICK_RIGHT ) {
				if ( BIT_CHK(s_state, STAT_DIRL) ) {
					player_calc_slide();
					return 1;
				}
				BIT_SET(s_state, STAT_DIRR);
				BIT_CLR(s_state, STAT_DIRL);
				BIT_SET(state_a[sprite], STAT_LDIRR);
				BIT_CLR(state_a[sprite], STAT_LDIRL);
				BIT_SET(state_a[sprite], STAT_INERT);
			}
			/* Move Left */
			if ( dirs & IN_STICK_LEFT ) {
				if ( BIT_CHK(s_state, STAT_DIRR) ) {
					player_calc_slide();
					return 1;
				}
				BIT_SET(s_state, STAT_DIRL);
				BIT_CLR(s_state, STAT_DIRR);
				BIT_SET(state_a[sprite], STAT_LDIRL);
				BIT_CLR(state_a[sprite], STAT_LDIRR);
				BIT_SET(state_a[sprite], STAT_INERT);
			}
			/* Set Tile according to current direction */
			state[sprite] = s_state; /* TODO FIXME! */
			tile[sprite] = spr_tile_dir( TILE_P1_RIGHT, sprite, TILE_P1_LEN);



			if ( !ay_is_playing() && !game_bonus ) ay_fx_play(ay_effect_20);
			spr_move_horizontal();
		return 1;
	} else {
		if (BIT_CHK(state_a[sprite], STAT_INERT)) {
			player_calc_slide();
			BIT_CLR(state_a[sprite], STAT_INERT);
		}
		/* Slide Handling */
		if ( sliding[index_player] > 0  ) {
			/* Sliding */
			if ( ay_is_playing() != AY_PLAYING_MUSIC ) ay_fx_play(ay_effect_01);
			sound_slide();
			spr_move_horizontal();
			sliding[index_player]--;
			sprite_speed_alt[sprite] = 0;
			tile[sprite] = spr_tile_dir(TILE_P1_SLIDR, sprite, TILE_P1_LEN);
			if ( sliding[index_player] == 0 ) {
				/* End Sliding */
				spr_timer[sprite] = 0;
				colint[sprite] = 0;
				tile[sprite] = spr_tile_dir(TILE_P1_STANR, sprite, 1);
				BIT_CLR(state[sprite],STAT_DIRR);
				BIT_CLR(state[sprite],STAT_DIRL);
				player_check_floor();
			}
		}
		return 0;
	}
}

unsigned char player_hit_brick(void){
	if ( ( hit_lin[index_player] == 0 ) && ( lin[sprite] > 16 )	) {
		for (enemies = 0; enemies < 6 ; ++enemies){
			//HIT ENEMIES
			if (
					class[enemies] != 0							&&
					( lin[sprite] - lin[enemies] == 24 )		&&
					( !BIT_CHK(state[enemies], STAT_KILL) )     &&
					( abs(col[sprite] - col[enemies]) <= 2 )
				) {
					enemy_hit();
					sound_hit_enemy();
				}
		}

		BIT_SET( state_a[sprite] , STAT_HITBRICK );
		player_jump_c[index_player] = PLAYER_JUMP_MAX-PLAYER_JUMP_SUSPEND;
		spr_timer[sprite] = zx_clock();
		hit_lin[index_player] = 8 + (( (lin[sprite]-8) >> 4) << 4 );

		if ( (col[sprite] & 1) == 0 ) {
		 hit_col[index_player] = col[sprite];
		} else {

			if ( BIT_CHK(state[sprite], STAT_DIRL) ) {
				if (g_hit_right) {
					 hit_col[index_player] = col[sprite]+1;
				} else {
					 hit_col[index_player] = col[sprite]-1;
				}
			} else {
				if (g_hit_left) {
					 hit_col[index_player] = col[sprite]-1;
				} else {
					 hit_col[index_player] = col[sprite]+1;
				}
			}
		}
		spr_brick_anim(1);
		sound_hit_brick();
		return 1;
	}
	return 0;
}

void player_hit_brick_clear(void){
	//CLEAR HITTED BRICKS N MAKES THE PLAYER FALL
	if ( hit_lin[index_player] > 0 ) {
		index1 = spr_calc_index( hit_lin[index_player] , hit_col[index_player] );
		spr_brick_anim(0);
		hit_lin[index_player] = 0;
		hit_col[index_player] = 0;
	}
}

void player_coin(unsigned char f_enemies, unsigned char f_score) {
	if (class[f_enemies] == SLIPICE) {
		ay_fx_play(ay_effect_12); //SLIPICE
		sprite_speed_alt[f_enemies] = SPRITE_FALL_SPEED;
	} else {
		ay_fx_play(ay_effect_10); //COIN1 or COIN2
		sprite_speed_alt[f_enemies] = ENEMY_KILLED_SPEED;
		sound_coin();
	}
  BIT_CLR(state[f_enemies], STAT_JUMP);
	BIT_SET(state[f_enemies], STAT_KILL);
	spr_timer[f_enemies] = zx_clock();
	if (game_bonus){
		++phase_bonus_total[index_player];
		--phase_left;
		if(phase_left <= 0) phase_end = 1;
	} else {
		player_score_add(f_score);
	}
}


void player_score_add(unsigned int f_score) __z88dk_fastcall {
	player_score[index_player] = player_score[index_player] + f_score;
	//CHECK FOR TOP SCORE
	if ( player_score[index_player] > game_score_top ) {
		game_score_top = player_score[index_player];
	}
	//CHECK FOR EXTRA LIFE
	if ( player_score[index_player] > player_next_extra[index_player] ) {
		player_next_extra[index_player] += GAME_EXTRA_LIFE;
		++game_lives[index_player];
		//GAME_EXTRA_LIFE
		hit_count = 16;
		if(!game_bonus) game_print_lives();

		for (tmp0 = 0; tmp0 < 6 ; ++tmp0){
			if (index_player == 0) {
				zx_border(INK_BLUE);
			} else {
		    zx_border(INK_WHITE);
			}
			z80_delay_ms(5);
			if (index_player == 0) {
				zx_border(INK_RED);
			} else {
			  zx_border(INK_GREEN);
			}
			z80_delay_ms(5);
		}

		sound_coin();
		sound_coin();
		sound_coin();
	  sound_coin();
		zx_border(INK_BLACK);

		hit_count = 8; //IMPOSIBLE WE HAVE 6 ENEMIES
	}
	game_print_score();
}

void player_check_floor(void){
	unsigned int index_d;
	unsigned char v1;
	unsigned char v2;

	index_d = spr_calc_index( lin[sprite] + 16 , col[sprite] );
	v1 = scr_map[index_d];

	if ((col[sprite] & 1) == 0) {
		v2 = 1;
	} else {
		v2 = scr_map[index_d+1];
	}


	if (s_lin0 == GAME_LIN_FLOOR) {
		v1 = TILE_CEIL;
		v1 = TILE_CEIL;
	}

	if ( ( v1 < TILE_FLOOR || v1 > TILE_END ) && ( v2 < TILE_FLOOR || v2 > TILE_END )  ) {
		sprite_speed_alt[sprite] = PLAYER_FALL_SPEED;
		BIT_SET(s_state, STAT_FALL);
	}
}
