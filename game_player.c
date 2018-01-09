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

void player_init(unsigned char f_lin, unsigned char f_col,
                 unsigned char f_tile) {
  // COMMON SPRITE VARIABLES
  class[SPR_P1] = PLAYER;
  tile[SPR_P1] = f_tile;
  lin[SPR_P1] = f_lin; //*SPRITELIN(SPR_P1);
  col[SPR_P1] = f_col; //*SPRITECOL(SPR_P1);
  colint[SPR_P1] = 0;
  state[SPR_P1] = 0;
  state_a[SPR_P1] = 0;
  jump_lin[SPR_P1] = f_lin;
  last_time[SPR_P1] = zx_clock();
  sprite_speed_alt[SPR_P1] = 0;
  BIT_SET(state_a[SPR_P1], STAT_LOCK);
  // PLAYER ONLY VARIABLES
  BIT_SET(state_a[SPR_P1], STAT_LDIRR);
  player_hit_col = 0;
  player_hit_lin = 0;
  player_slide = 0;
  NIRVANAP_spriteT(SPR_P1, f_tile, f_lin, f_col);
}

void player_turn(void) {
  if (spr_chktime(&sprite)) {
    dirs = (joyfunc1)(&k1);
    player_move();
    player_collision();
  }
}

unsigned char player_move(void) {

  /* Player initial Values */
  s_lin0 = lin[SPR_P1];
  s_col0 = col[SPR_P1];
  s_tile0 = tile[SPR_P1] + colint[SPR_P1];
  s_state = state[SPR_P1];

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
      if (!player_onstair) {
        player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
      }
    }
  } else {
    /* Read player input */
    player_move_input();
    /* Check if the player have floor, and set fall if not */
    if (!player_onstair)
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
  state[SPR_P1] = s_state;
  return 0;
}

unsigned char player_check_input(void) {
  if (dirs & IN_STICK_LEFT && dirs & IN_STICK_RIGHT) {
    // Error on reading both horizontal's
    dirs = 0;
  }

  return dirs & IN_STICK_FIRE || dirs & IN_STICK_LEFT ||
         dirs & IN_STICK_RIGHT || dirs & IN_STICK_UP || dirs & IN_STICK_DOWN;
}

unsigned char player_move_input(void) {
  // TODO CLEAN THIS!
  /* User have pressed valid input */
  if (player_check_input()) {

    if (player_fire()) {
      dirs = 0;
      return 0;
    }

    /* New jump */
    if ((dirs & IN_STICK_FIRE) && (dirs & IN_STICK_UP)) {
      player_vel_inc = 1;
      if (ay_is_playing() != AY_PLAYING_MUSIC) {
        ay_fx_play(ay_effect_03);
      }
      player_onstair = 0;
      sound_jump();

      // colint[SPR_P1] = FRAMES_PLAYER / 2;
      BIT_SET(s_state, STAT_JUMP);
      BIT_CLR(s_state, STAT_FALL);
      jump_lin[SPR_P1] = lin[SPR_P1];
      state[SPR_P1] = s_state; /*TODO FIXME!*/
      player_tile(TILE_P1_JUMPR, TILE_P1_LEN);
      sprite_speed_alt[SPR_P1] = PLAYER_JUMP_SPEED;
      player_slide = 0;
      player_vel_y = player_vel_y0; // -negative up / positive down
      return 1;
    }

    /* Move Right */
    if (dirs & IN_STICK_RIGHT) {
      player_fix_walk_lin();
      BIT_SET(s_state, STAT_DIRR);
      BIT_CLR(s_state, STAT_DIRL);
      BIT_SET(state_a[SPR_P1], STAT_LDIRR);
      BIT_CLR(state_a[SPR_P1], STAT_LDIRL);
      if (player_onstair) {
        player_check_stairs(0);
      }
      spr_move_horizontal();
    }

    /* Move Left */
    if (dirs & IN_STICK_LEFT) {
      player_fix_walk_lin();
      BIT_SET(s_state, STAT_DIRL);
      BIT_CLR(s_state, STAT_DIRR);
      BIT_SET(state_a[SPR_P1], STAT_LDIRL);
      BIT_CLR(state_a[SPR_P1], STAT_LDIRR);
      if (player_onstair) {
        player_check_stairs(0);
      }
      spr_move_horizontal();
    }

    if (dirs & IN_STICK_UP) {
      player_check_stairs(0);
      if (!player_onstair) {
        player_check_stairs(1);
        if (!player_onstair) {
          player_check_stairs_vertical(-SPRITE_LIN_INC);
        }

      }

      if (player_onstair) {
        spr_move_up();
      } else {
        if (!((dirs & IN_STICK_RIGHT) || (dirs & IN_STICK_LEFT)))
          return 0;
      }
    }
    if (dirs & IN_STICK_DOWN) {
      player_check_stairs(0);
      if (!player_onstair) {
        player_check_stairs(1);
        if (!player_onstair) {
          player_check_stairs_vertical(16+SPRITE_LIN_INC);
        }
      }
      if (player_onstair) {
        spr_move_down();
      } else {
        if (!((dirs & IN_STICK_RIGHT) || (dirs & IN_STICK_LEFT)))
          return 0;
      }
    }

    /* Set Tile according to current direction */
    state[SPR_P1] = s_state;
    if (player_onstair) {
      player_tile(TILE_P1_STAIR, 0);
    } else {
      player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
    }
    return 1;
  } else {
    colint[SPR_P1] = 0;
    if (player_onstair) {
      player_tile(TILE_P1_STAIR, 0);
    } else {
      player_tile(TILE_P1_STANR, 1);
    }
    BIT_CLR(s_state, STAT_DIRL);
    BIT_CLR(s_state, STAT_DIRR);
  }
  return 0;
}

void player_fix_walk_lin() {
  if (!player_onstair) {
    s_lin1 = (s_lin0 >> 4) << 4;
    if (s_lin0 != s_lin1) {
      lin[SPR_P1] = s_lin1;
    }
  }
}
unsigned char player_fire() {
  // if ((dirs & IN_STICK_FIRE) && (dirs & IN_STICK_DOWN)) {
  if ((dirs & IN_STICK_FIRE) && !(dirs & IN_STICK_UP)) {
    /*Fireball*/
    if (player_mana > 0 && (bullet_col[SPR_P1] == 0xFF)) {
      if (player_mana > 5) {
        sound_hit_enemy();
        if (!game_god_mode)
          player_mana = player_mana - 5;
      } else {
        player_mana = 0;
      }
      game_update_stats();
      game_shoot_fire(SPR_P1, TILE_FIREBALL);
    }
    return 1;
  }
  return 0;
}

unsigned char player_collision(void) {
  unsigned char v0;
  if (game_check_time(player_hit_time, 25)) { // HACK REPATED ON player_hit

    sprite = 0;
    s_col1 = col[SPR_P1];
    s_lin1 = lin[SPR_P1];

    sprite_curr_index = spr_calc_index(s_lin1 + 8, s_col1);
    v0 = scr_map[sprite_curr_index];

    if (v0 > TILE_ITEM_E && v0 < TILE_FLOOR) {
      // if ((s_lin1  & 3) != 0) {
      // DEADLY BACKGROUNDS
      if (v0 == TILE_WORLD_EXIT) {
        game_worldup = 1;
      } else {
        zx_border(INK_YELLOW);
        player_hit(50);
      }

      //}
    }

    while (sprite < SPR_P1) {
      if (class[sprite] > 0) {
        if (abs(col[sprite] - s_col1) < 2) {
          if (abs(lin[sprite] - s_lin1) < 14) {
            /*MUSHROMS*/
            if (class[sprite] == MUSHROOM_VITA) {
              player_vita = player_vita + 25;
              if (player_vita > player_max_vita) {
                player_vita = player_max_vita;
              }
              player_score_add(1);
              game_update_stats();
              spr_destroy(sprite);
              --game_mush_count;
              return 0;
            }
            if (class[sprite] == MUSHROOM_MANA) {
              player_mana = player_mana + 25;
              if (player_mana > player_max_mana) {
                player_mana = player_max_mana;
              }
              player_score_add(1);
              game_update_stats();
              spr_destroy(sprite);
              --game_mush_count;
              return 0;
            }
            if (class[sprite] == MUSHROOM_EXTRA) {

              player_1up();
              player_score_add(1);
              game_update_stats();
              spr_destroy(sprite);
              --game_mush_count;
              return 0;
            }
            zx_border(INK_RED);
            player_hit(20);
            return 1;
          }
        }
      }
      ++sprite;
    }
  }
  return 0;
}

void player_1up() {
  if (player_lives < 255) {
    ++player_lives;
    zx_print_str(12, 12, "1 UP!");
    game_colour_message(12, 12, 12 + 9, 25, 0);
    game_update_stats();
  }
}

void player_check_stairs_vertical(signed char f_inc) {
  unsigned char v0;
  unsigned char v1;
  if ( (col[SPR_P1] & 1) == 0 ) {
    sprite_curr_index = spr_calc_index(lin[SPR_P1] + f_inc, col[SPR_P1]);
    v0 = scr_map[sprite_curr_index];
    v0 = (v0 >= TILE_STAIR_S && v0 <= TILE_STAIR_E);
    v1 = 0;
  } else {
    sprite_curr_index = spr_calc_index(lin[SPR_P1] + f_inc, col[SPR_P1] - f_inc);
    v0 = scr_map[sprite_curr_index];
    v0 = (v0 >= TILE_STAIR_S && v0 <= TILE_STAIR_E);
    sprite_curr_index = spr_calc_index(lin[SPR_P1] + f_inc, col[SPR_P1] - f_inc);
    v1 = scr_map[sprite_curr_index];
    v1 = (v0 >= TILE_STAIR_S && v0 <= TILE_STAIR_E);
  }

  if (v0 || v1) {
    player_onstair = 1;
  } else {
    player_onstair = 0;
  }
}

void player_check_stairs(unsigned char f_inc) __z88dk_fastcall {
  unsigned char v0;
  unsigned char v1;

  sprite_curr_index = spr_calc_index(lin[SPR_P1], col[SPR_P1] + f_inc);
  v0 = scr_map[sprite_curr_index];
  v0 = (v0 >= TILE_STAIR_S && v0 <= TILE_STAIR_E);

  sprite_curr_index = spr_calc_index(lin[SPR_P1] + 15, col[SPR_P1] + f_inc);
  v1 = scr_map[sprite_curr_index];
  v1 = (v1 >= TILE_STAIR_S && v1 <= TILE_STAIR_E);

  if (v0 || v1) {
    // OVER STAIR
    if (!player_onstair) {
      player_onstair = 1;
      // colint[SPR_P1] = 0;
      player_tile(TILE_P1_STAIR, 0);
    }
  } else {
    if (player_onstair) {
      player_onstair = 0;
      player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
    }
  }
}

void player_tile(unsigned char f_tile, unsigned char f_inc) {

  tile[SPR_P1] = spr_tile_dir(f_tile, sprite, f_inc);
}

void player_check_map() {
  sprite_curr_index = spr_calc_index(lin[SPR_P1], col[SPR_P1]);
  if ((col[SPR_P1] & 1) == 0) { // Par
    if ((lin[SPR_P1] & 15) == 0) {
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
      if ((lin[SPR_P1] & 15) == 0) {
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
    scr_map[sprite_curr_index] =
        game_match_back(sprite_curr_index); // TILE_EMPTY;

    game_obj_set(sprite_curr_index);
    s_lin1 = (sprite_curr_index >> 4) << 4;
    s_col1 = (sprite_curr_index & 15) << 1;
    spr_add_anim(s_lin1, s_col1, TILE_ANIM_PICK, 3, 0, 0);
    switch (v0) {
    case TILE_KEY_WHITE:
      player_keys[0] = 1;
      game_update_stats();
      break;
    case TILE_KEY_RED:
      player_keys[1] = 1;
      game_update_stats();
      break;
    case TILE_KEY_GREEN:
      player_keys[2] = 1;
      game_update_stats();
      break;
    case TILE_KEY_CYAN:
      player_keys[3] = 1;
      game_update_stats();
      break;
    case TILE_MONEY:
      player_score_add(1);
      ++player_coins;
      if (player_coins == 100) {
        player_1up();
        player_coins = 0;
      }
      game_update_stats();
      break;
    case TILE_CHEST:
      player_score_add(10);
      break;
    case TILE_SHIELD:
      player_str++;
      if (player_max_vita < GAME_MAX_VITA) {
        player_max_vita = player_max_vita + 10;
        game_update_stats();
      }
      break;
    case TILE_HELMET:
      player_str++;
      if (player_max_vita < GAME_MAX_VITA) {
        player_max_vita = player_max_vita + 10;
        game_update_stats();
      }
      break;
    case TILE_SWORD:
      player_str++;
      if (player_max_vita < GAME_MAX_VITA) {
        player_max_vita = player_max_vita + 10;
        game_update_stats();
      }
      break;
    case TILE_POTION:
      player_int++;
      if (player_max_mana < GAME_MAX_MANA) {
        player_max_mana = player_max_mana + 10;
        game_update_stats();
      }
      break;
    case TILE_ORB:
      player_int++;
      if (player_max_mana < GAME_MAX_MANA) {
        player_max_mana = player_max_mana + 10;
        game_update_stats();
      }
      break;
    case TILE_SCROLL:
      player_int++;
      if (player_max_mana < GAME_MAX_MANA) {
        player_max_mana = player_max_mana + 10;
        game_update_stats();
      }
      break;
    }
  }
}

unsigned char player_hit_platform(void) {

  if ((player_hit_lin == 0) && (lin[SPR_P1] > 16) &&
      (scr_map[index1] >= TILE_HIT)) {
    spr_timer[SPR_P1] = zx_clock();
    player_hit_lin = 8 + (((lin[SPR_P1] - 8) >> 4) << 4);

    if ((col[SPR_P1] & 1) == 0) {
      player_hit_col = col[SPR_P1];
    } else {

      if (BIT_CHK(state[SPR_P1], STAT_DIRL)) {
        if (g_player_hit_right) {
          player_hit_col = col[SPR_P1] + 1;
        } else {
          player_hit_col = col[SPR_P1] - 1;
        }
      } else {
        if (g_player_hit_left) {
          player_hit_col = col[SPR_P1] - 1;
        } else {
          player_hit_col = col[SPR_P1] + 1;
        }
      }
    }

    if (scr_map[index1] == TILE_DIRT) {
      // Destroy Bricks
      scr_map[index1] = game_match_back(index1); // TILE_EMPTY;
      game_obj_set(index1);
      spr_add_anim((index1 >> 4) << 4, (index1 & 15) << 1, TILE_ANIM_FIRE, 3, 0,
                   0);
      return 1;
    }
    if (scr_map[index1] == TILE_SPECIAL ||
        scr_map[index1] == TILE_HIDDEN_BRICK) {

      tmp0 = 0;

      if (game_boss && game_mush_count > 0) {
        return 0;
      }

      while (tmp0 < SPR_P1) {
        if (mush_index[tmp0] == index1) {
          s_lin1 = ((lin[SPR_P1] >> 4) << 4) - 32;
          s_col1 = (col[SPR_P1] >> 1) << 1;
          tmp = 0;
          while (tmp < SPR_P1) {
            if (mush_index[tmp] == index1) {
              break;
            }
            ++tmp;
          }
          switch (mush_class[tmp]) {
          case INDEX_MUSH_VITA_L:
            zx_border(INK_RED);
            enemy_init(s_lin1, s_col1, MUSHROOM_VITA, DIR_LEFT);
            break;
          case INDEX_MUSH_VITA_R:
            zx_border(INK_RED);
            enemy_init(s_lin1, s_col1, MUSHROOM_VITA, DIR_RIGHT);
            break;
          case INDEX_MUSH_MANA_R:
            zx_border(INK_BLUE);
            enemy_init(s_lin1, s_col1, MUSHROOM_MANA, DIR_RIGHT);
            break;
          case INDEX_MUSH_MANA_L:
            zx_border(INK_BLUE);
            enemy_init(s_lin1, s_col1, MUSHROOM_MANA, DIR_LEFT);
            break;
          case INDEX_MUSH_EXTRA_R:
            zx_border(INK_GREEN);
            enemy_init(s_lin1, s_col1, MUSHROOM_EXTRA, DIR_RIGHT);
            break;
          case INDEX_MUSH_EXTRA_L:
            zx_border(INK_GREEN);
            enemy_init(s_lin1, s_col1, MUSHROOM_EXTRA, DIR_LEFT);
            break;
          }
          ++game_mush_count;
          if (!game_boss) {
            // Only restore to no special brick if not boos on the map
            index0 = index1 - 16;
            scr_map[index0] = game_match_back(index0); // TILE_EMPTY;
            if (scr_map[index1] == TILE_SPECIAL ||
                scr_map[index1] == TILE_HIDDEN_BRICK) {
              scr_map[index1] = TILE_NOSPECIAL;
            } else {
              scr_map[index1] = TILE_NORMAL_BRICK;
            }

            game_obj_set(index1 - 16);
            NIRVANAP_drawT(TILE_NOSPECIAL, s_lin1 + 16, s_col1);
          }
        }
        ++tmp0;
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
  if (game_check_time(spr_timer[SPR_P1], PLAYER_HIT_BRICK_TIME)) {
    index1 = spr_calc_index(player_hit_lin, player_hit_col);
    spr_brick_anim(0);
    player_hit_lin = 0;
    player_hit_col = 0;
  }
}

void player_score_add(unsigned int f_score) __z88dk_fastcall {
  player_score = player_score + f_score;

  if (player_lvl < GAME_MAX_LEVEL) {
    if (player_score >= player_lvl_table[player_lvl]) {
      player_lvl++;
      zx_print_str(12, 12, "LEVEL UP!");
      game_colour_message(12, 12, 12 + 9, 25, 0);

      game_update_stats();
    }
  }

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

  index_d = spr_calc_index(lin[SPR_P1] + 16, col[SPR_P1]);
  v1 = scr_map[index_d];

  if (s_lin0 == GAME_LIN_FLOOR) {
    tmp1 = 1 + (scr_curr / map_heigth);
    if (tmp1 < map_heigth) {
      v1 = TILE_EMPTY;
    } else {
      v1 = TILE_CEIL;
    }
  } else {
    if ((col[SPR_P1] & 1) == 0) {
      v2 = TILE_EMPTY;
    } else {
      v2 = scr_map[index_d + 1];
    }
  }

  if ((v1 < TILE_FLOOR || v1 >= TILE_END) &&
      (v2 < TILE_FLOOR || v2 >= TILE_END)) {
    sprite_speed_alt[SPR_P1] = PLAYER_FALL_SPEED;
    BIT_SET(s_state, STAT_FALL);
    player_check_stairs(0);
    if (!player_onstair && s_lin0 == GAME_LIN_FLOOR) {
      spr_page_down();
    }

  } else {
    if (game_check_time(player_brick_time, 16)) {
      player_brick_time = zx_clock();
      if (v1 == TILE_BRICK3) {
        scr_map[index_d] = game_match_back(index_d); // TILE_EMPTY;
        player_gasta_brick();
      }
      if (v1 == TILE_BRICK2) {
        scr_map[index_d] = TILE_BRICK3;
        player_gasta_brick();
      }
      if (v1 == TILE_BRICK1) {
        scr_map[index_d] = TILE_BRICK2;
        player_gasta_brick();
      }

      if (v2 == TILE_BRICK3) {
        scr_map[index_d + 1] = game_match_back(index_d); // TILE_EMPTY;
        player_gasta_brick();
      }
      if (v2 == TILE_BRICK2) {
        scr_map[index_d + 1] = TILE_BRICK3;
        player_gasta_brick();
      }
      if (v2 == TILE_BRICK1) {
        scr_map[index_d + 1] = TILE_BRICK2;
        player_gasta_brick();
      }
    }
  }
}

void player_gasta_brick() {
  s_lin1 = s_lin0;
  s_col1 = s_col0;
  s_lin0 = lin[SPR_P1] + 16;
  s_col0 = col[SPR_P1];
  spr_back_repaint(); // restore background
  s_lin0 = s_lin1;
  s_col0 = s_col1;
}

unsigned char player_move_jump(void) {

  signed int val_yc;
  unsigned char f_check;
  player_vel_y = player_vel_y + game_gravity;
  sprite_on_air = 1;
  // JUMP BOOST

  if ((player_vel_inc)) {
    if (!((dirs & IN_STICK_FIRE) && (dirs & IN_STICK_UP)) &&
        (player_vel_y > player_vel_y1) && (player_vel_y < 0)) {
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
  // Nirvana don't support odds lines

  if ((s_lin1 & 1) != 0) {
    s_lin1--;
  }

  s_lin1 = lin[SPR_P1] + s_lin1;

  if (s_lin1 > GAME_LIN_FLOOR) {
    if (s_lin1 > GAME_LIN_FLOOR + 56) {
      if (spr_page_up()) {
        return 0;
        // s_lin1 = GAME_LIN_FLOOR;
      } else {
        s_lin1 = 0;
      }
    } else {
      if (spr_page_down()) {
        s_lin1 = 16;
      } else {
        s_lin1 = GAME_LIN_FLOOR;
      }
    }
  }

  if (val_yc < 0) {
    BIT_SET(s_state, STAT_JUMP);
    BIT_CLR(s_state, STAT_FALL);
    // Asending
    f_check = (s_lin0 >> 4) != (s_lin1 >> 0);

    if (f_check) {
      if (game_check_map(s_lin1, col[SPR_P1])) {
        // Hit Platforms
        if (player_hit_platform()) {
          lin[SPR_P1] = (lin[SPR_P1] >> 4) << 4;
        }
        player_vel_y = 0;
      } else {
        lin[SPR_P1] = s_lin1;
      }
    } else {
      lin[SPR_P1] = s_lin1;
    }
  } else {
    // Falling
    BIT_SET(s_state, STAT_FALL);
    BIT_CLR(s_state, STAT_JUMP);
    s_lin1 = s_lin1 + 16;
    f_check = (s_lin0 >> 4) != (s_lin1 >> 0);
    if (f_check) {
      if (game_check_map(s_lin1, col[SPR_P1])) {
        // Jump end
        s_lin1 = s_lin1 - 16;
        player_check_stairs(0);
        if (!player_onstair) {
          player_check_stairs(1);
        }
        if (!player_onstair) {
          s_lin1 = (s_lin1 >> 4) << 4;
          if (lin[SPR_P1] > s_lin1) {
            lin[SPR_P1] = s_lin1 + 16;
            return 0;
          } else {
            lin[SPR_P1] = s_lin1;
          }
        }
        player_vel_y = 0;

        BIT_CLR(s_state, STAT_FALL);
        BIT_CLR(s_state, STAT_JUMP);
        if (lin[SPR_P1] >= GAME_LIN_FLOOR) {
          spr_page_down();
        }
        colint[SPR_P1] = 0;
        return 1;
      } else {
        s_lin1 = s_lin1 - 16;
        lin[SPR_P1] = s_lin1;
      }
    } else {
      s_lin1 = s_lin1 - 16;
      lin[SPR_P1] = s_lin1;
    }
  }

  if (!BIT_CHK(s_state, STAT_DIRL) && !BIT_CHK(s_state, STAT_DIRR)) {
    if (dirs & IN_STICK_LEFT) {
      BIT_SET(s_state, STAT_DIRL);
      BIT_CLR(s_state, STAT_DIRR);
    }
    if (dirs & IN_STICK_RIGHT) {
      BIT_SET(s_state, STAT_DIRR);
      BIT_CLR(s_state, STAT_DIRL);
    }
  }

  if (spr_move_horizontal()) {
    BIT_CLR(s_state, STAT_DIRL);
    BIT_CLR(s_state, STAT_DIRR);
  }
  return 0;
}

void player_open_door(unsigned int f_index, unsigned char f_tile) {
  unsigned char f_open;
  f_open = 0;
  switch (f_tile) {
  case TILE_DOOR_WHITE:
    if (player_keys[0]) {
      zx_border(INK_WHITE);
      f_open = 1;
    }
    break;
  case TILE_DOOR_RED:
    if (player_keys[1]) {
      zx_border(INK_RED);
      f_open = 1;
    }
    break;
  case TILE_DOOR_GREEN:
    if (player_keys[2]) {
      zx_border(INK_GREEN);
      f_open = 1;
    }
    break;
  case TILE_DOOR_CYAN:
    if (player_keys[3]) {
      zx_border(INK_CYAN);
      f_open = 1;
    }
    break;
  }

  if (f_open || game_inmune) {
    scr_map[f_index] = game_match_back(f_index); // TILE_EMPTY;
    game_obj_set(f_index);
    spr_draw_index(f_index);
  }
}

void player_lost_life() {
  unsigned char f_anim;

  s_lin0 = lin[SPR_P1];
  s_col0 = col[SPR_P1];
  spr_init_anim_bullets();
  for (f_anim = 0; f_anim < 8; f_anim++) {
    NIRVANAP_spriteT(f_anim, TILE_EMPTY, 0, 0);
    anim_lin[f_anim] = 0xFF;
  }
  anim_count = 0;
  NIRVANAP_halt();
  // Player Explode
  spr_add_anim(s_lin0 - 16, s_col0, TILE_ANIM_FIRE, 3, 0, 0);
  spr_add_anim(s_lin0, s_col0 - 2, TILE_ANIM_FIRE, 3, 0, 0);
  spr_add_anim(s_lin0, s_col0, TILE_ANIM_FIRE, 3, 0, 0);
  spr_add_anim(s_lin0, s_col0 + 2, TILE_ANIM_FIRE, 3, 0, 0);
  spr_add_anim(s_lin0 + 16, s_col0, TILE_ANIM_FIRE, 3, 0, 0);
  zx_border(INK_BLACK);
  f_anim = 1;
  anim_time = zx_clock();
  while (f_anim) {
    if (game_check_time(anim_time, TIME_ANIM_PLAYER_EXPODE)) {
      anim_time = zx_clock();
      if (anim_count) {
        spr_play_anim();
      } else {
        f_anim = 0;
      }
    }
  }
  for (sprite = 0; sprite < SPR_P1; ++sprite) {
    if (class[SPR_P1] != 0) {
      NIRVANAP_spriteT(sprite, tile[SPR_P1] + colint[SPR_P1], lin[SPR_P1],
                       col[SPR_P1]);
    }
    // Clean Sprites
    class[SPR_P1] = 0;
    // Clean Bullets
    bullet_col[SPR_P1] = 0xFF;
  }

  // Player lost life
  --player_lives;
  player_mana = GAME_START_MAX_MANA;
  player_vita = GAME_START_MAX_VITA;
  // game_update_stats();

  if (player_lives > 0) {
    z80_delay_ms(500);
    game_round_init();
  } else {
    // Game End
    player_lives = 0;
    game_over = 1;
  }
}

void player_hit(unsigned char f_val) __z88dk_fastcall {
  if (player_vita > f_val) {
    if (!game_inmune)
      player_vita = player_vita - f_val;
    player_hit_time = curr_time;
    game_update_stats();
  } else {
    player_vita = 0;
    game_update_stats();
    player_lost_life();
  }
}
