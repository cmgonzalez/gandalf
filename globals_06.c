#include "globals.h"
#include <arch/zx.h>
// PLACED IN BANK 06
// zsdcc can only place code and rodata into different banks at the moment
// anything else use assembly language and sections
// Compressed MAP
// FIRST ONE BYTE FOR EACH SCREEN SIZE, THEN THE DATA, ALSO THE PAPER BACKGROUND
const unsigned char world0_w[]   = {16, 4, 8,  1};
const unsigned char world0_h[]   = { 1, 4, 2, 16};
const unsigned char start_scr0[] = {0,  0, 0, 15};
const unsigned char paper0[] = {
  PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLACK,PAPER_BLUE,PAPER_BLUE,
  PAPER_BLUE,PAPER_BLACK,PAPER_BLACK,PAPER_BLUE,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,
  PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,
  PAPER_BLACK , PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK, PAPER_BLACK
};
const unsigned char lenght0[] = {
6,95,97,82,99,99,96,101,98,96,93,88,113,107,93,93
,92,94,72,96,93,81,90,100,98,88,108,102,99,84,103,95
,104,106,88,56,93,104,89,80,95,102,92,57,81,63,33,48
,14,50,50,50,50,50,50,50,50,50,50,50,50,50,50,47

};
const unsigned char world0[] = {
 208,0,176,40,144,82
,135,0,40,130,0,121,0,40,7,8,130,0,7,8,132,0,84,130,0,92,0,84,9,10,130,0,9,10,130,0,58,142,0,35,11,12,111,11,12,96,11,12,36,11,12,131,0,40,0,81,138,82,130,0,40,80,70,86,134,29,40,29,118,29,130,0,81,86,70,139,29,11,12,131,29,96,131,29,40,130,29,40,130,29,41,134,82,83,130,48,78,48,81,132,82
,131,0,40,0,124,130,0,40,131,0,7,8,133,0,84,0,92,130,0,84,131,0,9,10,140,0,58,130,0,126,130,0,11,12,0,11,12,96,11,12,111,11,12,35,0,93,130,0,138,82,83,0,40,131,0,29,118,29,40,134,29,86,70,80,40,130,0,139,29,70,86,83,130,0,131,29,40,130,29,40,130,29,96,132,29,11,12,132,82,83,48,78,130,48,81,134,82
,132,0,140,19,0,7,8,0,17,139,84,0,9,10,130,0,27,84,135,52,84,28,132,0,130,40,84,135,29,84,40,132,0,130,40,72,29,35,29,113,131,29,84,40,0,40,0,70,136,84,70,131,84,0,84,0,70,84,131,29,108,29,108,29,70,130,29,84,0,11,12,30,84,29,43,134,29,40,130,29,144,82
,19,137,0,7,8,132,0,18,131,0,7,8,131,0,35,9,10,35,134,0,40,9,10,138,0,40,130,0,58,133,0,108,130,0,108,44,130,0,40,58,133,0,130,58,0,122,130,0,58,130,0,84,0,40,130,0,130,40,130,0,35,92,0,35,131,0,84,0,58,130,0,130,58,132,0,5,6,131,0,29,11,12,134,0,53,1,2,3,4,53,0,131,82,83,132,48,81,135,82
,137,0,7,8,0,35,0,130,19,0,7,8,134,0,9,10,40,130,0,17,84,0,9,10,131,0,40,130,0,40,130,0,110,130,0,84,131,0,40,32,133,0,11,12,40,30,0,75,132,0,32,0,11,12,30,11,81,133,82,130,0,123,30,33,30,81,130,82,83,52,39,52,29,130,52,130,0,92,81,82,83,29,40,136,29,11,12,132,29,99,29,98,135,29,140,82,83,131,48
,130,19,130,0,35,131,0,7,8,134,0,84,18,131,0,40,130,0,9,10,134,0,84,130,0,110,130,0,40,131,0,40,135,0,30,40,11,12,133,0,32,0,40,130,0,134,82,12,30,11,12,0,32,132,0,130,52,29,52,29,52,81,130,82,83,30,33,30,122,130,0,136,29,40,29,81,82,83,92,130,0,135,29,99,29,98,131,29,0,11,12,131,48,81,140,82
,135,0,7,8,137,0,7,8,131,0,9,10,131,0,7,8,132,0,9,10,0,35,0,11,12,102,35,0,9,10,132,0,40,0,40,0,132,61,0,40,0,40,136,0,40,130,0,40,135,0,99,40,11,12,40,0,130,40,0,40,11,12,40,98,130,0,70,132,60,70,130,60,70,132,60,70,130,0,70,132,0,70,11,12,70,132,0,70,0,82,83,132,48,81,130,82,83,132,48,81,82
,136,0,35,135,0,7,8,130,0,35,0,40,130,0,40,0,35,132,0,9,10,130,0,108,130,40,130,0,130,40,108,130,0,7,8,131,0,40,0,40,130,0,108,0,40,0,40,0,9,10,131,0,40,130,0,60,0,35,60,130,0,40,136,0,60,132,0,60,136,0,60,35,132,0,5,6,35,60,131,0,11,12,134,0,1,2,3,4,130,0,11,12,82,83,140,48,81,82
,133,0,7,8,142,0,9,10,32,131,0,32,0,7,8,130,0,108,130,0,130,40,0,32,131,0,32,0,9,10,131,0,123,0,130,40,112,33,30,40,102,33,30,40,132,0,92,0,81,139,82,131,0,11,131,86,52,86,52,130,86,52,86,52,86,130,0,11,131,86,135,29,108,29,86,40,11,132,86,29,130,40,29,40,29,45,29,42,40,136,82,83,48,81,133,82
,132,0,7,8,138,0,7,8,34,40,9,10,0,40,0,40,130,0,40,34,130,0,9,10,34,138,0,34,131,0,35,33,100,11,12,35,11,12,35,0,30,100,33,30,35,144,82,86,52,29,86,130,29,52,130,29,52,130,29,52,131,29,86,130,29,108,140,29,73,133,29,40,133,29,40,131,29,82,83,48,86,130,48,86,130,48,86,130,48,86,130,48,81
,131,0,7,8,142,0,9,10,130,0,7,8,131,0,7,8,132,0,32,132,0,9,10,0,37,0,9,10,132,0,33,30,112,11,12,137,0,133,82,83,70,80,136,0,29,86,130,29,130,77,70,130,0,122,134,0,29,108,130,29,130,77,70,0,5,92,0,32,0,32,0,32,132,29,130,77,0,1,2,3,4,33,96,33,96,33,144,82
,130,0,7,8,134,0,35,0,35,131,19,130,0,9,10,0,7,8,134,0,17,130,84,133,0,9,10,0,63,130,0,63,0,17,84,14,132,0,35,130,0,40,133,0,47,130,84,130,0,35,132,0,58,132,0,130,61,130,84,0,34,108,32,108,34,131,0,34,108,32,108,34,130,84,0,34,0,32,0,34,0,58,0,34,0,32,0,34,130,84,0,33,40,33,40,33,131,0,33,40,33,40,33,130,29,133,82,83,131,48,81,134,82
,84,134,0,52,130,0,52,130,0,52,27,130,84,131,0,40,30,40,131,0,30,130,0,30,38,130,84,135,61,130,63,61,130,63,130,61,130,84,61,141,0,130,84,46,0,26,0,25,130,0,25,131,0,26,130,0,130,84,61,130,0,100,130,0,78,0,30,132,0,61,130,84,130,0,136,61,131,0,108,84,130,0,25,130,0,30,130,0,30,132,0,25,0,74,130,84,0,131,61,132,84,0,130,40,0,130,84
,130,19,137,0,56,130,55,56,0,84,18,130,0,7,8,132,0,35,133,0,14,18,130,0,9,10,131,0,58,108,133,0,84,18,138,0,120,131,0,84,0,123,133,0,58,131,0,120,131,0,84,0,92,0,34,139,0,84,131,0,34,0,5,6,0,58,35,133,0,29,35,0,96,33,1,2,3,4,130,0,54,130,55,54,0,134,82,83,130,48,81,134,82
,136,0,21,136,0,7,8,133,0,20,131,0,21,132,0,9,10,130,0,40,130,0,20,131,0,20,135,0,40,84,70,0,20,130,0,131,19,133,0,40,130,84,70,0,20,130,0,17,13,18,132,0,40,131,84,70,0,20,0,19,131,16,19,130,0,40,132,84,70,0,20,0,17,13,14,13,18,130,0,133,84,70,0,20,0,17,13,57,13,18,0,144,82
,142,0,11,81,0,7,8,138,0,11,81,86,0,9,10,122,131,0,7,8,131,0,11,81,130,86,131,0,92,131,0,9,10,130,0,11,81,130,86,84,138,0,11,81,130,86,130,84,0,40,0,40,0,40,130,0,36,0,81,130,86,131,84,0,60,0,60,0,60,132,0,52,110,130,29,110,84,136,0,104,35,133,29,72,131,82,83,131,48,81,136,82
,148,84,28,131,52,130,0,131,52,27,78,132,84,28,0,35,0,40,130,0,40,131,0,27,131,84,28,131,0,35,40,113,0,40,35,131,0,130,84,28,35,26,0,108,0,58,130,0,58,0,26,25,0,130,84,0,108,130,0,40,0,26,130,0,26,0,25,130,0,130,84,132,0,58,136,0,112,84,131,0,40,132,0,30,40,0,30,0,98,143,84,70,130,84
,148,84,28,0,56,130,55,56,0,27,135,84,28,131,0,120,132,0,27,133,84,28,26,131,0,120,131,0,26,0,27,131,84,28,124,0,58,133,0,25,131,0,27,130,84,0,92,140,0,130,84,131,0,58,138,0,130,84,133,0,54,130,55,54,133,0,29,130,84,70,141,78
,82,12,140,0,7,8,86,82,12,136,0,21,130,0,9,10,130,86,82,12,130,0,7,8,131,0,20,132,0,84,130,86,82,12,0,9,10,130,0,131,19,131,0,130,84,130,86,82,133,0,17,13,18,131,0,130,84,130,86,82,132,0,19,131,16,19,130,0,84,132,29,0,131,40,17,13,14,13,18,130,0,133,29,0,131,40,17,13,57,13,18,130,0,143,82,84
,145,84,28,136,0,132,77,27,130,84,131,0,26,130,0,96,40,97,0,130,77,130,0,130,84,26,35,132,0,131,40,0,77,0,40,26,130,84,130,0,121,131,0,131,85,0,77,25,77,40,130,84,0,25,92,131,0,131,52,0,77,40,77,131,84,0,108,0,25,0,25,132,0,131,77,130,0,84,12,131,0,35,133,0,35,77,131,0,130,84,70,141,84
,141,84,70,131,84,28,139,52,70,27,130,84,26,139,0,70,26,130,84,131,0,26,130,0,43,130,0,26,130,0,70,0,130,84,40,25,35,130,40,0,58,0,130,40,25,35,70,0,131,84,0,110,130,58,0,26,0,130,58,0,110,70,0,84,77,142,0,84,77,133,0,99,131,0,40,130,0,78,0,145,84
,130,84,70,142,84,27,70,141,52,84,35,70,132,0,35,135,0,25,84,113,70,135,0,25,0,25,131,0,84,106,70,131,0,25,130,0,125,131,0,124,130,0,84,130,0,130,40,132,0,93,131,0,92,130,0,84,130,0,130,63,135,0,25,0,25,0,84,35,132,0,25,130,0,112,99,132,0,40,84,131,0,130,58,131,0,131,63,70,131,63
,144,84,130,0,52,0,52,0,52,84,134,0,27,84,0,25,131,0,25,0,84,0,40,0,108,40,130,0,84,130,0,40,0,44,0,40,84,0,60,130,0,60,130,0,84,130,0,84,26,84,26,130,84,135,0,84,40,134,0,84,131,0,40,131,0,130,84,134,0,84,131,0,60,131,0,84,0,40,0,40,130,0,104,74,0,40,0,40,0,99,37,84,130,63,130,78,70,138,78,84
,130,84,70,142,84,28,70,135,0,121,130,0,52,68,130,84,0,70,135,0,92,131,0,68,130,84,0,70,130,0,102,136,0,68,130,84,0,70,0,65,66,67,131,0,103,131,0,68,75,84,0,70,130,0,23,131,0,65,66,67,130,0,68,130,84,0,70,130,0,22,132,0,23,131,0,68,130,84,12,70,0,40,22,132,40,22,40,0,53,68,130,84,80,70,81,139,82,83
,145,84,28,130,0,35,52,130,0,52,35,52,130,0,52,27,130,84,131,0,108,40,130,0,40,108,40,130,0,130,40,130,84,131,0,130,40,130,0,40,43,40,130,0,40,47,84,0,131,40,130,78,130,0,131,78,130,0,130,78,130,84,58,130,63,84,138,48,130,84,131,0,84,138,49,130,84,131,53,84,138,49,84,81,141,82,83,84
,84,35,40,132,0,35,132,0,70,0,27,0,84,0,130,58,130,0,40,133,0,70,131,0,84,132,0,130,58,133,0,70,0,25,0,84,130,0,58,134,0,40,0,70,0,40,0,84,130,0,58,0,25,0,25,0,26,58,26,70,0,58,0,84,0,25,130,0,40,134,0,70,131,0,84,131,0,130,58,0,107,132,0,70,131,0,84,38,134,0,112,0,40,0,70,0,40,0,84,130,78,130,58,130,0,35,136,78
,0,27,130,78,70,138,78,84,25,131,0,70,133,0,68,0,25,0,27,84,130,0,35,113,70,0,35,131,0,68,132,0,84,40,131,0,70,0,26,40,130,0,68,0,26,45,0,84,0,25,130,0,70,130,0,58,130,0,68,130,0,58,0,84,130,0,35,113,70,0,35,136,0,84,132,0,70,130,0,25,135,0,84,132,0,70,99,0,35,53,0,53,0,53,0,53,84,78,70,141,78,84
,130,84,70,142,84,28,70,130,52,0,131,52,0,130,52,132,0,84,0,70,132,0,118,136,0,84,0,70,0,40,133,0,40,133,0,84,26,70,0,84,0,40,0,40,0,84,130,0,26,130,0,84,0,70,0,52,0,131,84,0,52,0,25,131,0,84,0,70,131,0,52,40,52,132,0,35,25,0,84,12,0,130,40,130,0,96,130,0,130,40,0,114,130,0,141,84,50,130,84
,144,84,142,52,27,84,26,68,0,122,130,0,40,132,0,40,130,0,26,84,0,68,0,92,35,26,58,35,131,0,58,0,25,130,0,25,68,134,0,40,99,40,130,0,26,0,40,0,68,84,135,58,84,131,0,40,84,0,68,139,0,119,130,84,0,68,130,0,114,40,41,114,133,0,134,84,130,50,130,84,134,50,131,84
,84,130,78,132,0,35,136,78,84,78,28,26,0,58,130,0,27,28,27,28,27,28,27,28,84,135,0,107,139,0,58,0,40,132,0,26,130,0,26,0,40,133,0,58,132,0,123,25,124,25,0,84,58,130,0,119,0,35,0,40,130,0,92,0,92,130,0,84,130,0,130,58,131,0,58,0,25,0,25,131,0,84,130,53,130,0,58,114,35,0,119,0,130,40,0,100,0,131,84,132,50,137,84
,78,70,141,78,84,27,70,27,28,27,28,27,28,27,84,132,0,27,84,0,70,131,0,25,131,0,84,26,25,117,26,0,84,0,70,131,0,39,131,0,84,133,0,84,0,70,0,26,0,58,0,26,112,73,131,40,42,40,84,0,70,130,0,25,0,25,40,58,84,133,63,84,0,70,132,0,40,58,0,84,133,50,84,132,0,130,40,58,130,0,84,133,51,145,84
,7,8,133,0,7,8,0,7,8,132,0,9,10,7,8,7,8,0,9,10,0,9,10,7,8,34,131,0,9,10,9,10,7,8,130,0,34,101,9,10,34,7,0,34,0,34,130,0,9,10,34,0,130,34,130,0,34,9,132,34,130,0,121,0,132,34,130,0,34,0,133,34,0,92,0,132,34,130,0,130,35,133,34,131,0,132,34,131,0,115,133,33,131,40,132,33,130,40,114,0,142,82,50,48
,0,7,8,0,7,8,81,137,82,0,9,10,0,9,10,132,24,110,131,24,130,86,8,40,102,40,0,40,133,24,40,24,41,130,86,10,133,63,80,131,24,81,132,82,83,134,0,86,131,24,86,131,24,130,86,131,0,63,130,0,86,130,24,81,83,132,24,78,34,132,0,63,86,24,81,82,83,132,24,72,33,111,0,111,130,0,86,81,130,82,83,24,97,24,130,86,82,83,69,81,136,82,83,69,130,86
,81,134,82,83,131,0,40,35,0,40,35,136,86,133,0,40,0,115,86,0,97,35,40,97,0,130,40,35,40,130,0,80,130,0,86,130,0,69,81,83,69,81,83,0,80,0,115,131,0,86,130,40,69,86,0,69,86,136,0,133,86,69,130,86,136,48,131,0,97,0,69,130,86,136,49,86,132,40,69,131,86,135,49,130,86,69,141,86
,0,40,35,0,35,134,0,35,0,35,40,132,0,40,136,0,40,131,0,40,130,0,80,0,131,40,43,130,40,0,80,130,0,40,80,0,115,0,115,81,132,82,83,115,0,115,0,80,144,0,144,48,160,49,144,86
,35,0,40,134,0,81,134,82,145,0,40,0,130,40,130,0,31,136,0,115,80,0,81,82,83,69,81,82,83,132,0,69,81,132,0,111,40,69,40,0,86,0,40,31,40,69,86,132,48,86,48,69,130,48,86,69,81,130,82,83,86,132,49,86,49,69,130,49,86,69,0,86,130,0,86,131,49,131,86,69,49,130,86,69,40,73,40,47,135,86,69,137,86
,137,82,83,137,0,135,40,136,0,69,135,80,0,34,130,0,34,0,82,83,69,132,80,0,69,80,0,34,130,0,34,0,130,86,69,130,0,69,130,80,69,80,0,34,130,0,34,0,130,86,69,130,80,69,130,80,69,80,0,34,130,0,34,0,130,86,69,130,80,69,130,80,69,80,0,34,130,0,34,0,130,86,69,130,80,69,130,0,69,80,0,33,40,31,33,40,130,86,69,135,80,81,133,82
,161,0,34,0,34,130,0,34,130,0,34,132,0,34,130,0,34,0,34,130,0,34,130,0,34,132,0,34,130,0,34,0,34,130,0,34,130,0,34,132,0,34,130,0,34,0,34,130,0,34,130,0,34,132,0,34,130,0,34,0,34,130,0,34,130,0,34,132,0,34,0,31,33,40,33,111,40,33,110,31,33,40,111,31,40,33,40,144,82
,135,0,56,130,55,56,0,131,80,137,0,120,131,0,131,80,131,0,34,133,0,120,131,0,131,80,131,0,34,137,0,131,80,131,0,34,137,0,131,80,131,0,34,137,0,131,80,131,0,34,137,0,131,80,0,31,40,33,130,0,31,0,54,130,55,54,40,0,57,130,0,130,82,83,130,50,80,50,137,80
,145,78,28,97,130,0,35,132,0,35,133,0,78,130,0,25,132,0,25,132,0,25,130,0,78,26,132,0,26,132,0,26,131,0,26,78,35,0,36,0,108,0,113,130,40,108,35,130,0,25,0,78,25,0,58,130,0,40,0,130,58,130,0,40,0,40,25,78,58,132,0,58,131,0,25,0,58,0,58,0,78,131,40,0,35,132,0,35,133,0,132,78,139,50,78
,130,78,69,138,78,69,130,78,130,0,69,137,0,113,69,27,78,130,0,69,131,0,35,0,35,131,0,40,69,0,78,26,0,69,130,0,26,131,0,26,40,0,63,0,26,78,130,0,69,131,0,108,25,108,0,63,132,0,78,130,0,69,0,25,0,40,0,40,131,0,40,130,0,78,130,0,69,131,0,63,0,63,130,0,25,63,130,0,78,0,97,69,140,0,134,78,133,50,78,132,50,78
,80,86,69,131,86,139,80,28,69,141,0,80,0,69,0,25,0,26,132,0,35,26,130,0,35,80,0,69,132,0,35,0,26,133,0,26,80,0,69,132,0,25,132,0,130,40,130,0,80,0,69,130,0,40,130,0,130,40,130,0,130,80,25,0,80,0,69,130,0,80,130,0,130,80,134,0,80,0,69,132,0,114,131,0,114,131,0,114,133,80,139,50
,144,80,145,0,25,137,0,25,137,0,26,135,0,26,133,0,37,132,0,26,41,134,0,40,130,0,80,130,0,40,130,0,80,0,40,130,0,25,0,80,133,0,80,25,131,0,80,130,0,80,25,143,0,40,143,50,80
,134,80,69,137,80,134,0,69,133,0,131,69,80,130,0,26,131,0,69,133,0,108,69,108,80,133,0,26,69,133,0,132,69,26,130,0,25,130,0,69,133,0,131,69,80,132,0,25,0,69,130,0,25,130,0,131,69,80,0,25,132,0,69,133,0,69,108,69,80,133,40,0,69,0,131,40,0,131,69,145,80
,130,80,69,142,80,28,69,26,140,0,80,0,69,131,0,26,137,0,131,69,0,25,133,0,26,133,0,80,0,69,130,0,25,135,0,26,130,0,80,0,69,136,0,25,132,0,80,0,69,137,0,25,131,0,80,0,69,141,0,131,80,141,50
,144,80,130,0,26,140,0,26,133,0,26,141,0,25,132,0,26,139,0,25,134,0,26,141,0,25,144,0,25,149,0,144,50
,144,80,142,0,27,80,130,0,26,140,0,80,25,142,0,80,0,25,132,0,26,136,0,80,137,0,26,133,0,80,135,0,25,133,0,80,0,80,136,0,25,134,0,80,138,50,134,80
,80,183,0,120,143,0,120,183,0,130,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,131,80,70,141,80
,130,80,70,142,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,130,80,0,70,140,0,145,80
};
