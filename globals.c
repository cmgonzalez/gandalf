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
udk_t k1;

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
unsigned char player_hit_lin;				//HIT BRICK LINE
unsigned char player_hit_col;				//HIT BRICK COL
unsigned char player_jump_c;			//JUMP CNT TIME y = a*t^2 + b*t + c https://www.wired.com/2016/12/lets-go-physics-jumping-super-mario-run/
unsigned char player_slide;				  //SLIDING COUNTER
unsigned int  player_score;	     		//SCORE
unsigned int  player_joy;			      //JOYSTICK
unsigned char	player_lives;


unsigned char	sprite;
unsigned char	player_hit_count;

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

unsigned char g_player_hit_left;
unsigned char g_player_hit_right;

unsigned int	curr_time;
unsigned int	entry_time;
unsigned int	frame_time;
unsigned int	osd_update_time;
unsigned int	osd_show_time;
unsigned char	spr_count;
unsigned int  sprite_curr_index;


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
unsigned char	game_osd;
unsigned char	game_type;
unsigned char	game_menu_sel;


unsigned int	game_score_top;
//PHASE RELATED VARIABLES
unsigned char phase_tot;
unsigned char phase_pop;
unsigned char phase_coins;
unsigned char phase_end;
unsigned char phase_curr;
unsigned char phase_angry;
unsigned char screen_paper;
unsigned char screen_ink;

//SCORES OSD
unsigned char score_osd_lin;		//TO CLEAR POINTS ON SCREEN
unsigned char score_osd_col;		//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_update_time;	//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_tile;	//TO CLEAR POINTS ON SCREEN

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
unsigned char scr_curr;
unsigned char map_width;
unsigned char map_heigth;
unsigned char scr_0[] = 	{141,0,7,8,131,0,32,0,32,131,0,24,25,26,27,0,9,10,132,0,73,141,0,32,0,32,0,32,130,0,32,33,34,35,36,0,11,12,135,0,65,66,67,68,69,70,71,56,68,0,72,131,73,74,136,0,56,139,0,5,6,130,0,56,138,0,1,2,3,4,0,56,0,72,131,73,74,131,42,72,134,73,74}	;
unsigned char scr_1[] = 	{141,0,7,8,135,0,24,0,25,0,26,0,27,9,10,151,0,11,12,136,0,138,70,131,0,70,58,70,130,0,131,68,131,44,134,0,58,131,0,68,130,0,32,132,0,5,6,130,0,58,133,0,68,132,0,1,2,3,4,0,58,0,134,70,130,42,136,70}	;
unsigned char scr_2[] = 	{132,0,131,32,134,0,7,8,133,0,131,32,132,0,70,0,9,10,133,0,131,32,163,0,68,70,68,131,0,65,66,67,56,65,67,141,0,56,134,0,5,6,133,0,11,12,56,133,0,1,2,3,4,131,0,72,131,73,74,131,42,72,134,73,74}	;
unsigned char scr_3[] = 	{130,0,7,8,131,0,137,71,130,0,9,10,131,0,133,71,130,0,27,136,0,133,71,146,0,42,135,0,57,137,71,134,0,57,137,71,134,0,57,137,71,134,0,57,137,71,72,131,73,74,130,42,72,135,73,74}	;
unsigned char scr_4[] = 	{138,71,131,0,7,8,131,0,26,0,71,130,0,27,0,71,131,0,9,10,133,0,59,132,0,71,138,0,59,139,0,71,44,45,71,59,71,130,45,134,71,57,0,132,71,59,133,71,131,0,71,57,0,71,0,130,32,59,130,32,130,0,71,5,6,0,71,57,0,71,0,130,32,59,130,32,130,0,1,2,3,4,0,57,0,72,142,73,74}	;
unsigned char scr_5[] = 	{0,7,8,135,0,32,0,32,0,32,130,0,9,10,134,0,53,133,54,55,140,0,23,135,0,32,0,32,0,32,131,0,22,135,0,53,131,54,55,131,0,22,132,0,32,132,0,23,133,0,22,131,0,53,54,55,131,0,22,133,0,22,132,0,23,132,0,22,133,0,22,131,0,72,142,73,74}	;
unsigned char scr_6[] = 	{130,0,135,37,130,0,7,8,142,0,9,10,133,0,48,62,131,42,62,48,138,0,62,131,43,62,139,0,133,68,138,0,48,133,44,48,145,0,5,6,133,0,11,12,134,0,1,2,3,4,131,0,72,131,73,74,131,42,72,134,73,74}	;
unsigned char scr_7[] = 	{136,0,21,132,0,7,8,137,0,20,131,0,21,9,10,137,0,20,131,0,20,136,0,67,58,0,20,130,0,131,19,134,0,130,67,58,0,20,130,0,17,13,18,133,0,131,67,58,0,20,0,19,131,16,19,131,0,132,67,58,0,20,0,17,13,14,13,18,130,0,133,67,58,0,20,0,17,13,15,13,18,0,72,142,73,74}	;//###############################################################################################
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
