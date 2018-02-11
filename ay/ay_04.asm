;; AY RELATED FOR BANK_4

; OWN COPY OF VTII PLAYER

SECTION BANK_4_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_4_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_4_AUDIO

;PUBLIC _ay_song_04_ciel
PUBLIC _ay_song_04_luna
PUBLIC _ay_song_04_sunflower
;PUBLIC _ay_song_04_regis
PUBLIC _ay_song_04_lotr

_ay_song_04_lotr:
   BINARY "songs/lotr.bin"

_ay_song_04_luna:
  BINARY "songs/luna.bin"

_ay_song_04_sunflower:
   BINARY "songs/sunflower.bin"

;_ay_song_04_regis:
;   BINARY "songs/regis.bin"

;_ay_song_04_ciel:
;  BINARY "songs/ciel_2.bin"
