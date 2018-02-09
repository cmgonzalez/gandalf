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
#include "game_audio.h"
#include "game_menu.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include "nirvana+.h"
#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>

void main(void) {
  unsigned int counter;
  //DEBUG
  game_inmune = 0;    // GAME_INMUNE;
  game_inf_lives = 0; // GAME_INF_LIVES;
  game_debug = 0;
  game_world = 0;
  scr_curr = 0xff;//0xff;

  // INTERRUPTS ARE DISABLED
  // RESET AY CHIP
  ay_reset();
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


  // Wait for Keypress and Randomize //
  /* Default Values for menu */
  joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_sinclair1);
  joyfunc2 = (uint16_t(*)(udk_t *))(in_stick_keyboard);

  zx_border(INK_BLACK);
  if (!game_debug) {
    in_wait_nokey();
  for (counter = 31416; !in_test_key(); counter += 10061)
    ;
  srand(counter);
  }
  //Clear Screen and init Nirvana
  game_cls();
  // Init Game
  game_start_timer();
  // Load Tiles
  NIRVANAP_tiles(_btiles);
  game_attribs();
  game_over = 1;
  if (!game_debug) {
    zx_print_str(12, 6, "FOR FELIPE AND EDDIE");
    game_colour_message(12, 6, 6 + 20, 30, 0);
  }
  // Init Screen
  frame_time = zx_clock();

  menu_curr_sel = 1;
  map_paper_last = PAPER_BLUE;
  /*MAIN LOOP*/
  while (1) {
    /*Init Objects*/
    game_obj_clear();


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
    game_set_checkpoint();
    game_round_up = 0;
    game_respawn_curr_time = 255;
    game_boss_alive = 1;
    game_control_mode = 0; // 0)2button - default 1) 1button: 1 Button UP+FIRE Shoots 3) 1 button: fire shoots, up jump , up+fire for ladders
    game_over = 0;
    dirs = 0x00;
    map_paper = PAPER_BLACK;
    game_attribs();
    spr_btile_paint_back();
    // MENU
    if (!game_debug) menu_main();
    //GAME
    game_loop();
    //GAME OVER
    spr_flatten();
    game_update_stats();
    zx_print_str(12, 12, "GAME OVER");
    game_over = 0; // Hack game_colour_message to render background
    game_colour_message(12, 12, 12 + 9, 250, 0);

    spr_clear_scr();
    game_world = 0;
    scr_curr = 0xFF;

    game_cls();
  }
}

void test_proc() {}

unsigned char test_func() { return 0; }
