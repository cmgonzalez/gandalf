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
#ifndef GAME_ENEMIES_H
#define GAME_ENEMIES_H

extern void          enemy_vertical(void);
extern void          enemy_horizontal(void);
extern void          enemy_walk(void);
extern void          enemy_static(void);
extern void          enemy_coin1(void);
extern void          enemy_hit(void);
extern void          enemy_turn(void);
extern void          enemy_move(void);
extern void          enemy_init(unsigned char f_lin,unsigned  char f_col,unsigned  char f_class,unsigned  char f_dir);
extern void          enemy_respawn(unsigned char f_anim);
extern void          enemy_avoid_fall(void);
extern unsigned char enemy_avoid_dead(void);
extern void          boss_turn(void);
#endif
