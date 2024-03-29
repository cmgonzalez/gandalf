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
#include "game_audio.h"
#include "game_ay.h"
#include "game_enemies.h"
#include "game_engine.h"
#include "game_player.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include <arch/zx/nirvana+.h>
#include <arch/zx.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

void menu_main() {
  unsigned char f_input;
  unsigned char s_col;
  unsigned char s_col_e;
  unsigned char s_row;
  unsigned char c;

  f_input = 1;
  s_col = 10;
  s_col_e = 10 + 12;
  s_row = 7;
  c = 0;

  map_paper = PAPER_BLACK;

  audio_menu();
  menu_main_print();

  while (f_input) {
    z80_delay_ms(40);
    // in_wait_key();
    c = in_inkey();
    // in_wait_nokey();

    game_rotate_attrib();
    s_row = ((6 + menu_curr_sel) << 3) + 8;
    game_paint_attrib(&attrib_hl, s_col + 1, s_col_e, s_row);
    // 48
    c = c - 48;
    switch (c) {
    case 1: // SINCLAIR
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_sinclair1);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 1;
      break;
    case 2: // KEYBOARD
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 2;
      break;
    case 3: // KEMPSTON
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_kempston);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 3;
      break;
    case 4: // CURSOR
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_cursor);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 4;
      break;
    case 5: // DEFINE
      menu_redefine();
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 2;
      break;
    case 6: // CONTROL
      ++game_control_mode;
      if (game_control_mode > 2) {
        game_control_mode = 0;
      }
      menu_main_print();
      break;
    case 0:
      intrinsic_halt();
      game_cls();
      ay_reset();
      audio_coin();
      z80_delay_ms(300);
      f_input = 0; // Exit Loop
      break;
    }
    if (c > 0 && c < 5)
      game_paint_attrib(&attrib, s_col, s_col_e, s_row);
  }
}

void menu_main_print(void) {

  unsigned char s_row;
  unsigned char s_col;
  unsigned char s_col_e;
  s_row = 7;
  s_col = 10;
  s_col_e = 20;
  // intrinsic_halt();
  game_cls();
  // Gandalf Logo
  NIRVANAP_spriteT(0, TILE_TITLE, 32, 11);
  NIRVANAP_spriteT(1, TILE_TITLE + 1, 32, 13);
  NIRVANAP_spriteT(2, TILE_TITLE + 2, 32, 15);
  NIRVANAP_spriteT(3, TILE_TITLE + 3, 32, 17);
  game_attribs();
  zx_print_str(s_row, s_col, "1 SINCLAIR");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "2 KEYBOARD");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "3 KEMPSTON");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "4 CURSOR");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "5 DEFINE");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "6 CONTROL");
  switch (game_control_mode) {
  case 0:
    zx_print_str(s_row, s_col + 10, "2B ");
    break;
  case 1:
    zx_print_str(s_row, s_col + 10, "1BA");
    break;
  case 2:
    zx_print_str(s_row, s_col + 10, "1BB");
    break;
  }
  game_paint_attrib(&attrib, s_col, s_col_e + 4, (s_row << 3) + 8);
  s_row = s_row + 2;
  zx_print_str(s_row, s_col, "0 START");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  s_row = s_row + 5;
  zx_print_ink(INK_BLUE);
  zx_print_str(s_row, 2, "CODE C.GONZALEZ A.ALBRECHT");
  ++s_row;
  zx_print_str(s_row, 4, "MUSIC S9 / BEYKERSOFT");
  ++s_row;
  zx_print_str(s_row, 1, "TEST ABU SIMBEL  VER 1.99DLX");
  s_row = s_row + 2;
  zx_print_ink(INK_CYAN);
  zx_print_str(s_row, 8, "2018 NOENTIENDO");
  game_paint_attrib(&attrib, 0, 31, (s_row << 3) + 8);
}

void menu_redefine() {
  intrinsic_halt();
  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  for (tmp0 = 8; tmp0 < 14; ++tmp0)
    game_paint_attrib(&attrib, 10, 16, (tmp0 << 3) + 8);

  zx_print_str(8, 10, "UP");
  k1.up = menu_define_key();
  zx_print_str(9, 10, "DOWN");
  k1.down = menu_define_key();
  zx_print_str(10, 10, "LEFT");
  k1.left = menu_define_key();
  zx_print_str(11, 10, "RIGHT");
  k1.right = menu_define_key();
  if (game_control_mode == 0) {
    zx_print_str(12, 10, "JUMP");
    k1.fire = menu_define_key();

    zx_print_str(13, 10, "FIRE");
    k2.fire = menu_define_key();
  } else {
    zx_print_str(12, 10, "FIRE");
    k1.fire = menu_define_key();
  }

  game_fill_row(12, 32);
  menu_main_print();
}

unsigned int menu_define_key() {

  while (1) {
    in_wait_key();
    tmp1 = in_inkey();
    in_wait_nokey();
    tmp0 = 0;
    while (tmp0 < 38) {
      if (tmp1 == key_map[tmp0]) {
        return scan_map[tmp0];
      }
      ++tmp0;
    }
  }
  // return 0;
}
