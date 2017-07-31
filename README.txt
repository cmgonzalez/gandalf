==============================================================================

README - Gandalf v1.2


This games was build using the Pietro Engine, with the help of z88dk compiler and using NIRVANA+ for the multicolour graphic engine.


==============================================================================

BUILD INSTRUCTIONS:

* Install or update to the current Z88DK

https://github.com/z88dk/z88dk#installation
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
Open a shell in the game_bros home directory
Run "make"

To build zx7 compresed run "make zx7" (twice)

==============================================================================

RUNTIME MEMORY MAP:

23296 - 23551     Stack (256 bytes)
23552 - 55786ish  Game
56323 - 65378     Nirvana+ Engine (nirvana hole contains program variables)

128K ONLY BANK 6:

49152 - 55587     AY Music, Sound Effects and Players

==============================================================================
