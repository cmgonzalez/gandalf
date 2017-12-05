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
/*
unsigned char spr_move_up_fast(void) {
  s_lin1 = lin[sprite] - SPRITE_LIN_INC;
  if (s_lin1 > lin_min[sprite]) {
    lin[sprite] = s_lin1;
    return 0;
  } else {
    lin[sprite] = lin_min[sprite];
    return 1;
  }
}
*/

/*
unsigned char spr_move_down_fast(void) {
  s_lin1 = lin[sprite] + SPRITE_LIN_INC;
  if (s_lin1 < lin_max[sprite]) {
    lin[sprite] = s_lin1;
    return 0;
  } else {
    lin[sprite] = lin_max[sprite];
    return 1;
  }
}
*/

unsigned char spr_move_up(void) {
  unsigned char f_check;

  s_lin1 = lin[sprite] - SPRITE_LIN_INC;
  f_check = (s_lin1 >> 4) != (s_lin0 >> 4);

  if (f_check) {
    if (game_check_map(s_lin1, col[sprite])) {
      /*
      if (sprite == SPR_P1) {
        if (!BIT_CHK(state_a[SPR_P1], STAT_HITBRICK)) {
          player_hit_platform();
        }
      }
      */
      return 1;
    }
  }

  if (s_lin1 > GAME_LIN_FLOOR) {
    lin[sprite] = 0;
    // NIRVANAP_fillT(18, s_lin0, s_col0);
    return 1;
  } else {
    lin[sprite] = s_lin1;
    return 0;
  }
}

unsigned char spr_move_down(void) {
  unsigned char f_check;

  s_lin1 = lin[sprite] + SPRITE_LIN_INC + 14;
  f_check = (s_lin1 >> 4) != (s_lin0 >> 4);
  if (f_check) {
    if (game_check_map(s_lin1, col[sprite])) {
      return 1;
    }
  }

  if (s_lin1 > GAME_LIN_FLOOR) {
    lin[sprite] = GAME_LIN_FLOOR;
    return 1;
  } else {
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
  if (colint[sprite] == sprite_frames[class[sprite]]) {
    s_lin1 = lin[sprite];
    if (col[sprite] < 30) {
      s_col1 = col[sprite] + 1;
      if (game_check_map(s_lin1, s_col1) ||
          game_check_map(s_lin1 + 15, s_col1)) {
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
      if (game_check_map(s_lin1, s_col1) ||
          game_check_map(s_lin1 + 15, s_col1)) {
        ++colint[sprite];
        if (!sprite_on_air) {
          ++colint[sprite];
        }
        return 1;
      }
    }
    --col[sprite];
    colint[sprite] = sprite_frames[class[sprite]] - 1;

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

  if (scr_curr < map_width) {
    ++scr_curr;
    spr_page_map();
    game_draw_screen();
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
    game_draw_screen();
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
  unsigned int add_index;
  unsigned int start_index;

  k = 16;
  intrinsic_di();
  // Read Player start screen on world map
  GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
  IO_7FFD = 0x10 + 6;
  v0 = start_scr0; // TODO n LEVELS
  GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
  IO_7FFD = 0x10 + 0;
  game_start_scr = v0;
  // Calculate the current screen start index in the world map
  j = 0;
  start_index = 0;
  add_index = 0;
  if (scr_curr == 255) {
    scr_curr = game_start_scr;
  }
  while (j < scr_curr) {
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
        scr_map[k] = TILE_EMPTY;
      }
      ++k;
    } else {
      vr = v0 - 128; // Repeat counter Should be < 128!!

      for (j = 0; j < vr; j++) {
        if (!game_obj_chk(k)) {
          scr_map[k] = v1;
        } else {
          scr_map[k] = TILE_EMPTY;
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
  spr_init_anim_bullets();
  intrinsic_ei();
  NIRVANAP_start();

  // NIRVANAP_start();
  // Remove all enemies fast
  for (i = 0; i < SPR_P1; ++i) {
    class[i] = 0;
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
  }
}

unsigned char spr_redraw(void) {
  unsigned char f_lin8;

  s_tile1 = tile[sprite] + colint[sprite];
  s_col1 = col[sprite];
  s_lin1 = lin[sprite];

  if ((s_lin1 != s_lin0) || (s_col1 != s_col0)) {
    /* Column Movement */
    if (sprite == SPR_P1) {
      /*Stair Anim*/
      if (player_over_stair && ((lin[sprite] & 3) == 0)) {
        s_tile1 = s_tile1 + 4;
      }
    }
    // Speed UP hack
    if (sprite == SPR_P1) {
      spr_back_repaint();
    } else {
      if ((s_lin1 & 7) == 0) {
        spr_back_repaint();
      } else {
        if (BIT_CHK(s_state, STAT_JUMP)) {
          f_lin8 = s_lin0 + 8;
          NIRVANAP_fillC(map_paper_clr, f_lin8, s_col0);
          NIRVANAP_fillC(map_paper_clr, f_lin8, s_col0 + 1);
        } else {
          NIRVANAP_fillC(map_paper_clr, s_lin0, s_col0);
          NIRVANAP_fillC(map_paper_clr, s_lin0, s_col0 + 1);
        }
      }
    }
    // End hack
    NIRVANAP_spriteT(sprite, s_tile1, s_lin1, s_col1);
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
  case DRAGON:
    return spr_tile_dir(TILE_ENEMY_DRAGON, f_sprite, DIRINC_ENEMY_DRAGON);
    break;
  case BAT:
    return spr_tile_dir(TILE_ENEMY_BAT, f_sprite, DIRINC_ENEMY_BAT);
    break;
  case WYVERN:
    return spr_tile_dir(TILE_ENEMY_WYVERN, f_sprite, DIRINC_ENEMY_WYVERN);
    break;
  case SPIDER:
    return spr_tile_dir(TILE_ENEMY_SPIDER, f_sprite, DIRINC_ENEMY_SPIDER);
    break;
  case PLANT:
    return spr_tile_dir(TILE_ENEMY_PLANT, f_sprite, DIRINC_ENEMY_PLANT);
    break;
  case SNAKE:
    return spr_tile_dir(TILE_ENEMY_SNAKE, f_sprite, DIRINC_ENEMY_SNAKE);
    break;
  case BAT_H:
    return spr_tile_dir(TILE_ENEMY_BAT_H, f_sprite, DIRINC_ENEMY_BAT_H);
    break;
  case MUSHROOM_VITA:
    return spr_tile_dir(TILE_ENEMY_MUSH_VITA, f_sprite,
                        DIRINC_ENEMY_MUSH_EXTRA);
    break;
  case MUSHROOM_MANA:
    return spr_tile_dir(TILE_ENEMY_MUSH_MANA, f_sprite, DIRINC_ENEMY_MUSH_MANA);
    break;
  case MUSHROOM_EXTRA:
    return spr_tile_dir(TILE_ENEMY_MUSH_EXTRA, f_sprite,
                        DIRINC_ENEMY_MUSH_MANA);
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

void spr_draw_clear(void) {
  intrinsic_di();
  zx_paper_fill(INK_BLACK);
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

void spr_draw_index(unsigned int f_index) {
  s_col1 = (f_index & 15) << 1;
  s_lin1 = f_index;
  s_lin1 = (s_lin1 >> 4) << 4;
  intrinsic_di();
  NIRVANAP_drawT_raw(scr_map[f_index], s_lin1, s_col1);
  intrinsic_ei();
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

void spr_back_repaint(void) {
  unsigned char s_row;

  NIRVANAP_halt();

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
        spr_tile_paint(scr_map[sprite_curr_index], s_lin0, s_col0 - 1);
        sprite_curr_index = sprite_curr_index + 1;
        spr_tile_paint(scr_map[sprite_curr_index], s_lin0, s_col0 + 1);
      } else {
        // Paint up n down tiles
        s_row = s_lin0 >> 4;
        s_row = s_row << 4;
        // up
        spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0 - 1);
        sprite_curr_index = sprite_curr_index + 1;
        spr_tile_paint(scr_map[sprite_curr_index], s_row, s_col0 + 1);
        // down
        s_row = s_row + 16;
        sprite_curr_index = sprite_curr_index + 15;

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
    NIRVANAP_fillT_raw(map_paper_clr, f_lin, f_col);
    break;
  case 0XFF:
    // ANIM PLAYING
    break;
  default:
    NIRVANAP_drawT_raw(f_tile, f_lin, f_col);
  }
}

void spr_init_anim_bullets(void) {
  unsigned char f_anim;
  for (f_anim = 0; f_anim < 8; f_anim++) {
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
  if (f_col < 31 && f_lin < SCR_LINS ) {
    for (f_anim = 0; f_anim < 8; f_anim++) {
      if (anim_lin[f_anim] == 0XFF) {
        ++anim_count;
        anim_lin[f_anim] = f_lin;
        anim_col[f_anim] = f_col;
        anim_tile[f_anim] = f_tile;
        anim_loop[f_anim] = f_loops;
        anim_int[f_anim] = 0;
        anim_end[f_anim] = f_end;
        anim_respanwn[f_anim] = f_respawn;
        intrinsic_di();
        NIRVANAP_drawT_raw(anim_tile[f_anim], anim_lin[f_anim],
                           anim_col[f_anim]);
        intrinsic_ei();
        index0 = spr_calc_index(f_lin, f_col);

        if (scr_map[index0] == TILE_EMPTY)
          scr_map[index0] = 0xFF;
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

      if (anim_int[f_anim] < anim_end[f_anim]) {
        intrinsic_di();
        NIRVANAP_drawT_raw(anim_tile[f_anim] + anim_int[f_anim],
                           anim_lin[f_anim], anim_col[f_anim]);
        intrinsic_ei();
        ++anim_int[f_anim];
      } else {
        if (anim_loop[f_anim] == 0) {
          --anim_count;
          s_col0 = anim_col[f_anim];
          s_lin0 = anim_lin[f_anim];
          f_index = spr_calc_index(s_lin0, s_col0);
          if (scr_map[spr_calc_index(s_lin0, s_col0)] == 0xFF) {
            scr_map[spr_calc_index(s_lin0, s_col0)] = TILE_EMPTY;
          }
          spr_back_repaint();

          if (anim_respanwn[f_anim]) {
            // Respawn an enemy after anim...
            enemy_respawn(f_anim);
          }
          anim_lin[f_anim] = 0xFF;

        } else {
          --anim_loop[f_anim];
          anim_int[f_anim] = 0;
        }
      }
    }
  }
}

void spr_play_bullets(void) {

  unsigned char f_bullet;
  unsigned char f_lin0;
  unsigned char f_col0;
  unsigned char f_lin1;
  unsigned char f_col1;
  signed int val_yc;

  for (f_bullet = 0; f_bullet < 8; ++f_bullet) {
    if (bullet_col[f_bullet] == 0xFF) // Not active bullet
      continue;

    s_lin0 = bullet_lin[f_bullet];
    s_col0 = bullet_col[f_bullet];
    f_col0 = s_col0;

    spr_back_repaint(); // restore background
    if (bullet_class[f_bullet] == BULLET_FIREBALL_UP) {
      if (s_lin0 - bullet_lin0[f_bullet] <= 8) {
        bullet_lin[f_bullet] = bullet_lin[f_bullet] + 2;
        s_lin0 = bullet_lin[f_bullet];
      } else {
        bullet_class[f_bullet] = BULLET_FIREBALL_DOWN;
      }
    }
    if (bullet_class[f_bullet] == BULLET_FIREBALL_DOWN) {
      if (bullet_lin0[f_bullet] - s_lin0 <= 8) {
        bullet_lin[f_bullet] = bullet_lin[f_bullet] - 2;
        s_lin0 = bullet_lin[f_bullet];
      } else {
        bullet_class[f_bullet] = BULLET_FIREBALL_UP;
      }
    }
    if (bullet_class[f_bullet] == BULLET_AXE) {
      bullet_vel[f_bullet] = bullet_vel[f_bullet] + game_gravity;

      if (bullet_vel[f_bullet] > 120) {
        bullet_vel[f_bullet] = 120;
      }

      if (bullet_vel[f_bullet] < -120) {
        bullet_vel[f_bullet] = -120;
      }

      val_yc = bullet_vel[f_bullet];

      s_lin1 = (unsigned char)val_yc;
      // Nirvana don't support odds lines
      if ((s_lin1 & 1) != 0) {
        s_lin1--;
      }

      s_lin0 = bullet_lin[f_bullet] + s_lin1;
      if (s_lin0 > GAME_LIN_FLOOR) {
        --bullet_count;
        bullet_col[f_bullet] = 0XFF;
        continue;
      } else {
        bullet_lin[f_bullet] = s_lin0;
      }
    }
    // Move Bullets
    if (bullet_dir[f_bullet] == 0xFF) {
      // Move Right
      ++bullet_colint[f_bullet];
      if (bullet_colint[f_bullet] >= bullet_frames[f_bullet]) {
        f_col0 = bullet_col[f_bullet] + 1;
        bullet_colint[f_bullet] = 0;
        if ((f_col0 & 1) == 1) {
          s_col0 = f_col0 + 1;
        }
      }
    } else {
      // Move Left
      --bullet_colint[f_bullet];
      if (bullet_colint[f_bullet] == 0xFF) {
        f_col0 = bullet_col[f_bullet] - 1;
        bullet_colint[f_bullet] = bullet_frames[f_bullet] - 1;
        if ((f_col0 & 1) == 1) {
          s_col0 = f_col0 - 1;
        }
      }
    }

    tmp1 = 0;
    if (bullet_max[f_bullet] > 0) {
      // Max Fireball
      tmp0 = abs(f_col0 - bullet_col0[f_bullet]);
      if (tmp0 > bullet_max[f_bullet]) {
        tmp1 = 1;
      }
    }
    if (f_col0 >= 30) {
      // Out of Screen
      tmp1 = 1;
    }

    if (tmp1) {
      --bullet_count;
      bullet_col[f_bullet] = 0XFF;
      continue;
    }
    // Colission with map
    if (f_col0 != bullet_col[f_bullet]) {
      // Column Movement

      index0 = spr_calc_index(s_lin0 + 8, s_col0);

      if (scr_map[index0] == TILE_DIRT && f_bullet == SPR_P1) {
        // Destroy Bricks
        scr_map[index0] = TILE_EMPTY;
        game_obj_set(index0);
        bullet_lin[f_bullet] = (index0 >> 4) << 4;
        bullet_col[f_bullet] = (index0 & 15) << 1;
        spr_explode_bullet(f_bullet);
        continue;
      }

      if ((scr_map[index0] >= TILE_FLOOR && scr_map[index0] != 0xFF)) {
        // Explode Bullet
        spr_explode_bullet(f_bullet);
        continue;
      }

      // Now we can move
      bullet_col[f_bullet] = f_col0;

      // Colission with sprites
      if (bullet_class[f_bullet] == BULLET_AXE) {
        f_lin0 = s_lin0 - 16;
        f_lin1 = s_lin0 + 16;
        f_col0 = f_col0 - 1;
        f_col1 = f_col0 + 1;
      } else {
        f_lin0 = s_lin0 - 8;
        f_lin1 = s_lin0 + 8;
        f_col0 = f_col0 + 0;
        f_col1 = f_col0 + 1;
      }

      if (f_bullet == SPR_P1) {
        // PLAYER BULLETS
        for (tmp0 = 0; tmp0 < SPR_P1; ++tmp0) {
          if (class[tmp0] != 0 && lin[tmp0] >= f_lin0 && lin[tmp0] <= f_lin1 &&
              col[tmp0] >= f_col0 && col[tmp0] <= f_col1) {
            s_lin0 = lin[tmp0];
            s_col0 = col[tmp0];
            player_score_add(rand() % 6);
            game_respawn_time[tmp0] = zx_clock();
            spr_destroy(tmp0);
            bullet_col[f_bullet] = s_col0;
            spr_explode_bullet(f_bullet);
            break;
          }
        }
      } else {
        // ENEMY BULLET
        if (lin[SPR_P1] >= f_lin0 && lin[SPR_P1] <= f_lin1 &&
            col[SPR_P1] >= f_col0 && col[SPR_P1] <= f_col1) {
          // s_lin0 = lin[tmp0];
          // s_col0 = col[tmp0];
          // spr_destroy(tmp0);
          zx_border(INK_MAGENTA);
          player_hit(10);

          bullet_col[f_bullet] = s_col0;
          spr_explode_bullet(f_bullet);
          break;
        }
      }
    }
    if (bullet_col[f_bullet] == 0xFF)
      continue;
    // We can draw
    intrinsic_di();
    NIRVANAP_drawT_raw(bullet_tile[f_bullet] + bullet_colint[f_bullet], s_lin0,
                       f_col0);
    intrinsic_ei();
  }
}

void spr_explode_bullet(unsigned char f_bullet) __z88dk_fastcall {
  spr_add_anim(bullet_lin[f_bullet], bullet_col[f_bullet], TILE_ANIM_FIRE, 3, 0,
               0);
  --bullet_count;
  bullet_col[f_bullet] = 0XFF;
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

void spr_btile_paint_back() {

  tmp_ui = 32;

  while (tmp_ui < (32 + (48 * 12 * 18))) {
    tmp0 = 0;
    while (tmp0 < 16) {
      tmp = PEEK(&btiles + tmp_ui + tmp0);

      if ((tmp & 0x38) == map_paper_last) { // 00111000
        tmp = tmp & 0xC7;                   // 11000111
        tmp = tmp | map_paper; // TODO we can hava a map array for ink to
                               // prevent using the same paper n ink
        POKE(&btiles + tmp_ui + tmp0, tmp);
      }

      ++tmp0;
    }
    tmp_ui = tmp_ui + 48;
  }
}
