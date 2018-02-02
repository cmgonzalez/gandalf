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

unsigned int spr_calc_index(unsigned char f_lin, unsigned char f_col) {
  return (f_col >> 1) + ((f_lin >> 4) << 4);
}

unsigned char spr_chktime(unsigned char *sprite) __z88dk_fastcall {
  // if (FULL_SPEED) return 1;
  if (sprite_speed_alt[*sprite] == 0) {
    tmp = sprite_speed[class[*sprite]];
  } else {
    tmp = sprite_speed_alt[*sprite];
  }
  if (game_check_time(&last_time[*sprite], tmp)) {
    last_time[*sprite] = zx_clock();
    return 1;
  }
  return 0;
}

void spr_set_up(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_JUMP);
  BIT_CLR(*f_state, STAT_FALL);
}
void spr_set_down(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_FALL);
  BIT_CLR(*f_state, STAT_JUMP);
}

void spr_set_left(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_DIRL);
  BIT_CLR(*f_state, STAT_DIRR);
}
void spr_set_right(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_DIRR);
  BIT_CLR(*f_state, STAT_DIRL);
}

unsigned char spr_move_up(void) {
  unsigned char f_check;

  if (lin[sprite] <= 16) {
    if (sprite == SPR_P1) {
      if (spr_page_up()) {
        return 0;
      }
    }
    return 1;

  } else {
    s_lin1 = lin[sprite] - SPRITE_LIN_INC;
    f_check = (s_lin1 >> 4) != (s_lin0 >> 4);

    if (f_check && game_check_map(s_lin1, col[sprite])) {
      return 1;
    }

    lin[sprite] = s_lin1;
    return 0;
  }
}

unsigned char spr_move_down(void) {
  unsigned char f_check;

  if (lin[sprite] >= GAME_LIN_FLOOR) {

    if (sprite == SPR_P1) {

      if (spr_page_down()) {
        return 1;
      } else {
        lin[sprite] = GAME_LIN_FLOOR;
        return 1;
      }
    } else {
      lin[sprite] = GAME_LIN_FLOOR;
      return 1;
    }

  } else {
    s_lin1 = lin[sprite] + SPRITE_LIN_INC + 14;
    f_check = (s_lin1 >> 4) != (s_lin0 >> 4);

    if (f_check) {
      if (game_check_map(s_lin1, col[sprite])) {
        return 1;
      }
    }

    lin[sprite] = s_lin1 - 14;
    return 0;
  }
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
  if (colint[sprite] == sprite_frames[s_class]) {
    s_lin1 = lin[sprite];
    if (col[sprite] < 30) {
      s_col1 = col[sprite] + 1;
      sprite_horizontal_check = 1;
      tmp0 = game_check_map(s_lin1, s_col1);
      if (!tmp0) {
        tmp0 = game_check_map(s_lin1 + 15, s_col1);
      }
      sprite_horizontal_check = 0;

      if (tmp0) {
        --colint[sprite];
        if (!sprite_on_air) {
          --colint[sprite];
        }
        return 1;
      }
    }
    ++col[sprite];
    colint[sprite] = 0;
    if (col[sprite] > SCR_COLS_M) {
      if (sprite == SPR_P1) {
        if (spr_page_right()) {
          col[sprite] = 0;
          player_lin_scr = lin[SPR_P1];
          player_col_scr = 0;
        } else {
          col[sprite] = SCR_COLS_M;
        }
      } else {
        col[sprite] = SCR_COLS_M;
        // spr_destroy(sprite);
      }
      return 1;
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
      sprite_horizontal_check = 1;
      tmp0 = game_check_map(s_lin1, s_col1);
      if (!tmp0) {
        tmp0 = game_check_map(s_lin1 + 15, s_col1);
      }
      sprite_horizontal_check = 0;

      if (tmp0) {
        ++colint[sprite];
        if (!sprite_on_air) {
          ++colint[sprite];
        }
        return 1;
      }
    }
    --col[sprite];
    colint[sprite] = sprite_frames[s_class] - 1;

    if (col[sprite] == 255) {
      if (sprite == SPR_P1) {
        if (spr_page_left()) {
          col[sprite] = SCR_COLS_M;
          player_lin_scr = lin[SPR_P1];
          player_col_scr = SCR_COLS_M;
        } else {
          col[sprite] = 0;
        }
      } else {
        col[sprite] = 0;
        // spr_destroy(sprite);
      }
      return 1;
    }
  }
  return 0;
}

unsigned char spr_page_right() {
  tmp1 = 1 + (scr_curr % map_width);
  if (tmp1 < map_width && (!game_boss || game_inmune)) {
    ++scr_curr;
    spr_page_map();
    game_draw_screen();
    return 1;
  }
  return 0;
}

unsigned char spr_page_left() {
  tmp1 = scr_curr % map_width;
  if (tmp1 > 0 && (!game_boss || game_inmune)) {
    --scr_curr;
    spr_page_map();
    game_draw_screen();
    return 1;
  }
  return 0;
}

unsigned char spr_page_down() {
  tmp1 = 1 + (scr_curr / map_width);
  if (tmp1 < map_heigth && (!game_boss || game_inmune)) {
    scr_curr = scr_curr + map_width;
    spr_page_map();
    game_draw_screen();
    player_lin_scr = 8;
    lin[sprite] = 8;
    player_col_scr = col[SPR_P1];
    return 1;
  }
  return 0;
}

unsigned char spr_page_up() {
  tmp1 = scr_curr / map_width;
  if (tmp1 > 0 && (!game_boss || game_inmune)) {
    scr_curr = scr_curr - map_width;
    spr_page_map();
    game_draw_screen();
    player_lin_scr = GAME_LIN_FLOOR - 4;
    lin[sprite] = player_lin_scr;
    player_lin_scr = player_lin_scr;
    player_col_scr = col[SPR_P1];
    return 1;
  }
  return 0;
}

void spr_clear_scr() {
  unsigned char i;
  unsigned char j;
  for (i = 0; i < 8; ++i) {
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
  }
  NIRVANAP_halt();

  intrinsic_di();
  for (i = 0; i < 8; ++i) {

    for (j = 1; j < 10; ++j) {
      NIRVANAP_fillT_raw(INK_BLACK || PAPER_BLACK, j << 4, i << 1);
    }

    for (j = 1; j < 10; ++j) {
      NIRVANAP_fillT_raw(INK_BLACK || PAPER_BLACK, j << 4, 30 - (i << 1));
    }

    intrinsic_ei();
    NIRVANAP_halt();
    intrinsic_di();
  }
  intrinsic_ei();
}
void spr_page_map(void) {

  unsigned char v0;
  unsigned char v1;
  unsigned char vr;
  unsigned char i;
  unsigned char j;
  unsigned char k;
  unsigned int add_index;
  unsigned int start_index;
  unsigned char l_world;
  unsigned char l_world_w;
  unsigned char l_world_h;
  unsigned char l_paper;
  unsigned char l_scr;
  unsigned char l_scr_map;

  l_world = game_world;
  l_scr = scr_curr;
  l_scr_map = (l_world << 4) + l_scr;

  k = 16;

  spr_clear_scr();
  intrinsic_di();
  // Read Player start screen on world map
  GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
  IO_7FFD = 0x10 + 6;

  if (l_scr == 255) {
    l_scr = start_scr0[l_world];
    l_scr_map = (l_world << 4) + l_scr;
  }
  l_paper = paper0[l_scr_map];
  l_world_w = world0_w[l_world];
  l_world_h = world0_h[l_world];

  GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
  IO_7FFD = 0x10 + 0;

  // Calculate the current screen start index in the world map
  j = 0;
  start_index = 0;
  add_index = 0;

  while (j < l_scr_map) {
    GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
    IO_7FFD = 0x10 + 6;
    add_index = lenght0[j]; // TODO n LEVELS
    GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
    IO_7FFD = 0x10 + 0;
    start_index = start_index + add_index;
    ++j;
  }
  intrinsic_ei();

  intrinsic_di();
  for (i = 0; i < GAME_SCR_MAX_INDEX; ++i) {

    // Page in BANK 06 - Note that global variables are in page 0
    GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
    IO_7FFD = 0x10 + 6;
    v0 = world0[start_index + i];     // TODO n LEVELS
    v1 = world0[start_index + i + 1]; // TODO n LEVELS
    // Page in BANK 00
    GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
    IO_7FFD = 0x10 + 0;

    // REMEMBER OBJECT PICK
    if (v0 < 128) {
      if (!game_obj_chk(k)) {
        scr_map[k] = v0;
      } else {
        scr_map[k + 1] = TILE_EMPTY;
        scr_map[k] = game_match_back(k); // TILE_EMPTY;
      }
      ++k;
    } else {
      vr = v0 - 128; // Repeat counter Should be < 128!!

      for (j = 0; j < vr; j++) {
        if (!game_obj_chk(k)) {
          scr_map[k] = v1;
        } else {
          scr_map[k + 1] = TILE_EMPTY;
          scr_map[k] = game_match_back(k); // TILE_EMPTY;
        }

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

  scr_curr = l_scr;
  map_width = l_world_w;
  map_heigth = l_world_h;
  map_paper = l_paper;

  spr_init_effects();
  if (map_paper_last != map_paper) {
    spr_btile_paint_back();
  }


  intrinsic_ei();
  NIRVANAP_start();
  zx_print_ink(INK_YELLOW);
  if (game_debug) {
    zx_print_str(23, 12, "SCR:");
    zx_print_chr(23, 12 + 4, scr_curr);
  }
}

unsigned char spr_redraw_player(void) {

  s_col1 = col[SPR_P1];
  s_lin1 = lin[SPR_P1];

  if ((s_lin1 != s_lin0) || (s_col1 != s_col0)) {
    /* Column or lin Movement */
    // Speed UP hack

    player_anim_tile();
    spr_back_repaint();

    NIRVANAP_spriteT(sprite, s_tile1, s_lin1, s_col1);
    return 1;
  } else {
    s_tile0 = *SPRITEVAL(sprite);
    s_tile1 = tile[SPR_P1] + colint[SPR_P1];
    if (s_tile1 != s_tile0) {
      /* Internal Movement, no clean needed */
      NIRVANAP_spriteT(sprite, s_tile1, s_lin1, s_col1);
      return 0;
    }
  }
  return 0;
}

unsigned char spr_redraw(void) {

  s_col1 = col[sprite];
  s_lin1 = lin[sprite];

  if ((s_lin1 != s_lin0) || (s_col1 != s_col0)) {
    /* Column or lin Movement */
    // Speed UP hack
    s_tile1 = tile[sprite] + colint[sprite];
    if ((s_lin1 & 7) == 0) {
      spr_back_repaint();
    } else {
      NIRVANAP_fillT(map_paper_clr, s_lin0, s_col0);
    }
    NIRVANAP_spriteT(sprite, s_tile1, s_lin1, s_col1);
    return 1;
  } else {
    s_tile0 = *SPRITEVAL(sprite);
    s_tile1 = tile[sprite] + colint[sprite];
    if (s_tile1 != s_tile0) {
      /* Internal Movement, no clean needed */
      NIRVANAP_spriteT(sprite, s_tile1, s_lin1, s_col1);
      return 0;
    }
  }
  return 0;
}

void spr_destroy(unsigned char f_sprite) __z88dk_fastcall {
  spr_count--;
  s_lin0 = lin[f_sprite];
  s_col0 = col[f_sprite];
  index0 = spr_calc_index(s_lin0, s_col0);
  s_tile0 = game_match_back(index0);
  NIRVANAP_spriteT(f_sprite, s_tile0, 0, 0);
  spr_back_repaint();

  tile[f_sprite] = s_tile0;
  col[f_sprite] = 0;
  lin[f_sprite] = 0;
  class[f_sprite] = 0;
  state[f_sprite] = 0;
  state_a[f_sprite] = 0;
}

unsigned char spr_tile(unsigned char *f_sprite) __z88dk_fastcall {

  tmp0 = 0;
  while (tmp0 < GAME_TOTAL_CLASSES) {
    tmp1 = tmp0 * 3;
    if (spr_map_tile[tmp1] == class[*f_sprite]) {
      return spr_tile_dir(&spr_map_tile[tmp1 + 1], f_sprite,
                          &spr_map_tile[tmp1 + 2]);
    }
    ++tmp0;
  }
  return 0;
}

unsigned char spr_tile_dir(unsigned char *f_tile, unsigned char *f_sprite,
                           unsigned char *f_inc) {

  if (BIT_CHK(state[*f_sprite], STAT_DIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(state[*f_sprite], STAT_DIRL)) {
    return *f_tile + *f_inc;
  }
  if (BIT_CHK(state_a[*f_sprite], STAT_LDIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(state_a[*f_sprite], STAT_LDIRL)) {
    return *f_tile + *f_inc;
  }
  return *f_tile;
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
    // NIRVANAP_fillC(map_paper_clr, f_lin, f_col);
    // NIRVANAP_fillC(map_paper_clr, f_lin, f_col + 1);
    NIRVANAP_fillT_raw(map_paper_clr, f_lin, f_col); // TODO BORRA SI HAY BRICK
                                                     // ARRIBA!!!
    NIRVANAP_drawT_raw(v1, tmp, f_col);
  } else {
    // NIRVANAP_fillC( map_paper_clr, f_lin-16, f_col  );
    // NIRVANAP_fillC( map_paper_clr, f_lin-16, f_col+1);
    NIRVANAP_drawT_raw(v0, tmp - 16, f_col);
    NIRVANAP_drawT_raw(v1, tmp, f_col);
  }
  intrinsic_ei();
}

void spr_draw_index(unsigned int *f_index) __z88dk_fastcall {
  s_col1 = (*f_index & 15) << 1;
  s_lin1 = *f_index;
  s_lin1 = (s_lin1 >> 4) << 4;
  intrinsic_di();
  NIRVANAP_drawT_raw(scr_map[*f_index], s_lin1, s_col1);
  intrinsic_ei();
}

void spr_back_repaint(void) {
  // unsigned char s_row;
  // TODO VSYNC - BY DISABLING NIRVANAP_halt, YOU CAN GAIN PERFORMANCE AND
  // FLICKERING ALSO, WHAT'S THE MAGIC FORMULA?
  if (!spr_hack) {
    NIRVANAP_halt();
    intrinsic_di();
  }
  sprite_curr_index = spr_calc_index(s_lin0, s_col0);
  if ((s_col0 & 1) == 0) { // Par
    stp_tile = scr_map[sprite_curr_index];
    stp_col = s_col0;

    if ((s_lin0 & 15) == 0) {
      // Paint single tile
      stp_row = s_lin0;
      spr_tile_paint();
    } else {
      // Paint up n down tiles
      stp_row = (s_lin0 >> 4) << 4;
      spr_tile_paint();
      stp_row = stp_row + 16;
      stp_tile = scr_map[sprite_curr_index + 16];
      spr_tile_paint();
    }
  } else
     { // Impar
      stp_tile = scr_map[sprite_curr_index];
      stp_col = s_col0 - 1;
      if ((s_lin0 & 15) == 0) {
        // Paint single tile
        stp_row = s_lin0;
        spr_tile_paint();
        ++sprite_curr_index;
        stp_tile = scr_map[sprite_curr_index];
        stp_col = s_col0 + 1;
        spr_tile_paint();

      } else {
        // Paint up n down tiles
        stp_row = (s_lin0 >> 4) << 4;
        spr_tile_paint();
        stp_col = s_col0 + 1;
        ++sprite_curr_index;
        stp_tile = scr_map[sprite_curr_index];
        spr_tile_paint();

        stp_col = s_col0 - 1;
        stp_row = stp_row + 16;
        sprite_curr_index = sprite_curr_index + 15;
        stp_tile = scr_map[sprite_curr_index];
        spr_tile_paint();
        ++sprite_curr_index;
        stp_tile = scr_map[sprite_curr_index];
        stp_col = s_col0 + 1;
        spr_tile_paint();
      }
    }
  if (!spr_hack) {
    intrinsic_ei();
  }
}

void spr_tile_paint(void) {
  unsigned char i;

  if (stp_tile == TILE_EMPTY) {
    NIRVANAP_fillT_raw(map_paper_clr, stp_row, stp_col);
  } else {
    if (stp_tile < 0x80) {
      // Normal Tile
      if (stp_tile != TILE_STOPPER) {
        NIRVANAP_drawT_raw(stp_tile, stp_row, stp_col);
      } else {
        stp_tile = game_match_back(sprite_curr_index);
        NIRVANAP_drawT_raw(stp_tile, stp_row, stp_col);
      }
    } else {
      // Animation
      i = stp_tile - 0x80;

      if (anim_end[i] > anim_int[i]) {
        NIRVANAP_drawT_raw(anim_tile[i] + anim_int[i], stp_row, stp_col);
      } else {
        NIRVANAP_fillT_raw(map_paper_clr, stp_row, stp_col);
      }
    }
  }
}

void spr_init_effects(void) {
  unsigned char f_anim;
  for (f_anim = 0; f_anim <= SPR_P1; f_anim++) {
    anim_lin[f_anim] = 0xFF;
    bullet_col[f_anim] = 0xFF;
  }
  bullet_count = 0;
  anim_count = 0;
}

void spr_add_anim(unsigned char f_lin, unsigned char f_col,
                  unsigned char f_tile, unsigned char f_end,
                  unsigned char f_loops, unsigned char f_respawn) {
  unsigned char f_anim;

  if (f_col < 31 && f_lin < SCR_LINS) {
    for (f_anim = 0; f_anim < 8; f_anim++) {
      if (anim_lin[f_anim] == 0xFF) {
        ++anim_count;
        anim_lin[f_anim] = f_lin;
        anim_col[f_anim] = f_col;
        anim_tile[f_anim] = f_tile;
        anim_loop[f_anim] = f_loops;
        anim_int[f_anim] = 0xFF;
        anim_end[f_anim] = f_end;
        anim_respanwn[f_anim] = f_respawn;
        intrinsic_di();
        NIRVANAP_drawT_raw(anim_tile[f_anim], anim_lin[f_anim],
                           anim_col[f_anim]);
        intrinsic_ei();
        index0 = spr_calc_index(f_lin, f_col);

        if (scr_map[index0] == TILE_EMPTY)
          scr_map[index0] = 0x80 + f_anim; // 0xFF;
        break;
      }
    }
  }
}

void spr_play_anim(void) {
  unsigned char f_anim;
  unsigned int f_index;

  for (f_anim = 0; f_anim < 8; f_anim++) {
    if (anim_lin[f_anim] != 0xFF) {

      // z80_delay_ms(1); // TODO I don't get that...
      ++anim_int[f_anim];
      if (anim_int[f_anim] < anim_end[f_anim]) {
        //NIRVANAP_halt();
        intrinsic_di();
        NIRVANAP_drawT_raw(anim_tile[f_anim] + anim_int[f_anim],
                           anim_lin[f_anim], anim_col[f_anim]);
        intrinsic_ei();

      } else {
        if (anim_loop[f_anim] == 0) {
          --anim_count;
          s_col0 = anim_col[f_anim];
          s_lin0 = anim_lin[f_anim];
          f_index = spr_calc_index(s_lin0, s_col0);
          if (scr_map[spr_calc_index(s_lin0, s_col0)] >= 0x80) {
            index0 = spr_calc_index(s_lin0, s_col0);
            scr_map[index0] = game_match_back(index0);
          }

          spr_back_repaint();

          if (anim_respanwn[f_anim]) {
            // Respawn an enemy after an anim...
            enemy_respawn(f_anim);
          }
          // End Animation
          anim_lin[f_anim] = 0xFF;

        } else {
          // loops animation
          --anim_loop[f_anim];
          anim_int[f_anim] = 0xFF;
        }
      }
    }
  }
}

void spr_bullet_fireball(signed char f_inc, unsigned char f_tile) {
  if (abs(s_lin0 - bullet_lin0[bullet]) <= 8) {
    s_lin0 = s_lin0 + f_inc;
    bullet_lin[bullet] = s_lin0;
  } else {
    s_lin0 = s_lin0 - f_inc;
    bullet_lin[bullet] = s_lin0;
    bullet_class[bullet] = f_tile;
  }
}

void spr_bullet_axe() {
  // TODO OPTIMIZE - SLOW DOWNS GAME
  signed int val_yc;
  s_lin0 = bullet_lin[bullet];
  s_col0 = bullet_col[bullet] - 1;
  // if (s_col0 < 32) {
  // spr_back_repaint(); // TODO OPTIMIZE REPAINT
  //};

  bullet_vel[bullet] = bullet_vel[bullet] + game_gravity;

  if (bullet_vel[bullet] > 120) {
    bullet_vel[bullet] = 120;
  }

  if (bullet_vel[bullet] < -120) {
    bullet_vel[bullet] = -120;
  }

  val_yc = bullet_vel[bullet];

  s_lin1 = (unsigned char)val_yc;
  // Nirvana don't support odds lines
  if ((s_lin1 & 1) != 0) {
    s_lin1--;
  }

  s_lin0 = bullet_lin[bullet] + s_lin1;
  if (s_lin0 > GAME_LIN_FLOOR) {
    --bullet_count;
    bullet_col[bullet] = 0XFF;
  } else {
    bullet_lin[bullet] = s_lin0;
  }
}

unsigned char spr_colision_b(unsigned char f_sprite, unsigned char f_bullet) {
  // Colission Sprite -> Bullet
  if (bullet_class[f_bullet] == BULLET_AXE) {
    if (abs(lin[f_sprite] - bullet_lin[f_bullet]) < 16) {
      if (abs(col[f_sprite] - bullet_col[f_bullet]) < 2) {
        return 1;
      }
    }
  } else { // ARROWS N FIREBALLS

    if (abs(lin[f_sprite] - bullet_lin[f_bullet]) <= 8) {
      if (abs(col[f_sprite] - bullet_col[f_bullet]) < 2) {
        return 1;
      }
    }
  }

  return 0;
}

unsigned char spr_colision_boss(unsigned char f_lin, unsigned char f_col) {
  // Colission Boss -> Bullet or sprite

  tmp0 = abs((f_col + 1) - (boss_col + 2));
  if (tmp0 < 2) {
    tmp1 = abs((f_lin + 8) - (boss_lin + 16));
    if (tmp1 < 16) {
      return 1;
    }
  }
  return 0;
}

void spr_bullets_play(void) {

  unsigned char f_col0;

  for (bullet = 0; bullet < 8; ++bullet) {
    if (bullet_col[bullet] == 0xFF) {
      continue;
    }

    s_lin0 = bullet_lin[bullet];
    s_col0 = bullet_col[bullet];
    f_col0 = s_col0;
    spr_back_repaint(); // restore background
    switch (bullet_class[bullet]) {
    case BULLET_FIREBALL_UP:
      spr_bullet_fireball(-2, BULLET_FIREBALL_DOWN);
      break;
    case BULLET_FIREBALL_DOWN:
      spr_bullet_fireball(2, BULLET_FIREBALL_UP);
      break;
    case BULLET_AXE:
      spr_bullet_axe();
      if (bullet_col[bullet] == 0xFF) {
        continue;
      }
      break;
    }

    // Move Horizontal Bullets
    if (bullet_dir[bullet] == 0xFF) {
      // Move Right
      ++bullet_colint[bullet];
      if (bullet_colint[bullet] >= bullet_frames[bullet]) {
        f_col0 = bullet_col[bullet] + 1;
        bullet_colint[bullet] = 0;
        if ((f_col0 & 1) == 1) {
          s_col0 = f_col0 + 1;
        }
      }
    } else {
      // Move Left
      --bullet_colint[bullet];
      if (bullet_colint[bullet] == 0xFF) {
        f_col0 = bullet_col[bullet] - 1;
        bullet_colint[bullet] = bullet_frames[bullet] - 1;
        if ((f_col0 & 1) == 1) {
          s_col0 = f_col0 - 1;
        }
      }
    }

    if (bullet_max[bullet] > 0) {
      // Max Fireball
      tmp0 = abs(f_col0 - bullet_col0[bullet]);
      if (tmp0 > bullet_max[bullet]) {
        --bullet_count;
        bullet_col[bullet] = 0xFF;
        continue;
      }
    }
    if (f_col0 > 30) {
      // Out of Screen
      --bullet_count;
      bullet_col[bullet] = 0xFF;
      continue;
    }

    // Colission with map
    if (f_col0 != bullet_col[bullet]) {
      // Column Movement
      index0 = spr_calc_index(s_lin0 + 8, s_col0);
      if (scr_map[index0] == TILE_DIRT && bullet == SPR_P1) {
        // Destroy Bricks
        scr_map[index0] = game_match_back(index0); // TILE_EMPTY;
        game_obj_set(index0);
        bullet_lin[bullet] = (index0 >> 4) << 4;
        bullet_col[bullet] = (index0 & 15) << 1;
        spr_bullet_explode();
        continue;
      }

      if ((scr_map[index0] >= TILE_CEIL && scr_map[index0] < 0x80)) {
        // Explode Bullet
        spr_bullet_explode();
        continue;
      }

      // Now we can move
      bullet_col[bullet] = f_col0;
      // Colission with sprites
      if (bullet == SPR_P1 && bullet_col[SPR_P1] != 0xFF) {
        // PLAYER BULLETS
        spr_bullet_player_colision();
      } else {
        // Enemy Bullet - TODO REVIEW AXES
        spr_bullet_enemy_colision();
      }
    }

    if (bullet_col[bullet] != 0xFF && f_col0 < 32) {
      // Draw Bullet
      intrinsic_di();
      NIRVANAP_drawT_raw(bullet_tile[bullet] + bullet_colint[bullet], s_lin0,
                         f_col0);
      intrinsic_ei();
    }
  }
}
void spr_bullet_enemy_colision() {
  if (spr_colision_b(SPR_P1, bullet)) {
    zx_border(INK_MAGENTA);
    bullet_col[bullet] = s_col0;
    spr_bullet_explode();
    if (game_boss) {
      player_damage(20);
    } else {
      player_damage(10);
    }
  }
}

void spr_bullet_player_colision() {
  unsigned char f_sprite;
  // TODO BUGGY Respawn!!!!! FIX ME
  f_sprite = 0;
  // for (f_sprite = 0; f_sprite < SPR_P1; ++f_sprite) {
  while (f_sprite < SPR_P1) {
    if (class[f_sprite] != 0 && spr_colision_b(f_sprite, bullet)) {
      // Player Bullet hit an enemy
      player_score_add(rand() % 6);
      enemy_kill(f_sprite);
      break;
    }
    ++f_sprite;
  }
  if (game_boss) {
    // Player Bullet hit the boss
    if (spr_colision_boss(bullet_lin[SPR_P1], bullet_col[SPR_P1])) {
      spr_bullet_explode();
      --game_boss_hit;
      game_update_stats();
      if (game_boss_hit == 0) {
        game_boss_kill();
      }
    }
  }
}
void spr_bullet_explode() {

  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_explosion);
  spr_add_anim(bullet_lin[bullet], bullet_col[bullet], TILE_ANIM_FIRE, 3, 0, 0);
  --bullet_count;
  bullet_col[bullet] = 0xFF;
  // ay_fx_play(ay_effect_11);
}

void spr_turn_horizontal(void) {
  if (BIT_CHK(s_state, STAT_DIRR)) {
    spr_set_left(&s_state);
  } else {
    spr_set_right(&s_state);
  }
  state[sprite] = s_state;
  tile[sprite] = spr_tile(&sprite);
}

void spr_btile_paint_back() {
  unsigned char f_tile;
  unsigned char f_paper_c;

  f_tile = 0;
  tmp_ui = 32;
  map_paper_clr = map_paper | (map_paper >> 3) | BRIGHT;
  while (tmp_ui < (32 + (48 * 12 * 20))) { // 12*20 btiles
    if (
        (f_tile < 73 && f_tile != 13 && f_tile != 14) ||
        ( f_tile > 90 )
       ) { // TODO AN ARRAY WILL BE A MORE ELEGANT SOLUTION

      // f_half = 0;
      tmp0 = 0;
      f_paper_c = map_paper_last;

      if ((f_tile > 56 && f_tile < 65) || (f_tile > 16 && f_tile < 20)) {
        if (map_paper == PAPER_RED) {
          tmp0 = 16;
          map_paper_last_a = map_paper_last;
        }
        if (map_paper_last == PAPER_RED) {
          f_paper_c = map_paper_last_a;
        }
      }

      while (tmp0 < 16) {
        tmp = PEEK(&btiles + tmp_ui + tmp0);
        if ((tmp & 0x38) == f_paper_c) { // 00111000
          tmp = tmp & 0xC7;              // 11000111
          tmp = tmp | map_paper; // TODO we can hava a map array for ink to
                                 // prevent using the same paper n ink
          POKE(&btiles + tmp_ui + tmp0, tmp);
        }
        ++tmp0;
        // if ( f_half & tmp0 == 8 ) tmp0 = 12;
      }
    }
    tmp_ui = tmp_ui + 48;
    ++f_tile;
  }
  map_paper_last = map_paper;
  game_attribs();
}

void spr_flatten(void) {
  unsigned char i;
  for (i = 0; i <= SPR_P1; ++i) {
    s_lin1 = *SPRITELIN(i);
    s_col1 = *SPRITECOL(i);
    s_tile1 = *SPRITEVAL(i);
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
    NIRVANAP_drawT(s_tile1, s_lin1, s_col1);
  }
}

void spr_unflatten(void) {
  // Only for SPR_P1
  player_anim_tile(); // Returns to s_tile1
  NIRVANAP_drawT(s_tile1, lin[SPR_P1], col[SPR_P1]);
}
