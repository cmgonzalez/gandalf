;; AY RELATED FOR BANK_4

; OWN COPY OF VTII PLAYER

SECTION BANK_4_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_4_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_4_AUDIO

PUBLIC _ay_song_04_super_ring_fellows

PUBLIC _ay_fx_04_golpe
PUBLIC _ay_fx_04_magia1
PUBLIC _ay_fx_04_6
PUBLIC _ay_fx_04_charge_barrier
PUBLIC _ay_fx_04_coin
PUBLIC _ay_fx_04_coin2
PUBLIC _ay_fx_04_dano_por_toque
PUBLIC _ay_fx_04_dano_por_toque2
PUBLIC _ay_fx_04_disparo1
PUBLIC _ay_fx_04_disparo2
PUBLIC _ay_fx_04_efecto
PUBLIC _ay_fx_04_explosion
PUBLIC _ay_fx_04_item
PUBLIC _ay_fx_04_pierto_abierta
PUBLIC _ay_fx_04_salto
PUBLIC _ay_fx_04_salto2
PUBLIC _ay_fx_04_salto3
PUBLIC _ay_fx_04_sin_mana
PUBLIC _ay_fx_04_sirena

_ay_song_04_super_ring_fellows:
   BINARY "songs/super-ring-fellows.bin"


_ay_fx_04_golpe:
   BINARY "fx/golpe.bin"

_ay_fx_04_magia1:
   BINARY "fx/magia1.bin"

_ay_fx_04_6:
   BINARY "fx/6.bin"

_ay_fx_04_charge_barrier:
   BINARY "fx/Charge Barrier.bin"

_ay_fx_04_coin:
   BINARY "fx/coin.bin"

_ay_fx_04_coin2:
   BINARY "fx/coin2.bin"

_ay_fx_04_dano_por_toque:
   BINARY "fx/dano por toque.bin"

_ay_fx_04_dano_por_toque2:
   BINARY "fx/dano por toque2.bin"

_ay_fx_04_disparo1:
   BINARY "fx/disparo1.bin"

_ay_fx_04_disparo2:
   BINARY "fx/disparo2.bin"

_ay_fx_04_efecto:
   BINARY "fx/efecto.bin"

_ay_fx_04_explosion:
   BINARY "fx/explosion.bin"

_ay_fx_04_item:
   BINARY "fx/Item.bin"

_ay_fx_04_pierto_abierta:
   BINARY "fx/pierto abierta.bin"

_ay_fx_04_salto:
   BINARY "fx/salto.bin"

_ay_fx_04_salto2:
   BINARY "fx/salto2.bin"

_ay_fx_04_salto3:
   BINARY "fx/salto3.bin"

_ay_fx_04_sin_mana:
   BINARY "fx/sin mana.bin"

_ay_fx_04_sirena:
   BINARY "fx/sirena.bin"
