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
#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H
extern unsigned char    spr_chktime(unsigned char *sprite) __z88dk_fastcall;
extern unsigned int	    spr_calc_index(unsigned char l, unsigned char c);
//extern unsigned char    spr_move_up_fast(void);
//extern unsigned char    spr_move_down_fast(void);
extern unsigned char    spr_move_up(void);
extern unsigned char    spr_move_down(void);
extern unsigned char    spr_move_horizontal(void);
extern unsigned char    spr_move_right(void);
extern unsigned char    spr_move_left(void);
extern unsigned char    spr_redraw(void);
extern unsigned char    spr_tile_dir(unsigned char f_tile, unsigned char f_sprite, unsigned char f_inc);
extern void             spr_destroy(unsigned char f_sprite) __z88dk_fastcall;
extern unsigned char    spr_tile(unsigned char f_sprite) __z88dk_fastcall;
extern void             spr_back_repaint(void);
extern void             spr_brick_anim(unsigned char f_hit) __z88dk_fastcall;
//extern void             spr_draw_clear(void);
extern void             spr_draw_index(unsigned int f_index);
extern void             spr_draw_row(unsigned char f_row);
extern unsigned char    spr_calc_hor(unsigned char f_sprite);
extern void             spr_tile_paint( unsigned char f_tile, unsigned char f_lin, unsigned char f_col );
unsigned char           spr_page_left();
unsigned char           spr_page_right();
unsigned char           spr_page_up();
unsigned char           spr_page_down();
extern void             spr_page_map(void);
extern void             spr_add_anim( unsigned char f_lin, unsigned char f_col, unsigned char f_tile, unsigned char f_end, unsigned char f_loops, unsigned char f_respawn );
extern void             spr_play_anim(void);
extern void             spr_play_bullets(void);
extern void             spr_turn_horizontal(void);
extern void             spr_init_anim_bullets(void);
extern void             spr_explode_bullet(unsigned char f_bullet) __z88dk_fastcall;
extern void             spr_btile_paint_back(void);

#endif
