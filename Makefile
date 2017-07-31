

CSRC = globals.c game.c game_enemies.c game_engine.c game_player.c game_sprite.c game_zx.c
ASRC = btiles.asm font.asm game_ay.asm game_ay_res_fx.asm game_ay_res_midi.asm game_engine.asm game_sound.asm game_zx.asm

OBJS = $(CSRC:.c=.o)
INCL = $(ZCCCFG)/../../include/_DEVELOPMENT/sdcc

OUT = bin/game_release.tap
OUTC = bin/game_release_zx7.tap
ORG = 23552

CFLAGS = -SO3 --max-allocs-per-node200000 --opt-code-size

.PHONY: depend clean

default: loader.tap game_scr.bin game.font $(OUT)
	-ls -l *.bin
	-rm -f *.bin *.tap *.font *.zx7 game_bros zcc_proj.lst zcc_opt.def
	grep code_compiler_size game_bros.map

zx7: loader.tap game_scr.bin game.font $(OUTC)
	-ls -l *.bin
	-rm -f *.bin *.tap *.font *.zx7 game_bros zcc_proj.lst zcc_opt.def
	grep code_compiler_size game_bros.map

loader.tap: src_tap/loader.bas
	src_tap/bas2tap -sPietro -a10 src_tap/loader.bas loader.tap > /dev/null

game_scr.bin: src_tap/game_scr.bin
	cp src_tap/game_scr.bin game_scr.bin

game.font: src_font/game.font
	cp src_font/game.font game.font

$(OUT): $(OBJS) $(ASRC) mmap.inc
	-rm -f $(OUT)
	zcc +zx -vn -m -zorg=$(ORG) -startup=31 -clib=sdcc_iy $(OBJS) $(ASRC) game_loader.asm -o game_bros -pragma-include:zpragma.inc
	appmake +inject -b game_bros_NIRVANAP.bin -o nirvanap_final.bin -i game_bros_NIRVANA_HOLE.bin --offset 6627
	appmake +zx -b game_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
	appmake +zx -b game_bros_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
	appmake +zx -b game_scr.bin -o game_scr.tap --org 16384 --noloader --noheader
	appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org 56323 --noloader --noheader
	appmake +zx -b game_bros_CODE.bin -o game.tap --org $(ORG) --noloader --noheader
	appmake +zx -b game_bros_BANK_06.bin -o game_ay.tap --org 49152 --noloader --noheader
	cat loader.tap mcload.tap mcloader.tap game_scr.tap nirvanap.tap game.tap game_ay.tap > $(OUT)
	grep code_compiler_size game_bros.map

$(OUTC): $(OBJS) $(ASRC) mmap.inc
	-rm -f $(OUTC)
	zcc +zx -vn -zorg=$(ORG) -startup=31 -clib=sdcc_iy $(OBJS) $(ASRC) game_loader.asm -o game_bros -pragma-include:zpragma.inc
	appmake +inject -b game_bros_NIRVANAP.bin -o nirvanap_final.bin -i game_bros_NIRVANA_HOLE.bin --offset 6627
	zx7 -f game_scr.bin
	zx7 -f nirvanap_final.bin
	zx7 -f game_bros_CODE.bin
	zx7 -f game_bros_BANK_06.bin
	echo PUBLIC LEN_SCREEN, LEN_NIRVANAP, LEN_GAME, LEN_BANK_06 > zx7_game_sizes.asm
	echo defc LEN_SCREEN = $(shell wc -c < game_scr.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_NIRVANAP = $(shell wc -c < nirvanap_final.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_GAME = $(shell wc -c < game_bros_CODE.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_BANK_06 = $(shell wc -c < game_bros_BANK_06.bin.zx7) >> zx7_game_sizes.asm
	zcc +zx -vn -zorg=$(ORG) -startup=31 -clib=sdcc_iy -Ca-DPCOMPRESS $(OBJS) $(ASRC) zx7_game_sizes.asm game_loader.asm -o game_bros -pragma-include:zpragma.inc
	appmake +zx -b game_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
	appmake +zx -b game_bros_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
	appmake +zx -b game_scr.bin.zx7 -o game_scr.tap --org 16384 --noloader --noheader
	appmake +zx -b nirvanap_final.bin.zx7 -o nirvanap.tap --org 56323 --noloader --noheader
	appmake +zx -b game_bros_CODE.bin.zx7 -o game.tap --org $(ORG) --noloader --noheader
	appmake +zx -b game_bros_BANK_06.bin.zx7 -o game_ay.tap --org 49152 --noloader --noheader
	cat loader.tap mcload.tap mcloader.tap game_scr.tap nirvanap.tap game.tap game_ay.tap > $(OUTC)
	grep code_compiler_size game_bros.map

.c.o:
	zcc +zx -vn --list -c -clib=sdcc_iy $(CFLAGS) --fsigned-char $<

clean:
	-rm -f *.o *.lis *.bin *.tap *.font *.map *.zx7 game_bros zcc_proj.lst zcc_opt.def

depend: $(CSRC)
	makedepend $(INCL) $^

# DO NOT DELETE THIS LINE -- make depend needs it