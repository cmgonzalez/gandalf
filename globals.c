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
#include "game.h"
#include "globals.h"
#include <input.h>

unsigned char spec128;
//###############################################################################################
//# # # SCREEN GAME MAP
//# #
//#
//###############################################################################################
// TILE MAP'S
//  0..23   BACKGROUND
// 24..31   OBJECTS (CAN BE PICKED)
// 32..39   BACKGROUND DEADLY
// 40..47   PLATFORMS (FLOOR ONLY)
// 48..51   SPECIAL PLATFORMS (STAIRS)
// 52..55   BRICKS (FLOOR AND CEIL)
// 56..63   SPECIAL BRICKS (FLORR AND CEIL CAN BE HITTED)
unsigned char scr_map[GAME_SCR_MAX_INDEX];
unsigned char scr_obj[GAME_SCR_MAX_INDEX]; //TODO TO BE USED AS INT, TO STORE OBJECT PICKING ON WORLD

unsigned char scr_curr;
unsigned char map_width;
unsigned char map_heigth;
//Compressed MAP


unsigned char scr_0[] = 	{135,32,58,143,32,58,143,32,58,143,32,58,136,32,135,70,58,136,70,135,32,58,143,32,58,143,32,58,136,32,144,70}	;
unsigned char scr_1[] = 	{141,0,7,8,142,0,9,10,130,0,32,0,32,0,32,0,32,149,0,84,134,0,87,0,11,12,132,0,130,70,58,139,70,132,0,58,130,0,68,131,32,44,130,0,44,0,5,6,130,0,58,130,0,130,32,68,32,132,0,1,2,3,4,87,58,0,134,70,130,42,136,70}	;
unsigned char scr_2[] = 	{135,32,134,0,7,8,0,135,32,132,0,32,0,9,10,0,135,32,0,48,130,0,70,133,0,93,130,0,92,133,0,131,32,131,0,131,70,56,130,70,132,0,131,70,134,0,56,143,0,56,134,0,5,6,133,0,11,12,56,133,0,1,2,3,4,0,88,0,72,142,73,74}	;
unsigned char scr_3[] = 	{130,0,7,8,131,0,137,71,130,0,9,10,131,0,133,71,0,130,32,136,0,133,71,142,0,92,139,0,57,134,71,130,42,71,134,0,57,137,71,134,0,57,137,71,0,130,32,132,0,137,71,72,131,73,74,130,42,72,135,73,74}	;
unsigned char scr_4[] = 	{138,71,131,0,7,8,131,0,32,0,71,130,0,32,0,71,131,0,9,10,133,0,59,132,0,71,138,0,59,135,0,92,131,0,71,130,42,71,59,71,130,42,134,71,57,0,132,71,59,133,71,131,0,71,57,0,71,0,130,32,59,130,32,130,0,71,5,6,0,71,57,0,71,0,130,32,59,130,32,0,92,1,2,3,4,0,57,0,72,142,73,74}	;
unsigned char scr_5[] = 	{0,7,8,142,0,9,10,167,0,32,0,32,88,32,133,0,32,0,32,88,32,0,53,131,54,55,0,32,0,32,0,53,131,54,55,131,0,23,131,0,53,54,55,131,0,23,133,0,22,132,0,23,132,0,22,132,0,92,22,130,0,88,72,142,73,74}	;
unsigned char scr_6[] = 	{133,0,45,32,45,0,45,32,45,0,45,32,133,0,92,134,0,86,133,0,142,48,134,0,87,136,0,32,130,0,130,48,0,137,48,0,48,138,0,92,135,0,138,48,130,0,130,48,145,0,144,70}	;
unsigned char scr_7[] = 	{136,0,21,132,0,7,8,137,0,20,131,0,21,9,10,137,0,20,131,0,20,136,0,67,58,0,20,130,0,131,19,134,0,130,67,58,0,20,130,0,17,13,18,133,0,131,67,58,0,20,0,19,131,16,19,131,0,132,67,58,0,20,0,17,13,14,13,18,130,0,133,67,58,0,20,0,17,13,15,13,18,0,72,142,73,74}	;

//#########################################################################################
//# # # CONTROL VARIABLES
//# #
//#
//###############################################################################################

const char *joynames[] = {"SJ1", "SJ2", "KB1", "KB2", "KEM", "CUR", "FUL"};
uint16_t (*joyfunc1)(udk_t *); // pointer to joystick function Player 1
udk_t k1;

unsigned char dirs;
unsigned char tbuffer[7]; // temporary buffer

const JOYFUNC control_method[7] = {
    (JOYFUNC)(in_stick_sinclair1), (JOYFUNC)(in_stick_sinclair2),
    (JOYFUNC)(in_stick_keyboard),  (JOYFUNC)(in_stick_keyboard),
    (JOYFUNC)(in_stick_kempston),  (JOYFUNC)(in_stick_cursor),
    (JOYFUNC)(in_stick_fuller)};

// SPRITES GAME ARRAYS
unsigned char class[8];    // CLASS OF SPRITE
unsigned char state[8];    // SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state_a[8];  // SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char tile[8];     // TILE
unsigned char lin[8];      // LINE

//unsigned char lin_max[7];      // MAX LINE FOR VERTICAL ENEMIES
//unsigned char lin_min[7];      // MIN LINE FOR VERTICAL ENEMIES

unsigned char col[8];      // COLUMN
unsigned char colint[8];   // INTERNAL COLUMN/TILE INCREMENT
unsigned int spr_timer[8]; // SPRITE GENERAL TIMER MILISECONDS
unsigned int last_time[8]; // LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned char jump_lin[8]; // START JUMP LINE

// PLAYER ONLY
unsigned char player_hit_lin; // HIT BRICK LINE
unsigned char player_hit_col; // HIT BRICK COL
unsigned char player_slide; // SLIDING COUNTER
unsigned int player_score;  // SCORE
unsigned int player_joy;    // JOYSTICK
unsigned char player_lives;
unsigned char player_over_stair;

signed char player_vel_x;
//signed int player_vel_y;
//signed int player_vel_y0;
//signed int player_vel_y1;

signed char player_vel_y;
signed char player_vel_y0;
signed char player_vel_y1;

signed char player_vel_inc;

signed int game_gravity;

unsigned char sprite;
unsigned char sprite_on_air;
unsigned char sprite_horizontal_check;

unsigned char player_hit_count;

unsigned char s_tile0;
unsigned char s_tile1;
unsigned char s_lin0;
unsigned char s_lin1;
unsigned char s_col0;
unsigned char s_col1;
unsigned int loop_count;
unsigned int index0;
unsigned int index1;
unsigned char tmp;
unsigned char tmp0;
unsigned char tmp1;
unsigned char tmp_uc;
signed char tmp_sc;
unsigned int tmp_ui;
unsigned char enemies;
unsigned char zx_val_asm;
unsigned char attrib[4];
unsigned char attrib_hl[4];
unsigned char s_state;

unsigned char g_player_hit_left;
unsigned char g_player_hit_right;

unsigned int curr_time;
unsigned int entry_time;
unsigned int frame_time;
unsigned int anim_time;
unsigned int bullet_time;

unsigned char anim_count;
unsigned char bullet_count;



unsigned char spr_count;
unsigned char sprite_curr_index;
//ANIMATIONS
unsigned char anim_lin[8];
unsigned char anim_col[8];
unsigned char anim_tile[8];
unsigned char anim_int[8];
unsigned char anim_end[8];

//BULLETS
unsigned char bullet_lin[8];
unsigned char bullet_col[8];
unsigned char bullet_tile[8];
unsigned char bullet_dir[8];
unsigned char bullet_colint[8];
unsigned char bullet_frames[8];



//###############################################################################################
//# # # GAME VARIABLES
//# #
//#
//###############################################################################################

unsigned char game_brick_tile;
unsigned char game_god_mode;
unsigned char game_inmune;

unsigned char game_sound;
unsigned char game_over;
unsigned char game_menu_sel;

unsigned int game_score_top;
// PHASE RELATED VARIABLES
unsigned char phase_curr;
unsigned char screen_paper;
unsigned char screen_ink;



//###############################################################################################
//# # # ENEMIES ANIMATION SPEEDS - INTERRUPTS VALUES 50HZ
//# #
//#
//###############################################################################################
unsigned char sprite_speed[] = {
    0,
    SPEED_ENEMY_DRAGON,
    SPEED_ENEMY_BAT,
    SPEED_ENEMY_WYVERN,
    SPEED_ENEMY_SPIDER,

    SPEED_ENEMY_SKELETON,
    SPEED_ENEMY_ORC,
    SPEED_ENEMY_WARG,
    SPEED_ENEMY_DWARF,
    SPEED_ENEMY_ELF,

    SPEED_ENEMY_PLANT,
    SPEED_ENEMY_SNAKE,
    0,            // 12 ???
    0,            // 13 ???
    0,            // 14 ???
    0,            // 15 ???
    PLAYER_SPEED, // 16 PLAYER
};

unsigned char sprite_frames[] = {
    0,
    FRAMES_ENEMY_DRAGON,
    FRAMES_ENEMY_BAT,
    FRAMES_ENEMY_WYVERN,
    FRAMES_ENEMY_SPIDER,

    FRAMES_ENEMY_SKELETON,
    FRAMES_ENEMY_ORC,
    FRAMES_ENEMY_WARG,
    FRAMES_ENEMY_DWARF,
    FRAMES_ENEMY_ELF,

    FRAMES_ENEMY_PLANT,
    FRAMES_ENEMY_SNAKE,
    0,            // 12 ???
    0,            // 13 ???
    0,            // 14 ???
    0,            // 15 ???
    FRAMES_PLAYER,
};

// TEMPORARY SPEED FOR SPEED UP
unsigned char sprite_speed_alt[8];
unsigned char draw_count;
