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
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "globals.h"


extern void          game_fill_row(unsigned char f_row, unsigned char f_asc) __z88dk_callee;
extern void          game_print_footer(void);
extern void			     game_print_lives(void);
extern void			     game_tick(void);
extern void			     game_start_timer(void);
extern void			     game_phase_init(void);
extern void			     game_loop(void);
extern unsigned char game_check_maze(int f_index) __z88dk_fastcall;
extern unsigned char game_check_maze_floor(int f_index) __z88dk_fastcall;
extern unsigned char game_enemy_add_get_index(unsigned char f_search) __z88dk_fastcall;
extern void          game_freeze(unsigned char f_lin, unsigned char f_col);
extern void          game_freeze_all(void);
extern void			     game_unfreeze_all(void);
extern void			     game_print_score(void);
extern void			     game_menu_back(unsigned char f_start) __z88dk_fastcall;
extern void			     game_menu_e(unsigned char f_col, unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign);
extern unsigned char game_menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout);
extern void          game_menu_top_paint(void);
extern void 		     game_menu_paint(void);
extern void			     game_paint_attrib(unsigned char e_r1) __z88dk_fastcall;
extern void			     game_paint_attrib_lin(unsigned char f_start,unsigned char f_end,unsigned char f_lin);
extern void			     game_paint_attrib_lin_h(unsigned char f_start,unsigned char f_end,unsigned char f_lin);
extern void			     game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_miliseconds	);
extern void			     game_rotate_attrib(void);
extern void			     game_menu(void);
extern void			     game_menu_config(void);
extern unsigned char game_check_time(unsigned int start, unsigned int lapse);
extern void          game_bonus_summary(void);
extern void          game_bonus_summary_player(unsigned char f_index) __z88dk_fastcall;
extern void          game_bonus_clock(void);
extern void          game_phase_print_score_back(void);
extern void          game_score_osd(unsigned char);
extern void          game_end(void);
extern void          game_print_header(void);
extern void          game_joystick_change(unsigned char f_player_index) __z88dk_fastcall;
extern void          game_joystick_set(void);
extern void          game_joystick_set_menu(void);


#endif