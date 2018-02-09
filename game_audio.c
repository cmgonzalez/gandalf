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
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

void audio_ingame(void) {

  if (!game_song_play && !ay_is_playing()) {
    game_song_play = 1;
    switch (game_world) {
    case 0:
      ay_song_play(AY_SONG_LOOP, 4, ay_song_04_ciel);
      break;
    case 1:
      ay_song_play(AY_SONG_LOOP, 4, ay_song_04_luna);
      break;
    case 2:
      ay_song_play(AY_SONG_LOOP, 4, ay_song_04_ciel);
      break;
    case 3:
      ay_song_play(AY_SONG_LOOP, 4, ay_song_04_luna);
      break;
    }
  }
}
void audio_explosion(void) {
  ay_fx_stop();
  ay_fx_play(4, ay_fx_04_explosion);
}
void audio_explosion1(void) {
  ay_fx_play(4, ay_fx_04_explosion);
  // ay_song_play(AY_SONG_ONCE, 4, 4, ay_fx_04_explosion1);
}
void audio_disparo1(void) { ay_fx_play(4, ay_fx_04_disparo1); }
void audio_disparo2(void) { ay_fx_play(4, ay_fx_04_disparo2); }
void audio_efecto(void) { ay_fx_play(4, ay_fx_04_efecto); }
void audio_salto(void) { ay_fx_play(4, ay_fx_04_salto); }
void audio_golpe(void) { ay_fx_play(4, ay_fx_04_golpe); }
void audio_coin(void) {
  // ay_fx_stop();
  ay_fx_play(4, ay_fx_04_coin);
}
void audio_magic1(void) { ay_fx_play(4, ay_fx_04_magic1); }
void audio_sin_mana(void) { ay_fx_play(4, ay_fx_04_sin_mana); }
void audio_puerta(void) { ay_fx_play(4, ay_fx_04_puerta); }
void audio_mushroom(void) { ay_fx_play(4, ay_fx_04_mushroom); }
void audio_lotr_lose_a_life(void) {
  ay_reset();
  ay_song_play(AY_SONG_ONCE, 4, ay_song_04_lotr_lose_a_life);
}
void audio_level_start(void) {
  ay_reset();
  ay_song_play(AY_SONG_ONCE, 4, ay_song_04_lotr_level_start);
}
void audio_level_complete(void) {
  ay_reset();
  ay_song_play(AY_SONG_ONCE, 4, ay_song_04_lotr_level_complete);
}
void audio_game_end(void) {
  ay_reset();
  ay_song_play(AY_SONG_ONCE, 6, ay_song_06_sunflower);
}
void audio_menu(void) {
  ay_reset();
  ay_song_play(AY_SONG_LOOP, 6, ay_song_06_lotr);
}
void audio_checkpoint(void) { ay_fx_play(4, ay_fx_04_checkpoint); }
void audio_levelup(void) { ay_fx_play(4, ay_fx_04_levelup); }
