
; ay sound effects converted from wav by voicenc

SECTION BANK_06

; test 128 memory store!
;PUBLIC _lvl1
;_lv1:
;    defb 58,130,70,130,32,90,130,32,70,130,0,32,0,32,130,0,58,130,0,130,70,58,130,70,130,0,32,0,49,0,32,0,58,0,32,130,0,58,132,0,32,131,0,32,0,58,132,0,58,134,0,49,131,0,58,0,132,70,132,0,93,131,0,92,0,58,135,0,58,135,70,58,135,0,58,134,32,70,58,135,0,58,138,70,130,40,130,70,137,40


PUBLIC _ay_effect_01

_ay_effect_01:

   include "res_ayfx/ay_effect_01.asm"
   defw -1

PUBLIC _ay_effect_02

_ay_effect_02:

   include "res_ayfx/ay_effect_02.asm"
   defw -1

PUBLIC _ay_effect_03

_ay_effect_03:

   include "res_ayfx/ay_effect_03.asm"
   defw -1

PUBLIC _ay_effect_04

_ay_effect_04:

   include "res_ayfx/ay_effect_04.asm"
   defw -1

PUBLIC _ay_effect_05

_ay_effect_05:

   include "res_ayfx/ay_effect_05.asm"
   defw -1

PUBLIC _ay_effect_06

_ay_effect_06:

   include "res_ayfx/ay_effect_06.asm"
   defw -1

PUBLIC _ay_effect_07

_ay_effect_07:

   include "res_ayfx/ay_effect_07.asm"
   defw -1

PUBLIC _ay_effect_08

_ay_effect_08:

   include "res_ayfx/ay_effect_08.asm"
   defw -1

PUBLIC _ay_effect_09

_ay_effect_09:

   include "res_ayfx/ay_effect_09.asm"
   defw -1

PUBLIC _ay_effect_10

_ay_effect_10:

   include "res_ayfx/ay_effect_10.asm"
   defw -1

PUBLIC _ay_effect_11

_ay_effect_11:

   include "res_ayfx/ay_effect_11.asm"
   defw -1

PUBLIC _ay_effect_12

_ay_effect_12:

   include "res_ayfx/ay_effect_12.asm"
   defw -1

PUBLIC _ay_effect_13

_ay_effect_13:

   include "res_ayfx/ay_effect_13.asm"
   defw -1

PUBLIC _ay_effect_14

_ay_effect_14:

   include "res_ayfx/ay_effect_14.asm"
   defw -1

PUBLIC _ay_effect_15

_ay_effect_15:

   include "res_ayfx/ay_effect_15.asm"
   defw -1

PUBLIC _ay_effect_16

_ay_effect_16:

   include "res_ayfx/ay_effect_16.asm"
   defw -1

PUBLIC _ay_effect_17

_ay_effect_17:

   include "res_ayfx/ay_effect_17.asm"
   defw -1

PUBLIC _ay_effect_18

_ay_effect_18:

   include "res_ayfx/ay_effect_18.asm"
   defw -1

; background effects only below this point

PUBLIC _ay_effect_background

defc _ay_effect_background = _ay_effect_19

PUBLIC _ay_effect_19

_ay_effect_19:

   include "res_ayfx/ay_effect_19.asm"
   defw $feff, _ay_effect_19

PUBLIC _ay_effect_20

_ay_effect_20:

   include "res_ayfx/ay_effect_20.asm"
   defw $feff, _ay_effect_20
