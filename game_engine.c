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
#include "game_menu.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include "nirvana+.h"
#include <arch/zx.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

void game_loop(void) {
  unsigned int fps;
  ay_fx_play(ay_effect_10);
  sound_coin();
  z80_delay_ms(200);
  ay_reset();

  player_lives = 4;

  player_score = 0;

  /* screen init */
  game_over = 0;
  /* phase init */
  game_phase_init();
  /* game loop start */
  dirs = 0x00;
  game_joystick_set();
  fps = 0;
  while (!game_over) {
    zx_border(INK_BLACK);

    if (game_check_time(anim_time, TIME_ANIM)) {
      anim_time = zx_clock();
      if (anim_count)
        spr_play_anim();
    }

    if (game_check_time(bullet_time, TIME_BULLETS)) {
      bullet_time = zx_clock();
      if (bullet_count)
        spr_play_bullets();
    }

    /*player 1 turn*/
    sprite = SPR_P1;
    player_turn();
    enemy_turn();
    /*each second aprox - update fps/score/phase left/phase advance*/
    if (game_check_time(frame_time, 100)) {
      zx_border(INK_RED);
      zx_print_ink(INK_WHITE);
      zx_print_int(23, 24, fps);
      fps = 0;
      frame_time = zx_clock();
      if (game_respawn_index == 0) {
        tmp = (rand() & 7);
        if (class[tmp] == 0 && game_respawn[tmp] != 0) {
          game_respawn_index = tmp;
          index1 = game_respawn[tmp];
          s_col1 = (index1 & 15) << 1;
          s_lin1 = index1;
          s_lin1 = (s_lin1 >> 4) << 4;
          scr_map[index1] = 0xFF;
          spr_add_anim(s_lin1, s_col1, TILE_ANIM_RESPAWN, 3, 8);

          // game_add_enemy(game_respawn_tile_index[tmp]);
        }
      }
    }
    ++loop_count;
    ++fps;
  }
}

void game_draw_screen(void) {

  NIRVANAP_halt();
  spr_count = 0;
  while (spr_count < SPR_P1) {
    game_respawn[spr_count] = 0;
    game_respawn_tile_index[spr_count] = 0;
    class[spr_count] = 0;
    ++spr_count;
  }
  spr_count = 0;
  index1 = 16;
  s_lin1 = 0;
  s_col1 = 2;
  // intrinsic_ei();
  spr_count = 0;
  intrinsic_di();
  while (index1 < ((GAME_ROWS - 1) * 16)) {

    if ((index1 & 15) == 0) {
      s_lin1 = s_lin1 + 16;
      s_col1 = 0;
    }

    if (scr_map[index1] < TILE_END) {
      // TILES
      NIRVANAP_drawT_raw(scr_map[index1], s_lin1, s_col1);
    } else {
      // ENEMIES
      if (spr_count < 8) {
        game_respawn[spr_count] = index1;
        game_respawn_tile_index[spr_count] = scr_map[index1];
        game_add_enemy(scr_map[index1]);
      }
      scr_map[index1] = TILE_EMPTY;
    }

    // NIRVANAP_drawT(TILE_BRICK, s_lin1, s_col1);

    s_col1 = s_col1 + 2;
    ++index1;
  }
  intrinsic_ei();
}

void game_add_enemy(unsigned char enemy_tile_index) {
  switch (enemy_tile_index) {
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
  case INDEX_DRAGON_RIGHT:
    enemy_init(s_lin1, s_col1, DRAGON, DIR_RIGHT);
    break;
  case INDEX_DRAGON_LEFT:
    enemy_init(s_lin1, s_col1, DRAGON, DIR_LEFT);
    break;
  case INDEX_BAT:
    enemy_init(s_lin1, s_col1, BAT, DIR_RIGHT);
    break;
  case INDEX_WYVERN:
    enemy_init(s_lin1, s_col1, WYVERN, DIR_RIGHT);
    break;
  case INDEX_SPIDER:
    enemy_init(s_lin1, s_col1, SPIDER, DIR_RIGHT);
    break;
  case INDEX_PLANT:
    enemy_init(s_lin1, s_col1, PLANT, DIR_RIGHT);
    break;
  case INDEX_SNAKE:
    enemy_init(s_lin1, s_col1, SNAKE, DIR_RIGHT);
    break;
  case INDEX_BAT_H:
    enemy_init(s_lin1, s_col1, BAT_H, DIR_RIGHT);
    break;
  }
}

void game_print_footer(void) {
  zx_print_ink(INK_CYAN);

  game_fill_row(19, 98);
  zx_print_str(19, 0, "a");
  zx_print_str(19, 31, "c");
  zx_print_str(20, 0, "d");
  zx_print_str(20, 31, "e");
  zx_print_str(21, 0, "d");
  zx_print_str(21, 31, "e");
  zx_print_str(22, 0, "d");
  zx_print_str(22, 31, "e");
  game_fill_row(23, 98);
  zx_print_str(23, 0, "f");
  zx_print_str(23, 31, "g");
  zx_print_ink(INK_RED);
  zx_print_str(20, 1, "<"); // live p1 hut
  zx_print_ink(INK_YELLOW);
  zx_print_str(21, 1, "\\"); // live p1 face
  /* phase osd bottom*/
  zx_print_str(23, 20, "LPS:");
  game_print_lives();
}

void game_phase_print_score_back(void) {
  zx_print_ink(INK_RED);
  zx_print_str(0, 11, "$%|"); // Top
}

void game_print_lives(void) {
  zx_print_ink(INK_WHITE);
  tmp = player_lives - 1;
  if (tmp < 255) {
    zx_print_chr(20, 3, tmp); // LIVE P1
  }
}

void game_tick(void) {
  ++curr_time;
  zx_isr();
}

void game_start_timer(void) {
  NIRVANAP_ISR_HOOK[0] = 205;                                // call
  z80_wpoke(&NIRVANAP_ISR_HOOK[1], (unsigned int)game_tick); // game_tick
}

void game_phase_init(void) {

  /*PHASE INIT*/
  loop_count = 0;
  entry_time = 0;
  zx_set_clock(0);
  frame_time = 0;

  /* Phase Tune */
  ay_reset();
  /* Phase Draw Start */
  spr_draw_clear();
  /*Draw Platforms*/
  // zx_paper_fill(INK_BLACK | PAPER_BLACK);
  game_draw_screen();
  game_print_header();
  game_print_footer();
  /* Player(s) init */
  if (player_lives)
    player_init(SPR_P1, GAME_LIN_FLOOR - 16, 2, TILE_P1_STANR);
}

void game_print_header(void) {
  game_phase_print_score_back();
  zx_print_ink(INK_WHITE);
  /* Print score */
  game_print_score();
}

void game_print_phase() {
  zx_print_str(23, 11, "PHASE");
  zx_print_chr(23, 18, phase_curr + 1);
}

unsigned int game_check_map(unsigned char f_lin, unsigned char f_col) {
  // TODO A SINGLE FUNCTION TO SAVE BYTES
  if ((f_col & 1) == 0) {
    index1 = spr_calc_index(f_lin, f_col);

    return game_check_cell(index1);
  } else {

    g_player_hit_left = 0;
    g_player_hit_right = 0;

    index1 = spr_calc_index(f_lin, f_col - 1);
    if (game_check_cell(index1)) {
      g_player_hit_left = 1;
    }
    if (game_check_cell(index1 + 1)) {
      g_player_hit_right = 1;
    }
    return g_player_hit_left || g_player_hit_right;
  }
}

unsigned char game_check_cell(int f_index) __z88dk_fastcall {
  unsigned char f_tile;
  // OUT OFF SCREEN
  if (f_index > GAME_SCR_MAX_INDEX) {
    return 1;
  }

  f_tile = scr_map[f_index];

  /*
    if (class[sprite] <= SPIDER) {
      if (f_tile <= TILE_ITEM_E) {
        return 0;
      } else {
        return 1;
      }
    }
  */
  // TILE_EMPTY -> TILE_FLOOR
  if (f_tile < TILE_FLOOR) {
    return 0;
  }

  // TILE_FLOOR -> TILE_STAIR_S
  if (f_tile < TILE_STAIR_S) {
    if (BIT_CHK(s_state, STAT_FALL)) {
      return 1;
    } else {
      return 0;
    }
  }

  // TILE_STAIR_S -> TILE_CEIL
  if (f_tile < TILE_CEIL) {
    if (sprite_horizontal_check) {
      return 0;
    } else {
      if (BIT_CHK(s_state, STAT_FALL)) {
        return 1;
      } else {
        return 0;
      }
    }
  }

  if (f_tile < TILE_DOOR_E) { // DOOR EXIT
    if (sprite == SPR_P1) {
      player_open_door(f_index, f_tile);
      return 1;
    }
  }
  // TILE_CEIL -> TILE_END
  if (f_tile < TILE_END) {
    return 1;
  }

  // NOT A TILE f_tile > TILE_END
  return 0;
}

unsigned char
game_enemy_add_get_index(unsigned char f_search) __z88dk_fastcall {
  for (enemies = 0; enemies <= 5; ++enemies) {
    if (class[enemies] == (unsigned char)f_search) {
      return enemies;
    }
  }
  return 255;
}

void game_print_score(void) {
  zx_print_ink(INK_WHITE);
  zx_print_paper(PAPER_BLACK);
  // zx_print_int(0, 3, player_score);
  zx_print_int(0, 14, game_score_top); // SCORE TOP
}

void game_paint_attrib(unsigned char e_r1) __z88dk_fastcall {
  for (tmp0 = e_r1; tmp0 <= 19; ++tmp0) {
    game_paint_attrib_lin(1, 31, (tmp0 << 3) + 8);
  }
}

void game_paint_attrib_lin(unsigned char f_start, unsigned char f_end,
                           unsigned char f_lin) {
  for (tmp_uc = f_start; tmp_uc < f_end; ++tmp_uc) {
    NIRVANAP_paintC(attrib, f_lin, tmp_uc);
  }
}

void game_paint_attrib_lin_h(unsigned char f_start, unsigned char f_end,
                             unsigned char f_lin) {
  for (tmp_uc = f_start; tmp_uc < f_end; ++tmp_uc) {
    NIRVANAP_paintC(attrib_hl, f_lin, tmp_uc);
  }
}

void game_joystick_change(void) {
  ++player_joy;
  if (player_joy == 7)
    player_joy = 0; /* Rotate Joystick*/
}

void game_joystick_set_menu(void) {
  /* Default Values for menu */
  joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
}

void game_joystick_set(void) { joyfunc1 = control_method[player_joy]; }

void game_end(void) { spr_draw_clear(); }

void game_rotate_attrib(void) {
  // OUT OF MEMORY
  // UP
  // tmp_uc = attrib_hl[0];
  // attrib_hl[0] = attrib_hl[1];
  // attrib_hl[1] = attrib_hl[2];
  // attrib_hl[2] = attrib_hl[3];
  // attrib_hl[3] = tmp_uc;
  // DOWN
  tmp_uc = attrib_hl[3];
  attrib_hl[3] = attrib_hl[2];
  attrib_hl[2] = attrib_hl[1];
  attrib_hl[1] = attrib_hl[0];
  attrib_hl[0] = tmp_uc;
}

void game_colour_message(unsigned char f_row, unsigned char f_col,
                         unsigned char f_col2, unsigned int f_microsecs) {
  tmp = 1;
  frame_time = zx_clock();
  entry_time = zx_clock();
  while (tmp && !game_check_time(entry_time, f_microsecs)) {
    if (game_check_time(frame_time, 5)) {
      // ROTATE ATTRIB ARRAY
      frame_time = zx_clock();
      game_paint_attrib_lin_h(f_col, f_col2, (f_row << 3) + 8);
      game_rotate_attrib();
    }
    while ((joyfunc1)(&k1) != 0)
      tmp = 0;
  };
}

unsigned char game_check_time(unsigned int start, unsigned int lapse) {
  if (zx_clock() - start > lapse) {
    return 1;
  } else {
    return 0;
  }
}
