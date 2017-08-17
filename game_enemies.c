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
        spr_redraw();

        state[sprite] = s_state;
        last_time[sprite] = zx_clock();
      }
    }
    ++sprite;
  }
}

void enemy_move(void) {

  if (class[sprite] <= SPIDER) {
    enemy_vertical();
  } else {
    if (class[sprite] < PLANT) {
      enemy_walk();
    } else {
      enemy_static();
    }
  }
}

void enemy_static() {

  ++colint[sprite];
  if (colint[sprite] == sprite_frames[class[sprite]]) {
    colint[sprite] = 0;
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
  ++colint[sprite];
  if (colint[sprite] == sprite_frames[class[sprite]]) {
    colint[sprite] = 0;
  }
}

void enemy_walk(void) {

  if (!BIT_CHK(s_state, STAT_FALL)) {
    if (spr_move_horizontal()) {
      spr_turn_horizontal();
    }
    if ((col[sprite] & 1) == 0) {
      // Check Floor
      index1 = spr_calc_index(lin[sprite] + 16, col[sprite]);
      if (!game_check_cell(index1)) {
        BIT_SET(s_state, STAT_FALL);
      }

      if (col[sprite] == 0 || col[sprite] == 30) {
        spr_turn_horizontal();
      }

      if (class[sprite] == ORC || class[sprite] == ELF) {
        enemy_avoid_edge();
      }
    }
  } else {
    if (spr_move_down()) {
      BIT_CLR(s_state, STAT_FALL);
    }
  }
}

void enemy_avoid_edge() {
  /* Don't fall on edge*/
  if (BIT_CHK(s_state, STAT_DIRR)) {
    index1 = spr_calc_index(lin[sprite] + 16, col[sprite] + 2);
  } else {
    index1 = spr_calc_index(lin[sprite] + 16, col[sprite] - 2);
  }
  if (!game_check_cell(index1)) {
    spr_turn_horizontal();
  }
}

void enemy_init(unsigned char f_lin, unsigned char f_col, unsigned char f_class,
                unsigned char f_dir) {
  //unsigned int f_index;
  //unsigned int f_lin1;
  if (spr_count < SPR_P1) {
    class[spr_count] = f_class;
    lin[spr_count] = f_lin;
    col[spr_count] = f_col;
    tmp = 0;
    state[spr_count] = 0;
    state_a[spr_count] = 0;
    jump_lin[spr_count] = 0;

    if (f_dir == DIR_RIGHT) {
      BIT_SET(state[spr_count], STAT_DIRR);
    }
    if (f_dir == DIR_LEFT) {
      BIT_SET(state[spr_count], STAT_DIRL);
    }
    colint[spr_count] = 0;
    tile[spr_count] = spr_tile(spr_count);

    spr_timer[spr_count] = zx_clock();
    last_time[spr_count] = 0;
    sprite_speed_alt[spr_count] = 0;
    /*
    if (f_class < SPIDER) {
      // Speed up enemies calculating max/min lin's

      // up
      f_index = index1 - 16;
      f_lin1 = f_lin - 16;
      while (1) {
        if (scr_map[f_index] >= TILE_ITEM_E)
           {
            lin_min[spr_count] = f_lin1 + 16;

            break;
          }
        if (f_lin1 > GAME_LIN_FLOOR) {
          lin_min[spr_count] = 0;
          break;
        }
        f_index = f_index - 16;
        f_lin1 = f_lin1 - 16;
      }

      // down
      f_index = index1 + 16;
      f_lin1 = f_lin + 16;
      while (1) {
        if (scr_map[f_index] >= TILE_ITEM_E)
           {
            lin_max[spr_count] = f_lin1 - 16;
            break;
          }
        if (f_lin1 > GAME_LIN_FLOOR) {
          lin_max[spr_count] = GAME_LIN_FLOOR;
          break;
        }
        f_index = f_index + 16;
        f_lin1 = f_lin1 + 16;
      }
    }
    */
    ++spr_count;
  }
}

void enemy_kill(unsigned char f_sprite) __z88dk_fastcall {
  tmp = f_sprite; // DUMMY
  sound_kill();
}
