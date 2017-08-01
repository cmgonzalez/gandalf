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
#include <input.h>
#include "game.h"
#include "globals.h"

unsigned char spec128;

//###############################################################################################
//#                                                                                             #
//# CONTROL VARIABLES                                                                           #
//#                                                                                             #
//###############################################################################################

const char *joynames[] = { "SJ1", "SJ2", "KB1", "KB2", "KEM", "CUR", "FUL" };
uint16_t (*joyfunc1)(udk_t *);			// pointer to joystick function Player 1
uint16_t (*joyfunc2)(udk_t *);			// pointer to joystick function Player 1
udk_t k1;
udk_t k2;
unsigned char dirs;
unsigned char tbuffer[7];			    // temporary buffer

const JOYFUNC control_method[7] = {
   (JOYFUNC)(in_stick_sinclair1),
   (JOYFUNC)(in_stick_sinclair2),
   (JOYFUNC)(in_stick_keyboard),
   (JOYFUNC)(in_stick_keyboard),
   (JOYFUNC)(in_stick_kempston),
   (JOYFUNC)(in_stick_cursor),
   (JOYFUNC)(in_stick_fuller)
};

struct Sprite {
   unsigned char  class;
   unsigned char  col;
   unsigned char  colint;
   unsigned char  lin;
};
struct Sprite sprites[8];

//SPRITES GAME ARRAYS
unsigned char class[8];					//CLASS OF SPRITE
unsigned char state[8];					//SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state_a[8];				//SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char tile[8];					//TILE
unsigned char lin[8];					//LINE
unsigned char col[8];					//COLUMNlisto
unsigned char colint[8];				//INTERNAL COLUMN/TILE INCREMENT
unsigned int  spr_timer[8];				//SPRITE GENERAL TIMER MILISECONDS
unsigned int  last_time[8];				//LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned char jump_lin[8];				//START JUMP LINE

// PLAYER ONLY
unsigned char hit_lin[2];				//HIT BRICK LINE
unsigned char hit_col[2];				//HIT BRICK COL
unsigned char player_jump_c[2];			//JUMP CNT TIME y = a*t^2 + b*t + c https://www.wired.com/2016/12/lets-go-physics-jumping-super-mario-run/
unsigned char sliding[2];				//SLIDING COUNTER
unsigned int  player_score[2];			//SCORE ARRAYS
unsigned int  player_next_extra[2];		//SCORE ARRAYS
unsigned int  player_joy[2];			//JOYSTICK ARRAYS

unsigned char	index_player;
unsigned char	sprite;
unsigned char	hit_count;

unsigned char	s_tile0;
unsigned char	s_tile1;
unsigned char	s_lin0;
unsigned char	s_lin1;
unsigned char	s_col0;
unsigned char	s_col1;
unsigned char	sprite_lin_inc_mul;
unsigned int	loop_count;
unsigned int	index1;
unsigned char	tmp;
unsigned char	tmp0;
unsigned char	tmp1;
unsigned char	tmp_uc;
signed char		tmp_sc;
unsigned int	tmp_ui;
unsigned char	enemies;
unsigned char	zx_val_asm;
unsigned char	attrib[4];
unsigned char	attrib_hl[4];
unsigned char	s_state;

unsigned char g_hit_left;
unsigned char g_hit_right;

unsigned int	curr_time;
unsigned int	entry_time;
unsigned int	frame_time;
unsigned int	osd_update_time;
unsigned int	osd_show_time;
unsigned char	spr_count;




//###############################################################################################
//#                                                                                             #
//# GAME VARIABLES                                                                              #
//#                                                                                             #
//###############################################################################################

unsigned char game_brick_tile;
unsigned char game_two_player;
unsigned char game_start_phase;
unsigned char game_god_mode;
unsigned char game_inmune;

unsigned char	game_sound;
unsigned char	game_over;
unsigned char	game_lives[2];
unsigned int	game_time_flipped;
unsigned int	game_time_fireball_start;
unsigned char	spr_water_clear;
unsigned int	spr_water_time;
unsigned char	game_bonus;
unsigned char	game_osd;
unsigned char	game_type;
unsigned char	game_menu_sel;


unsigned int	game_score_top;
//PHASE RELATED VARIABLES
unsigned char phase_quota[3];
unsigned char phase_left;
unsigned char phase_tot;
unsigned char phase_pop;
unsigned char phase_coins;
unsigned char phase_end;
unsigned char phase_curr;
unsigned char phase_angry;
unsigned char phase_bonus_total[2];
unsigned char screen_paper;
unsigned char screen_ink;
//SCORES OSD
unsigned char score_osd_lin[2];		//TO CLEAR POINTS ON SCREEN
unsigned char score_osd_col[2];		//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_update_time[2];	//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_tile[2];	//TO CLEAR POINTS ON SCREEN

//###############################################################################################
//#                                                                                             #
//# SCREEN GAME MAP                                                                             #
//#                                                                                             #
//###############################################################################################
//TILE MAP'S
//  0..23   BACKGROUND
// 24..31   OBJECTS (CAN BE PICKED)
// 32..39   BACKGROUND DEADLY
// 40..47   PLATFORMS (FLOOR ONLY)
// 48..51   SPECIAL PLATFORMS (STAIRS)
// 52..55   BRICKS (FLOOR AND CEIL)
// 56..63   SPECIAL BRICKS (FLORR AND CEIL CAN BE HITTED)
unsigned char scr_map[GAME_SCR_MAX_INDEX];
unsigned char scr_0[] = 	{141,0,7,8,139,0,24,130,0,9,10,159,0,11,12,131,0,56,132,0,131,68,70,132,68,65,66,67,56,65,67,141,0,56,134,0,5,6,135,0,56,133,0,1,2,3,4,131,0,133,66,131,44,136,66}	;
unsigned char scr_1[] = 	{141,0,7,8,135,0,24,0,25,0,26,0,27,9,10,151,0,11,12,136,0,131,68,70,134,68,160,0,5,6,141,0,1,2,3,4,131,0,133,66,131,44,136,66}	;
unsigned char scr_2[] = 	{141,0,7,8,140,0,70,0,9,10,171,0,68,70,68,131,0,65,66,67,56,65,67,141,0,56,134,0,5,6,133,0,11,12,56,133,0,1,2,3,4,131,0,133,66,131,44,136,66}	;
unsigned char scr_3[] = 	{130,0,7,8,131,0,137,69,130,0,9,10,131,0,133,69,130,0,27,136,0,133,69,154,0,57,137,69,134,0,57,137,69,134,0,57,137,69,134,0,57,137,69,132,66,130,44,138,66}	;
unsigned char scr_4[] = 	{138,69,131,0,7,8,131,0,26,0,69,130,0,27,0,69,131,0,9,10,133,0,69,132,0,69,150,0,69,130,44,131,69,130,44,130,69,68,70,130,68,130,0,138,69,134,0,138,69,5,6,132,0,138,69,2,3,4,131,0,144,66}	;
unsigned char scr_5[] = 	{0,7,8,142,0,9,10,131,0,53,133,54,55,140,0,23,143,0,22,135,0,53,133,54,55,0,22,138,0,23,132,0,22,138,0,22,132,0,22,130,0,53,54,55,133,0,22,132,0,22,131,0,23,130,0,144,66}	;
unsigned char scr_6[] = 	{152,0,7,8,142,0,9,10,192,0,5,6,133,0,11,12,134,0,1,2,3,4,131,0,133,66,131,44,136,66}	;
unsigned char scr_7[] = 	{130,0,135,37,132,0,7,8,141,0,21,9,10,131,0,48,62,131,42,62,48,131,0,20,134,0,62,131,43,62,131,0,131,19,130,0,68,130,0,133,68,131,0,17,13,18,132,0,48,133,46,48,0,19,131,16,19,139,0,17,13,14,13,18,139,0,17,13,15,13,18,0,144,66}	;
unsigned char scr_curr;
unsigned char map_width;
unsigned char map_heigth;

//###############################################################################################
//#                                                                                             #
//# ENEMIES ANIMATION SPEEDS - INTERRUPTS VALUES 50HZ                                           #
//#                                                                                             #
//###############################################################################################
unsigned char sprite_speed[] = {
	 0, // 0 NOT USED
	 4, // 1 SHELLCREEPER_GREEN
	 3, // 2 SHELLCREEPER_RED
	 1, // 3 SHELLCREEPER_BLUE
	 4, //04 SIDESTEPPER_RED
	 3, //05 SIDESTEPPER_GREEN
	 1, //06 SIDESTEPPER_MAGENTA
	 4, //07 SLIPICE
	 3, //08 COIN_1
	 3, //09 FIGHTERFLY
	 2, //10 FIREBALL_RED
	 1, //11 FIREBALL_GREEN
	 3, //12 COIN_2
	 0, //13
	 0, //14
	 0, //15
	 PLAYER_SPEED, //16 PLAYER
};
//TEMPORARY SPEED FOR SPEED UP
unsigned char sprite_speed_alt[8];
