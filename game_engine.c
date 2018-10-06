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
#include "game_audio.h"
#include "game_ay.h"
#include "game_enemies.h"
#include "game_engine.h"
#include "game_menu.h"
#include "game_player.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include <arch/zx.h>
#include <arch/zx/nirvana+.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

/* Main Game Loop  */

void game_loop(void) {
  game_print_footer();
  game_round_init();


  while (!game_over) {

    while (!game_round_up && !game_over) {

      /*Enemies turn*/
      enemy_turn();
      /*Player 1 turn*/
      // sprite = SPR_P1;
      player_turn();

      if (game_boss) {
        boss_turn();
      }
      /*Anim Bullets*/
      if (game_check_time(&bullet_time, TIME_BULLETS)) {
        bullet_time = zx_clock();
        if (bullet_count)
          spr_bullets_play();
      }

      /*Play animatios*/
      if (game_check_time(&anim_time, TIME_ANIM)) {
        // zx_border(INK_BLACK);
        anim_time = zx_clock();
        if (anim_count)
          spr_play_anim();
      }

      /*Each second aprox - update fps/score/phase left/phase advance*/
      if (game_check_time(&frame_time, TIME_EVENT)) {
        frame_time = zx_clock();
        intrinsic_halt();
        audio_ingame();
        game_respawn();
        if (game_debug) {
          game_fps();
        }
      }

      if (game_check_time(&player_inm_time,
                          90)) {
        player_inm_time = zx_clock();
        player_hit = 0;
      }
      // INGAME
      if (game_level_up) {
        game_level_up = 0;
        player_lvl++;
        game_update_stats();
        spr_flatten();
        audio_levelup();
        zx_print_str(12, 12, "LEVEL UP!");
        game_colour_message(12, 12, 12 + 9, 60, 0);
        spr_unflattenP1();
      }

      ++loop_count;
      ++fps;
    }


    if (game_round_up) {
      game_round_up = 0;
      game_respawn_curr_time = game_respawn_curr_time - 32;
      game_world++;
      game_boss_alive = 1;
      game_boss = 0;
      scr_curr = 255;
      player_lin_scr = 0;
      player_col_scr = 0;
      game_obj_clear();
      player_col_scr = 2;
      player_lin_scr = 128;
      game_round_init();
    }
  }
}

void game_fps(void) {
  zx_print_ink(INK_WHITE);
  zx_print_int(23, 24, fps);
  fps = 0;
}

void game_respawn(void) {
  // Enemy Respawn, add animatios on the screen that at the end will Respawn an
  // enemy
  sprite = 0;
  while (sprite < SPR_P1) {
    if (game_respawn_time[sprite] > 0) {
      if (game_check_time(&game_respawn_time[sprite], game_respawn_curr_time)) {
        index1 = game_respawn_index[sprite];
        s_col1 = (index1 & 15) << 1;
        s_lin1 = index1;
        s_lin1 = (s_lin1 >> 4) << 4;
        scr_map[index1] = TILE_ANIM + sprite; // 0xFF;
        game_respawn_time[sprite] = 0;
        audio_respawn();
        spr_add_anim(s_lin1, s_col1, TILE_ANIM_RESPAWN, 3, 4,
                     game_respawn_tile[sprite]);

        break;
      }
    }
    ++sprite;
  }
}

void game_draw_screen(void) {
  unsigned char f_mush;
  unsigned char f_tile;
  intrinsic_halt();
  game_boss = 0;
  game_boss_fix = 0;
  player_brick_time = zx_clock();

  f_mush = 0;
  spr_count = 0;
  spr_init_effects();
  while (spr_count < SPR_P1) { // TODO SINGLE CLEAR FUNC
    // Clear enemies related tables
    game_respawn_index[spr_count] = 0;
    game_respawn_tile[spr_count] = 0;
    game_respawn_time[spr_count] = 0;
    mush_index[spr_count] = 0;
    mush_class[spr_count] = 0;
    class[spr_count] = 0;
    ++spr_count;
  }
  index1 = 16;
  s_lin1 = 0;
  s_col1 = 2;
  // intrinsic_ei();
  spr_count = 0;
  game_mush_count = 0;

  intrinsic_di();

  while (index1 < GAME_SCR_MAX_INDEX) {
    if ((index1 & 15) == 0) {
      s_lin1 = s_lin1 + 16;
      s_col1 = 0;
    }
    f_tile = scr_map[index1];
    if (f_tile < TILE_END) {
      // TILES
      if (f_tile == TILE_HIDDEN_BRICK || f_tile == TILE_SPECIAL) {
        if (game_obj_chk(index1 - 16)) {
          // USED BRICKS
          scr_map[index1] = TILE_NOSPECIAL;
          f_tile = TILE_NOSPECIAL;
        } else {
          if (f_tile == TILE_HIDDEN_BRICK) {
            f_tile = TILE_EMPTY;
          }
        }
      }
      if (f_tile == TILE_STOPPER) {
        f_tile = game_match_back(index1);
      }
      // NORMAL TILE
      NIRVANAP_drawT_raw(f_tile, s_lin1, s_col1);
    } else {
      // SPRITES
      if (f_tile <= INDEX_ENEMY_BOSS1) {
        // ENEMIES
        if (spr_count < 8 && game_boss_alive) {
          game_respawn_index[spr_count] = index1;
          game_respawn_tile[spr_count] = f_tile;
          game_add_enemy(f_tile);
        }
      } else {
        if (!game_obj_chk(index1)) {
          // MUSHROMS
          mush_index[f_mush] = index1 + 16;
          mush_class[f_mush] = f_tile;
          ++f_mush;
        }
      }
      scr_map[index1] = game_match_back(index1);
      NIRVANAP_drawT_raw(scr_map[index1], s_lin1, s_col1);
    }
    s_col1 = s_col1 + 2;
    ++index1;
  }
  intrinsic_ei();
  if (!game_boss_alive) {
    game_boss_clear();
  }
  game_update_stats();
}

void game_boss_kill(void) {
  game_boss = 0;
  game_boss_alive = 0;
  audio_explosion();
  game_song_play_start = 0;
  ay_reset();
  audio_boss_explode();

  spr_add_anim(boss_lin, boss_col, TILE_ANIM_FIRE, 3, 0, 0);
  spr_add_anim(boss_lin, boss_col + 2, TILE_ANIM_FIRE, 3, 0, 0);
  spr_add_anim(boss_lin + 16, boss_col, TILE_ANIM_FIRE, 3, 0, 0);
  spr_add_anim(boss_lin + 16, boss_col + 2, TILE_ANIM_FIRE, 3, 0, 0);
  for (tmp0 = 0; tmp0 < SPR_P1; ++tmp0) {
    if (bullet_col[tmp0] != 0xFF) {
      s_lin0 = bullet_lin[tmp0];
      s_col0 = bullet_col[tmp0];
      spr_back_repaint(); // restore background
    }
    bullet_col[tmp0] = 0xFF;
    spr_destroy(tmp0);
    game_respawn_time[tmp0] = 0;
  }
  game_boss_clear();
  game_print_footer();
  if (game_world == 3) {
    game_end();
  } // else {
    // audio_ingame();
  //}
}

void game_end() {

  z80_delay_ms(500);

  scr_curr = 0;
  spr_page_map();
  game_draw_screen();
  audio_game_end();
  NIRVANAP_drawT(TILE_P1_STANR, 128, 2);
  z80_delay_ms(1500);

  zx_print_str(8, 8, "CONGRATULATIONS!");
  game_colour_message(8, 8, 8 + 16, 2000, 0);
  zx_print_str(8, 7, "YOU HAVE DEFEATED");
  game_colour_message(8, 7, 7 + 17, 2000, 0);
  zx_print_str(8, 5, "THE LEFT EYE OF SAURON");
  game_colour_message(8, 5, 5 + 22, 2000, 0);
  zx_print_str(8, 3, "ARDA WILL BE ON PEACE NOW");
  game_colour_message(8, 3, 3 + 25, 2000, 0);
  z80_delay_ms(1000);
  in_wait_key();
  ay_reset();
  // Keep Playing
  game_world = 0xFF;
  scr_curr = 0xFF;
  game_round_up = 1;
}

void game_add_enemy(unsigned char enemy_tile_index) __z88dk_fastcall {

  if (enemy_tile_index != INDEX_ENEMY_BOSS1) {
    tmp0 = 0;
    while (tmp0 <= GAME_TOTAL_INDEX_CLASSES) {
      tmp1 = tmp0 * 3;
      if (spr_init[tmp1] == enemy_tile_index) {
        enemy_init(s_lin1, s_col1, spr_init[tmp1 + 1], spr_init[tmp1 + 2]);
        tmp0 = 0xFF;
      } else {
        ++tmp0;
      }
    }
  } else {
    if (game_boss == 0 && game_boss_alive) {
      boss_lin = s_lin1;
      boss_col = s_col1;
      // zx_print_ink(INK_WHITE);
      // z80_delay_ms(200);
      switch (game_world) {
      case 0:
        boss_tile = TILE_ENEMY_BOSS1;
        break;
      case 1:
        boss_tile = TILE_ENEMY_BOSS1 + 4 + 12;
        break;
      case 2:
        boss_tile = TILE_ENEMY_BOSS1 + 8 + 12;
        break;
      case 3:
        boss_tile = TILE_ENEMY_BOSS1 + 12 + 12;
        break;
      }
      // boss_tile = TILE_ENEMY_BOSS1 + (game_world * 4);
      boss_stat = 0;
      spr_set_up(&boss_stat);
      game_boss = 1;
      game_boss_hit = 6;
      game_song_play_start = 0;
      // ay_reset();
      // audio_ingame();
    } else {
      if (!game_boss)
        game_boss_fix = 1;
    };
  }
}

void game_print_footer(void) {
  zx_print_paper(PAPER_YELLOW | BRIGHT);
  zx_print_ink(INK_BLACK);
  game_fill_row(19, 98);
  zx_print_str(19, 0, "a");
  zx_print_str(19, 31, "c");
  zx_print_paper(PAPER_YELLOW);
  game_fill_row(20, 32);
  game_fill_row(21, 32);
  game_fill_row(22, 32);
  game_fill_row(23, 104);

  zx_print_str(20, 0, "d");
  zx_print_str(20, 31, "e");
  zx_print_str(21, 0, "d");
  zx_print_str(21, 31, "e");
  zx_print_str(22, 0, "d");
  zx_print_str(22, 31, "e");
  zx_print_str(23, 0, "f");
  zx_print_str(23, 31, "g");

  zx_print_str(20, 1, "GANDALF");

  zx_print_ink(INK_RED);

  zx_print_str(22, 8, "l");  // COIN
  zx_print_str(22, 14, "m"); // STR
  zx_print_str(22, 20, "n"); // INT
  zx_print_str(22, 26, "o"); // LEVEL

  zx_print_str(20, 9, "VITA");

  zx_print_ink(INK_MAGENTA);
  zx_print_str(20, 22, "EXP");
  zx_print_str(21, 23, "000000");

  zx_print_ink(INK_BLUE);
  zx_print_str(20, 16, "MANA");

  zx_print_str(22, 2, "    ");
  zx_print_paper(PAPER_BLACK);
  game_update_stats();
  game_print_score();

  if (game_debug) {
    zx_print_str(23, 20, "LPS:");
  }

}

void game_print_score(void) {
  zx_print_paper(PAPER_YELLOW);
  zx_print_ink(INK_MAGENTA);
  zx_print_int(21, 23, player_score);
  zx_print_paper(PAPER_BLACK);
  zx_print_ink(INK_WHITE);
  zx_print_int(0, 14, game_score_top); // SCORE TOP
}

void game_cls() {
  NIRVANAP_stop();
  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  zx_print_ink(INK_WHITE);
  NIRVANAP_start();
}

void game_update_stats(void) {
  zx_print_paper(PAPER_YELLOW);

  zx_print_ink(INK_BLACK);
  zx_print_chr(21, 3, player_lives);
  zx_print_chr(22, 9, player_coins);
  zx_print_chr(22, 15, player_str);
  zx_print_chr(22, 21, player_int);
  zx_print_chr(22, 27, player_lvl);
  zx_print_ink(INK_RED);
  zx_print_chr(21, 10, player_vita);
  zx_print_ink(INK_BLUE);
  zx_print_chr(21, 17, player_mana);
  if (game_boss) {
    zx_print_ink(INK_MAGENTA);
    for (tmp0 = 0; tmp0 < 6; ++tmp0) {
      if (tmp0 < game_boss_hit) {
        zx_print_str(19, 13 + tmp0, "*");
      } else {
        zx_print_str(19, 13 + tmp0, " ");
      }
    }

    // zx_print_chr(1, 16, game_boss_hit);
  }


  if (player_keys[0]) {
    zx_print_ink(INK_WHITE);
    zx_print_str(22, 2, "]");
  }
  if (player_keys[1]) {
    zx_print_ink(INK_RED);
    zx_print_str(22, 3, "]");
  }
  if (player_keys[2]) {
    zx_print_ink(INK_GREEN);
    zx_print_str(22, 4, "]");
  }
  if (player_keys[3]) {
    zx_print_ink(INK_CYAN);
    zx_print_str(22, 5, "]");
  }
  zx_print_paper(PAPER_BLACK);
}

void game_tick(void) {
  ++curr_time;
  zx_isr();
}

void game_start_timer(void) {
  NIRVANAP_ISR_HOOK[0] = 205;                                // call
  z80_wpoke(&NIRVANAP_ISR_HOOK[1], (unsigned int)game_tick); // game_tick
}

void game_set_checkpoint() {
  game_checkpoint_col = (player_col_scr >> 1) << 1;
  game_checkpoint_lin = ((player_lin_scr >> 4) << 4);
  game_checkpoint_scr = scr_curr;
}

void game_round_init(void) {

  /* screen init */
  /*PHASE INIT*/

  loop_count = 0;
  zx_set_clock(0);
  frame_time = 0;
  game_level_up = 0;

  /* Phase Draw Start */
  // spr_draw_clear();
  /*Draw Platforms*/

  spr_init_effects();
  game_print_header();
  //game_print_footer();
  spr_page_map();
  ay_reset();
  audio_level_start();
  game_draw_screen();
  game_set_checkpoint();
  game_song_play_start = 0;

  // ay_reset();
  // ay_fx_play(ay_effect_12);
  /* Player(s) init */
  if (!game_over) {
    player_init(player_lin_scr, player_col_scr, TILE_P1_STANR);
  }
  zx_print_paper(PAPER_YELLOW);
  zx_print_str(22, 2, "    ");
  zx_print_paper(PAPER_BLACK);
  game_update_stats();
  // z80_delay_ms(50);

  if (!game_debug) {

    switch (game_world) {
    case 0:
      zx_print_str(12, 8, "WORLD 1 THE SHIRE");
      tmp0 = 8;
      tmp1 = 8 + 17;
      break;
    case 1:
      zx_print_str(12, 10, "WORLD 2 MORIA");
      tmp0 = 10;
      tmp1 = 10 + 13;
      break;
    case 2:
      zx_print_str(12, 9, "WORLD 3 MORDOR");
      tmp0 = 9;
      tmp1 = 9 + 14;
      break;
    case 3:
      zx_print_str(12, 8, "WORLD 4 BARAD DUR");
      tmp0 = 8;
      tmp1 = 8 + 17;
      break;
    }
    game_paint_attrib(&attrib_osd, tmp0, tmp1, (12 << 3) + 8);
    game_colour_message(12, tmp0, tmp1, 200, 0);
  }
  audio_ingame();
}

void game_print_header(void) {

  zx_print_ink(INK_RED);
  zx_print_str(0, 11, "$%|");
  zx_print_ink(INK_WHITE);
  zx_print_str(0, 19, "0");
  // zx_print_ink(INK_WHITE);
  /* Print score */
  game_print_score();
}

unsigned int game_check_map(unsigned char f_lin, unsigned char f_col) {
  // TODO A SINGLE FUNCTION TO SAVE BYTES
  if ((f_col & 1) == 0) {
    index1 = spr_calc_index(f_lin, f_col);

    return game_check_cell(&index1);
  } else {

    g_player_hit_left = 0;
    g_player_hit_right = 0;

    index1 = spr_calc_index(f_lin, f_col - 1);
    if (game_check_cell(&index1)) {
      g_player_hit_left = 1;
    }
    if (g_player_hit_left == 0) {
      ++index1;
      if (game_check_cell(&index1)) {
        g_player_hit_right = 1;
      }
    }

    return g_player_hit_left || g_player_hit_right;
  }
}

unsigned char game_check_cell(unsigned int *f_index) __z88dk_fastcall {
  unsigned char f_tile;
  unsigned char f_check;
  unsigned char f_kind;
  unsigned char f_class;
  // OUT OFF SCREEN
  if (*f_index > GAME_SCR_MAX_INDEX) {
    return 1;
  }

  f_tile = scr_map[*f_index];

  if (sprite != SPR_P1) {
    f_class = class[sprite];
    f_kind = sprite_kind[f_class];
    if (f_kind == E_GHOST) {
      return 0;
    }
    if (f_tile >= TILE_ANIM) {
      // Animation
      return 0;
    }
    if (f_tile == TILE_STOPPER && f_class != MUSHROOM_VITA &&
        f_class != MUSHROOM_MANA && f_class != MUSHROOM_EXTRA) {
      return 1;
    }

    if (f_kind == E_VERTICAL || f_kind == E_GOTA) {
      if (f_class == FIRE || f_class == PIRANHA) {
        if (f_tile <= TILE_FLOOR) {
          return 0;
        } else {
          return f_tile;
        }
      } else {
        // VERTICAL ENEMIES
        if (f_tile <= TILE_ITEM_E ||
            (f_tile >= TILE_STAIR_S && f_tile <= TILE_STAIR_E)) {
          return 0;
        } else {
          return f_tile;
        }
      }
    }

    if (f_kind == E_HORIZONTAL || f_kind == E_WALK) {
      // HORIZONTAL ENEMIES
      if (sprite_horizontal_check) {
        f_check = TILE_CEIL;
      } else {
        f_check = TILE_FLOOR;
      }

      if (f_tile < f_check) {
        return 0;
      } else {
        return f_tile;
      }
    }
  } else {
    if (!player_jump_check) {
      // TILE_EMPTY -> TILE_FLOOR
      if (f_tile < TILE_FLOOR) {
        return 0;
      }
      /*
            // TILE_FLOOR -> TILE_STAIR_S
            if (f_tile < TILE_STAIR_S) {
              if (BIT_CHK(s_state, STAT_FALL)) {
                return 1;
              } else {
                return 0;
              }
            }
      */
      // TILE_STAIR_S -> TILE_CEIL

      if (f_tile < TILE_CEIL) {

        if (BIT_CHK(s_state, STAT_FALL)) {
          return 1;
        } else {
          return 0;
        }
      }
    } else {
      if (f_tile <= TILE_STAIR_E) {
        return 0;
      }
    }

    if (f_tile < TILE_DOOR_E) { // DOOR EXIT
      player_open_door(*f_index, f_tile);
      return 1;
    }
    // TILE_CEIL -> TILE_END
    if (f_tile < TILE_END) {
      return 1;
    }
  }

  // NOT A TILE f_tile > TILE_END
  return 0;
}

void game_paint_attrib(unsigned char *f_attrib[], char f_start,
                       unsigned char f_end, unsigned char f_lin) {
  for (tmp_uc = f_start; tmp_uc < f_end; ++tmp_uc) {
    NIRVANAP_paintC(f_attrib, f_lin, tmp_uc);
  }
}

void game_colour_message(unsigned char f_row, unsigned char f_col,
                         unsigned char f_col2, unsigned int f_microsecs,
                         unsigned char skip) {
  unsigned int entry_time;

  tmp = 1;
  frame_time = zx_clock();
  entry_time = zx_clock();
  while (tmp && !game_check_time(&entry_time, f_microsecs)) {
    if (game_check_time(&frame_time, 5)) {
      // ROTATE ATTRIB ARRAY
      frame_time = zx_clock();
      if (game_over) {
        game_paint_attrib(&attrib_hl, f_col, f_col2, (f_row << 3) + 8);
        game_rotate_attrib();
      } else {
        game_paint_attrib(&attrib_osd, f_col, f_col2, (f_row << 3) + 8);
        game_rotate_attrib_osd();
      }
    }
    if (skip) {
      while ((joyfunc1)(&k1) != 0)
        tmp = 0;
    };
  };
  if (game_over) {
    // Clear Message
    tmp1 = f_col2 - f_col;
    for (tmp0 = 0; tmp0 < tmp1; tmp0++) {
      zx_print_str(f_row, f_col + tmp0, " "); // SPACE
    }
  } else {
    tmp1 = f_col2 - f_col;
    s_lin0 = f_row << 3;        //* 8; // TODO OPTIMIZE
    s_col1 = (f_col >> 1) << 1; // (x*2)/2
    for (tmp0 = 0; tmp0 < tmp1; tmp0 = tmp0 + 2) {
      s_col0 = s_col1 + tmp0;
      spr_back_repaint();
    }
  }
}

unsigned char game_check_time(unsigned int *start, unsigned char lapse) {
  if (zx_clock() - *start > lapse) {
    return 1;
  } else {
    return 0;
  }
}

unsigned char game_shoot_fire(unsigned char f_sprite, unsigned char f_tile) {
  unsigned char f_dir;
  if (bullet_col[f_sprite] == 0xFF) {
    // ay_fx_play(ay_effect_08);
    ++bullet_count;
    bullet_dir[f_sprite] = 0;
    bullet_lin0[f_sprite] = lin[f_sprite];
    bullet_col0[f_sprite] = col[f_sprite];
    bullet_lin[f_sprite] = lin[f_sprite];
    bullet_frames[f_sprite] = 2;

    if (f_sprite == SPR_P1) {
      bullet_max[f_sprite] = 4 + (player_lvl >> 1);
      f_dir = BIT_CHK(state_a[SPR_P1], STAT_LDIRL);
    } else {
      bullet_max[f_sprite] = 0;
      f_dir = BIT_CHK(s_state, STAT_DIRL);
    }
    if (f_tile == TILE_ARROW) {
      bullet_class[f_sprite] = BULLET_ARROW;
      bullet_max[f_sprite] = 8 + (game_world * 4);
    }

    if (f_tile == TILE_FIREBALL) {
      if (f_sprite == SPR_P1) {
        bullet_class[f_sprite] = BULLET_ARROW;
      } else {
        bullet_class[f_sprite] = BULLET_FIREBALL_UP;
      }
    }

    if (f_tile == TILE_AXE) {
      bullet_class[f_sprite] = BULLET_AXE;
      bullet_lin[f_sprite] = lin[f_sprite] - 4;
      bullet_frames[f_sprite] = 4;
      bullet_vel[f_sprite] = player_vel_y0;
    }

    index1 = spr_calc_index(lin[f_sprite], col[f_sprite]);
    if (f_dir) {
      // Left
      if (col[f_sprite] < 1)
        return 1;
      bullet_col[f_sprite] = col[f_sprite];
      if (f_tile != TILE_AXE) {
        bullet_tile[f_sprite] = f_tile + 2;
      } else {
        bullet_tile[f_sprite] = f_tile;
      }
      bullet_dir[f_sprite] = 0x01;
      bullet_colint[f_sprite] = 2;
    } else {
      // Right n default
      if (col[f_sprite] > 29)
        return 1;
      bullet_col[f_sprite] = col[f_sprite];
      bullet_tile[f_sprite] = f_tile;
      bullet_dir[f_sprite] = 0xFF;
      bullet_colint[f_sprite] = 0xFF;
    }
  }
  return 0;
}

unsigned char game_shoot_fire_boss(unsigned char f_tile) {

  unsigned char f_sprite;
  unsigned char f_dir;

  if (boss_col > col[SPR_P1]) {
    f_dir = 1;
  }

  if (boss_col < col[SPR_P1]) {
    f_dir = 0;
  }

  f_sprite = 0;
  while (f_sprite < SPR_P1) {
    if (bullet_col[f_sprite] == 0xFF) {
      break;
    }
    ++f_sprite;
  }
  if (f_sprite == SPR_P1) {
    return 0;
  }

  // if (bullet_col[f_sprite] == 0xFF) {
  ++bullet_count;
  bullet_dir[f_sprite] = 0;

  if (game_world != 1) {
    bullet_lin0[f_sprite] = boss_lin + 16;
    bullet_class[f_sprite] = BULLET_FIREBALL_UP;
  } else {
    bullet_class[f_sprite] = BULLET_FIREBALL;
    if (tmp0 == 0) {
      bullet_lin0[f_sprite] = boss_lin - 8; // Dual Fire
    } else {
      bullet_lin0[f_sprite] = boss_lin + 24; // Dual Fire
    }
  }

  bullet_lin[f_sprite] = bullet_lin0[f_sprite];
  bullet_frames[f_sprite] = 2;

  bullet_max[f_sprite] = 8;
  index1 = spr_calc_index(boss_lin, boss_col);
  if (f_dir) {
    // Left
    if (boss_col < 1)
      return 1;
    bullet_tile[f_sprite] = f_tile + 2;
    bullet_dir[f_sprite] = 0x01;
    bullet_colint[f_sprite] = 2;
    bullet_col[f_sprite] = boss_col;
  } else {
    // Right n default
    if (boss_col > 29)
      return 1;
    bullet_tile[f_sprite] = f_tile;
    bullet_dir[f_sprite] = 0xFF;
    bullet_colint[f_sprite] = 0xFF;
    bullet_col[f_sprite] = boss_col + 6;
  }
  bullet_col0[f_sprite] = bullet_col[f_sprite];
  //}
  return 0;
}

void game_obj_set(unsigned int f_index) __z88dk_fastcall {
  unsigned char f_scr_surr1;
  if (scr_curr < 8) {
    BIT_SET(scr_obj0[f_index], scr_curr);
  } else {
    f_scr_surr1 = scr_curr - 8;
    BIT_SET(scr_obj1[f_index], f_scr_surr1);
  }
}

unsigned char game_obj_chk(unsigned int f_index) __z88dk_fastcall {
  unsigned char f_scr_surr1;

  if (scr_curr < 8) {
    return BIT_CHK(scr_obj0[f_index], scr_curr);
  } else {
    f_scr_surr1 = scr_curr - 8;
    return BIT_CHK(scr_obj1[f_index], f_scr_surr1);
  }
}

void game_obj_clear() {

  for (tmp = 0; tmp < 160; tmp++) {
    // scr_map[tmp] = game_match_back(tmp); // TILE_EMPTY;
    scr_obj0[tmp] = 0;
    scr_obj1[tmp] = 0;
  }

  if (game_inmune) {
    tmp = 1;
  } else {
    tmp = 0;
  }
  player_keys[0] = tmp;
  player_keys[1] = tmp;
  player_keys[2] = tmp;
  player_keys[3] = tmp;
}

void game_boss_clear() {
  unsigned int f_index;
  for (f_index = 0; f_index < GAME_SCR_MAX_INDEX; ++f_index) {
    if (scr_map[f_index] == 55) {
      scr_map[f_index] = TILE_EMPTY;
      spr_draw_index(&f_index);
    }
  }
}

void game_rotate_attrib(void) {

  tmp_uc = attrib_hl[3];
  attrib_hl[3] = attrib_hl[2];
  attrib_hl[2] = attrib_hl[1];
  attrib_hl[1] = attrib_hl[0];
  attrib_hl[0] = tmp_uc;
}

void game_rotate_attrib_osd(void) {

  tmp_uc = attrib_osd[3];
  attrib_osd[3] = attrib_osd[2];
  attrib_osd[2] = attrib_osd[1];
  attrib_osd[1] = attrib_osd[0];
  attrib_osd[0] = tmp_uc;
}

void game_attribs() {
  // ATTRIB NORMAL
  attrib[0] = map_paper | BRIGHT | INK_WHITE;
  attrib[1] = map_paper | BRIGHT | INK_CYAN;
  attrib[2] = map_paper | INK_CYAN;
  attrib[3] = map_paper | BRIGHT | INK_BLUE;

  // ATTRIB HIGHLIGHT
  attrib_hl[0] = map_paper | BRIGHT | INK_BLUE;
  attrib_hl[1] = map_paper | BRIGHT | INK_BLUE;
  attrib_hl[2] = map_paper | BRIGHT | INK_MAGENTA;
  attrib_hl[3] = map_paper | BRIGHT | INK_CYAN;

  // ATTRIB OSD
  attrib_osd[0] = map_paper | BRIGHT | INK_YELLOW;
  attrib_osd[1] = map_paper | BRIGHT | INK_WHITE;
  attrib_osd[2] = map_paper | BRIGHT | INK_WHITE;
  attrib_osd[3] = map_paper | BRIGHT | INK_YELLOW;
}
unsigned char game_match_back_strict(unsigned int f_index) __z88dk_fastcall {

  unsigned char v0;
  unsigned char v1;
  if ((f_index > 1) && ((f_index & 15) != 0)) {
    v0 = scr_map[f_index - 1];
    v1 = scr_map[f_index + 1];
    if (v0 == TILE_EMPTY_DARK && v1 == TILE_EMPTY_DARK)
      return TILE_EMPTY_DARK;
    if (v0 == TILE_EMPTY_DARK_A && v1 == TILE_EMPTY_DARK_A)
      return TILE_EMPTY_DARK_A;
    if (v0 == TILE_EMPTY_DARK_B && v1 == TILE_EMPTY_DARK_B)
      return TILE_EMPTY_DARK_B;
  }
  return TILE_EMPTY;
}

unsigned char game_match_back(unsigned int f_index) __z88dk_fastcall {

  unsigned char v0;
  unsigned char v1;
  if ((f_index > 1) && ((f_index & 15) != 0)) {
    v0 = scr_map[f_index - 1];
    v1 = scr_map[f_index + 1];
    if (v0 == TILE_EMPTY_DARK || v1 == TILE_EMPTY_DARK)
      return TILE_EMPTY_DARK;
    if (v0 == TILE_EMPTY_DARK_A || v1 == TILE_EMPTY_DARK_A)
      return TILE_EMPTY_DARK_A;
    if (v0 == TILE_EMPTY_DARK_B || v1 == TILE_EMPTY_DARK_B)
      return TILE_EMPTY_DARK_B;
  }
  return TILE_EMPTY;
}
