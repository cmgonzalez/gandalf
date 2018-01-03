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
#include <stdlib.h>

void enemy_hit(void) {
  // TODO
  sound_coin();
}

void enemy_turn(void) {

  sprite = 0;

  while (sprite < SPR_P1) {
    if (class[sprite] > 0) {
      if (spr_chktime(&sprite)) {
        s_lin0 = lin[sprite];
        s_col0 = col[sprite];
        s_tile0 = tile[sprite] + colint[sprite];
        s_state = state[sprite];
        enemy_move();
        if ( spr_redraw() ) {
          //The sprite have moved...
          index1 = spr_calc_index(lin[sprite], col[sprite]);
          if (scr_map[index1] > TILE_ITEM_E & scr_map[index1] < TILE_FLOOR) {
            // Deadly Backgrounds
            s_lin0 = lin[sprite];
            s_col0 = col[sprite];
            spr_destroy(sprite);
            spr_add_anim(s_lin0, s_col0, TILE_ANIM_FIRE, 3, 0, 0);
          }
        }
        state[sprite] = s_state;
        last_time[sprite] = zx_clock();
      }
    }
    ++sprite;
  }
}

void boss_turn() {

  if (game_check_time(boss_time, 6)) {
    s_col1 = boss_col;
    s_lin1 = boss_lin;
    if (game_world == 0) {

      // MOVE VERTICAL
      if (BIT_CHK(boss_stat, STAT_JUMP)) {
        boss_lin = boss_lin - 2;
        if (boss_lin < 32) {
          BIT_CLR(boss_stat, STAT_JUMP);
          BIT_SET(boss_stat, STAT_FALL);
        }
      } else {
        boss_lin = boss_lin + 2;
        if (boss_lin >= (GAME_LIN_FLOOR - 48)) {
          BIT_CLR(boss_stat, STAT_FALL);
          BIT_SET(boss_stat, STAT_JUMP);
        }
      }
      if (game_check_time(boss_time_fire, 64)) {
        --boss_col;
        --boss_col;
        game_shoot_fire_boss(TILE_FIREBALL, 1);
        boss_time_fire = zx_clock();
        ++boss_col;
        ++boss_col;
      }
    }
    // PAINT
    spr_hack = 1;
    NIRVANAP_halt();
    intrinsic_di();
    if (boss_inc) {
      boss_inc = 0;
      s_col0 = s_col1;
      s_lin0 = s_lin1;
      spr_back_repaint();
      s_lin0 = s_lin1 + 16;
      spr_back_repaint();
      s_col0 = s_col1 + 2;
      spr_back_repaint();
      s_lin0 = s_lin1;
      spr_back_repaint();
      NIRVANAP_drawT_raw(boss_tile, boss_lin, boss_col + 1);
      NIRVANAP_drawT_raw(boss_tile + 12, boss_lin + 16, boss_col + 1);
    } else {
      boss_inc = 1;
      if (boss_lin > s_lin1) {
        s_col0 = s_col1 + 1;
        s_lin0 = s_lin1;
        spr_back_repaint();
      }
      if (boss_lin < s_lin1) {
        s_col0 = s_col1 + 1;
        s_lin0 = s_lin1 + 16;
        spr_back_repaint();
      }
      NIRVANAP_drawT_raw(boss_tile + 1, boss_lin, s_col1);
      NIRVANAP_drawT_raw(boss_tile + 2, boss_lin, s_col1 + 2);
      NIRVANAP_drawT_raw(boss_tile + 13, boss_lin + 16, s_col1);
      NIRVANAP_drawT_raw(boss_tile + 14, boss_lin + 16, s_col1 + 2);
    }
    intrinsic_ei();
    spr_hack = 0;
    boss_time = zx_clock();
  }
}

void enemy_move(void) {

  if (class[sprite] <= SPIDER) {
    enemy_vertical();
  } else {
    if (class[sprite] < PLANT) {
      enemy_walk();
    } else {
      if (class[sprite] < BAT_H) {
        enemy_static();
      } else {
        enemy_horizontal();
      }
    }
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

  if (BIT_CHK(s_state, STAT_JUMP)) {
    if (spr_move_up()) {
      BIT_CLR(s_state, STAT_JUMP);
      BIT_SET(s_state, STAT_FALL);
    }
  } else {
    if (spr_move_down()) {
      BIT_CLR(s_state, STAT_FALL);
      BIT_SET(s_state, STAT_JUMP);
    }
  }

  if (class[sprite] == DRAGON) {
    if (abs(lin[SPR_P1] - lin[sprite]) < 8) {
      if (BIT_CHK(s_state, STAT_DIRR) && col[SPR_P1] > col[sprite]) {
        game_shoot_fire(sprite, TILE_FIREBALL);
      }
      if (BIT_CHK(s_state, STAT_DIRL) && col[SPR_P1] < col[sprite]) {
        game_shoot_fire(sprite, TILE_FIREBALL);
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

      if (class[sprite] == MUSHROOM_MANA || class[sprite] == MUSHROOM_MANA ||
          class[sprite] == MUSHROOM_MANA) {
        if (!BIT_CHK(s_state, STAT_FALL)) { // TODO MOVE TO ENEMY WALK
          if (col[sprite] == 0 || col[sprite] == 30) {
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
        if (abs(lin[SPR_P1] - lin[sprite]) < 32) { // TODO VARIABLE
          if (BIT_CHK(s_state, STAT_DIRR) && col[SPR_P1] > col[sprite]) {
            game_shoot_fire(sprite, TILE_ARROW);
          }
          if (BIT_CHK(s_state, STAT_DIRL) && col[SPR_P1] < col[sprite]) {
            game_shoot_fire(sprite, TILE_ARROW);
          }
        }
      }

      if (class[sprite] == DWARF) {
        enemy_avoid_fall();
        s_col1 = abs(col[SPR_P1] - col[sprite]);
        if (s_col1 < 6) {
          if ((BIT_CHK(s_state, STAT_DIRL) && (col[sprite] > col[SPR_P1])) ||
              (BIT_CHK(s_state, STAT_DIRR) && (col[sprite] < col[SPR_P1]))) {
            game_shoot_fire(sprite, TILE_AXE);
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

  s_lin1 = s_lin0;
  s_col1 = s_col0;
  index1 = spr_calc_index(s_lin1, s_col1);
  sprite = 0;
  while (sprite < SPR_P1) {
    if (game_respawn_index[sprite] == index1) {
      game_add_enemy(anim_respanwn[f_anim]);
      break;
    }
    ++sprite;
  }
}

void enemy_init(unsigned char f_lin, unsigned char f_col, unsigned char f_class,
                unsigned char f_dir) {
  unsigned char f_sprite;
  // Get the first available sprite
  f_sprite = 0;
  while (f_sprite < SPR_P1) {
    if (class[f_sprite] == 0) {
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
    tmp = 0;
    state[f_sprite] = 0;
    state_a[f_sprite] = 0;
    jump_lin[f_sprite] = 0;
    colint[f_sprite] = 0;
    if (f_dir == DIR_RIGHT) {
      BIT_SET(state[f_sprite], STAT_DIRR);
    }
    if (f_dir == DIR_LEFT) {
      BIT_SET(state[f_sprite], STAT_DIRL);
      colint[f_sprite] = sprite_frames[class[sprite]];
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
