;; AY RELATED FOR BANK_4

; OWN COPY OF VTII PLAYER

SECTION BANK_4_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_4_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_4_AUDIO


PUBLIC _ay_song_04_lotr_lose_a_life
PUBLIC _ay_song_04_lotr_level_complete
PUBLIC _ay_song_04_lotr_level_start
PUBLIC _ay_song_04_ciel
;PUBLIC _ay_song_04_super_ring_fellow
PUBLIC _ay_song_04_luna

PUBLIC _ay_fx_04_coin
PUBLIC _ay_fx_04_dano_por_toque
PUBLIC _ay_fx_04_explosion
PUBLIC _ay_fx_04_golpe
PUBLIC _ay_fx_04_magic1
PUBLIC _ay_fx_04_puerta
PUBLIC _ay_fx_04_salto
PUBLIC _ay_fx_04_sin_mana
PUBLIC _ay_fx_magic
PUBLIC _ay_fx_04_disparo1
PUBLIC _ay_fx_04_disparo2
PUBLIC _ay_fx_04_efecto
PUBLIC _ay_fx_04_explosion1
PUBLIC _ay_fx_04_mushroom
PUBLIC _ay_fx_04_checkpoint
PUBLIC _ay_fx_04_levelup

_ay_song_04_lotr_lose_a_life:
   BINARY "songs/lotr-losealife.bin"
_ay_song_04_lotr_level_complete:
   BINARY "songs/lotr-lvlcomplete.bin"
_ay_song_04_lotr_level_start:
   BINARY "songs/lotr-lvlstart.bin"
;_ay_song_04_super_ring_fellow:
;  BINARY "songs/super-ring-fellows.bin"
_ay_song_04_luna:
  BINARY "songs/luna.bin"


_ay_fx_04_coin:
   BINARY "fx/coin.afx"
_ay_fx_04_dano_por_toque:
   BINARY "fx/dano_por_toque.afx"
_ay_fx_04_explosion:
   BINARY "fx/explosion.afx"
_ay_fx_04_golpe:
   BINARY "fx/golpe.afx"
_ay_fx_04_magic1:
   BINARY "fx/magic1.afx"
_ay_fx_04_puerta:
   BINARY "fx/puerta.afx"
_ay_fx_04_salto:
   BINARY "fx/salto.afx"
_ay_fx_04_sin_mana:
   BINARY "fx/sin_mana.afx"
_ay_fx_magic:
   BINARY "fx/magic.afx"
_ay_song_04_ciel:
   BINARY "songs/ciel.bin"
_ay_fx_04_disparo1:
   BINARY "fx/disparo1.afx"
_ay_fx_04_disparo2:
   BINARY "fx/disparo2.afx"
_ay_fx_04_efecto:
   BINARY "fx/efecto.afx"
_ay_fx_04_explosion1:
   BINARY "fx/explosion1.afx"
_ay_fx_04_mushroom:
    BINARY "fx/mushroom.afx"
_ay_fx_04_checkpoint:
    BINARY "fx/checkpoint.afx"
_ay_fx_04_levelup:
    BINARY "fx/levelup.afx"
