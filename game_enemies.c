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
  for (sprite = 0; sprite < 6; ++sprite) {
    if (class[sprite] > 0 && spr_chktime(&sprite)) {
      s_lin0 = lin[sprite];
      s_col0 = col[sprite];
      s_tile0 = tile[sprite] + colint[sprite];
      s_state = state[sprite];
      last_time[sprite] = zx_clock();
      enemy_move();
      if (class[sprite] != 0) {
        // The enemy can be out of screen or dead
        spr_redraw();
        state[sprite] = s_state;
      }
    }
  }
}

void enemy_move(void) {
  switch (class[sprite]) {
  case SKELETON:
    enemy_walk();
    break;
  }
}

void enemy_walk(void) {

  if (!BIT_CHK(s_state, STAT_FALL)) {
    if( spr_move_horizontal() ) {
      if (BIT_CHK(s_state, STAT_DIRR)) {
        BIT_SET(s_state, STAT_DIRL);
        BIT_CLR(s_state, STAT_DIRR);
      } else {
        BIT_CLR(s_state, STAT_DIRL);
        BIT_SET(s_state, STAT_DIRR);
      }
    }
    if ((col[sprite] & 1) == 0) {
      // Check Floor
      index1 = spr_calc_index(lin[sprite] + 16, col[sprite]);
      if (!game_check_maze(index1)) {
        BIT_SET(s_state, STAT_FALL);
      }
    }
  } else {
    if ( spr_move_down() ) {
      BIT_CLR(s_state, STAT_FALL);
    }
  }

  /*
  if (!BIT_CHK(s_state, STAT_JUMP) && !BIT_CHK(s_state, STAT_FALL)) {
          if ( (col[sprite] & 1) && !BIT_CHK(s_state, STAT_HIT) ) {
                  tmp = 0;
          } else {
                  tmp = game_check_maze(spr_calc_index(lin[sprite] +
  16,col[sprite]));
          }
  } else {
          tmp = game_check_maze( spr_calc_index( lin[sprite] + 16 , col[sprite]
  ) );
  }

  if ( tmp ) {
          //FALLING
          if ( !BIT_CHK(s_state, STAT_JUMP) ) {
                  BIT_SET(s_state, STAT_FALL);
          }
          if (jump_lin[sprite] == 0) {
                  jump_lin[sprite] = lin[sprite];
          }
          if ( BIT_CHK(s_state, STAT_FALL) ) {
                  spr_move_down();
          } else {
                  spr_move_up();
          }
          sprite_speed_alt[sprite] = ENEMY_FALL_SPEED;
          tmp = 1;
          tmp0 = game_check_maze( spr_calc_index( lin[sprite]+16, col[sprite] +
  1 ) ); tmp1 = game_check_maze( spr_calc_index( lin[sprite]+16, col[sprite] - 1
  ) ); tmp = !tmp0 && !tmp1; if ( tmp && ( lin[sprite] - jump_lin[sprite] <= 8 )
  ) spr_move_horizontal(); } else {
          //OVER PLATFORM
          if ( BIT_CHK(s_state, STAT_FALL) ) {
                  BIT_CLR(s_state, STAT_FALL);
                  jump_lin[sprite] = 0;
                  sprite_speed_alt[sprite] = 0;
          }
          if ( !BIT_CHK(state_a[sprite], STAT_TURN) ) {
                  spr_move_horizontal();
          } else {
                  spr_timer[sprite] = zx_clock();
                  if (colint[sprite] < 2) {
                          ++colint[sprite];
                  } else {
                          BIT_CLR(state_a[sprite], STAT_TURN);
                          tile[sprite] = spr_tile(sprite);
                  }
          }
  }
  */
}

void enemy_init(unsigned char f_sprite, unsigned char f_lin,
                unsigned char f_col, unsigned char f_class,
                unsigned char f_dir) {
  ++spr_count;
  if (spr_count < SPR_P1) {
    class[f_sprite] = f_class;
    lin[f_sprite] = f_lin;
    col[f_sprite] = f_col;
    tmp = 0;
    state[f_sprite] = 0;
    state_a[f_sprite] = 0;
    jump_lin[f_sprite] = 0;

    if (f_dir == DIR_RIGHT) {
      BIT_SET(state[f_sprite], STAT_DIRR);
    }
    if (f_dir == DIR_LEFT) {
      BIT_SET(state[f_sprite], STAT_DIRL);
    }
    colint[f_sprite] = 0;
    tile[f_sprite] = spr_tile(f_sprite);
    last_time[f_sprite] = 0;
    spr_timer[f_sprite] = zx_clock();
    sprite_speed_alt[f_sprite] = 0;
  }
}

void enemy_kill(unsigned char f_sprite) __z88dk_fastcall {
  tmp = f_sprite; // DUMMY
  sound_kill();
}
