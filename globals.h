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
#ifndef GLOBALS_H
#define GLOBALS_H
#include <input.h>

#define GAME_ROWS                     11
#define GAME_SCR_MAX_INDEX            160


#define GAME_GRAVITY                  1
#define GAME_VELOCITY                 -8
#define GAME_MIN_VELOCITY             -7
#define GAME_START_LIVES              3
#define GAME_START_MAX_MANA           100
#define GAME_START_MAX_VITA           100
#define GAME_MAX_LEVEL                10
#define GAME_MAX_MANA                 200
#define GAME_MAX_VITA                 200
#define GAME_COLLISION_TIME           30 //TIME BETWN COLISION CHECKS

/* Map foreground tiles */

#define TILE_EMPTY                    0
#define TILE_EMPTY_DARK               29 //INTERIOR BACK 2
#define TILE_EMPTY_DARK_A             24 //INTERIOR BACK 2
#define TILE_EMPTY_DARK_B             13 //BRICKS BACK
#define TILE_WORLD_EXIT               57
#define TILE_BACKGROUND_S             0  //START OF BACKGROUND
#define TILE_BACKGROUND_E             35 //END OF BACKGROUND

#define TILE_CHECKPOINT               21

#define TILE_ITEM_S                   36 //START OF ITEMS
#define TILE_ITEM_E                   47 //END OF ITEMS

#define TILE_FLOOR                    58 //START FLOOR TILES
#define TILE_CEIL                     72 //START CEILS TILES

#define TILE_DOOR_E                   76 //END OF DOOR TILES
#define TILE_END                      96 //END OF TILES

#define TILE_STAIR_S                  68 //START OF STAIRS
#define TILE_STAIR_E                  71 //END OF STAIRS

#define TILE_DIRT                     77 //CAN BE DESTROYED BY FIREBALLS
#define TILE_SPECIAL                  88 //HAVE A MUSHROM VITA / MANA
#define TILE_HIDDEN_BRICK             93 //HIDDEN TREASURE (COINS LIKE MARIO?)
#define TILE_NORMAL_BRICK             84
#define TILE_NOSPECIAL                79 //AFTER HIT

#define TILE_HIT                      69 //UP FROM THIS CAN BE HIT BY PLAYER JUMP
#define TILE_STOPPER                  35 //TURN ENEMY DIRECTION


#define TILE_BRICK1                   62
#define TILE_BRICK2                   63
#define TILE_BRICK3                   64

/* Enemies Class indexes */
#define PLAYER                        0xFF
#define GOTA                          1 //
#define DRAGON                        2 //
#define BAT                           3 //
#define GHOST                         4 //
#define SPIDER                        5 //
#define MUSHROOM_VITA                 6  //
#define MUSHROOM_MANA                 7  //
#define MUSHROOM_EXTRA                8  //
#define FIRE                          9  //
#define PIRANHA                       10 //
#define SKELETON                      11 //
#define ORC                           12 //
#define WARG                          13 //
#define DWARF                         14 //
#define ELF                           15 //
#define PLANT                         16 //
#define TRAP                          17 //
#define SNAKE                         18 //
#define BAT_H                         19 //
#define DEVIL                         20
#define FIRE_S                        21 //

#define GAME_TOTAL_CLASSES            20  //Total class of enemies
#define GAME_TOTAL_INDEX_CLASSES      23  //Total class of enemies variations, the same enemy can be left/right etc...

#define E_HORIZONTAL                  1
#define E_VERTICAL                    2
#define E_WALK                        3
#define E_STATIC                      4
#define E_GHOST                       5
#define E_GOTA                        6


//MUSHROM'S INIT
#define INDEX_MUSH_VITA_L             121
#define INDEX_MUSH_VITA_R             122

#define INDEX_MUSH_MANA_L             123
#define INDEX_MUSH_MANA_R             124

#define INDEX_MUSH_EXTRA_L            125
#define INDEX_MUSH_EXTRA_R            126

//BOSSES
#define INDEX_ENEMY_BOSS1             120
#define TILE_ENEMY_BOSS1              200



#define TILE_TITLE                    204
#define TILE_ANIM                     128




//TILE MAP'S
//  0..23   BACKGROUND
// 24..31   OBJECTS (CAN BE PICKED)
// 32..39   BACKGROUND DEADLY
// 40..47   PLATFORMS (FLOOR ONLY)
// 48..51   SPECIAL PLATFORMS (STAIRS)
// 52..55   BRICKS (FLOOR AND CEIL)
// 56..63   SPECIAL BRICKS (FLORR AND CEIL CAN BE HITTED)

/* Player 1 tiles */
#define TILE_P1_STANR                 112
#define TILE_P1_RIGHT                 96
#define TILE_P1_STAIR                 114
#define TILE_P1_JUMPR                 100
#define TILE_P1_LEN                   8 //LEN OF PLAYER SPRITES FOR EACH DIRECTION
#define TILE_ANIM_PICK                132
#define TILE_ANIM_FIRE                135
#define TILE_ANIM_RESPAWN             138
#define TILE_ANIM_DEAD                141
#define TILE_P1_KILL                  76
#define TILE_P1_HITR                  76
#define TILE_P1_FIRE_R                208
#define TILE_P1_FIRE_L                209

#define TILE_FIREBALL                 120
#define TILE_ARROW                    124
#define TILE_AXE                      128

#define TILE_KEY_WHITE                36
#define TILE_KEY_RED                  37
#define TILE_KEY_GREEN                38
#define TILE_KEY_CYAN                 39

#define TILE_MONEY                    40

#define TILE_SHIELD                   41
#define TILE_HELMET                   42
#define TILE_SWORD                    43

#define TILE_POTION                   44
#define TILE_ORB                      45
#define TILE_SCROLL                   46

#define TILE_CHEST                    47

#define TILE_DOOR_WHITE               72
#define TILE_DOOR_RED                 73
#define TILE_DOOR_GREEN               74
#define TILE_DOOR_CYAN                75

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
#define DIR_NONE                      0
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
#define STAT_STAIR                    5
#define STAT_RESTART                  6
#define STAT_HITBRICK                 7
/* General */
#define SPRITE_FALL_SPEED             3  /* Speed for falling from the screen after a kill */
#define SPRITE_RESTART_SPEED          8
#define SPRITE_LIN_INC                2
#define SPRITE_HEIGHT                 16
#define SPRITE_VCOL_MARGIN            14 //VERTICAL COLLISION MARGIN

/* Player */

#define PLAYER_HIT_BRICK_TIME         4

#define PLAYER_HCOL_MARGIN            2  //HORIZONTAL COLLISION MARGIN
#define PLAYER_HCOL_MARGIN_INT        4  //HORIZONTAL COLLISION MARGIN INTERNAL (1 COL = 3 INT)
#define PLAYER_JUMP_SPEED             2
#define PLAYER_FALL_SPEED             2
#define PLAYER_INERT_TIME             8  /* Time for Player Inertia */
#define PLAYER_ANIM_HIT_TIME          40 /* Time for Player Hit Sprite display after kill */


/* Enemies */
#define ENEMIES_MAX                   6   //MAX QUANTITY OF ENEMIES ON SCREEN (0->6)
#define ENEMIES_MAXJUMP               12 //MAX JUMP WHEN HIT
#define ENEMY_JUMP_SPEED              1
#define ENEMY_FALL_SPEED              1
#define ENEMY_KILLED_SPEED            8


/* Game times */
#define TIME_EVENT                    100
#define TIME_ANIM                     8
#define TIME_ANIM_PLAYER_EXPODE       12
#define TILE_ANIM_FIRE_END            89
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

#define BULLET_ARROW                 0x01
#define BULLET_FIREBALL_UP           0x02
#define BULLET_FIREBALL_DOWN         0x03
#define BULLET_AXE                   0x04
#define BULLET_FIREBALL              0x05

//typedef uint16_t (*JOYFUNC)(udk_t *);
#endif


extern unsigned char spec128;
extern unsigned char btiles[];
extern uint16_t (*joyfunc1)(udk_t *); //TODO REMOVE THIS AS IS PART NOW OF input.h
extern uint16_t (*joyfunc2)(udk_t *); //TODO REMOVE THIS AS IS PART NOW OF input.h
extern udk_t k1;
extern udk_t k2;


extern unsigned char dirs;
extern unsigned char dirs_alt;

extern unsigned char class[8];
extern unsigned char state[8];
extern unsigned char state_a[8];
extern unsigned char tile[8];
extern unsigned char lin[8];
extern unsigned char tbuffer[7];

extern unsigned char col[8];
extern unsigned char colint[8];
extern unsigned char player_hit_lin;
extern unsigned char player_hit_col;
extern unsigned char jump_lin[8];
extern unsigned int spr_timer[8];

extern unsigned int last_time[8];
extern unsigned int player_score;
extern signed char  player_vel_x;
extern signed char  player_vel_y;
extern signed char  player_vel_y0;
extern signed char  player_vel_y1;
extern signed char  player_vel_inc;
extern unsigned char player_keys[4];

extern unsigned char player_stand;
//PLAYER ATTRIBUTES
extern unsigned char player_mana;
extern unsigned char player_vita;

extern unsigned char player_max_mana;
extern unsigned char player_max_vita;


extern unsigned char player_str;
extern unsigned char player_int;
extern unsigned char player_lvl;
extern unsigned char player_col_scr;
extern unsigned char player_lin_scr;
extern unsigned char player_jump_check;

extern unsigned char game_checkpoint_col;
extern unsigned char game_checkpoint_lin;
extern unsigned char game_checkpoint_scr;
extern unsigned char game_level_up;

extern unsigned int player_inm_time;
extern unsigned int player_hit_time;
extern unsigned char player_hit;

extern unsigned char player_anim_stair;
extern unsigned char player_onfire;


extern signed   int  game_gravity;
extern unsigned char game_world;
extern unsigned char game_boss;
extern unsigned char game_boss_alive;
extern unsigned char game_boss_hit;
extern unsigned char game_boss_fix;
extern unsigned char game_song_play;
extern unsigned char game_song_play_start;

extern unsigned char game_mush_count;

extern unsigned char game_round_up;
extern unsigned char game_respawn_curr_time;
extern unsigned char game_control_mode;
extern unsigned char game_control_fire_lock;
extern unsigned char game_control_jump_lock;
extern unsigned char menu_curr_sel;


extern unsigned int fps;



extern unsigned char sprite;
extern unsigned char sprite_on_air;
extern unsigned char sprite_horizontal_check;
extern unsigned char game_debug;

extern unsigned char player_hit_count;
extern unsigned int  player_brick_time;
extern unsigned char s_tile0;
extern unsigned char s_tile1;
extern unsigned char s_lin0;
extern unsigned char s_lin1;
extern unsigned char s_col0;
extern unsigned char s_col1;
extern unsigned char s_class;
extern unsigned int loop_count;
extern unsigned int index0;
extern unsigned int index1;
extern unsigned char tmp;
extern unsigned char tmp0;
extern unsigned char tmp1;
extern unsigned char tmp_uc;
extern signed char tmp_sc;
extern unsigned int tmp_ui;
extern unsigned char enemies;
extern unsigned char zx_val_asm;
extern unsigned char attrib[4];
extern unsigned char attrib_hl[4];
extern unsigned char attrib_osd[4];
extern unsigned char s_state;
extern unsigned int curr_time;
extern unsigned int frame_time;
extern unsigned int anim_time;
extern unsigned int bullet_time;
extern unsigned char spr_count;
extern unsigned char sprite_curr_index;


extern unsigned char game_inmune;
extern unsigned char game_inf_lives;
extern unsigned char game_sound;

extern unsigned char game_over;
extern unsigned char player_lives;
extern unsigned char player_coins;
extern unsigned int game_score_top;
extern unsigned int game_respawn_time[7];
extern unsigned char game_respawn_index[7];
extern unsigned char game_respawn_tile[7];
extern unsigned char game_start_scr;

extern unsigned char player_onstair;


extern unsigned char screen_paper, screen_ink;
extern unsigned char sprite_speed[];
extern unsigned char sprite_base_speed[];
extern unsigned char sprite_frames[];
extern unsigned char spr_map_tile[];
extern unsigned char spr_init[];

∫extern unsigned char scr_map[];
extern unsigned char scr_obj0[];
extern unsigned char scr_obj1[];
extern unsigned char sprite_kind[];


//Bank 6 Level Data
extern const unsigned char start_scr0[];
extern const unsigned char lenght0[];
extern const unsigned char paper0[];
extern const unsigned char world0[];
extern const unsigned char world0_w[];
extern const unsigned char world0_h[];

extern unsigned char scr_curr;
extern unsigned char map_width;
extern unsigned char map_heigth;

extern unsigned char anim_lin[8];
extern unsigned char anim_col[8];
extern unsigned char anim_tile[8];
extern unsigned char anim_int[8];
extern unsigned char anim_end[8];
extern unsigned char anim_loop[8];
extern unsigned char anim_respanwn[8];

extern unsigned char anim_count;

extern unsigned char mush_index[8];
extern unsigned char mush_class[8];

//BULLETS
//BULLETS
extern unsigned char bullet_class[8];
extern unsigned char bullet_max[8];
extern unsigned char bullet_lin0[8];
extern unsigned char bullet_col0[8];
extern unsigned char bullet_lin[8];
extern unsigned char bullet_col[8];
extern unsigned char bullet_tile[8];
extern unsigned char bullet_dir[8];
extern unsigned char bullet_colint[8];
extern unsigned char bullet_frames[8];
extern signed char bullet_vel[8];
extern unsigned char bullet;

extern unsigned char boss_lin;
extern unsigned char boss_col;
extern unsigned char boss_inc;
extern unsigned char boss_tile;
extern unsigned int  boss_time;
extern unsigned int  boss_time_fire;
extern unsigned char boss_stat;


extern unsigned char bullet_count;

//extern const JOYFUNC control_method[7];

extern unsigned char g_player_hit_left;
extern unsigned char g_player_hit_right;
extern unsigned char draw_count;

extern unsigned char map_paper;
extern unsigned char map_paper_clr;
extern unsigned char map_paper_last;
extern unsigned char map_paper_last_a;
extern unsigned char stp_tile;
extern unsigned char stp_col;
extern unsigned char stp_row;


extern unsigned int player_lvl_table[];

extern unsigned char key_map[];
extern unsigned int scan_map[];
