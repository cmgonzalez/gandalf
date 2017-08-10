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

unsigned int spr_calc_index(unsigned char f_lin, unsigned char f_col) {
  return (f_col >> 1) + ((f_lin >> 4) << 4);
}

unsigned int spr_check_map(unsigned char f_lin, unsigned char f_col) {
  // TODO A SINGLE FUNCTION TO SAVE BYTES
  if ((f_col & 1) == 0) {
    index1 = spr_calc_index(f_lin, f_col);

    return game_check_maze(index1);
  } else {
    index1 = spr_calc_index(f_lin, f_col - 1);
    g_player_hit_left = 0;
    g_player_hit_right = 0;

    if (game_check_maze(index1)) {
      g_player_hit_left = 1;
    }
    if (game_check_maze(index1 + 1)) {
      g_player_hit_right = 1;
    }
    return g_player_hit_left || g_player_hit_right;
  }
}

unsigned int spr_check_map_fall(unsigned char f_lin, unsigned char f_col) {
  // TODO A SINGLE FUNCTION TO SAVE BYTES
  if ((f_col & 1) == 0) {
    index1 = spr_calc_index(f_lin, f_col);

    return game_check_maze_floor(index1);
  } else {
    index1 = spr_calc_index(f_lin, f_col - 1);
    g_player_hit_left = 0;
    g_player_hit_right = 0;

    if (game_check_maze_floor(index1)) {
      g_player_hit_left = 1;
    }
    if (game_check_maze_floor(index1 + 1)) {
      g_player_hit_right = 1;
    }
    return g_player_hit_left || g_player_hit_right;
  }
}

unsigned char spr_chktime(unsigned char *sprite) __z88dk_fastcall {
  // if (FULL_SPEED) return 1;
  if (sprite_speed_alt[*sprite] == 0) {
    tmp = sprite_speed[class[*sprite]];
  } else {
    tmp = sprite_speed_alt[*sprite];
  }
  if (game_check_time(last_time[*sprite], tmp)) {
    last_time[*sprite] = zx_clock();
    return 1;
  }
  return 0;
}

unsigned char spr_move_jump(void) {
  signed int val_yc;

  player_vel_y = player_vel_y + game_gravity;

  if (player_vel_y > 120) {
    player_vel_y = 120;
  }

  if (player_vel_y < -120) {
    player_vel_y = -120;
  }

  val_yc = player_vel_y / 10;

  s_lin1 = (unsigned char)val_yc;

  if ((s_lin1 & 1) != 0) {
    // Nirvana don't support odd lin's
    s_lin1++;
  }
  s_lin1 = lin[sprite] + s_lin1;

  if (s_lin1 > GAME_LIN_FLOOR) {
    s_lin1 = GAME_LIN_FLOOR;
  }

  if (val_yc < 0) {
    // Asending
    if (spr_check_map(s_lin1, col[sprite])) {
      // Start Falling
      if (player_hit_platform()) {
        lin[sprite] = (lin[sprite] >> 4) << 4;
      }
      player_vel_y = 0;
      BIT_CLR(s_state, STAT_JUMP);
      BIT_SET(s_state, STAT_FALL);
    } else {
      lin[sprite] = s_lin1;
    }
  } else {
    // Falling
    if (spr_check_map_fall(s_lin1 + 16, col[sprite]) ||
        (s_lin1 >= GAME_LIN_FLOOR)) {
      // Jump end
      lin[sprite] = (s_lin1 >> 4) << 4;
      player_vel_y = 0;
      BIT_CLR(s_state, STAT_FALL);
      BIT_CLR(s_state, STAT_JUMP);
      colint[sprite] = 0;

      return 1;
    } else {
      lin[sprite] = s_lin1;
    }
  }

  spr_move_horizontal();
  return 0;
}

unsigned char spr_move_up(void) {
  // 12
  // 43
  tmp1 = lin[sprite] - (SPRITE_LIN_INC << sprite_lin_inc_mul);
  if (class[sprite] == PLAYER) {
    if (spr_check_map(tmp1, col[sprite])) {
      /* Only Players can hit objects */
      if (sprite == SPR_P1 && !BIT_CHK(state_a[SPR_P1], STAT_HITBRICK)) {
        player_hit_platform();
      }
      return 1;
    }
  }
  lin[sprite] = tmp1;
  if (lin[sprite] == 0 || lin[sprite] > GAME_LIN_FLOOR) {
    lin[sprite] = 0;
    NIRVANAP_fillT(18, s_lin0, s_col0);
    return 1;
  }
  return 0;
}

unsigned char spr_move_down(void) {
  // 12
  // 43
  tmp1 = lin[sprite] + (SPRITE_LIN_INC << sprite_lin_inc_mul);
  if (spr_check_map_fall(tmp1 + 16, col[sprite])) {
    lin[sprite] = (tmp1 >> 3) << 3; // div 8 mul 8
    return 1;
  }
  lin[sprite] = tmp1;
  if (lin[sprite] > GAME_LIN_FLOOR) {
    lin[sprite] = GAME_LIN_FLOOR;
    return 1;
  }
  return 0;
}

unsigned char spr_move_horizontal(void) {
  if (BIT_CHK(state[sprite], STAT_DIRR)) {
    return spr_move_right();
  } else {
    if (BIT_CHK(state[sprite], STAT_DIRL)) {
      return spr_move_left();
    }
  }
  return 0;
}

unsigned char spr_move_right(void) {
  ++colint[sprite];
  if (colint[sprite] == sprite_frames[class[sprite]]) {
    s_lin1 = lin[sprite];
    if (col[sprite] < 30) {
      s_col1 = col[sprite] + 1;
      if (spr_check_map(s_lin1, s_col1) || spr_check_map(s_lin1 + 15, s_col1)) {
        --colint[sprite];
        --colint[sprite];
        return 1;
      }
    } // else {
    //	s_col1 = 0;
    //}

    ++col[sprite];
    colint[sprite] = 0;
    if (col[sprite] > SCR_COLS_M) {
      if (sprite == SPR_P1) {
        if (spr_page_right()) {
          col[sprite] = 0;
        } else {
          col[sprite] = SCR_COLS_M;
        }
      } else {
        col[sprite] = SCR_COLS_M;
        spr_destroy(sprite);
      }
    }
  }
  return 0;
}

unsigned char spr_move_left(void) {
  --colint[sprite];
  if (colint[sprite] == 255) {
    s_lin1 = lin[sprite];
    if (col[sprite] > 0) {
      s_col1 = col[sprite] - 1;
      if (spr_check_map(s_lin1, s_col1) || spr_check_map(s_lin1 + 15, s_col1)) {
        ++colint[sprite];
        ++colint[sprite];
        return 1;
      }
    } // else {
      // s_col1 = 30;
    //}

    --col[sprite];
    colint[sprite] = sprite_frames[class[sprite]] - 1;
    if (col[sprite] == 255) {
      if (sprite == SPR_P1) {
        if (spr_page_left()) {
          col[sprite] = SCR_COLS_M;
        } else {
          col[sprite] = 0;
        }
      } else {
        col[sprite] = 0;
        spr_destroy(sprite);
      }
    }
  }
  return 0;
}

unsigned char spr_page_right() {

  if (scr_curr < map_width) {
    ++scr_curr;
    spr_page_map();
    spr_draw_map();
    return 1;
  }
  return 0;
}

unsigned char spr_page_left() {
  if (class[sprite] != PLAYER)
    return 0;

  if (scr_curr > 0) {
    --scr_curr;
    spr_page_map();
    spr_draw_map();
    return 1;
  }
  return 0;
}

void spr_page_map(void) {
  unsigned char v0;
  unsigned char v1;
  unsigned char vr;
  unsigned char i;
  unsigned char j;
  unsigned char k;

  k = 16;
  for (i = 0; i < GAME_SCR_MAX_INDEX; ++i) {
    // Decompress map TODO POINTERS!
    switch (scr_curr) {
    case 0:
      v0 = scr_0[i];
      v1 = scr_0[i + 1];
      break;
    case 1:
      v0 = scr_1[i];
      v1 = scr_1[i + 1];
      break;
    case 2:
      v0 = scr_2[i];
      v1 = scr_2[i + 1];
      break;
    case 3:
      v0 = scr_3[i];
      v1 = scr_3[i + 1];
      break;
    case 4:
      v0 = scr_4[i];
      v1 = scr_4[i + 1];
      break;
    case 5:
      v0 = scr_5[i];
      v1 = scr_5[i + 1];
      break;
    case 6:
      v0 = scr_6[i];
      v1 = scr_6[i + 1];
      break;
    case 7:
      v0 = scr_7[i];
      v1 = scr_7[i + 1];
      break;
    }

    if (v0 < 128) {
      scr_map[k] = v0;
      ++k;
    } else {
      vr = v0 - 128; // Repeat counter Should be < 128!!

      for (j = 0; j < vr; j++) {
        scr_map[k] = v1;
        ++k;
        if (k >= GAME_SCR_MAX_INDEX) {
          break;
        }
      }
      ++i;
    }
    if (k >= GAME_SCR_MAX_INDEX) {
      break;
    }
  }
  // Remove all enemies fast
  for (i = 0; i < SPR_P1; ++i) {
    class[i] = 0;
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
  }
  spr_init_anim();
  spr_init_bullet();
}

unsigned char spr_redraw(void) {
  s_tile1 = tile[sprite] + colint[sprite];
  s_col1 = col[sprite];
  s_lin1 = lin[sprite];

  if ((s_lin1 != s_lin0) || (s_col1 != s_col0)) {
    /* Column Movement */
    if (s_lin0 <= 16) {
      intrinsic_di();
      NIRVANAP_fillT_raw(
          PAPER, 8,
          s_col0); // TODO REVIEW ESTO HACE EL CUADRO EN LA PRIMERA LINEA
      intrinsic_ei();
    }
    if (sprite == SPR_P1 && player_over_stair && ((lin[sprite] & 3) == 0)) {
      s_tile1 = s_tile1 + 4;
    }
    NIRVANAP_spriteT(sprite, s_tile1, s_lin1, s_col1);
    spr_back_repaint();
    return 1;
  } else if (s_tile1 != s_tile0) {
    /* Internal Movement, no clean needed */
    NIRVANAP_spriteT(sprite, s_tile1, s_lin1, s_col1);
    return 0;
  }
  return 0;
}

void spr_destroy(unsigned char f_sprite) __z88dk_fastcall {
  spr_count--;
  s_lin0 = lin[f_sprite];
  s_col0 = col[f_sprite];
  NIRVANAP_spriteT(f_sprite, TILE_EMPTY, 0, 0);
  spr_back_repaint();

  tile[f_sprite] = TILE_EMPTY;
  col[f_sprite] = 0;
  lin[f_sprite] = 0;
  class[f_sprite] = 0;
  state[f_sprite] = 0;
  state_a[f_sprite] = 0;
}

unsigned char spr_tile(unsigned char f_sprite) __z88dk_fastcall {
  unsigned char f_inc;
  f_inc = 0;
  switch (class[f_sprite]) {
  case SKELETON:
    return spr_tile_dir(TILE_ENEMY_SKELETON, f_sprite, DIRINC_ENEMY_SKELETON);
    break;
  case ORC:
    return spr_tile_dir(TILE_ENEMY_ORC, f_sprite, DIRINC_ENEMY_ORC);
    break;
  case WARG:
    return spr_tile_dir(TILE_ENEMY_WARG, f_sprite, DIRINC_ENEMY_WARG);
    break;
  case DWARF:
    return spr_tile_dir(TILE_ENEMY_DWARF, f_sprite, DIRINC_ENEMY_DWARF);
    break;
  case ELF:
    return spr_tile_dir(TILE_ENEMY_ELF, f_sprite, DIRINC_ENEMY_ELF);
    break;
  }
  return 0;
}

unsigned char spr_tile_dir(unsigned char f_tile, unsigned char f_sprite,
                           unsigned char f_inc) {

  if (BIT_CHK(state[f_sprite], STAT_DIRR)) {
    return f_tile;
  }
  if (BIT_CHK(state[f_sprite], STAT_DIRL)) {
    return f_tile + f_inc;
  }
  if (BIT_CHK(state_a[f_sprite], STAT_LDIRR)) {
    return f_tile;
  }
  if (BIT_CHK(state_a[f_sprite], STAT_LDIRL)) {
    return f_tile + f_inc;
  }
  return f_tile;
}

void spr_draw_map(void) {

  NIRVANAP_halt();
  index1 = 16;
  s_lin1 = 0;
  s_col1 = 2;
  // intrinsic_ei();
  spr_count = 0;
  intrinsic_di();
  while (index1 < ((GAME_ROWS - 1) * 16)) {

    if (index1 % 16 == 0) {
      s_lin1 = s_lin1 + 16;
      s_col1 = 0;
    }

    if (scr_map[index1] < TILE_END) {
      // TILES
      NIRVANAP_drawT_raw(scr_map[index1], s_lin1, s_col1);
    } else {
      // ENEMIES
      switch (scr_map[index1]) {
      case INDEX_SKELETON_RIGHT:
        enemy_init(s_lin1, s_col1, SKELETON, DIR_RIGHT);
        break;
      case INDEX_SKELETON_LEFT:
        enemy_init(s_lin1, s_col1, SKELETON, DIR_LEFT);
        break;
      case INDEX_ORC_RIGHT:
        enemy_init(s_lin1, s_col1, ORC, DIR_RIGHT);
        break;
      case INDEX_ORC_LEFT:
        enemy_init(s_lin1, s_col1, ORC, DIR_LEFT);
        break;
      case INDEX_WARG_RIGHT:
        enemy_init(s_lin1, s_col1, WARG, DIR_RIGHT);
        break;
      case INDEX_WARG_LEFT:
        enemy_init(s_lin1, s_col1, WARG, DIR_LEFT);
        break;
      case INDEX_DWARF_RIGHT:
        enemy_init(s_lin1, s_col1, DWARF, DIR_RIGHT);
        break;
      case INDEX_DWARF_LEFT:
        enemy_init(s_lin1, s_col1, DWARF, DIR_LEFT);
        break;
      case INDEX_ELF_RIGHT:
        enemy_init(s_lin1, s_col1, ELF, DIR_RIGHT);
        break;
      case INDEX_ELF_LEFT:
        enemy_init(s_lin1, s_col1, ELF, DIR_LEFT);
        break;
      }

      scr_map[index1] = TILE_EMPTY;
    }

    // NIRVANAP_drawT(TILE_BRICK, s_lin1, s_col1);

    s_col1 = s_col1 + 2;
    ++index1;
  }
  intrinsic_ei();
}

void spr_draw_clear(void) {
  intrinsic_di();
  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  // todo an asm routine to clear the screen fast (nirvana)
  for (s_lin1 = 16; s_lin1 <= 162; s_lin1 += 16) {
    for (s_col1 = 0; s_col1 < 32; s_col1 += 2) {
      NIRVANAP_drawT_raw(TILE_EMPTY, s_lin1, s_col1);
    }
  }
  intrinsic_ei();
}

void spr_brick_anim(unsigned char f_hit) __z88dk_fastcall {
  unsigned char v1;
  unsigned char v0;
  unsigned char f_lin;
  unsigned char f_col;
  /*f_hit should be even*/

  f_col = player_hit_col;
  f_lin = player_hit_lin;

  index1 = spr_calc_index(f_lin, f_col);
  v0 = scr_map[index1 - 16];
  v1 = scr_map[index1];

  if (f_hit) {
    tmp = f_lin - 10;
  } else {
    tmp = f_lin - 8;
  }

  /* Draw Brick */
  intrinsic_di();
  if (f_hit) {
    // NIRVANAP_fillC(PAPER, f_lin, f_col);
    // NIRVANAP_fillC(PAPER, f_lin, f_col + 1);
    NIRVANAP_fillT_raw(PAPER, f_lin, f_col); // TODO BORRA SI HAY BRICK
                                             // ARRIBA!!!
    NIRVANAP_drawT_raw(v1, tmp, f_col);
  } else {
    // NIRVANAP_fillC( PAPER, f_lin-16, f_col  );
    // NIRVANAP_fillC( PAPER, f_lin-16, f_col+1);
    NIRVANAP_drawT_raw(v0, tmp - 16, f_col);
    NIRVANAP_drawT_raw(v1, tmp, f_col);
  }
  intrinsic_ei();
}

void spr_draw_index(unsigned int f_index) {
  /* n % 2^i = n & (2^i - 1) */
  s_col1 = f_index & 31; // OPTIMIZED % 32
  s_lin1 = f_index >> 5; // div 32
  s_lin1 = s_lin1 << 3;  // mod 32
  // NIRVANAP_drawT_raw(spr_idx[scr_map[f_index]], s_lin1, s_col1);
  NIRVANAP_drawT_raw(scr_map[f_index], s_lin1, s_col1);
}

void spr_draw_row(unsigned char f_row) {
  intrinsic_di();
  tmp = 0;
  while (tmp < 32) {
    index1 = (f_row << 5) + tmp;
    NIRVANAP_drawT_raw(game_brick_tile, f_row << 3, tmp);
    tmp = tmp + 2;
  }
  intrinsic_ei();
}

unsigned char spr_calc_hor(unsigned char f_sprite) {
  /*
    if (class[f_sprite] != COIN_2) {
      return col[f_sprite] * 3 + colint[f_sprite];
    } else {
      return col[f_sprite] * 3;
    }
   */
  return col[f_sprite] * sprite_frames[f_sprite];
}

unsigned char spr_collision_check(unsigned char f_sprite1,
                                  unsigned char f_sprite2,
                                  unsigned char f_vert_diff) {
  unsigned char v1;
  unsigned char v2;
  if (class[f_sprite2] == 0 || BIT_CHK(state[f_sprite2], STAT_KILL))
    return 0;
  tmp_ui = abs(lin[f_sprite2] - lin[f_sprite1]);
  if (tmp_ui > f_vert_diff)
    return 0;
  v1 = spr_calc_hor(f_sprite1);
  v2 = spr_calc_hor(f_sprite2);
  tmp_ui = abs(v1 - v2);
  if (tmp_ui > 3) {
    return 0;
  } else {
    return 1;
  }
}

void spr_kill_all(void) {
  for (sprite = 0; sprite < 8; ++sprite) {
    spr_destroy(sprite);
  }
}

void spr_back_repaint(void) {
  unsigned char s_row;

  intrinsic_di();
  sprite_curr_index = spr_calc_index(s_lin0, s_col0);
  if ((s_col0 & 1) == 0) { // Par
    if ((s_lin0 & 15) == 0) {
      // Paint single tile
      spr_tile_paint(scr_map[sprite_curr_index], s_lin0, s_col0);
    } else {
      // Paint up n down tiles
      s_row = s_lin0 >> 4;
      s_row = s_row << 4;
      spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0);
      s_row = s_row + 16;
      sprite_curr_index = sprite_curr_index + 16;
      spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0);
    }
  } else
     { // Impar
      if ((s_lin0 & 15) == 0) {
        // Paint single tile
        intrinsic_ei();
        NIRVANAP_halt();
        intrinsic_di();
        spr_tile_paint(scr_map[sprite_curr_index], s_lin0, s_col0 - 1);
        sprite_curr_index = sprite_curr_index + 1;
        spr_tile_paint(scr_map[sprite_curr_index], s_lin0, s_col0 + 1);
      } else {
        // Paint up n down tiles
        s_row = s_lin0 >> 4;
        s_row = s_row << 4;
        // up
        intrinsic_ei();
        NIRVANAP_halt();
        intrinsic_di();
        spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0 - 1);
        sprite_curr_index = sprite_curr_index + 1;
        spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0 + 1);
        // down
        s_row = s_row + 16;
        sprite_curr_index = sprite_curr_index + 15;
        intrinsic_ei();
        NIRVANAP_halt();
        intrinsic_di();
        // NIRVANAP_halt();
        spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0 - 1);
        sprite_curr_index = sprite_curr_index + 1;
        spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0 + 1);
      }
    }
  intrinsic_ei();
}

void spr_tile_paint(unsigned char f_tile, unsigned char f_lin,
                    unsigned char f_col) {

  switch (f_tile) {
  case TILE_EMPTY:
    // NIRVANAP_fillC(PAPER, f_lin, f_col);
    // NIRVANAP_fillC(PAPER, f_lin + 8, f_col);
    // NIRVANAP_fillC(PAPER, f_lin, f_col + 1);
    // NIRVANAP_fillC(PAPER, f_lin + 8, f_col + 1);
    NIRVANAP_fillT_raw(PAPER, f_lin, f_col);
    break;
  case 0XFF:
    // ANIM PLAYING
    break;
  default:

    NIRVANAP_drawT_raw(f_tile, f_lin, f_col);

    // NIRVANAP_drawT(f_tile, f_lin, f_col);
  }
  /*
  if (f_tile == TILE_EMPTY) { // PERF || f_tile >= TILE_END) {
    NIRVANAP_fillC(PAPER, f_lin, f_col);
    NIRVANAP_fillC(PAPER, f_lin + 8, f_col);
    NIRVANAP_fillC(PAPER, f_lin, f_col + 1);
    NIRVANAP_fillC(PAPER, f_lin + 8, f_col + 1);
  } else {
    NIRVANAP_drawT(f_tile, f_lin, f_col);
  }
  */
}

void spr_init_anim(void) {
  unsigned char f_anim;
  for (f_anim = 0; f_anim < 8; f_anim++) {
    anim_lin[f_anim] = 0XFF;
  }
}

void spr_init_bullet(void) {
  unsigned char f_bullet;
  for (f_bullet = 0; f_bullet < 8; f_bullet++) {
    bullet_col[f_bullet] = 0XFF;
  }
}

void spr_add_anim(unsigned char f_lin, unsigned char f_col,
                  unsigned char f_tile, unsigned char f_end) {
  unsigned char f_anim;
  for (f_anim = 0; f_anim < 8; f_anim++) {
    if (anim_lin[f_anim] == 0XFF) {
      anim_lin[f_anim] = f_lin;
      anim_col[f_anim] = f_col;
      anim_tile[f_anim] = f_tile;
      anim_int[f_anim] = 0;
      anim_end[f_anim] = f_end;
      intrinsic_di();
      NIRVANAP_drawT_raw(anim_tile[f_anim], anim_lin[f_anim], anim_col[f_anim]);
      intrinsic_ei();
      index0 = spr_calc_index(f_lin, f_col);
      if (scr_map[index0] == TILE_EMPTY) scr_map[index0] = 0xFF;
      break;
    }
  }
}

void spr_play_anim(void) {
  unsigned char f_anim;
  unsigned int f_index;

  unsigned char f_clear;
  f_clear = 1;

  for (f_anim = 0; f_anim < 8; f_anim++) {
    if (anim_lin[f_anim] != 0xFF) {

      if (f_clear) {
        NIRVANAP_halt();
        f_clear = 0;
      }

      if (anim_int[f_anim] < anim_end[f_anim]) {
        // intrinsic_di();
        NIRVANAP_drawT(anim_tile[f_anim] + anim_int[f_anim], anim_lin[f_anim],
                       anim_col[f_anim]);
        // intrinsic_ei();
        ++anim_int[f_anim];
      } else {
        s_col0 = anim_col[f_anim];
        s_lin0 = anim_lin[f_anim];
        f_index = spr_calc_index(s_lin0, s_col0);
        if (scr_map[spr_calc_index(s_lin0, s_col0)] == 0xFF) {
          scr_map[spr_calc_index(s_lin0, s_col0)] = TILE_EMPTY;
        }
        spr_back_repaint();
        anim_lin[f_anim] = 0xFF;
        // scr_map[spr_calc_index(s_lin0, s_col0)] = TILE_EMPTY;
      }
    }
  }
}

void spr_play_bullets(void) {
  unsigned char f_bullet;

  for (f_bullet = 0; f_bullet < 8; ++f_bullet) {

    if (bullet_col[f_bullet] != 0xFF) {
      s_lin0 = bullet_lin[f_bullet];
      s_col0 = bullet_col[f_bullet];
      s_col1 = s_col0;

      spr_back_repaint();

      if (bullet_dir[f_bullet] == 0xFF) {
        // Right
        ++bullet_colint[f_bullet];

        if (bullet_colint[f_bullet] >= bullet_frames[f_bullet]) {
          ++bullet_col[f_bullet];
          bullet_colint[f_bullet] = 0;
        }
        if ((bullet_col[f_bullet] & 1) != 0) {
          s_col1 = bullet_col[f_bullet] + 1;
          tmp = bullet_col[f_bullet] - 1;
        }
      } else {
        // Left
        --bullet_colint[f_bullet];

        if (bullet_colint[f_bullet] == 0xFF) {
          --bullet_col[f_bullet];
          bullet_colint[f_bullet] = bullet_frames[f_bullet] - 1;
        }
        if ((bullet_col[f_bullet] & 1) != 0) {
          s_col1 = bullet_col[f_bullet] - 1;
          tmp = bullet_col[f_bullet] + 1;
        }
      }

      if (bullet_col[f_bullet] > 0 && bullet_col[f_bullet] < 30) {

        index0 = spr_calc_index(s_lin0, s_col1);
        for (tmp0 = 0; tmp0 < 7; ++tmp0) {

          if (class[tmp0] != 0) {
            index1 = spr_calc_index(lin[tmp0], col[tmp0]);
            if (index0 == index1) {
              s_lin0 = lin[tmp0];
              s_col0 = col[tmp0];
              spr_destroy(tmp0);
              bullet_col[f_bullet] = 0XFF;
              spr_add_anim(s_lin0, s_col0, TILE_ANIM_FIRE, 3);
              break;
            }
          }
        }
        if (bullet_col[f_bullet] != 0XFF) {
          if ((scr_map[index0] < TILE_CEIL)) {
            // intrinsic_di();
            NIRVANAP_drawT(bullet_tile[f_bullet] + bullet_colint[f_bullet],
                           s_lin0, bullet_col[f_bullet]);
            // intrinsic_ei();
          } else {
            spr_add_anim(s_lin0, tmp, TILE_ANIM_FIRE, 3);
            bullet_col[f_bullet] = 0XFF;
          }
        }
      } else {
        bullet_col[f_bullet] = 0XFF;
      }
    }
  }
}

void spr_turn_horizontal(void) {
  if (BIT_CHK(s_state, STAT_DIRR)) {
    BIT_SET(s_state, STAT_DIRL);
    BIT_CLR(s_state, STAT_DIRR);
  } else {
    BIT_CLR(s_state, STAT_DIRL);
    BIT_SET(s_state, STAT_DIRR);
  }
  state[sprite] = s_state;
  tile[sprite] = spr_tile(sprite);
}
