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


        Super Pietro Bros - Cristian Gonzalez - cmgonzalez@gmail.com
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

void main(void) {
  unsigned int counter;

  game_god_mode = 0;
  game_inmune = 0;
  // INTERRUPTS ARE DISABLED

  // RESET AY CHIP
  ay_reset();

  // ATTRIB NORMAL

  attrib[0] = BRIGHT | PAPER_BLACK | INK_WHITE;
  attrib[1] = BRIGHT | PAPER_BLACK | INK_YELLOW;
  attrib[2] = PAPER_BLACK | INK_YELLOW;
  attrib[3] = PAPER_BLACK | INK_WHITE;

  // ATTRIB HIGHLIGHT
  attrib_hl[0] = PAPER_BLACK | INK_RED;
  attrib_hl[1] = PAPER_BLACK | INK_YELLOW;
  attrib_hl[2] = PAPER_BLACK | INK_GREEN;
  attrib_hl[3] = PAPER_BLACK | INK_CYAN;

  // GAME OPTIONS

  // ENABLE SOUND BASED ON DETECTED MODEL

  game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON)
                       : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);
  player_joy = 0;   /*KB1*/
  game_gravity = 1; // 8;
  // vel_y0 + vel_y1 = -84
  player_vel_y0 = -8; //-88; // Velocity
  player_vel_y1 = -4; //-72; // Min Velocity

  // Keyboard Handling

  k1.fire = IN_KEY_SCANCODE_m;
  k1.left = IN_KEY_SCANCODE_o;
  k1.right = IN_KEY_SCANCODE_p;
  k1.up = IN_KEY_SCANCODE_q;   // must be defined otherwise up is always true
  k1.down = IN_KEY_SCANCODE_a; // must be defined otherwise down is always true

  game_joystick_set_menu();
  zx_border(INK_BLACK);

  // Wait for Keypress and Randomize

  in_wait_nokey();
  for (counter = 31416; !in_test_key(); counter += 10061)
    ;
  srand(counter);

  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  zx_print_paper(PAPER_BLACK);
  // Init Game
  game_start_timer();
  // Init Nirvana
  NIRVANAP_tiles(_btiles);
  NIRVANAP_start();
  map_paper_last = PAPER_BLUE;
  map_paper = PAPER_BLACK;
  map_paper_clr = map_paper | (map_paper >> 3) | BRIGHT;

  spr_btile_paint_back();

  // Init Screen

  frame_time = zx_clock();
  // GAME MENU
  // game_menu();
  for (tmp = 0; tmp < 160; tmp++) {
    scr_map[tmp] = TILE_EMPTY;
    scr_obj[tmp] = 0;
  }
  player_keys[0] = 0;
  player_keys[1] = 0;
  player_keys[2] = 0;
  player_keys[3] = 0;
  scr_curr = 0;
  map_width = 8 - 1; // OJO!
  map_heigth = 1;

  spr_page_map(); // INIT FIRST SCREEN

  game_loop();
}

void test_proc() {

    unsigned char v0;
    unsigned char v1;
    unsigned char vr;
    unsigned char scr_curr_tmp;
    unsigned char i;
    unsigned char j;
    unsigned char k;
    unsigned int add_index;
    unsigned int start_index;

    k = 16;
    scr_curr_tmp = scr_curr;

    //Calculate the current screen start index in the world map
  zx_print_ink(INK_MAGENTA);
    i = 0;
    start_index = 0;

    while (i < scr_curr) {
      GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
      IO_7FFD = 0x10 + 6;
      add_index = lenght0[i];
      GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
      IO_7FFD = 0x10 + 0;
      start_index = start_index + add_index; 
      ++i;
    }


    // NIRVANAP_stop();
    intrinsic_di();
    zx_print_chr(0,10,lenght0[i]);
    zx_print_chr(0,16,scr_curr);
    z80_delay_ms(20);
  zx_print_int(0,0,start_index);
    for (i = 0; i < GAME_SCR_MAX_INDEX; ++i) {

      // Page in BANK 06 - Note that global variables are in page 0
      GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
      IO_7FFD = 0x10 + 6;
      v0 = world0[start_index + i];
      v1 = world0[start_index + i + 1];

      // Page in BANK 00
      GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
      IO_7FFD = 0x10 + 0;

      if (v0 < 128) {
        if (!(BIT_CHK(scr_obj[k], scr_curr))) {
          scr_map[k] = v0;
        } else {
          scr_map[k] = TILE_EMPTY;
        }
        ++k;
      } else {
        vr = v0 - 128; // Repeat counter Should be < 128!!

        for (j = 0; j < vr; j++) {
          if (!(BIT_CHK(scr_obj[k], scr_curr))) {
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
    // NIRVANAP_start();
    // Remove all enemies fast
    for (i = 0; i < SPR_P1; ++i) {
      class[i] = 0;
      NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
    }
}

unsigned char test_func() { return 0; }
