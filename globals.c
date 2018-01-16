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

uint16_t (*joyfunc1)(udk_t *); // pointer to joystick function Player 1
uint16_t (*joyfunc2)(udk_t *); // pointer to joystick function for game_2buttons
udk_t k1;
udk_t k2; // For game_2buttons

unsigned char dirs;
unsigned char dirs_alt;
unsigned char tbuffer[7]; // temporary buffer

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
unsigned int spr_timer[8]; // SPRITE GENERAL TIMER MILISECONDS
unsigned int last_time[8]; // LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned char jump_lin[8]; // START JUMP LINE

// PLAYER ONLY
unsigned char player_hit_lin; // HIT BRICK LINE
unsigned char player_hit_col; // HIT BRICK COL
unsigned char player_slide;   // SLIDING COUNTER
unsigned int player_score;    // SCORE
unsigned char player_lives;
unsigned char player_coins;
unsigned char player_onstair;
signed char player_vel_x;
signed char player_vel_y;
signed char player_vel_y0;
signed char player_vel_y1;
signed char player_vel_inc;
unsigned char player_keys[4];
unsigned char player_hit_count;
unsigned int player_brick_time;
unsigned char player_anim_stair;
unsigned char player_onfire;
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
unsigned int  player_hit_time;

signed int    game_gravity;
unsigned char game_world;
unsigned char game_boss;
unsigned char game_boss_hit;
unsigned char game_boss_alive;
unsigned char game_boss_fix;

unsigned char game_mush_count;

unsigned char game_worldup;
unsigned char game_respawn_curr_time;
unsigned char game_2buttons;
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
unsigned char attrib_osd[4];
unsigned char s_state;

unsigned char g_player_hit_left;
unsigned char g_player_hit_right;

unsigned int curr_time;
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
unsigned char bullet;
signed char   bullet_vel[8];

unsigned char boss_lin;
unsigned char boss_col;
unsigned char boss_inc;
unsigned char boss_tile;
unsigned int  boss_time;
unsigned int  boss_time_fire;
unsigned char boss_stat;

unsigned char spr_hack;

//###############################################################################################
//# # # GAME VARIABLES
//# #
//#
//###############################################################################################

unsigned char game_god_mode;
unsigned char game_inmune;

unsigned char game_sound;
unsigned char game_over;

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
    SPEED_ENEMY_GHOST,
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
    FRAMES_ENEMY_GHOST,
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

//Sprite tile and animation frames for init, used with GAME_TOTAL_CLASSES
unsigned char spr_map_tile[] = {
  SKELETON,TILE_ENEMY_SKELETON,DIRINC_ENEMY_SKELETON,
  ORC,TILE_ENEMY_ORC,DIRINC_ENEMY_ORC,
  WARG,TILE_ENEMY_WARG,DIRINC_ENEMY_WARG,
  DWARF,TILE_ENEMY_DWARF,DIRINC_ENEMY_DWARF,
  ELF,TILE_ENEMY_ELF,DIRINC_ENEMY_ELF,
  DRAGON,TILE_ENEMY_DRAGON,DIRINC_ENEMY_DRAGON,
  BAT,TILE_ENEMY_BAT,DIRINC_ENEMY_BAT,
  GHOST,TILE_ENEMY_GHOST,DIRINC_ENEMY_GHOST,
  SPIDER,TILE_ENEMY_SPIDER,DIRINC_ENEMY_SPIDER,
  PLANT,TILE_ENEMY_PLANT,DIRINC_ENEMY_PLANT,
  SNAKE,TILE_ENEMY_SNAKE,DIRINC_ENEMY_SNAKE,
  BAT_H,TILE_ENEMY_BAT_H,DIRINC_ENEMY_BAT_H,
  MUSHROOM_VITA,TILE_ENEMY_MUSH_VITA,DIRINC_ENEMY_MUSH_VITA,
  MUSHROOM_MANA,TILE_ENEMY_MUSH_MANA,DIRINC_ENEMY_MUSH_MANA,
  MUSHROOM_EXTRA,TILE_ENEMY_MUSH_EXTRA,DIRINC_ENEMY_MUSH_EXTRA
};

//Enemy intialization variables, based on index on map array, used along GAME_TOTAL_INDEX_CLASSES.
unsigned char spr_init[] = {
  INDEX_SKELETON_RIGHT, SKELETON, DIR_RIGHT,
  INDEX_SKELETON_LEFT, SKELETON, DIR_LEFT,
  INDEX_ORC_RIGHT, ORC, DIR_RIGHT,
  INDEX_ORC_LEFT, ORC, DIR_LEFT,
  INDEX_WARG_RIGHT, WARG, DIR_RIGHT,
  INDEX_WARG_LEFT, WARG, DIR_LEFT,
  INDEX_DWARF_RIGHT, DWARF, DIR_RIGHT,
  INDEX_DWARF_LEFT, DWARF, DIR_LEFT,
  INDEX_ELF_RIGHT, ELF, DIR_RIGHT,
  INDEX_ELF_LEFT, ELF, DIR_LEFT,
  INDEX_DRAGON_RIGHT, DRAGON, DIR_RIGHT,
  INDEX_DRAGON_LEFT, DRAGON, DIR_LEFT,
  INDEX_BAT, BAT, DIR_RIGHT,
  INDEX_GHOST, GHOST, DIR_RIGHT,
  INDEX_SPIDER, SPIDER, DIR_RIGHT,
  INDEX_PLANT, PLANT, DIR_RIGHT,
  INDEX_SNAKE, SNAKE, DIR_RIGHT,
  INDEX_BAT_H, BAT_H, DIR_RIGHT
};

unsigned int player_lvl_table[] = {
    100,   500,   1000,  2000,  4000,  6000,  7000,  8000,  9000,  10000,
    12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000
};
// TEMPORARY SPEED FOR SPEED UP
unsigned char sprite_speed_alt[8];
unsigned char draw_count;

unsigned char key_map[] = {
    13,  32,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  97,
    98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
};
unsigned int scan_map[] = {
    IN_KEY_SCANCODE_ENTER, IN_KEY_SCANCODE_SPACE, IN_KEY_SCANCODE_0,
    IN_KEY_SCANCODE_1,     IN_KEY_SCANCODE_2,     IN_KEY_SCANCODE_3,
    IN_KEY_SCANCODE_4,     IN_KEY_SCANCODE_5,     IN_KEY_SCANCODE_6,
    IN_KEY_SCANCODE_7,     IN_KEY_SCANCODE_8,     IN_KEY_SCANCODE_9,
    IN_KEY_SCANCODE_a,     IN_KEY_SCANCODE_b,     IN_KEY_SCANCODE_c,
    IN_KEY_SCANCODE_d,     IN_KEY_SCANCODE_e,     IN_KEY_SCANCODE_f,
    IN_KEY_SCANCODE_g,     IN_KEY_SCANCODE_h,     IN_KEY_SCANCODE_i,
    IN_KEY_SCANCODE_j,     IN_KEY_SCANCODE_k,     IN_KEY_SCANCODE_l,
    IN_KEY_SCANCODE_m,     IN_KEY_SCANCODE_n,     IN_KEY_SCANCODE_o,
    IN_KEY_SCANCODE_p,     IN_KEY_SCANCODE_q,     IN_KEY_SCANCODE_r,
    IN_KEY_SCANCODE_s,     IN_KEY_SCANCODE_t,     IN_KEY_SCANCODE_u,
    IN_KEY_SCANCODE_v,     IN_KEY_SCANCODE_w,     IN_KEY_SCANCODE_x,
    IN_KEY_SCANCODE_y,     IN_KEY_SCANCODE_z
};
