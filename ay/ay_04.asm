;; AY RELATED FOR BANK_4

; OWN COPY OF VTII PLAYER

SECTION BANK_4_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_4_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_4_AUDIO

PUBLIC _ay_song_04_lotr
PUBLIC _ay_song_04_lotr_lose_a_life
PUBLIC _ay_song_04_lotr_level_complete
PUBLIC _ay_song_04_lotr_level_start
PUBLIC _ay_song_04_sunflower

_ay_song_04_lotr:
   BINARY "songs/lotr.bin"

_ay_song_04_lotr_lose_a_life:
   BINARY "songs/lotr-losealife.bin"

_ay_song_04_lotr_level_complete:
   BINARY "songs/lotr-lvlcomplete.bin"

_ay_song_04_lotr_level_start:
   BINARY "songs/lotr-lvlstart.bin"

_ay_song_04_sunflower:
   BINARY "songs/sunflower.bin"

PUBLIC _ay_fx_04_coin
PUBLIC _ay_fx_04_dano_por_toque
PUBLIC _ay_fx_04_explosion
PUBLIC _ay_fx_04_golpe
PUBLIC _ay_fx_04_magia1
PUBLIC _ay_fx_04_pierto_abierta
PUBLIC _ay_fx_04_salto
PUBLIC _ay_fx_04_sin_mana

_ay_fx_04_coin:
   BINARY "fx/coin.bin"

_ay_fx_04_dano_por_toque:
   BINARY "fx/dano por toque.bin"

_ay_fx_04_explosion:
   BINARY "fx/explosion.bin"

_ay_fx_04_golpe:
   BINARY "fx/golpe.bin"

_ay_fx_04_magia1:
   BINARY "fx/magia1.bin"

_ay_fx_04_pierto_abierta:
   BINARY "fx/pierto abierta.bin"

_ay_fx_04_salto:
   BINARY "fx/salto.bin"

_ay_fx_04_sin_mana:
   BINARY "fx/sin mana.bin"
