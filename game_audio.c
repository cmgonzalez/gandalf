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

void audio_explosion(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_explosion);
}
void audio_explosion1(void) {
  ay_song_play(AY_SONG_ONCE, 6, ay_fx_06_explosion1);
}
void audio_disparo1(void) {
  ay_song_play(AY_SONG_ONCE, 6, ay_fx_06_disparo1);
}
void audio_disparo2(void) {
  ay_song_play(AY_SONG_ONCE, 6, ay_fx_06_disparo2);
}

void audio_efecto(void) {
  ay_song_play(AY_SONG_ONCE, 6, ay_fx_06_efecto);
}
void audio_salto(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_salto);
}
void audio_golpe(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_golpe);
}
void audio_coin(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_coin);
}
void audio_magia1(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_magia1);
}
void audio_sin_mana(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_sin_mana);
}
void audio_pierto_abierta(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_fx_04_pierto_abierta);
}
void audio_lotr_lose_a_life(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_song_04_lotr_lose_a_life);
}
void audio_level_start(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_song_04_lotr_level_start);
}
void audio_level_complete(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_song_04_lotr_level_complete);
}
void audio_game_end(void) {
  ay_song_play(AY_SONG_ONCE, 4, ay_song_04_sunflower);
}
void audio_menu(void) {
  ay_song_play(AY_SONG_LOOP, 4, ay_song_04_lotr);
}
