#include "globals.h"
#include <arch/zx.h>

// PLACED IN BANK 06
// zsdcc can only place code and rodata into different banks at the moment
// anything else use assembly language and sections

// Compressed MAP
//TODO A SINGLE array
// FIRST ONE BYTE FOR EACH SCREEN SIZE, THEN THE DATA, ALSO THE PAPER BACKGROUND


//THE SHIRE
const unsigned char world0_w = 16;
const unsigned char world0_h = 0;
const unsigned char start_scr0 = 0;
const unsigned char paper0[] = {PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLACK,PAPER_BLUE,PAPER_BLUE};
const unsigned char lenght0[] = {76,94,93,84,98,100,96,101,96,92,92,92,109,109,89,86};
const unsigned char world0[] = {
 40,0,40,139,0,7,8,139,0,7,8,0,9,10,132,0,40,134,0,9,10,131,0,60,0,60,133,0,130,40,154,0,60,35,70,130,0,96,0,70,35,134,0,32,0,61,70,132,61,70,61,5,6,130,0,130,30,33,130,30,70,132,0,70,1,2,3,4,134,82,83,132,48,81,132,82
,141,0,7,8,130,0,7,8,132,0,40,130,0,121,0,40,9,10,40,0,9,10,135,0,92,137,0,35,11,12,111,11,12,104,11,12,36,11,12,131,0,40,0,81,138,82,130,0,40,86,70,86,134,29,40,131,29,130,0,81,83,70,137,29,110,29,11,12,29,40,29,96,40,130,29,40,130,29,40,130,29,41,134,82,83,130,48,78,130,48,78,130,48,81
,140,0,7,8,130,0,40,125,0,40,0,123,130,0,40,131,0,9,10,131,0,93,131,0,92,138,0,11,12,42,11,12,105,11,12,111,11,12,35,132,0,138,82,83,0,40,131,0,131,29,40,134,29,86,70,86,40,130,0,29,110,137,29,70,81,83,130,0,131,29,40,130,29,40,130,29,96,132,29,11,12,83,130,48,78,130,48,78,130,48,81,134,82
,132,0,140,19,0,7,8,0,17,139,84,0,9,10,130,0,27,84,135,52,84,28,132,0,130,40,84,29,108,133,29,84,40,132,0,130,40,72,130,29,40,130,29,108,29,84,40,0,40,0,70,136,84,70,131,84,131,0,70,84,131,29,108,29,108,29,70,130,29,84,0,11,12,30,84,29,43,134,29,40,130,29,144,82
,19,137,0,7,8,132,0,18,131,0,7,8,131,0,35,9,10,35,133,0,60,0,9,10,0,40,133,0,40,130,0,40,136,0,108,40,0,108,44,130,0,40,60,131,0,40,0,84,130,0,121,130,0,84,130,0,84,136,0,35,92,0,35,131,0,84,0,60,130,0,130,84,132,0,5,6,131,0,29,11,12,134,0,53,1,2,3,4,53,0,131,82,83,132,48,81,135,82
,137,0,7,8,0,35,0,130,19,0,7,8,134,0,9,10,40,130,0,17,84,0,9,10,131,0,40,130,0,40,130,0,110,130,0,84,131,0,40,32,133,0,11,12,40,30,0,75,132,0,32,0,11,12,30,11,81,133,82,130,0,120,30,33,30,81,130,82,83,52,39,132,52,130,0,92,81,82,83,29,40,136,29,11,12,29,99,130,29,99,130,29,98,134,29,140,82,83,131,50
,130,19,130,0,35,131,0,7,8,134,0,84,18,131,0,40,130,0,9,10,134,0,84,130,0,110,130,0,40,131,0,40,135,0,30,40,11,12,133,0,32,0,40,130,0,134,82,12,30,11,12,0,32,132,0,132,52,37,52,81,130,82,83,30,33,30,121,130,0,136,29,40,29,81,82,83,92,130,0,134,29,98,130,29,99,29,99,29,0,11,12,131,50,81,140,82
,135,0,7,8,137,0,7,8,131,0,9,10,131,0,7,8,132,0,9,10,0,35,0,11,12,102,35,0,9,10,132,0,40,0,40,0,132,61,0,40,0,40,136,0,40,130,0,40,135,0,99,40,11,12,40,0,130,40,0,40,11,12,40,98,130,0,70,132,60,70,130,60,70,132,60,70,130,0,70,132,0,70,11,12,70,132,0,70,0,82,83,132,48,81,130,82,83,132,48,81,82
,136,0,35,135,0,7,8,130,0,35,0,40,7,8,40,0,35,0,7,8,0,9,10,130,0,108,130,40,9,10,130,40,108,0,9,10,132,0,40,0,40,130,0,108,0,40,0,40,134,0,40,130,0,60,0,35,60,130,0,40,136,0,60,132,0,60,136,0,60,35,132,0,5,6,35,60,131,0,11,12,134,0,1,2,3,4,130,0,11,12,82,83,140,48,81,82
,133,0,7,8,138,0,35,131,0,9,10,32,131,0,32,0,7,8,130,0,108,133,0,32,131,0,32,0,9,10,131,0,122,0,130,40,112,33,30,40,102,33,30,40,132,0,92,40,81,139,82,131,0,40,131,86,52,86,52,130,86,52,86,130,52,131,0,131,86,135,29,108,130,29,0,40,132,86,29,130,40,29,40,29,45,131,29,137,82,50,134,82
,132,0,7,8,138,0,7,8,0,40,9,10,0,40,0,40,32,0,40,32,130,0,9,10,136,0,32,130,0,32,131,0,35,0,102,11,12,35,11,12,35,33,30,102,33,30,35,144,82,52,130,29,86,130,29,52,130,29,52,130,29,52,134,29,108,140,29,40,133,29,40,133,29,40,131,29,83,130,50,84,130,50,84,130,50,84,130,50,84,130,50,81
,131,0,7,8,142,0,9,10,130,0,7,8,131,0,7,8,132,0,32,132,0,9,10,0,37,0,9,10,131,0,102,33,30,112,11,12,137,0,134,82,70,83,136,0,29,86,130,29,130,77,70,130,0,121,0,32,0,32,0,32,29,108,130,29,130,77,70,0,5,92,0,32,0,32,0,32,132,29,130,77,0,1,2,3,4,33,96,33,96,33,144,82
,130,0,7,8,134,0,35,0,35,131,19,130,0,9,10,0,7,8,134,0,17,130,84,133,0,9,10,40,63,130,0,63,0,17,84,14,132,0,35,136,0,47,130,84,130,0,35,132,0,61,133,0,61,130,84,0,34,108,32,108,34,131,0,34,108,32,108,34,130,84,0,34,0,32,0,34,0,61,0,34,0,32,0,34,130,84,0,33,40,33,40,33,131,0,33,40,33,40,33,130,29,133,82,83,131,50,81,134,82
,84,134,0,52,130,0,52,130,0,52,27,130,84,131,0,40,30,40,131,0,30,130,0,30,39,130,84,135,61,130,63,61,130,63,130,61,130,84,61,141,0,130,84,46,0,26,0,25,130,0,25,131,0,26,130,0,130,84,61,130,0,100,130,0,78,0,30,132,0,61,130,84,130,0,136,61,131,0,108,84,130,0,25,130,0,30,130,0,30,130,0,130,40,25,0,75,130,84,0,131,61,132,84,0,130,40,0,130,84
,130,19,136,0,35,56,130,55,56,0,84,18,130,0,7,8,134,0,116,131,0,14,18,130,0,9,10,131,0,60,108,0,116,131,0,84,18,142,0,84,0,123,135,0,60,133,0,84,0,92,0,34,139,0,84,131,0,34,0,5,6,0,60,108,133,0,29,130,0,96,33,1,2,3,4,130,0,54,130,55,54,0,134,82,83,130,50,135,82
,136,0,21,136,0,7,8,133,0,20,131,0,21,132,0,9,10,133,0,20,131,0,20,136,0,84,70,0,20,130,0,131,19,134,0,130,84,70,0,20,130,0,17,13,18,133,0,131,84,70,0,20,0,19,131,16,19,131,0,132,84,70,0,20,0,17,13,14,13,18,130,0,133,84,70,0,20,0,17,13,57,13,18,0,144,82
};

//MORIA
const unsigned char world1_w = 4;
const unsigned char world1_h = 4;
const unsigned char start_scr1 = 0;
const unsigned char paper1[] = {PAPER_BLUE,PAPER_BLACK,PAPER_BLACK,PAPER_BLUE,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK};
const unsigned char lenght1[] = {74,84,59,76,58,68,66,42,89,73,87,82,42,37,64,47};
const unsigned char world1[] = {
 143,0,81,131,0,7,8,137,0,81,80,130,0,40,9,10,40,0,7,8,132,0,81,130,80,135,0,9,10,131,0,81,131,80,0,40,60,130,0,60,133,0,81,132,80,136,0,36,0,81,133,80,0,60,136,0,52,0,110,0,110,80,135,0,35,0,104,0,35,131,0,72,144,82
,148,80,28,136,52,27,78,132,80,28,131,0,40,130,0,40,131,0,27,131,80,28,131,0,35,0,113,130,0,35,131,0,130,80,28,35,130,0,40,0,61,130,0,61,130,0,25,0,130,80,0,108,132,0,26,130,0,26,0,25,130,0,130,80,132,0,61,137,0,80,136,0,30,130,0,30,131,0,80,141,78,70,78,80
,145,80,78,28,131,0,56,130,55,56,0,27,133,80,28,133,0,116,132,0,27,132,80,134,0,116,133,0,27,131,80,140,0,35,27,130,80,140,0,110,0,130,80,143,0,80,133,0,54,130,55,54,134,0,80,78,70,141,78
,82,141,0,7,8,130,82,137,0,21,130,0,9,10,130,80,82,131,0,7,8,131,0,20,132,0,131,80,82,130,0,9,10,130,0,131,19,131,0,132,80,82,133,0,17,13,18,131,0,132,80,82,132,0,19,131,16,19,139,0,17,13,14,13,18,139,0,17,13,76,13,18,130,0,144,82
,145,78,28,140,0,27,130,78,131,0,130,78,137,0,130,78,0,26,133,0,26,130,0,96,131,0,130,78,137,0,131,82,130,0,130,78,142,0,130,78,131,0,25,0,25,137,0,78,136,0,96,134,0,130,78,70,141,78
,141,78,70,78,80,78,28,139,0,70,27,80,78,140,0,70,0,80,78,131,0,26,133,0,26,130,0,70,0,80,78,0,25,35,135,0,25,0,70,0,80,78,130,0,110,130,0,26,0,26,132,0,70,0,80,77,142,0,80,77,140,0,78,0,80,143,78,80
,80,78,70,141,78,80,28,70,141,0,80,35,70,0,25,130,0,35,135,0,25,80,0,70,135,0,25,133,0,80,107,133,0,106,136,0,80,131,0,60,139,0,80,130,0,130,60,137,0,25,0,80,133,0,25,137,0,80,135,0,132,78,70,78,130,0
,144,78,143,0,78,133,0,25,137,0,78,143,0,78,143,0,78,130,0,25,140,0,78,143,0,78,134,0,104,136,0,78,130,0,130,78,70,134,78,70,132,78
,130,80,70,142,80,0,70,139,0,68,78,80,0,70,139,0,68,78,80,0,70,130,0,102,136,0,68,0,80,0,70,0,65,66,67,131,0,103,131,0,68,0,80,0,70,130,0,23,131,0,65,66,67,131,0,78,80,0,70,130,0,22,132,0,23,132,0,78,80,0,70,130,0,22,132,0,22,130,0,130,53,78,81,83,70,81,139,82,83
,143,78,80,78,28,130,0,35,52,130,0,52,35,52,130,0,52,27,80,78,131,0,108,132,0,108,133,0,80,142,0,47,80,132,0,130,78,130,0,131,84,130,0,130,78,80,78,0,84,0,78,138,48,80,78,131,0,78,138,49,80,78,131,53,78,138,49,80,81,141,82,83,80
,80,35,130,60,131,0,35,132,78,70,78,130,0,80,139,0,70,131,0,80,132,0,130,60,133,0,70,0,25,0,80,107,0,60,136,0,70,131,0,80,132,0,25,131,0,26,0,26,70,131,0,80,0,25,137,0,70,131,0,80,131,0,130,60,0,106,132,0,70,131,0,80,139,0,70,131,0,80,130,78,130,60,130,0,35,136,78
,0,25,130,78,70,134,78,70,132,78,132,0,70,134,0,70,131,0,78,132,0,70,134,0,70,131,0,78,132,0,70,134,0,70,131,0,78,0,25,130,0,70,134,0,70,131,0,78,132,0,70,134,0,70,131,0,78,132,0,70,134,0,70,131,0,78,132,0,70,130,0,100,131,0,70,131,0,130,78,70,142,78
,130,78,70,142,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,134,0,100,136,0,144,78
,143,78,80,142,0,78,80,143,0,80,150,0,106,152,0,80,136,0,131,84,132,0,80,142,0,130,80,131,78,130,50,130,78,134,50,131,80
,80,130,78,133,0,136,78,80,78,28,26,0,130,60,137,0,80,28,134,0,106,139,0,60,145,0,130,60,130,0,27,28,132,0,80,60,142,0,80,130,0,130,60,0,26,0,35,0,25,133,0,80,130,53,139,0,100,0,131,80,132,50,137,78
,78,70,142,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,135,0,84,133,50,78,134,0,100,130,0,84,133,51,145,78
};
