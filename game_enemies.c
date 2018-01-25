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
//#include "game_sound.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include "nirvana+.h"
#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>

void enemy_turn(void) {
unsigned char f_class;
  sprite = 0;

  while (sprite < SPR_P1) {
    f_class = class[sprite];
    if (f_class > 0 && spr_chktime(&sprite)) {
      s_lin0 = lin[sprite];
      s_col0 = col[sprite];
      s_tile0 = tile[sprite] + colint[sprite];
      s_state = state[sprite];
      enemy_move();
      if (spr_redraw()) {
        // The sprite have moved...
        index1 = spr_calc_index(lin[sprite], col[sprite]);
        if (scr_map[index1] > TILE_ITEM_E & scr_map[index1] < TILE_FLOOR) {
          // Deadly Backgrounds
          if (f_class != FIRE && f_class != PIRANHA ) {
            enemy_kill(sprite);
          } else {
            if ( BIT_CHK(s_state, STAT_FALL) ) {
              BIT_SET(s_state, STAT_JUMP);
              BIT_CLR(s_state, STAT_FALL);
              if (class[sprite] == PIRANHA) {
                tile[sprite] = tile[sprite] + 1;
              }
            } else {
              BIT_CLR(s_state, STAT_JUMP);
              BIT_SET(s_state, STAT_FALL);
              if (class[sprite] == PIRANHA) {
                tile[sprite] = tile[sprite] - 1;
              }
            }
          }
        }
      }
      state[sprite] = s_state;
      last_time[sprite] = zx_clock();
    }
    ++sprite;
  }
}

void boss_turn() {

  if (game_check_time(boss_time, 6)) {
    s_col1 = boss_col;
    s_lin1 = boss_lin;

    // MOVE VERTICAL
    if (BIT_CHK(boss_stat, STAT_JUMP)) {
      boss_lin = boss_lin - 2;
      if (boss_lin < 48) {
        BIT_CLR(boss_stat, STAT_JUMP);
        BIT_SET(boss_stat, STAT_FALL);
      }
    } else {
      boss_lin = boss_lin + 2;
      if (boss_lin >= (GAME_LIN_FLOOR - 64)) {
        BIT_CLR(boss_stat, STAT_FALL);
        BIT_SET(boss_stat, STAT_JUMP);
      }
    }
    if (game_check_time(boss_time_fire, 64)) {
      --boss_col;
      --boss_col;
      tmp0 = 0;
      game_shoot_fire_boss(TILE_FIREBALL, 1);
      if (game_world == 1) {
        tmp0 = 1;
        game_shoot_fire_boss(TILE_FIREBALL, 1);
      }
      boss_time_fire = zx_clock();
      ++boss_col;
      ++boss_col;
    }

    // PAINT
    spr_hack = 1;
    NIRVANAP_halt();
    intrinsic_di();
    if (boss_inc) {
      boss_inc = 0;
      boss_draw(0);
    } else {
      boss_inc = 1;
      boss_draw(2);
    }
    intrinsic_ei();
    spr_hack = 0;
    boss_time = zx_clock();
  }
}

void boss_draw(unsigned char f_inc) {
  NIRVANAP_drawT_raw(boss_tile + f_inc + 0, boss_lin, s_col1);
  NIRVANAP_drawT_raw(boss_tile + f_inc + 1, boss_lin, s_col1 + 2);
  NIRVANAP_drawT_raw(boss_tile + f_inc + 12, boss_lin + 16, s_col1);
  NIRVANAP_drawT_raw(boss_tile + f_inc + 13, boss_lin + 16, s_col1 + 2);
}

void enemy_move(void) {

  switch (sprite_kind[class[sprite]]) {
  case E_STATIC:
    enemy_static();
    break;
  case E_HORIZONTAL:
    enemy_horizontal();
    break;
  case E_WALK:
    enemy_walk();
    break;
  case E_VERTICAL:
    enemy_vertical();
    break;
  case E_GHOST:
    enemy_ghost();
    break;
  case E_GOTA:
    enemy_gota();
    break;
  }
}

void enemy_gota() {

  if (colint[sprite] == 0) {
    if (spr_move_down()) {
      colint[sprite] = 1;
    }
  } else {
    colint[sprite]++;
    if (colint[sprite] == sprite_frames[class[sprite]]) {
      // Respawn!
      lin[sprite] = ( game_respawn_index[sprite] >> 4) << 4;
      colint[sprite] = 0;
    }
  }
}

void enemy_ghost() {
  unsigned char v;
  v = 0;

  if (col[sprite] < col[SPR_P1]) {
    BIT_CLR(s_state, STAT_DIRL);
    BIT_SET(s_state, STAT_DIRR);
    tile[sprite] = spr_tile(sprite);
    spr_move_right();
    v = 1;
  }
  if (col[sprite] > col[SPR_P1]) {
    BIT_CLR(s_state, STAT_DIRR);
    BIT_SET(s_state, STAT_DIRL);
    tile[sprite] = spr_tile(sprite);
    spr_move_left();
    v = 1;
  }

  if (s_col0 == col[sprite]) {
    if (lin[sprite] > lin[SPR_P1]) {
      spr_move_up();
    }
    if (lin[sprite] < lin[SPR_P1]) {
      spr_move_down();
    }
    if (v == 0 && s_lin0 != lin[sprite]) {
      ++colint[sprite];
      if (colint[sprite] == sprite_frames[class[sprite]]) {
        colint[sprite] = 0;
      }
    }
  } else {
    spr_back_repaint();
    s_col0 = col[sprite];
  }
}

void enemy_static() {

  ++colint[sprite];
  if (colint[sprite] == sprite_frames[class[sprite]]) {
    colint[sprite] = 0;
  }
}

void enemy_horizontal() {
  if (BIT_CHK(s_state, STAT_DIRR)) {
    if (spr_move_right()) {
      BIT_CLR(s_state, STAT_DIRR);
      BIT_SET(s_state, STAT_DIRL);
    }
  } else {
    if (spr_move_left()) {
      BIT_CLR(s_state, STAT_DIRL);
      BIT_SET(s_state, STAT_DIRR);
    }
  }
}

void enemy_vertical() {

  if (BIT_CHK(s_state, STAT_FALL)) {
    if (spr_move_down()) {
      BIT_CLR(s_state, STAT_FALL);
      BIT_SET(s_state, STAT_JUMP);
      if (class[sprite] == PIRANHA) {
        tile[sprite] = tile[sprite] + 1;
      }
    }
  } else {
    if (spr_move_up()) {
      BIT_CLR(s_state, STAT_JUMP);
      BIT_SET(s_state, STAT_FALL);
      if (class[sprite] == PIRANHA) {
        tile[sprite] = tile[sprite] - 1;
      }
    }
  }

  if (class[sprite] == DRAGON) {
    if (bullet_col[sprite] == 0xFF) {
      if (abs(lin[SPR_P1] - lin[sprite]) < 8) {

        if ((BIT_CHK(s_state, STAT_DIRR) && col[SPR_P1] > col[sprite]) ||
            (BIT_CHK(s_state, STAT_DIRL) && col[SPR_P1] < col[sprite])) {
          game_shoot_fire(sprite, TILE_FIREBALL);
        }
      }
    }
  }

  ++colint[sprite];
  if (colint[sprite] >= sprite_frames[class[sprite]]) {
    colint[sprite] = 0;
  }
}

void enemy_walk(void) {

  if (!BIT_CHK(s_state, STAT_FALL)) {
    // WALKING
    if (spr_move_horizontal()) {
      spr_turn_horizontal();
    } else {
      // Check Floor
      index1 = spr_calc_index(lin[sprite] + 16, col[sprite]);
      if (scr_map[index1] < TILE_FLOOR) {
        BIT_SET(s_state, STAT_FALL);
      }
    }

    if ((col[sprite] & 1) == 0) {

      if (class[sprite] == MUSHROOM_VITA || class[sprite] == MUSHROOM_MANA ||
          class[sprite] == MUSHROOM_EXTRA) {
        if (!BIT_CHK(s_state, STAT_FALL)) {

          if (col[sprite] == 0 || col[sprite] == 30) {
            spr_back_repaint();
            spr_destroy(sprite);
            --game_mush_count;
          }
        }

      } else {
        if (col[sprite] == 0 || col[sprite] == 30) {
          spr_turn_horizontal();
        }
      }

      if (class[sprite] == SKELETON) {
        enemy_avoid_dead();
      }

      if (class[sprite] == ORC || class[sprite] == WARG) {
        enemy_avoid_fall();
      }

      if (class[sprite] == ELF) {
        enemy_avoid_fall();
        if (bullet_col[sprite] == 0xFF) {
          if (abs(lin[SPR_P1] - lin[sprite]) < 32) { // TODO VARIABLE
            if ((BIT_CHK(s_state, STAT_DIRR) && col[SPR_P1] > col[sprite]) || (
                    BIT_CHK(s_state, STAT_DIRL) && col[SPR_P1] < col[sprite])) {

              game_shoot_fire(sprite, TILE_ARROW);
            }
          }
        }
      }

      if (class[sprite] == DWARF) {
        enemy_avoid_fall();
        if (bullet_col[sprite] == 0xFF) {
          s_col1 = abs(col[SPR_P1] - col[sprite]);
          if (s_col1 < 6) {
            if ((BIT_CHK(s_state, STAT_DIRL) && (col[sprite] > col[SPR_P1])) ||
                (BIT_CHK(s_state, STAT_DIRR) && (col[sprite] < col[SPR_P1]))) {

              game_shoot_fire(sprite, TILE_AXE);
            }
          }
        }
      }
    }

  } else {
    if (spr_move_down()) {
      BIT_CLR(s_state, STAT_FALL);
    }
  }
}

void enemy_kill( unsigned char f_sprite ) {
  game_respawn_time[f_sprite] = zx_clock();
  s_lin0 = lin[f_sprite];
  s_col0 = col[f_sprite];
  spr_destroy(f_sprite);
  spr_bullet_explode();
  spr_add_anim(s_lin0, s_col0, TILE_ANIM_DEAD, 3, 0, 0);
  ay_fx_play(ay_effect_02);
}

void enemy_avoid_fall() {

  /* Don't fall on edge*/
  if (BIT_CHK(s_state, STAT_DIRR)) {
    index1 = spr_calc_index(lin[sprite] + 16, col[sprite] + 2);
  } else {
    index1 = spr_calc_index(lin[sprite] + 16, col[sprite] - 2);
  }
  if (scr_map[index1] < TILE_FLOOR) {
    spr_turn_horizontal();
  }
}

unsigned char enemy_avoid_dead() {

  index1 = spr_calc_index(lin[sprite], col[sprite]);
  if (BIT_CHK(s_state, STAT_DIRR)) {
    index1++;
  } else {
    index1--;
  }

  /* Don't fall on edge*/
  tmp0 = scr_map[index1];
  if (tmp0 > TILE_ITEM_E && tmp0 < TILE_FLOOR) {
    spr_turn_horizontal();
    return 1;
  }

  /* Don't hit on Deadly*/
  index1 = index1 + 16;
  tmp0 = scr_map[index1];
  if (tmp0 > TILE_ITEM_E && tmp0 < TILE_FLOOR) {
    spr_turn_horizontal();
    return 1;
  }

  return 0;
}

void enemy_respawn(unsigned char f_anim) {
  unsigned char i;
  s_lin1 = s_lin0;
  s_col1 = s_col0;
  index1 = spr_calc_index(s_lin1, s_col1);
  i = 0;
  while (i < SPR_P1) {
    if (game_respawn_index[i] == index1) {
      if (game_boss_alive)
        game_add_enemy(anim_respanwn[f_anim]);
      break;
    }
    ++i;
  }
}

void enemy_init(unsigned char f_lin, unsigned char f_col, unsigned char f_class,
                unsigned char f_dir) {
  unsigned char f_sprite;
  // Get the first available sprite
  f_sprite = 0;
  while (f_sprite < SPR_P1) {
    if (class[f_sprite] == 0 && game_respawn_time[f_sprite] == 0) {
      break;
    } else {
      f_sprite++;
    }
  }

  // Out of sprites
  if (f_sprite < SPR_P1) {

    class[f_sprite] = f_class;

    lin[f_sprite] = f_lin;
    col[f_sprite] = f_col;
    state[f_sprite] = 0;
    state_a[f_sprite] = 0;
    jump_lin[f_sprite] = 0;
    colint[f_sprite] = 0;
    if (f_dir == DIR_RIGHT) {
      BIT_SET(state[f_sprite], STAT_DIRR);
    }
    if (f_dir == DIR_LEFT) {
      BIT_SET(state[f_sprite], STAT_DIRL);
      colint[f_sprite] = sprite_frames[f_class] - 1;
    }
    tile[f_sprite] = spr_tile(f_sprite);
    spr_timer[f_sprite] = zx_clock();
    last_time[f_sprite] = 0;
    sprite_speed_alt[f_sprite] = 0;
    ++spr_count;
    if (!game_boss) {
      if (bullet_col[f_sprite] != 0xFF) {
        s_col0 = bullet_col[f_sprite];
        s_lin0 = bullet_lin[f_sprite];
        spr_back_repaint();
      }
      bullet_col[f_sprite] = 0xFF;
    }
  }
}
