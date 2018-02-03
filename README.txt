==============================================================================

README - Gandalf v1.2


This games was build using the Pietro Engine, with the help of z88dk compiler and using NIRVANA+ for the multicolour graphic engine.


==============================================================================
HISTORY

Long after the destruction of the Ring, Gandalf comebacks for a revenge with the four last demons of Middle earth. Help him to free the world of the remaining evil of Melkor.

An arcade / rpg mix with four rounds of 16 screens each. Each Round have a an unique boss that have to be defeated to move to the next Round. Control can be by either 1 button or 2 button control. For the ZX Spectrum models with 128 Kb only.

Gandalf can gain experience, this will increase your fire range, levels are upgraded based your points: Also you can pick items that will increase your maximum health and mana. You can have a maximum of 200 health and mana.

Damage Table:
Deadly damage 50 points
Enemy damage 20 points
Bullet damage 10 points
Boss Bullet damage 10 points
Boss damage instant dead.

Coded on C using the powerfull z88dk compiler , the fabulous Nirvana Plus, and the genial Vortex II Player for AY music. The code is GPL’ed https://bitbucket.org/CmGonzalez/gandalf/

Code: Cristián González, Alvin Albrecht
Music: s9
FX: BeykerSoft
Testing:Abu Simbel


Good luck wizard!



BUILD INSTRUCTIONS:

* Install or update to the current Z88DK

https://github.com/z88dk/z88dk#installation

Compile z88dk
cd z88dk
chmod 777 build.sh
chmod 777 config.sh
./build.sh
cd libsrc
cd _DEVELOPMENT
make TARGET=zx

Git clone if you will regularly update z88dk.
Nightly build if you just want to unzip the current build.

Pietro Bros uses zsdcc so linux users should make sure they also build zsdcc.
zsdcc is included among the pre-built Windows and OSX binaries already.

* Configure the nirvana+ library

Edit file "z88dk/libsrc/_DEVELOPMENT/target/zx/config_nirvanap.m4"
Change "define(`__NIRVANAP_TOTAL_ROWS', 23)" to "define(`__NIRVANAP_TOTAL_ROWS', 18)”

You can also disable wide tiles from Nirvana by changing, to save 300 bytes aprox.

define(`__NIRVANAP_OPTIONS_WIDE_DRAW',    0x00)
define(`__NIRVANAP_OPTIONS_WIDE_SPRITES', 0x00)

To build the Pentagon version instead of the regular Spectrum version:

Edit file "z88dk/libsrc/_DEVELOPMENT/target/zx/config_target.m4"
Change "define(`__SPECTRUM', 1)" to "define(`__SPECTRUM', 32)"

* Rebuild the zx library so that nirvana+ changes take effect

Open a shell and go to directory "z88dk/libsrc/_DEVELOPMENT"
Run "Winmake zx" (windows) or "make TARGET=zx" (anything else)

* Build
Open a shell in the game home directory
Run "make" or "make -j10" for 10 compiler threads

To build zx7 compresed run "make zx7" (twice)

==============================================================================

RUNTIME MEMORY MAP:

23296 - 23551     Stack (256 bytes)
23552 - 55786ish  Game
56323 - 65378     Nirvana+ Engine (nirvana hole contains program variables)

128K ONLY BANK 6:

49152 - 55587     AY Music, Sound Effects and Players

==============================================================================

Version 1.01

  -Shoot Animation.
  -Shoot Sound fix.
  -Brick Hit picks items, a la SMB1
  -Perfomance improved on animations
  -Remove Red Dot on loading screen
  -Repaint menu choice after a game
  -Mushroom remove block by stopper tiles
  -Mushroom fix popup
  -Game key repaint on screen after Round UP
  -Level table updated (10 Level Max)
  -Some Map Fixes
  -Boss fire left range fixed
  -Speed up animations
  -Background paint fixes , stric function.
  -Perfomance improved on animations
  -SPR_P1 / enemies different paint functions for perfomance
  -game_audio.c - audio abstract
  -game_menu.c - menu abstract
  -Tiles fixes
  -Corrected btiles paper restore after game end.
  -Code clean
