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
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "nirvana+.h"
#include "game.h"
#include "game_ay.h"
#include "game_enemies.h"
#include "game_engine.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_sprite.h"
#include "game_menu.h"
#include "game_zx.h"
#include "macros.h"




void game_print_footer(void) {
	zx_print_str(22, 1, " ");
	zx_print_str(22,30, " ");
	zx_print_str(23, 1, "     ");
	zx_print_str(23,30, "     ");
	zx_print_ink(INK_RED);
	zx_print_str(22, 1, "<"); //live p1 hut
	zx_print_ink(INK_YELLOW);
  zx_print_str(23, 1, "\\"); //live p1 face
	/* phase osd bottom*/
	game_print_lives();
}

void game_phase_print_score_back(void) {
	zx_print_ink(INK_RED);
	zx_print_str(0, 11, "$%|"); //top
	zx_print_ink(INK_BLUE);
	zx_print_str(0, 1, "[|"); //I
}


void game_print_lives(void) {
	zx_print_ink(INK_WHITE);
	tmp = player_lives - 1;
	if (tmp < 255) {
		zx_print_chr(23, 3, tmp ); //LIVE P1
	}
}


void game_tick(void) {
	++curr_time;
	zx_isr();
}

void game_start_timer(void) {
	NIRVANAP_ISR_HOOK[0] = 205;                                // call
	z80_wpoke(&NIRVANAP_ISR_HOOK[1], (unsigned int)game_tick); // game_tick
}

void game_phase_init(void) {

	/*PHASE INIT*/
	loop_count = 0;
	phase_end = 0;
	phase_angry = 0;
	phase_coins = 0;
	phase_pop = 0;
	entry_time = 0;
	zx_set_clock(0);
	frame_time = 0;
	spr_count = 0;

	/* Phase Tune */
	ay_reset();
	/* Phase Draw Start */
	spr_draw_clear();
	/*Draw Platforms*/
	//zx_paper_fill(INK_BLACK | PAPER_BLACK);
	spr_draw_map_background();
  game_print_header();
	game_print_footer();
	/* Player(s) init */
	if (player_lives) player_init(SPR_P1,GAME_LIN_FLOOR-16,2,TILE_P1_STANR);
}

void game_print_header(void) {
	game_phase_print_score_back();
	zx_print_ink(INK_WHITE);
	/* Print score */
	game_print_score();
}



void game_loop(void) {

	ay_fx_play(ay_effect_10);
	sound_coin();
	z80_delay_ms(200);
	ay_reset();

	player_lives = 4;


	player_score = 0;

	/* screen init */
	game_over = 0;
	/* phase init */
	phase_curr = game_start_phase;
	game_phase_init();
	/* game loop start */
	sprite_lin_inc_mul = 0;
	dirs = 0x00;
	game_joystick_set();
	while (!game_over) {


		if (game_check_time(anim_time, TIME_ANIM)) {
			anim_time = zx_clock();
			spr_play_anim();
		}

		if (game_check_time(bullet_time, TIME_BULLETS)) {
			bullet_time = zx_clock();
			spr_play_bullets();
		}

		/*player 1 turn*/
		sprite = SPR_P1;
		player_turn();
		enemy_turn();


		/*each second aprox - update fps/score/phase left/phase advance*/
		if (game_check_time(frame_time, TIME_EVENT)) {

			frame_time = zx_clock();

			/* end of phase */
			if (phase_end == 1) {
				/*silence background sound*/
				if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_reset();
				/*increment phase*/
				++phase_curr;
				if (phase_curr > 31) {
					/*game end*/
					game_end();
					game_over = 1;
				} else {
					/*next phase*/
					z80_delay_ms(400);
					game_phase_init();
				}
			}

		}
		++loop_count;
	}
}


void game_print_phase() {
	zx_print_str(23, 11, "PHASE");
	zx_print_chr(23, 18, phase_curr+1);
}



unsigned char game_check_maze(int f_index) __z88dk_fastcall {
  return  scr_map[f_index] >= TILE_CEIL; // PERF && scr_map[f_index] < TILE_END;
}

unsigned char game_check_maze_floor(int f_index) __z88dk_fastcall {

	//TODO MEJORAR CREANDO FUNCION NUEVA, REVISAR LLAMADAS HACIA ARRIBA TB...
	if (sprite == SPR_P1 && player_over_stair) {
    return  scr_map[f_index] >= TILE_STAIR_E;
	} else {
	  return  scr_map[f_index] >= TILE_FLOOR; //PERF  && scr_map[f_index] < TILE_END;
	}

}


unsigned char game_enemy_add_get_index( unsigned char f_search) __z88dk_fastcall {
	for (enemies = 0; enemies <= 5 ; ++enemies ) {
		if ( class[enemies] == (unsigned char)f_search ) {
			return enemies;
		}
	}
	return 255;
}


void game_print_score(void) {
	zx_print_ink(INK_WHITE);
	zx_print_paper(PAPER_BLACK);
	zx_print_int(0,3 , player_score);
	zx_print_int(0,14 , game_score_top); //SCORE TOP
}


void game_paint_attrib(unsigned char e_r1) __z88dk_fastcall {
	for ( tmp0=e_r1; tmp0 <= 19; ++tmp0){
		game_paint_attrib_lin(1,31,(tmp0<<3)+8);
	}
}

void game_paint_attrib_lin(unsigned char f_start,unsigned char f_end,unsigned char f_lin) {
	for (tmp_uc = f_start; tmp_uc < f_end ; ++tmp_uc) {
		NIRVANAP_paintC(attrib, f_lin, tmp_uc);
	}
}

void game_paint_attrib_lin_h(unsigned char f_start,unsigned char f_end,unsigned char f_lin) {
	for (tmp_uc =f_start; tmp_uc < f_end ; ++tmp_uc) {
		NIRVANAP_paintC(attrib_hl, f_lin, tmp_uc);
	}
}

void game_joystick_change(void) {
		++player_joy;
		if (player_joy == 7) player_joy = 0; /* Rotate Joystick*/
}

void game_joystick_set_menu(void){
	/* Default Values for menu */
	joyfunc1 = (uint16_t (*)(udk_t *))(in_stick_keyboard);
}

void game_joystick_set(void){
	joyfunc1 = control_method[ player_joy ];
}

void game_end(void) {
	spr_draw_clear();
}


void game_rotate_attrib(void) {
	//OUT OF MEMORY
	// UP
	// tmp_uc = attrib_hl[0];
	// attrib_hl[0] = attrib_hl[1];
	// attrib_hl[1] = attrib_hl[2];
	// attrib_hl[2] = attrib_hl[3];
	// attrib_hl[3] = tmp_uc;
	//DOWN
	tmp_uc = attrib_hl[3];
	attrib_hl[3] = attrib_hl[2];
	attrib_hl[2] = attrib_hl[1];
	attrib_hl[1] = attrib_hl[0];
	attrib_hl[0] = tmp_uc;
}

void game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_microsecs  ) {
	tmp = 1;
	frame_time = zx_clock();
	entry_time = zx_clock();
	while (tmp && !game_check_time( entry_time, f_microsecs)) {
		if ( game_check_time(frame_time,5) ) {
			//ROTATE ATTRIB ARRAY
			frame_time = zx_clock();
			game_paint_attrib_lin_h( f_col, f_col2 , (f_row<<3)+8);
			game_rotate_attrib();
		}
		while ((joyfunc1)(&k1) != 0)
			tmp = 0;
	};
}

unsigned char game_check_time( unsigned int start, unsigned int lapse) {
	if (zx_clock() - start	> lapse) {
		return 1;
	} else {
		return 0;
	}
}
