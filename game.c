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


        Gandalf - Cristian Gonzalez - cmgonzalez@gmail.com
*/

#include "game.h"
#include "game_ay.h"
#include "game_enemies.h"
#include "game_ay.h"
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

void main(void) {
  unsigned int counter;

  game_inmune = 0;    // GAME_INMUNE;
  game_inf_lives = 1; // GAME_INF_LIVES;
  // INTERRUPTS ARE DISABLED
  // RESET AY CHIP
  ay_reset();
  // GAME OPTIONS

  // ENABLE SOUND BASED ON DETECTED MODEL

  game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON)
                       : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);
  game_gravity = GAME_GRAVITY; // 8;
  // vel_y0 + vel_y1 = -84
  player_vel_y0 = GAME_VELOCITY;     //-88; // Velocity
  player_vel_y1 = GAME_MIN_VELOCITY; //-72; // Min Velocity

  // Keyboard Handling

  k1.fire = IN_KEY_SCANCODE_m;
  // TODO k1.fire1 = IN_KEY_SCANCODE_SPACE;
  k1.left = IN_KEY_SCANCODE_o;
  k1.right = IN_KEY_SCANCODE_p;
  k1.up = IN_KEY_SCANCODE_q;   // must be defined otherwise up is always true
  k1.down = IN_KEY_SCANCODE_a; // must be defined otherwise down is always true

  k2.left = IN_KEY_SCANCODE_DISABLE;
  k2.right = IN_KEY_SCANCODE_DISABLE;
  k2.up = IN_KEY_SCANCODE_DISABLE;
  k2.down = IN_KEY_SCANCODE_DISABLE;
  k2.fire = IN_KEY_SCANCODE_SPACE;

  zx_print_paper(PAPER_BLACK);
  zx_border(INK_BLACK);

  // Wait for Keypress and Randomize //
  /* Default Values for menu */
  joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_sinclair1);
  joyfunc2 = (uint16_t(*)(udk_t *))(in_stick_keyboard);

  in_wait_nokey();
  for (counter = 31416; !in_test_key(); counter += 10061)
    ;
  srand(counter);
  zx_print_paper(PAPER_BLACK);
  zx_border(INK_BLACK);
  zx_paper_fill(INK_BLACK | PAPER_BLACK);

  // Init Game
  game_start_timer();
  // Init Nirvana
  NIRVANAP_tiles(_btiles);
  NIRVANAP_start();

  // Init Screen
  frame_time = zx_clock();

  while (1) {
    /*Init Objects*/
    game_obj_clear();
    game_world = 0; // 1;//0;
    scr_curr = 0xFF; // 0xFF equals read default screen from map

    /*Player Init*/
    player_max_mana = GAME_START_MAX_MANA;
    player_max_vita = GAME_START_MAX_VITA;
    player_lives = GAME_START_LIVES;
    player_mana = player_max_mana;
    player_vita = player_max_vita;
    player_str = 0;
    player_lvl = 0;
    player_int = 0;
    player_score = 0;
    player_lin_scr = GAME_LIN_FLOOR - 16;
    player_col_scr = 2;

    game_checkpoint_col = player_col_scr;
    game_checkpoint_lin = player_lin_scr;
    game_checkpoint_scr = scr_curr;

    game_worldup = 0;
    game_respawn_curr_time = 255;
    game_mush_count = 0;
    game_boss_alive = 1;
    game_2buttons = 1; // Two button control - Space default 2 button
    /* phase init */
    game_over = 0;

    /* game loop start */
    dirs = 0x00;
    // MENU
    ay_reset();
    menu_main();
    map_paper_last = PAPER_BLUE;
    map_paper = PAPER_BLUE;
    map_paper_clr = map_paper | (map_paper >> 3) | BRIGHT;
    game_attribs();
    // END MENU
    game_loop();
    spr_flatten();
    zx_print_str(12, 12, "GAME OVER");
    game_over = 0; // Hack game_colour_message to render background
    game_colour_message(12, 12, 12 + 9, 250, 0);
    game_obj_clear();
    spr_clear_scr();
    NIRVANAP_stop();
    zx_print_paper(PAPER_BLACK);
    zx_border(INK_BLACK);
    zx_paper_fill(INK_BLACK | PAPER_BLACK);
    NIRVANAP_start();
  }
}

void test_proc() {}

unsigned char test_func() { return 0; }
