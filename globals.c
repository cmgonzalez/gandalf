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
unsigned char scr_obj0[GAME_SCR_MAX_INDEX]; // OBJECT PICK  0..15 SCREENS
unsigned char scr_obj1[GAME_SCR_MAX_INDEX]; // OBJECT PICK 16..31 SCREENS

unsigned char scr_curr;
unsigned char map_width;
unsigned char map_heigth;

// Compressed MAP is in globals_bank06.c

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
unsigned char class[8];   // CLASS OF SPRITE
unsigned char state[8];   // SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state_a[8]; // SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char tile[8];    // TILE
unsigned char lin[8];     // LINE

// unsigned char lin_max[7];      // MAX LINE FOR VERTICAL ENEMIES
// unsigned char lin_min[7];      // MIN LINE FOR VERTICAL ENEMIES

unsigned char col[8];      // COLUMN
unsigned char colint[8];   // INTERNAL COLUMN/TILE INCREMENT
unsigned int  spr_timer[8]; // SPRITE GENERAL TIMER MILISECONDS
unsigned int  last_time[8]; // LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned char jump_lin[8]; // START JUMP LINE

// PLAYER ONLY
unsigned char player_hit_lin; // HIT BRICK LINE
unsigned char player_hit_col; // HIT BRICK COL
unsigned char player_slide;   // SLIDING COUNTER
unsigned int  player_score;    // SCORE
unsigned int  player_joy;      // JOYSTICK
unsigned char player_lives;
unsigned char player_over_stair;
signed char   player_vel_x;
signed char   player_vel_y;
signed char   player_vel_y0;
signed char   player_vel_y1;
signed char   player_vel_inc;
unsigned char player_keys[4];
unsigned char player_hit_count;
// PLAYER ATTRIBUTES
unsigned char player_mana;
unsigned char player_vita;
// Maximun Mana n Vitality
unsigned char player_max_mana;
unsigned char player_max_vita;
// Player stats
unsigned char player_str;
unsigned char player_int;
unsigned char player_lvl;

unsigned char player_col_scr;
unsigned char player_lin_scr;

unsigned int player_hit_time;

signed   int  game_gravity;
unsigned char game_world;
unsigned char game_worldup;
unsigned char game_respawn_curr_time;
unsigned int fps;

unsigned char sprite;
unsigned char sprite_on_air;
unsigned char sprite_horizontal_check;

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

unsigned char mush_index[8];
unsigned char mush_class[8];

unsigned char spr_count;
unsigned char sprite_curr_index;
// ANIMATIONS
unsigned char anim_lin[8];
unsigned char anim_col[8];
unsigned char anim_tile[8];
unsigned char anim_int[8];
unsigned char anim_end[8];
unsigned char anim_loop[8];
unsigned char anim_respanwn[8];
// BULLETS
unsigned char bullet_class[8];
unsigned char bullet_max[8];
unsigned char bullet_lin0[8];
unsigned char bullet_col0[8];
unsigned char bullet_lin[8];
unsigned char bullet_col[8];
unsigned char bullet_tile[8];
unsigned char bullet_dir[8];
unsigned char bullet_colint[8];
unsigned char bullet_frames[8];
signed char bullet_vel[8];

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
unsigned int game_respawn_time[7]; // 8-1=7 Player don't respawn
unsigned char game_respawn_index[7];
unsigned char game_respawn_tile[7];
unsigned char game_start_scr;


// PHASE RELATED VARIABLES
unsigned char screen_paper;
unsigned char screen_ink;

unsigned char map_paper;
unsigned char map_paper_clr;
unsigned char map_paper_last;

//###############################################################################################
//# # # ENEMIES ANIMATION SPEEDS - INTERRUPTS VALUES 50HZ
//# #
//#
//###############################################################################################
unsigned char sprite_speed[] = {
    PLAYER_SPEED,
    0,
    SPEED_ENEMY_DRAGON,
    SPEED_ENEMY_BAT,
    SPEED_ENEMY_WYVERN,
    SPEED_ENEMY_SPIDER,
    SPEED_ENEMY_MUSH_VITA,
    SPEED_ENEMY_MUSH_MANA,
    SPEED_ENEMY_MUSH_EXTRA,
    0, // FIRE
    0, // SNOWMAN
    SPEED_ENEMY_SKELETON,
    SPEED_ENEMY_ORC,
    SPEED_ENEMY_WARG,
    SPEED_ENEMY_DWARF,
    SPEED_ENEMY_ELF,
    SPEED_ENEMY_PLANT,
    0, // TRAP
    SPEED_ENEMY_SNAKE,
    SPEED_ENEMY_BAT_H,
    0, // DEVIL
};

unsigned char sprite_frames[] = {
    FRAMES_PLAYER,
    0,
    FRAMES_ENEMY_DRAGON,
    FRAMES_ENEMY_BAT,
    FRAMES_ENEMY_WYVERN,
    FRAMES_ENEMY_SPIDER,
    FRAMES_ENEMY_MUSH_VITA,
    FRAMES_ENEMY_MUSH_MANA,
    FRAMES_ENEMY_MUSH_EXTRA,
    0,
    0,
    FRAMES_ENEMY_SKELETON,
    FRAMES_ENEMY_ORC,
    FRAMES_ENEMY_WARG,
    FRAMES_ENEMY_DWARF,
    FRAMES_ENEMY_ELF,
    FRAMES_ENEMY_PLANT,
    0, // TRAP
    FRAMES_ENEMY_SNAKE,
    FRAMES_ENEMY_BAT_H,
    0, // DEVIL
};

unsigned int player_lvl_table[] = {
    100,   500,   1000,  2000,  4000,  6000,  7000,  8000,  9000,  10000,
    12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000,
};
// TEMPORARY SPEED FOR SPEED UP
unsigned char sprite_speed_alt[8];
unsigned char draw_count;
