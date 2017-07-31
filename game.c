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

#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>
#include "nirvana+.h"
#include "game.h"
#include "game_ay.h"
#include "game_engine.h"
#include "game_sprite.h"
#include "game_player.h"
#include "game_enemies.h"
#include "game_sound.h"
#include "game_zx.h"
#include "macros.h"

void main(void) {
	unsigned int counter;

	game_start_phase = 0;
	game_god_mode = 0;
	game_inmune = 0;
	//INTERRUPTS ARE DISABLED

	//RESET AY CHIP
	ay_reset();

	//ATTRIB NORMAL

	attrib[0]= BRIGHT | PAPER_BLACK | INK_WHITE;
	attrib[1]= BRIGHT | PAPER_BLACK | INK_YELLOW;
	attrib[2]= PAPER_BLACK | INK_YELLOW;
	attrib[3]= PAPER_BLACK | INK_WHITE;

	//ATTRIB HIGHLIGHT
	attrib_hl[0]= PAPER_BLACK | INK_RED;
	attrib_hl[1]= PAPER_BLACK | INK_YELLOW;
	attrib_hl[2]= PAPER_BLACK | INK_GREEN;
	attrib_hl[3]= PAPER_BLACK | INK_CYAN;

	//GAME OPTIONS

	//ENABLE SOUND BASED ON DETECTED MODEL

	game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON) : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);

	//GAME TYPE A

	game_type = 0;

	player_joy[0] = 0; /* SJ1 */
	player_joy[1] = 1; /* SJ2 */

	//Keyboard Handling

	k1.fire	= IN_KEY_SCANCODE_m;
	k1.left	= IN_KEY_SCANCODE_o;
	k1.right = IN_KEY_SCANCODE_p;
	k1.up    = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise up is always true
	k1.down  = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise down is always true

	k2.fire	= IN_KEY_SCANCODE_z;
	k2.left	= IN_KEY_SCANCODE_q;
	k2.right = IN_KEY_SCANCODE_w;
	k2.up    = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise up is always true
	k2.down  = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise down is always true

	game_joystick_set_menu();

	zx_border(INK_BLACK);

	//Wait for Keypress and Randomize

	in_wait_nokey();
	for (counter = 31416; !in_test_key(); counter += 10061) ;
	srand(counter);


	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	zx_print_paper(PAPER_BLACK);
	zx_border(INK_BLACK);


	//Init Game
	game_start_timer();
	//Init Nirvana
	NIRVANAP_tiles(_btiles);
	NIRVANAP_start();
	//Init Screen
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	zx_print_paper(PAPER_BLACK);
	zx_border(INK_BLACK);
	frame_time = zx_clock();
	//GAME MENU
	//game_menu();
	game_loop();
}

void test_func() {
}
