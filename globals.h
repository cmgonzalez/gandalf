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
#ifndef GLOBALS_H
#define GLOBALS_H
#include <input.h>

#define GAME_ROWS                     11
#define GAME_SCR_MAX_INDEX            160

/* Map foreground tiles */
#define TILE_EMPTY                    0
#define TILE_FLOOR                    48 //START FLOOR TILES
#define TILE_CEIL                     60 //START CEILS TILES
#define TILE_END                      84 //END OF TILES
#define TILE_ITEM_S                   24 //START OF ITEMS
#define TILE_ITEM_E                   39 //END OF ITEMS
#define TILE_STAIR_S                  56 //START OF STAIRS
#define TILE_STAIR_E                  60 //END OF STAIRS


#define TILE_ENEMY_SKELETON           132
#define FRAMES_ENEMY_SKELETON         4
#define SPEED_ENEMY_SKELETON          4
#define DIRINC_ENEMY_SKELETON         0
#define INDEX_SKELETON_LEFT           84
#define INDEX_SKELETON_RIGHT          85


#define TILE_ENEMY_ORC                136
#define FRAMES_ENEMY_ORC              4
#define SPEED_ENEMY_ORC               3
#define DIRINC_ENEMY_ORC              0
#define INDEX_ORC_LEFT                86
#define INDEX_ORC_RIGHT               87

#define TILE_ENEMY_WARG               140
#define FRAMES_ENEMY_WARG             2
#define SPEED_ENEMY_WARG              1
#define DIRINC_ENEMY_WARG             2
#define INDEX_WARG_LEFT               88
#define INDEX_WARG_RIGHT              89

#define TILE_ENEMY_DWARF              144
#define FRAMES_ENEMY_DWARF            4
#define SPEED_ENEMY_DWARF             4
#define DIRINC_ENEMY_DWARF            0
#define INDEX_DWARF_LEFT              90
#define INDEX_DWARF_RIGHT             91

#define TILE_ENEMY_ELF                148
#define FRAMES_ENEMY_ELF              4
#define SPEED_ENEMY_ELF               6
#define DIRINC_ENEMY_ELF              4
#define INDEX_ELF_LEFT                92
#define INDEX_ELF_RIGHT               93

/* Enemies indexes */
#define SKELETON                      1
#define ORC                           2
#define WARG                          3
#define DWARF                         4
#define ELF                           5

#define PLAYER                        16


//TILE MAP'S
//  0..23   BACKGROUND
// 24..31   OBJECTS (CAN BE PICKED)
// 32..39   BACKGROUND DEADLY
// 40..47   PLATFORMS (FLOOR ONLY)
// 48..51   SPECIAL PLATFORMS (STAIRS)
// 52..55   BRICKS (FLOOR AND CEIL)
// 56..63   SPECIAL BRICKS (FLORR AND CEIL CAN BE HITTED)

/* Player 1 tiles */
#define TILE_P1_STANR                 100
#define TILE_P1_RIGHT                 84
#define TILE_P1_STAIR                 102
#define TILE_P1_JUMPR                 88
#define TILE_P1_LEN                   8 //LEN OF PLAYER SPRITES FOR EACH DIRECTION
#define TILE_ANIM_PICK                120
#define TILE_ANIM_FIRE                123
#define TILE_P1_KILL                  64
#define TILE_P1_HITR                  64

#define TILE_FIREBALL_R               112
#define TILE_FIREBALL_L               114

#define FRAMES_PLAYER                 4
#define FRAMES_PLAYER_STAIR           4

/* Screen tiles */
#define TILE_BRICK                    48


/* Player indexes */
#define SPR_P1                        7
#define SPR_P2                        6
/* Screen variables */
#define SPR_COLINT                    4   /* Internal colum increment */
#define SCR_COLI                      1   /* Start column */
#define SCR_COLS                      32  /* Columns max */
#define SCR_COLS_M                    30  /* Columns max */
#define SCR_ROWI                      0   /* Start row */
#define SCR_ROWS                      23  /* Rows max */
#define SCR_LINS                      184 /* Lins max */
#define PAPER                         PAPER_BLUE | INK_BLUE | BRIGHT/* Screen paper */
#define GAME_LIN_FLOOR                144
#define GAME_LIN_TOP_PLATFORM         48

#define ST_STAND_R                    0
#define ST_STAND_L                    1
#define ST_TURN_R                     10
#define ST_TURN_L                     11

/* Starting positions*/
#define DIR_LEFT                      255
#define DIR_RIGHT                     1
/*Stats (bit position number)*/
#define STAT_JUMP                     0
#define STAT_FALL                     1
#define STAT_DIRR                     2
#define STAT_DIRL                     3
#define STAT_HIT                      4
#define STAT_KILL                     5
#define STAT_ANGRY                    6
#define STAT_UPGR                     7
/*Stats alt (bit position number)*/
#define STAT_LOCK                     0
#define STAT_TURN                     1
#define STAT_PUSH                     2
#define STAT_LDIRL                    3
#define STAT_LDIRR                    4
#define STAT_INERT                    5
#define STAT_RESTART                  6
#define STAT_HITBRICK                 7
/* General */
#define SPRITE_FALL_SPEED             3  /* Speed for falling from the screen after a kill */
#define SPRITE_RESTART_SPEED          8
#define SPRITE_LIN_INC                2
#define SPRITE_HEIGHT                 16
#define SPRITE_VCOL_MARGIN            14 //VERTICAL COLLISION MARGIN

/* Player */
#define PLAYER_SLIDE_NORMAL           2
#define PLAYER_SLIDE_ICE              1

#define PLAYER_HIT_BRICK_TIME         4

#define PLAYER_HCOL_MARGIN            2  //HORIZONTAL COLLISION MARGIN
#define PLAYER_HCOL_MARGIN_INT        4  //HORIZONTAL COLLISION MARGIN INTERNAL (1 COL = 3 INT)
#define PLAYER_SPEED                  2
#define PLAYER_JUMP_SPEED             2
#define PLAYER_FALL_SPEED             2
#define PLAYER_INERT_TIME             8  /* Time for Player Inertia */
#define PLAYER_ANIM_HIT_TIME          40 /* Time for Player Hit Sprite display after kill */

#define PLAYER_JUMP_MAX               5  /* Max Jump Time 10 frames */
#define PLAYER_JUMP_SUSPEND           6


/* Enemies */
#define ENEMIES_MAX                   6   //MAX QUANTITY OF ENEMIES ON SCREEN (0->6)
#define ENEMIES_MAXJUMP               12 //MAX JUMP WHEN HIT
#define ENEMY_JUMP_SPEED              1
#define ENEMY_FALL_SPEED              1
#define ENEMY_KILLED_SPEED            8


/* Game times */
#define TIME_EVENT                    100
#define TIME_ANIM                     6
#define TIME_BULLETS                  2
#define GAME_RANDOM_TYPE              2

/* Sound Mode Variables */
#define GAME_SOUND_48_FX_ON          0x01
#define GAME_SOUND_48_FX_OFF         0xfe
#define GAME_SOUND_48_MUS_ON         0x02
#define GAME_SOUND_48_MUS_OFF        0xfd
#define GAME_SOUND_AY_FX_ON          0x04
#define GAME_SOUND_AY_FX_OFF         0xfb
#define GAME_SOUND_AY_MUS_ON         0x08
#define GAME_SOUND_AY_MUS_OFF        0xf7


typedef uint16_t (*JOYFUNC)(udk_t *);
#endif


extern unsigned char spec128;
extern unsigned char btiles[];
extern uint16_t (*joyfunc1)(udk_t *);
extern udk_t k1;


extern unsigned char dirs;
extern unsigned char tbuffer[7];
extern unsigned char class[8];
extern unsigned char state[8];
extern unsigned char state_a[8];
extern unsigned char tile[8];
extern unsigned char lin[8];
extern unsigned char col[8];
extern unsigned char colint[8];
extern unsigned char player_hit_lin;
extern unsigned char player_hit_col;
extern unsigned char jump_lin[8];
extern unsigned int  spr_timer[8];
//extern unsigned int  spr_timer_c[8];
extern unsigned char player_slide;
extern unsigned int  last_time[8];
extern unsigned int  player_score;
extern unsigned int  player_joy;

extern signed char player_vel_x;
extern signed int player_vel_y;
extern signed int player_vel_y0;
extern signed int player_vel_y1;
extern signed char player_vel_inc;
extern signed int game_gravity;

extern unsigned char	sprite;
extern unsigned char sprite_on_air;
extern unsigned char sprite_horizontal_check;

extern unsigned char	player_hit_count;
extern unsigned char	s_tile0;
extern unsigned char	s_tile1;
extern unsigned char	s_lin0;
extern unsigned char	s_lin1;
extern unsigned char	s_col0;
extern unsigned char	s_col1;
extern unsigned int		loop_count;
extern unsigned int		index0;
extern unsigned int		index1;
extern unsigned char	tmp;
extern unsigned char	tmp0;
extern unsigned char	tmp1;
extern unsigned char	tmp_uc;
extern signed char		tmp_sc;
extern unsigned int		tmp_ui;
extern unsigned char	enemies;
extern unsigned char	zx_val_asm;
extern unsigned char	attrib[4];
extern unsigned char	attrib_hl[4];
extern unsigned char	s_state;
extern unsigned int		curr_time;
extern unsigned int		entry_time;
extern unsigned int		frame_time;
extern unsigned int  	anim_time;
extern unsigned int  	bullet_time;
extern unsigned char	spr_count;
extern unsigned char   sprite_curr_index;


extern unsigned char 	game_brick_tile;
extern unsigned char 	game_two_player;
extern unsigned char  game_start_phase;
extern unsigned char  game_god_mode;
extern unsigned char  game_inmune;
extern unsigned char	game_sound;

extern unsigned char	game_over;
extern unsigned char	player_lives;
extern unsigned int		game_score_top;
extern unsigned char	game_type;
extern unsigned char	game_menu_sel;
extern unsigned char  player_over_stair;


extern unsigned char screen_paper, screen_ink;
extern unsigned char sprite_speed[];
extern unsigned char sprite_frames[];
extern unsigned char sprite_speed_alt[8];
extern unsigned char scr_map[];
extern unsigned char scr_0[];
extern unsigned char scr_1[];
extern unsigned char scr_2[];
extern unsigned char scr_3[];
extern unsigned char scr_4[];
extern unsigned char scr_5[];
extern unsigned char scr_6[];
extern unsigned char scr_7[];


extern unsigned char scr_curr;
extern unsigned char map_width;
extern unsigned char map_heigth;
extern unsigned char phase_tot;
extern unsigned char phase_pop;
extern unsigned char phase_coins;
extern unsigned char phase_end;
extern unsigned char phase_curr;
extern unsigned char phase_angry;


extern unsigned char anim_lin[8];
extern unsigned char anim_col[8];
extern unsigned char anim_tile[8];
extern unsigned char anim_int[8];
extern unsigned char anim_end[8];


//BULLETS
//BULLETS
extern unsigned char bullet_lin[8];
extern unsigned char bullet_col[8];
extern unsigned char bullet_tile[8];
extern unsigned char bullet_dir[8];
extern unsigned char bullet_colint[8];
extern unsigned char bullet_frames[8];


extern const JOYFUNC control_method[7];
extern unsigned char initials[8];
extern const char *joynames[];

extern unsigned char g_player_hit_left;
extern unsigned char g_player_hit_right;
