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
  player_vel_y1 = -7; //-72; // Min Velocity

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
    scr_obj0[tmp] = 0;
    scr_obj1[tmp] = 0;
  }
  player_keys[0] = 0;
  player_keys[1] = 0;
  player_keys[2] = 0;
  player_keys[3] = 0;
  scr_curr = 0;
  map_width = 16 - 1; // OJO!
  map_heigth = 1;

  spr_page_map(); // INIT FIRST SCREEN

  game_loop();
}

void test_proc() {

}

unsigned char test_func() { return 0; }
