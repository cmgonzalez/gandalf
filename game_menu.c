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
#include "game_menu.h"
#include "game_engine.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"


void menu_config(void) {
	char cont;
	game_paint_attrib(11);
	cont=1;
	while (cont) {
		while ((joyfunc1)(&k1) != 0);
		//MENU
    zx_print_str(14,10,spec128 ? "SOUND AY  " : "SOUND 48  ");
		zx_print_str(15,10,"GAME      ");
		zx_print_str(16,10,"P1 CTRL   ");
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
		zx_print_str(16,20,joynames[player_joy]);

		menu_sel = menu_handle(10, 1, 14, 18, 0);
		ay_fx_play(ay_effect_10);
		sound_coin();

		switch ( menu_sel ) {
		case 0: //Sound 48/AY
            game_sound ^= spec128 ? GAME_SOUND_AY_FX_ON : GAME_SOUND_48_FX_ON;
			break;
		case 1: //Game type
			++game_type;
			if (game_type > GAME_RANDOM_TYPE) game_type = 0;
			break;
		case 2: //P1 Controls
			game_joystick_change;
			break;
		case 3:  //Back
			menu_sel = 0;
			menu_paint();
			cont = 0;
			//z80_delay_ms(250);
			break;
		}
	}
}




void menu(void) {
	/*PLAY MIDI TITLE*/
	menu_sel = 0;
	menu_top_paint();
	menu_paint();
	ay_reset();
	ay_midi_play(pb_midi_title);
	while (1) { //ETHERNAL LOOP
		while ((joyfunc1)(&k1) != 0);
		tmp_uc = menu_handle(12, 2, 14, 18, 1000);
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
			menu_config();
			break;
		}
	}
}

void menu_top_paint(void) {
	spr_draw_clear();
	game_paint_attrib(0);
	//Draws menu

	//game_fill_row(0,32);
	//Blue frame top
	menu_e(16, 0,30,156,1);
	//Green frame bottom
	menu_e(80, 0,30,159,255); //menu_e(159,-1);
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

void menu_paint(void) {
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
	zx_print_str(23,3, "CODED BY CGONZALEZ VER 1.9");
	tmp_uc = 0; //fix menu return
	game_paint_attrib(11);
}

void menu_e(unsigned char f_col,unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign) {
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

unsigned char menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout) {
	entry_time = zx_clock();
	s_lin1 = f_start + (menu_sel*f_inc);
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
			menu_sel = 0;
			//s_lin1 = f_start + (menu_sel*f_inc);
			menu_back(0);
			return 255;
		}

	};
	return	(unsigned char) ( s_lin1 -	f_start	 ) / f_inc;
}

void menu_back(unsigned char f_start) __z88dk_fastcall
{
	menu_sel = 0;
	s_lin1 = f_start;
	menu_top_paint();
	menu_paint();
	entry_time = zx_clock();
}
