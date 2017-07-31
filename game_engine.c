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
#include "game_zx.h"
#include "macros.h"




void game_print_footer(void) {
	zx_print_str(22, 1, " ");
	zx_print_str(22,30, " ");
	zx_print_str(23, 1, "     ");
	zx_print_str(23,30, "     ");
	if (game_lives[0] > 0) {
		zx_print_ink(INK_RED);
		zx_print_str(22, 1, "<"); //live p1 hut
		zx_print_ink(INK_YELLOW);
		zx_print_str(23, 1, "\\"); //live p1 face
	}
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
	tmp = game_lives[0] - 1;
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
	game_bonus = 0;
	entry_time = 0;
	zx_set_clock(0);
	frame_time = 0;
	score_osd_col[0] = 0xFF;
	score_osd_col[1] = 0xFF;
	spr_count = 0;

	/* Phase Tune */
	ay_reset();
	/* Phase Draw Start */
	spr_draw_clear();
	/*Draw Platforms*/
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	spr_draw_background();
  game_print_header();
	game_print_footer();
	/* Player(s) init */
	if (game_lives[0]) player_init(SPR_P1,GAME_LIN_FLOOR-16,10,TILE_P1_STANR);
	game_osd = 1;
	osd_update_time = zx_clock();
	osd_show_time = osd_update_time;
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
	game_fill_row(22,32);
	ay_reset();

	game_lives[0] = 4;
	game_lives[1] = 0;


	player_score[0] = 0;
	player_score[1] = 0;
	player_next_extra[0] = GAME_EXTRA_LIFE;
	player_next_extra[1] = GAME_EXTRA_LIFE;

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

		if ((loop_count & 3) == 0) {
			game_score_osd(0);
			game_score_osd(1);
		}

		/*player 1 turn*/
		sprite = SPR_P1;
		index_player = 0;

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
	if (score_osd_col[0] != 0xFF) {
		NIRVANAP_drawT(  TILE_EMPTY , score_osd_lin[0] , score_osd_col[0]  );
	}
	if (score_osd_col[1] != 0xFF) {
		NIRVANAP_drawT(  TILE_EMPTY , score_osd_lin[1] , score_osd_col[1]  );
	}
  spr_kill_all();
	z80_delay_ms(400);
	NIRVANAP_halt();
	spr_draw_background();
	game_print_header();
	game_print_footer();
	game_print_phase();
	zx_print_str(8, 11, "GAME  OVER ");
	//NIRVANAP_halt();
	game_colour_message( 8, 11, 21, 200 );
  spr_draw_clear();
	ay_reset();
	game_menu_sel = 0;
	game_menu_paint();
	game_joystick_set_menu();
}


void game_print_phase() {
	zx_print_str(23, 11, "PHASE");
	zx_print_chr(23, 18, phase_curr+1);
}

void game_score_osd(unsigned char f_index_player) {

	if (score_osd_col[f_index_player] != 0xFF ) {
		tmp = score_osd_lin[f_index_player] - 2;

		index1 = spr_calc_index( tmp , score_osd_col[f_index_player]  );
		if ( lvl_1[index1] == TILE_EMPTY && lvl_1[index1+1] == TILE_EMPTY   ) {
			NIRVANAP_halt(); // synchronize with interrupts
			NIRVANAP_drawT( score_osd_tile[f_index_player], score_osd_lin[f_index_player], score_osd_col[f_index_player] );
			score_osd_lin[f_index_player] = tmp;
		}

		if ( game_check_time( score_osd_update_time[f_index_player] , 60 ) ) {
			NIRVANAP_fillT( PAPER, score_osd_lin[f_index_player], score_osd_col[f_index_player] );
			score_osd_col[f_index_player] = 255;
		}
	}
}

void game_bonus_clock(void) {
	tmp_ui = TIME_BONUS - zx_clock()*2;           // one frame is 0.02 seconds
	if (tmp_ui > TIME_BONUS) {
		tmp_ui = 0;          // if time remaining goes negative
		if (phase_left > 0) zx_print_bonus_time(2,14,tmp_ui);
		phase_left = 0;
		phase_end = 1;
		ay_reset();
	} else {
		if (phase_left > 0) {
			zx_print_bonus_time(2,14,tmp_ui);
			if ( game_check_time(frame_time,5) ) {
				/* rotate attrib array */
				frame_time = zx_clock();
				game_paint_attrib_lin_h(14,14+6,2*8 + 8);
				game_rotate_attrib();
			}
		}
		/* end bonus! */
	}
}

unsigned char game_check_maze(int f_index) __z88dk_fastcall {
  return  lvl_1[f_index] >= TILE_CEIL;
}

unsigned char game_check_maze_floor(int f_index) __z88dk_fastcall {
  return  lvl_1[f_index] >= TILE_FLOOR;
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
	zx_print_int(0,3 , player_score[0]);
	zx_print_int(0,14 , game_score_top); //SCORE TOP
	if (game_two_player) zx_print_int(0,24 , player_score[1]);
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

void game_menu_config(void) {
	char cont;
	game_paint_attrib(11);
	cont=1;
	while (cont) {
		while ((joyfunc1)(&k1) != 0);
		//MENU
    zx_print_str(14,10,spec128 ? "SOUND AY  " : "SOUND 48  ");
		zx_print_str(15,10,"GAME      ");
		zx_print_str(16,10,"P1 CTRL   ");
		zx_print_str(17,10,"P2 CTRL   ");
		zx_print_str(18,10,"BACK      ");

		/*Sound 48/AY*/
        zx_print_str(14,20,(game_sound & (GAME_SOUND_48_FX_ON | GAME_SOUND_AY_FX_ON)) ? "ON " : "OFF");

		/*Game Type*/
		switch ( game_type ) {	//0->3
		case 0:
			zx_print_str(15,20,"A  ");
			break;
		case 1:
			zx_print_str(15,20,"B  ");
			break;
		case 2:
			zx_print_str(15,20,"RND");
			break;
		}
		/*P1 Control*/
		zx_print_str(16,20,joynames[player_joy[0]]);
		/*P2 Control*/
		zx_print_str(17,20,joynames[player_joy[1]]);

		game_menu_sel = game_menu_handle(10, 1, 14, 18, 0);
		ay_fx_play(ay_effect_10);
		sound_coin();

		switch ( game_menu_sel ) {
		case 0: //Sound 48/AY
            game_sound ^= spec128 ? GAME_SOUND_AY_FX_ON : GAME_SOUND_48_FX_ON;
			break;
		case 1: //Game type
			++game_type;
			if (game_type > GAME_RANDOM_TYPE) game_type = 0;
			break;
		case 2: //P1 Controls
			game_joystick_change(0);
			break;
		case 3: //P2 Controls
			game_joystick_change(1);
			break;
		case 4: //Back
			game_menu_sel = 0;
			game_menu_paint();
			cont = 0;
			//z80_delay_ms(250);
			break;
		}
	}
}

void game_joystick_change(unsigned char f_player_index) __z88dk_fastcall {
		++player_joy[f_player_index];
		if (player_joy[f_player_index] == 7) player_joy[f_player_index] = 0; /* Rotate Joystick*/

		if (f_player_index == 0 && player_joy[f_player_index] == 1) ++player_joy[0];  /* player 1 cant have SJ2*/
		if (f_player_index == 1 && player_joy[f_player_index] == 0) ++player_joy[1];  /* player 2 cant have SJ1*/


		if (player_joy[0] == player_joy[1]) {
			if (f_player_index == 0) {
				player_joy[1] = 1; /* default */
			} else {
				player_joy[0] = 0; /* default */
			}
		}
}

void game_joystick_set_menu(void){
	/* Default Values for menu */
	joyfunc1 = (uint16_t (*)(udk_t *))(in_stick_sinclair1);
	joyfunc2 = (uint16_t (*)(udk_t *))(in_stick_sinclair2);
}

void game_joystick_set(void){
	joyfunc1 = control_method[ player_joy[0] ];
	joyfunc2 = control_method[ player_joy[1] ];
}


void game_menu(void) {
	/*PLAY MIDI TITLE*/
	game_menu_sel = 0;
	game_menu_top_paint();
	game_menu_paint();
	ay_reset();
	ay_midi_play(pb_midi_title);
	while (1) { //ETHERNAL LOOP
		while ((joyfunc1)(&k1) != 0);
		tmp_uc = game_menu_handle(12, 2, 14, 18, 1000);
		switch ( tmp_uc ) {
		case 0: //1 PLAYER
			game_two_player = 0;
			game_loop();
			break;
		case 1: //2 PLAYER
			game_two_player = 1;
			game_loop();
			break;
		case 2: //OPTIONS
			game_menu_config();
			break;
		}
	}
}

void game_menu_top_paint(void) {
	spr_draw_clear();
	game_paint_attrib(0);
	//Draws menu

	//game_fill_row(0,32);
	//Blue frame top
	game_menu_e(16, 0,30,156,1);
	//Green frame bottom
	game_menu_e(80, 0,30,159,255); //game_menu_e(159,-1);
	//Pietro logo

	tmp = 24;
	intrinsic_di();
	for (tmp_uc =0; tmp_uc < 10 ; ++tmp_uc) {
		NIRVANAP_drawT_raw(192 +	   tmp_uc , tmp + 16 , 5 + (tmp_uc*2) );
		NIRVANAP_drawT_raw(192 + 12	 + tmp_uc , tmp + 32 , 5 + (tmp_uc*2) );
		//NIRVANAP_drawT(180 + 24	 + tmp_uc , tmp + 48 , 5 + (tmp_uc*2) );
	}
	intrinsic_ei();

}

void game_menu_paint(void) {
	NIRVANAP_halt();
	NIRVANAP_fillC(INK_RED | PAPER_RED , tmp + 40 , 26);//POINT
	//Menu

	game_fill_row(0,32);
	game_fill_row(0,32);
	zx_print_str(14,10, "  1 PLAYER   ");
	game_fill_row(15,32);
	zx_print_str(16,10, "  2 PLAYER   ");
	game_fill_row(17,32);
	zx_print_str(18,10, "   CONFIG    ");
	game_fill_row(19,32);
	zx_print_ink(INK_WHITE);
	zx_print_str(21,7, "SELECT:6789 START:0");
	zx_print_ink(INK_BLUE);
	zx_print_str(23,3, "CODED BY CGONZALEZ VER 0.1");
	tmp_uc = 0; //fix menu return
	game_paint_attrib(11);
}

void game_menu_e(unsigned char f_col,unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign) {
	intrinsic_di();
	for (tmp_uc =e_c0; tmp_uc <= e_c1 ; tmp_uc = tmp_uc + 2) {
		if (tmp_uc == e_c0) {
			NIRVANAP_drawT_raw(e_start , f_col, tmp_uc );
			continue;
		}
		if (tmp_uc == e_c1) {
			NIRVANAP_drawT_raw(e_start + 2 , f_col, tmp_uc );
			continue;
		}
		if (tmp_uc == 14 || tmp_uc == 16) {
			continue;
		}
		NIRVANAP_drawT_raw(e_start + 1 , f_col, tmp_uc );
	}
	if ( f_sign == 255 ) {
		f_col = f_col-16;
	} else {
		f_col = f_col+16;
	}
	NIRVANAP_drawT_raw(e_start + 12 , f_col, e_c0 );
	NIRVANAP_drawT_raw(e_start + 14 , f_col, e_c1 );
	intrinsic_ei();

}

unsigned char game_menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout) {
	entry_time = zx_clock();
	s_lin1 = f_start + (game_menu_sel*f_inc);
	while ((dirs = (joyfunc1)(&k1)) != IN_STICK_FIRE) {
		if ( game_check_time(frame_time,5) ) {
			if (dirs != 0) {
				ay_fx_play(ay_effect_03);  /* effect 19 now repeats */
				sound_slide();
				game_paint_attrib_lin( f_col, 20 , (s_lin1*8)+8);
				s_lin1 += f_inc;
				if (s_lin1 > f_end	) {
					s_lin1 = f_start;
				}
			}
			/* rotate attrib array */
			frame_time = zx_clock();
			game_paint_attrib_lin_h( f_col, 20 , (s_lin1*8)+8);
			game_rotate_attrib();
		}
		if (timeout > 0 && game_check_time(entry_time, timeout) && !ay_is_playing() ) {
			game_menu_sel = 0;
			//s_lin1 = f_start + (game_menu_sel*f_inc);
			game_menu_back(0);
			return 255;
		}

	};
	return	(unsigned char) ( s_lin1 -	f_start	 ) / f_inc;
}

void game_end(void) {
	spr_draw_clear();
}

void game_menu_back(unsigned char f_start) __z88dk_fastcall
{
	game_menu_sel = 0;
	s_lin1 = f_start;
	game_menu_top_paint();
	game_menu_paint();
	entry_time = zx_clock();
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
		while (((joyfunc1)(&k1) | (joyfunc2)(&k2)) != 0)
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
