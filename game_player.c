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
#include "game_ay.h"
#include "game_enemies.h"
#include "game_engine.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include "nirvana+.h"
#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>

void player_init(unsigned char f_sprite, unsigned char f_lin,
                 unsigned char f_col, unsigned char f_tile) {
  // COMMON SPRITE VARIABLES
  class[f_sprite] = PLAYER;
  tile[f_sprite] = f_tile;
  lin[f_sprite] = f_lin; //*SPRITELIN(f_sprite);
  col[f_sprite] = f_col; //*SPRITECOL(f_sprite);
  colint[f_sprite] = 0;
  state[f_sprite] = 0;
  state_a[f_sprite] = 0;
  jump_lin[f_sprite] = f_lin;
  last_time[f_sprite] = zx_clock();
  sprite_speed_alt[f_sprite] = 0;

  BIT_SET(state_a[f_sprite], STAT_LOCK);
  // PLAYER ONLY VARIABLES
  BIT_SET(state_a[f_sprite], STAT_LDIRR);
  player_hit_col = 0;
  player_hit_lin = 0;
  player_slide = 0;
  NIRVANAP_spriteT(f_sprite, f_tile, f_lin, f_col);
}

unsigned char player_check_input(void) {
  if (dirs & IN_STICK_LEFT && dirs & IN_STICK_RIGHT)
    dirs = 0;
  return dirs & IN_STICK_FIRE || dirs & IN_STICK_LEFT ||
         dirs & IN_STICK_RIGHT || dirs & IN_STICK_UP || dirs & IN_STICK_DOWN;
}

unsigned char player_collision(void) {
  sprite = 0;
  s_col1 = col[SPR_P1];
  s_lin1 = lin[SPR_P1];
  while (sprite < SPR_P1) {
    if (class[sprite] > 0) {
      if (abs(col[sprite] - s_col1) < 2) {
        if (abs(lin[sprite] - s_lin1) < 14) {
          zx_border(INK_RED);
          return 1;
        }
      }
    }
    ++sprite;
  }
  return 0;
}

void player_kill(void) {
  if (!BIT_CHK(state[sprite], STAT_KILL)) {
    sound_hit_brick();
    BIT_SET(state[sprite], STAT_KILL);
    sprite_speed_alt[sprite] = SPRITE_FALL_SPEED;
    if (BIT_CHK(state[sprite], STAT_DIRR)) {
      tile[sprite] = TILE_P1_HITR;
    } else {
      tile[sprite] = TILE_P1_HITR + TILE_P1_LEN;
    }
    player_hit_platform_clear();
    NIRVANAP_spriteT(sprite, tile[sprite], lin[sprite], col[sprite]);
    spr_timer[sprite] = zx_clock();
    if (sprite == SPR_P1) {
      BIT_SET(state_a[sprite], STAT_LDIRL);
      BIT_CLR(state_a[sprite], STAT_LDIRR);
    } else {
      BIT_SET(state_a[sprite], STAT_LDIRR);
      BIT_CLR(state_a[sprite], STAT_LDIRL);
    }
  }
}

void player_restart(unsigned char f_sprite) __z88dk_fastcall {

  ay_fx_play(ay_effect_15);
  player_init(SPR_P1, 0, 14, TILE_P1_STANR);
  BIT_SET(state[f_sprite], STAT_HIT);
  BIT_SET(state_a[f_sprite], STAT_LOCK);
  spr_timer[f_sprite] = zx_clock();
  sprite_speed_alt[f_sprite] = SPRITE_RESTART_SPEED;
}

unsigned char player_lost_life(void) {
  sound_hit_enemy();
  --player_lives;

  if (player_lives == 0) {
    /* Player dies */
    game_over = 1;
    /* Do not restart player */
    return 0;
  }
  game_print_lives();
  return 1;
}

void player_turn(void) {
  if (class[sprite] == PLAYER && player_lives > 0) {
    if (spr_chktime(&sprite)) {
      zx_border(INK_BLACK);
      // dirs = 0;
      dirs = (joyfunc1)(&k1);
      player_move();
      player_collision();
    }
  }
}

unsigned char player_move(void) {
  /* Player initial Values */
  s_lin0 = lin[sprite];
  s_col0 = col[sprite];
  s_tile0 = tile[sprite] + colint[sprite];
  s_state = state[sprite];

  if (BIT_CHK(s_state, STAT_JUMP) || BIT_CHK(s_state, STAT_FALL)) {
    player_fire();

    // If the player push fire alone and then move HORIZONTAL
    if (player_vel_y < player_vel_y1) {
      if (dirs & IN_STICK_LEFT) {
        BIT_SET(s_state, STAT_DIRL);
        BIT_CLR(s_state, STAT_DIRR);
      }
      if (dirs & STAT_DIRR) {
        BIT_SET(s_state, STAT_DIRR);
        BIT_CLR(s_state, STAT_DIRL);
      }
    }

    /* Jump Handling */
    if (player_move_jump()) {
      // Jump Ends
      if (!player_over_stair)
        player_tile(TILE_P1_RIGHT);
    }
  } else {
    /* Read player input */
    if (!player_move_input()) {
      colint[sprite] = 0;
      player_tile(TILE_P1_STANR);
      BIT_CLR(s_state, STAT_DIRL);
      BIT_CLR(s_state, STAT_DIRR);
    }
    /* Check if the player have floor, and set fall if not */
    player_check_floor();
  }
  // player_check_stairs(0);

  /* Draw Player sprite */
  if (spr_redraw()) {
    // The player have moved so we need to check to pick
    player_check_map();
  }

  /* Anim Restored hitted platforms */
  if (player_hit_lin > 0) {
    player_hit_platform_clear();
  }
  /* Store State variable */
  state[sprite] = s_state;
  return 0;
}

void player_check_stairs_down(void) {
  unsigned char v0;
  sprite_curr_index = spr_calc_index(lin[sprite] + 16, col[sprite]);
  v0 = scr_map[sprite_curr_index];
  v0 = (v0 >= TILE_STAIR_S && v0 <= TILE_STAIR_E);
  if (v0)
    player_over_stair = 1;
}

void player_check_stairs(unsigned char f_inc) __z88dk_fastcall {
  unsigned char v0;
  unsigned char v1;

  sprite_curr_index = spr_calc_index(lin[sprite], col[sprite] + f_inc);
  v0 = scr_map[sprite_curr_index];
  v0 = (v0 >= TILE_STAIR_S && v0 <= TILE_STAIR_E);

  sprite_curr_index = spr_calc_index(lin[sprite] + 15, col[sprite] + f_inc);
  v1 = scr_map[sprite_curr_index];
  v1 = (v1 >= TILE_STAIR_S && v1 <= TILE_STAIR_E);

  if (v0 || v1) {
    // OVER STAIR
    if (!player_over_stair) {
      player_over_stair = 1;
      player_tile(TILE_P1_STAIR);
    }
  } else {
    if (player_over_stair) {
      player_over_stair = 0;
      player_tile(TILE_P1_RIGHT);
    }
  }
}

void player_tile(unsigned char f_tile) __z88dk_fastcall {
  if (player_over_stair)
     { tile[sprite] = TILE_P1_STAIR; }
  else
     {
      if (f_tile == TILE_P1_STANR) {
        tile[sprite] = spr_tile_dir(f_tile, sprite, 1);
      } else {
        tile[sprite] = spr_tile_dir(f_tile, sprite, TILE_P1_LEN);
      }
    }
}

unsigned char player_move_input(void) {
  // TODO CLEAN THIS!
  /* User have pressed valid input */
  if (player_check_input()) {

    if (player_fire()) {
      return 0;
    }

    /* New jump */
    if (dirs & IN_STICK_FIRE) {
      player_vel_inc = 1;
      if (ay_is_playing() != AY_PLAYING_MUSIC) {
        ay_fx_play(ay_effect_03);
      }
      player_over_stair = 0;
      sound_jump();

      // colint[sprite] = FRAMES_PLAYER / 2;
      BIT_SET(s_state, STAT_JUMP);
      BIT_CLR(s_state, STAT_FALL);
      jump_lin[sprite] = lin[sprite];
      state[sprite] = s_state; /*TODO FIXME!*/
      player_tile(TILE_P1_JUMPR);
      sprite_speed_alt[sprite] = PLAYER_JUMP_SPEED;
      player_slide = 0;
      player_vel_y = player_vel_y0; // -negative up / positive down
      return 1;
    }

    /* Move Right */
    if (dirs & IN_STICK_RIGHT) {
      BIT_SET(s_state, STAT_DIRR);
      BIT_CLR(s_state, STAT_DIRL);
      BIT_SET(state_a[sprite], STAT_LDIRR);
      BIT_CLR(state_a[sprite], STAT_LDIRL);
      if (player_over_stair) {
        player_check_stairs(0);
      }
      spr_move_horizontal();
    }

    /* Move Left */
    if (dirs & IN_STICK_LEFT) {
      BIT_SET(s_state, STAT_DIRL);
      BIT_CLR(s_state, STAT_DIRR);
      BIT_SET(state_a[sprite], STAT_LDIRL);
      BIT_CLR(state_a[sprite], STAT_LDIRR);
      if (player_over_stair) {
        player_check_stairs(0);
      }
      spr_move_horizontal();
    }

    if (dirs & IN_STICK_UP) {
      player_check_stairs(0);
      if (player_over_stair) {
        if (spr_move_up()) {
          player_over_stair = 0;
        }
      } else {
        if (!((dirs & IN_STICK_RIGHT) || (dirs & IN_STICK_LEFT)))
          return 0;
      }
    }

    if (dirs & IN_STICK_DOWN) {
      player_check_stairs_down();
      if (player_over_stair) {
        if (spr_move_down()) {
          player_over_stair = 0;
        }
      } else {
        if (!((dirs & IN_STICK_RIGHT) || (dirs & IN_STICK_LEFT)))
          return 0;
      }
    }

    /* Set Tile according to current direction */
    state[sprite] = s_state;
    player_tile(TILE_P1_RIGHT);
    return 1;
  }
  return 0;
}

unsigned char player_fire() {
  if ((dirs & IN_STICK_FIRE) && (dirs & IN_STICK_DOWN)) {

    /*Fireball*/
    if (bullet_col[SPR_P1] == 0xFF) {
      ++bullet_count;
      bullet_dir[SPR_P1] = 0;
      bullet_lin[SPR_P1] = lin[SPR_P1];
      bullet_frames[SPR_P1] = 2;

      if (BIT_CHK(state_a[SPR_P1], STAT_LDIRL)) {
        // Left
        if (col[SPR_P1] < 1)
          return 1;
        bullet_col[SPR_P1] = col[SPR_P1] - 1;
        bullet_tile[SPR_P1] = TILE_FIREBALL_L;
        bullet_dir[SPR_P1] = 0x01;
        bullet_colint[SPR_P1] = 2;
      } else {
        // Right n default
        // if ( BIT_CHK(state_a[SPR_P1], STAT_LDIRR) ) {
        if (col[SPR_P1] > 29)
          return 1;
        bullet_col[SPR_P1] = col[SPR_P1] + 1;
        bullet_tile[SPR_P1] = TILE_FIREBALL_R;
        bullet_dir[SPR_P1] = 0xFF;
        bullet_colint[SPR_P1] = 0xFF;
      }
    }
    return 1;
  }
  return 0;
}

void player_check_map() {

  sprite_curr_index = spr_calc_index(lin[sprite], col[sprite]);
  if ((col[sprite] & 1) == 0) { // Par
    if ((lin[sprite] & 15) == 0) {
      // Pick single tile
      player_pick_item();
    } else {
      // Pick up n down tiles
      player_pick_item();
      sprite_curr_index = sprite_curr_index + 16;
      player_pick_item();
    }
  } else
     { // Impar
      if ((lin[sprite] & 15) == 0) {
        // Pick single tile
        player_pick_item();
        sprite_curr_index = sprite_curr_index + 1;
        player_pick_item();
      } else {
        // Pick up n down tiles
        player_pick_item();
        sprite_curr_index = sprite_curr_index + 1;
        player_pick_item();
        sprite_curr_index = sprite_curr_index + 15;
        player_pick_item();
        sprite_curr_index = sprite_curr_index + 1;
        player_pick_item();
      }
    }
}

void player_pick_item(void) {
  unsigned char v0;
  v0 = scr_map[sprite_curr_index];
  if (v0 >= TILE_ITEM_S && v0 <= TILE_ITEM_E) {

    // PICK ITEM
    ay_reset();
    ay_fx_play(ay_effect_10);
    sound_coin();
    scr_map[sprite_curr_index] = TILE_EMPTY;

    BIT_SET(scr_obj[sprite_curr_index], scr_curr);
    s_lin1 = (sprite_curr_index >> 4) << 4;
    s_col1 = (sprite_curr_index & 15) * 2;
    spr_add_anim(s_lin1, s_col1, TILE_ANIM_PICK, 3);
    // NIRVANAP_drawT(TILE_EMPTY, s_lin1, s_col1);
    switch (v0) {
    case 28: // KEY WHITE
      player_keys[0] = 1;
      zx_print_ink(INK_WHITE);
      zx_print_str(22, 3, "]");
      break;
    case 29: // KEY RED
      player_keys[1] = 1;
      zx_print_ink(INK_RED);
      zx_print_str(22, 4, "]");
      break;
    case 30: // KEY GREEN
      player_keys[2] = 1;
      zx_print_ink(INK_GREEN);
      zx_print_str(22, 5, "]");
      break;
    case 31: // KEY CYAN
      player_keys[3] = 1;
      zx_print_ink(INK_CYAN);
      zx_print_str(22, 6, "]");
      break;
    }
  }
  if (v0 > TILE_ITEM_E && v0 < TILE_FLOOR) {

    if ((lin[SPR_P1] & 3) != 0) {
      // DEADLY BACKGROUNDS
      zx_border(INK_YELLOW);
    }
  }
}

unsigned char player_hit_platform(void) {
  if ((player_hit_lin == 0) && (lin[sprite] > 16)) {
    for (enemies = 0; enemies < 6; ++enemies) {
      // HIT ENEMIES
      if (class[enemies] != 0 && (lin[sprite] - lin[enemies] == 24) &&
          (!BIT_CHK(state[enemies], STAT_KILL)) &&
          (abs(col[sprite] - col[enemies]) <= 2)) {
        enemy_hit();
        sound_hit_enemy();
      }
    }

    // BIT_SET( state_a[sprite] , STAT_HITBRICK );
    spr_timer[sprite] = zx_clock();
    player_hit_lin = 8 + (((lin[sprite] - 8) >> 4) << 4);

    if ((col[sprite] & 1) == 0) {
      player_hit_col = col[sprite];
    } else {

      if (BIT_CHK(state[sprite], STAT_DIRL)) {
        if (g_player_hit_right) {
          player_hit_col = col[sprite] + 1;
        } else {
          player_hit_col = col[sprite] - 1;
        }
      } else {
        if (g_player_hit_left) {
          player_hit_col = col[sprite] - 1;
        } else {
          player_hit_col = col[sprite] + 1;
        }
      }
    }
    spr_brick_anim(1);
    sound_hit_brick();
    return 1;
  }
  return 0;
}

void player_hit_platform_clear(void) {
  // CLEAR HITTED BRICKS N MAKES THE PLAYER FALL
  if (game_check_time(spr_timer[sprite], PLAYER_HIT_BRICK_TIME)) {
    index1 = spr_calc_index(player_hit_lin, player_hit_col);
    spr_brick_anim(0);
    player_hit_lin = 0;
    player_hit_col = 0;
  }
}

void player_score_add(unsigned int f_score) __z88dk_fastcall {
  player_score = player_score + f_score;
  // CHECK FOR TOP SCORE
  if (player_score > game_score_top) {
    game_score_top = player_score;
  }
  game_print_score();
}

void player_check_floor(void) {
  unsigned int index_d;
  unsigned char v1;
  unsigned char v2;

  index_d = spr_calc_index(lin[sprite] + 16, col[sprite]);
  v1 = scr_map[index_d];

  if ((col[sprite] & 1) == 0) {
    v2 = 1;
  } else {
    v2 = scr_map[index_d + 1];
  }

  if (s_lin0 == GAME_LIN_FLOOR) {
    v1 = TILE_CEIL;
    v1 = TILE_CEIL;
  }

  if ((v1 < TILE_FLOOR || v1 >= TILE_END) &&
      (v2 < TILE_FLOOR || v2 >= TILE_END)) {
    sprite_speed_alt[sprite] = PLAYER_FALL_SPEED;
    BIT_SET(s_state, STAT_FALL);
    player_check_stairs(0);
  }
}

unsigned char player_move_jump(void) {

  signed int val_yc;
  unsigned char f_check;
  player_vel_y = player_vel_y + game_gravity;
  sprite_on_air = 1;
  // JUMP BOOST
  if ((player_vel_inc)) {
    if (!(dirs & IN_STICK_FIRE) && (player_vel_y > player_vel_y1) &&
        (player_vel_y < 0)) {
      player_vel_y = 0; // TODO FIX WHEN FALLING!
      player_vel_inc = 0;
    }
  }
  // MAX SPEEDS
  if (player_vel_y > 120) {
    player_vel_y = 120;
  }

  if (player_vel_y < -120) {
    player_vel_y = -120;
  }
  // CONVER TO PIXEL'S
  // zx_print_int(0,0,player_vel_y);
  // val_yc = player_vel_y / 10;
  val_yc = player_vel_y;

  s_lin1 = (unsigned char)val_yc;
  // Nirvana don't support odd lin's

  if ((s_lin1 & 1) != 0) {
    s_lin1--;
  }

  s_lin1 = lin[sprite] + s_lin1;

  if (s_lin1 > GAME_LIN_FLOOR) {
    if (s_lin1 > GAME_LIN_FLOOR + 56) {
      s_lin1 = 0;
    } else {
      s_lin1 = GAME_LIN_FLOOR;
    }
  }

  if (val_yc < 0) {
    BIT_SET(s_state, STAT_JUMP);
    BIT_CLR(s_state, STAT_FALL);
    // Asending
    f_check = (s_lin0 >> 4) != (s_lin1 >> 0);

    if (f_check) {
      if (game_check_map(s_lin1, col[sprite])) {
        // Hit Platforms
        if (player_hit_platform()) {

          lin[sprite] = (lin[sprite] >> 4) << 4;
        }
        player_vel_y = 0;
      } else {
        lin[sprite] = s_lin1;
      }
    } else {
      lin[sprite] = s_lin1;
    }
  } else {
    // Falling
    BIT_SET(s_state, STAT_FALL);
    BIT_CLR(s_state, STAT_JUMP);
    s_lin1 = s_lin1 + 16;
    f_check = (s_lin0 >> 4) != (s_lin1 >> 0);
    if (f_check) {
      if (game_check_map(s_lin1, col[sprite])) {
        // Jump end
        s_lin1 = s_lin1 - 16;
        player_check_stairs(0);
        if (!player_over_stair) {
          player_check_stairs(1);
        }
        if (!player_over_stair) {
          s_lin1 = (s_lin1 >> 4) << 4;
          if (lin[sprite] > s_lin1) {
            lin[sprite] = s_lin1 + 16;
            return 0;
          } else {
            lin[sprite] = s_lin1;
          }
        }
        player_vel_y = 0;
        BIT_CLR(s_state, STAT_FALL);
        BIT_CLR(s_state, STAT_JUMP);
        colint[sprite] = 0;
        return 1;
      } else {
        s_lin1 = s_lin1 - 16;
        lin[sprite] = s_lin1;
      }
    } else {
      s_lin1 = s_lin1 - 16;
      lin[sprite] = s_lin1;
    }
    /*
    // index1 = spr_calc_index(s_lin1 + 16, col[sprite]);
    if (game_check_map(s_lin1 + 16, col[sprite])) {
      // Jump end
      player_check_stairs(0);
      if (!player_over_stair) {
        player_check_stairs(1);
      }

      if (!player_over_stair) {
        s_lin1 = (s_lin1 >> 4) << 4;
        if (lin[sprite] > s_lin1) {
          lin[sprite] = s_lin1 + 16;
          return 0;
        } else {
          lin[sprite] = s_lin1;
        }
      }
      player_vel_y = 0;
      BIT_CLR(s_state, STAT_FALL);
      BIT_CLR(s_state, STAT_JUMP);
      colint[sprite] = 0;
      return 1;

    } else {
      lin[sprite] = s_lin1;
    }
    */
  }
  sprite_horizontal_check = 1;
  if (spr_move_horizontal()) {
    BIT_CLR(s_state, STAT_DIRL);
    BIT_CLR(s_state, STAT_DIRR);
  }
  sprite_horizontal_check = 0;
  return 0;
}

void player_open_door(unsigned int f_index, unsigned char f_tile) {
  unsigned char f_open;
  f_open = 0;
  switch (f_tile) {
  case 60: // DOOR WHITE
    if (player_keys[0]) {
      zx_border(INK_WHITE);
      f_open = 1;
    }
    break;
  case 61: // DOOR RED
    if (player_keys[1]) {
      zx_border(INK_RED);
      f_open = 1;
    }
    break;
  case 62: // DOOR GREEN
    if (player_keys[2]) {
      zx_border(INK_GREEN);
      f_open = 1;
    }
    break;
  case 63: // DOOR CYAN
    if (player_keys[3]) {
      zx_border(INK_CYAN);
      f_open = 1;
    }
    break;
  }

  if (f_open) {
    zx_print_int(0,0,f_index);
    scr_map[f_index] = TILE_EMPTY;
    BIT_SET(scr_obj[f_index], scr_curr);
    spr_draw_index(f_index);
  }
}
