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
extern void          game_draw_screen(void);
extern void          game_respawn(void);
extern void          game_fps(void);
extern void          game_cls(void);
extern void          game_fill_row(unsigned char f_row, unsigned char f_asc) __z88dk_callee;
extern void          game_print_footer(void);
extern void			     game_update_stats(void);
extern void			     game_start_timer(void);
extern void          game_set_checkpoint();
extern void			     game_round_init(void);
extern void			     game_loop(void);
extern unsigned int  game_check_map(unsigned char f_lin, unsigned char f_col);
extern unsigned char game_check_cell(unsigned int *f_index) __z88dk_fastcall;
extern void			     game_print_score(void);
extern void          game_paint_attrib( unsigned char *f_attrib[], char f_start, unsigned char f_end, unsigned char f_lin);
extern void			     game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_miliseconds, unsigned char skip);
extern void			     game_rotate_attrib(void);
extern void			     game_rotate_attrib_osd(void);
extern unsigned char game_check_time(unsigned int *start, unsigned char lapse);
extern void          game_print_header(void);
extern void          game_add_enemy( unsigned char enemy_tile_index) __z88dk_fastcall;
extern unsigned char game_shoot_fire( unsigned char f_sprite, unsigned char f_tile);
extern unsigned char game_shoot_fire_boss( unsigned char f_tile  );
extern void          game_obj_set( unsigned int f_index ) __z88dk_fastcall;
extern unsigned char game_obj_chk( unsigned int f_index ) __z88dk_fastcall;
extern void          game_obj_clear( );
extern void          game_boss_kill(void);
extern void          game_boss_clear( );
extern void          game_attribs( );
unsigned char        game_match_back(unsigned int f_index ) __z88dk_fastcall;
unsigned char        game_match_back_strict(unsigned int f_index ) __z88dk_fastcall;
#endif
