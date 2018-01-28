/*
   This file is part of Gandalf.

   Gandalf is free software: you can redistribute it and/or modify
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
#ifndef GAME_AY_H
#define GAME_AY_H

// Bit Flags Returned from ay_is_playing()

#define AY_PLAYING_NONE          0
#define AY_PLAYING_SONG          1
#define AY_PLAYING_FX            2

extern unsigned char ay_is_playing(void) __preserves_regs(d,e);

// AY Effects Player

extern void ay_fx_play(unsigned char bank, void *effect) __preserves_regs(a,b,c) __z88dk_callee;
extern void ay_fx_stop(void) __preserves_regs(b,c,d,e,h,l);

// AY Song Player

#define AY_SONG_LOOP             0
#define AY_SONG_ONCE             1

extern void ay_song_play(unsigned char flag, unsigned char bank, void *song) __preserves_regs(b,c) __z88dk_callee;
extern void ay_song_stop(void) __preserves_regs(b,c,d,e,h,l);

// AY Miscellaneous

extern void ay_reset(void) __preserves_regs(b,c,d,e,h,l);

// AY Sound Effects

extern unsigned char ay_fx_04_golpe[];
extern unsigned char ay_fx_04_magia1[];
extern unsigned char ay_fx_04_6[];
extern unsigned char ay_fx_04_charge_barrier[];
extern unsigned char ay_fx_04_coin[];
extern unsigned char ay_fx_04_coin2[];
extern unsigned char ay_fx_04_dano_por_toque[];
extern unsigned char ay_fx_04_dano_por_toque2[];
extern unsigned char ay_fx_04_disparo1[];
extern unsigned char ay_fx_04_disparo2[];
extern unsigned char ay_fx_04_efecto[];
extern unsigned char ay_fx_04_explosion[];
extern unsigned char ay_fx_04_item[];
extern unsigned char ay_fx_04_pierto_abierta[];
extern unsigned char ay_fx_04_salto[];
extern unsigned char ay_fx_04_salto2[];
extern unsigned char ay_fx_04_salto3[];
extern unsigned char ay_fx_04_sin_mana[];
extern unsigned char ay_fx_04_sirena[];

extern unsigned char ay_fx_06_explosion1[];
extern unsigned char ay_fx_06_fx1[];
extern unsigned char ay_fx_06_fx2[];
extern unsigned char ay_fx_06_fx3[];
extern unsigned char ay_fx_06_fx4[];

// AY Songs

extern unsigned char ay_song_03_luna[];
extern unsigned char ay_song_03_sunflower[];

extern unsigned char ay_song_04_super_ring_fellows[];

extern unsigned char ay_song_06_ciel[];
extern unsigned char ay_song_06_lotr[];
extern unsigned char ay_song_06_lotr_lose_a_life[];
extern unsigned char ay_song_06_lotr_level_complete[];
extern unsigned char ay_song_06_lotr_level_start[];

#endif
