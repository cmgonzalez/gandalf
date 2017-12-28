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
const unsigned char start_scr0 = 3;
const unsigned char paper0[] = {PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLUE,PAPER_BLACK,PAPER_BLUE,PAPER_BLUE};
const unsigned char lenght0[] = {67,82,82,73,90,93,83,81,88,75,88,57,103,109,81,78};
const unsigned char world0[] = {
 40,0,40,139,0,7,8,142,0,9,10,132,0,40,139,0,60,0,60,133,0,130,40,154,0,60,35,70,130,0,96,0,70,35,136,0,61,70,132,61,70,61,5,6,0,11,12,11,12,11,12,70,132,0,70,1,2,3,4,134,82,83,132,48,81,132,82
,141,0,7,8,136,0,40,130,0,121,0,40,9,10,139,0,92,137,0,35,11,12,111,11,12,104,11,12,36,11,12,132,0,70,81,138,82,131,40,0,70,143,0,70,0,40,130,0,40,130,0,40,5,6,40,0,11,12,0,70,96,131,0,110,130,0,1,2,3,4,134,82,83,130,48,78,130,48,78,130,48,81
,140,0,7,8,133,0,40,0,123,130,0,40,131,0,9,10,135,0,92,138,0,11,12,41,11,12,105,11,12,111,11,12,35,132,0,138,82,83,70,143,0,70,0,40,0,130,40,130,0,40,130,0,40,132,0,70,5,6,136,0,110,130,0,96,0,70,2,3,4,0,83,130,48,78,130,48,78,130,48,81,134,82
,134,0,137,19,135,0,137,84,40,132,0,130,40,84,135,52,84,40,134,0,84,0,108,133,0,84,40,132,0,11,12,72,130,0,130,40,0,108,11,84,12,0,40,0,70,136,84,70,131,84,131,0,70,84,131,0,108,0,108,0,70,132,0,11,12,11,84,12,46,137,0,144,82
,138,0,7,8,132,0,40,134,0,108,130,0,9,10,0,108,130,0,40,0,60,132,0,40,130,0,108,130,0,40,130,0,40,137,0,40,134,0,60,131,0,40,0,84,130,0,121,130,0,84,130,0,84,137,0,92,133,0,84,0,60,130,0,130,84,132,0,5,6,132,0,11,12,134,0,53,1,2,3,4,53,0,131,82,83,132,48,81,135,82
,137,0,7,8,132,0,84,137,0,9,10,40,130,0,40,84,134,0,40,130,0,40,133,0,75,131,0,40,134,0,11,12,0,11,130,84,134,0,11,12,0,11,81,133,82,130,0,120,11,12,11,81,130,82,83,52,39,132,52,130,0,92,81,82,83,40,132,0,5,6,131,0,11,12,0,99,0,99,0,11,12,98,1,2,3,4,130,0,140,82,83,131,50
,130,84,134,0,7,8,134,0,130,84,40,130,0,40,130,0,9,10,135,0,39,133,0,40,130,0,40,133,0,131,84,12,0,11,12,134,0,40,130,0,134,82,83,12,0,11,12,133,0,133,52,37,52,81,130,82,83,12,11,12,140,0,40,81,82,83,136,0,98,0,11,12,0,99,0,99,11,12,131,50,81,140,82
,135,0,7,8,142,0,9,10,140,0,35,130,0,102,0,35,136,0,130,40,0,132,61,0,130,40,138,0,130,40,137,0,40,0,98,40,0,130,40,0,40,0,99,40,131,0,70,132,60,70,130,60,70,132,60,70,130,0,70,132,0,70,130,0,70,132,0,70,0,82,83,132,48,81,130,82,83,132,48,81,82
,134,0,35,7,8,35,138,0,35,130,0,9,10,130,0,35,136,0,108,40,0,130,40,0,40,108,135,0,40,0,40,0,130,40,0,40,0,40,134,0,40,130,0,108,130,0,108,130,0,40,136,0,60,35,130,60,35,60,136,0,60,35,132,0,5,6,35,60,131,0,11,12,134,0,1,2,3,4,130,0,11,12,82,83,140,48,81,82
,131,0,40,0,7,8,140,0,40,0,9,10,0,40,131,0,40,134,0,40,143,0,71,130,0,112,35,11,12,102,11,12,134,0,71,81,139,82,131,0,71,81,82,83,140,0,71,81,82,83,0,40,5,6,40,130,0,108,132,0,71,81,82,83,12,1,2,3,4,47,44,130,0,144,82
,132,0,7,8,141,0,40,9,10,0,40,0,40,130,0,40,148,0,35,0,102,11,12,35,11,12,35,130,0,102,0,35,0,144,82,52,130,0,52,35,0,52,130,0,52,130,0,52,130,0,52,132,0,35,131,0,5,6,134,0,40,133,0,40,1,2,3,4,0,40,131,0,83,130,50,84,130,50,84,130,50,84,130,50,84,130,50,81
,131,0,7,8,142,0,9,10,149,0,37,134,0,102,11,12,112,11,12,137,0,134,82,70,83,140,0,130,77,70,141,0,130,77,70,0,5,6,135,0,11,12,0,130,77,0,1,2,3,4,0,96,0,96,0,144,82
,130,0,7,8,134,0,35,0,35,131,19,130,0,9,10,137,0,17,130,84,136,0,63,130,0,63,0,17,84,14,132,0,35,136,0,47,130,84,130,0,35,132,0,61,133,0,61,130,84,0,34,108,32,108,34,131,0,34,108,32,108,34,130,84,0,34,0,32,0,34,0,61,0,34,0,32,0,34,130,84,0,33,40,33,40,33,131,0,33,40,33,40,33,130,15,133,82,83,131,50,81,134,82
,84,134,0,52,130,0,52,130,0,52,27,130,84,131,0,40,30,40,131,0,30,130,0,30,39,130,84,135,61,130,63,61,130,63,130,61,130,84,61,141,0,130,84,43,0,26,0,25,130,0,25,131,0,26,130,0,130,84,61,130,0,100,130,0,78,0,30,132,0,61,130,84,130,0,136,61,131,0,108,84,130,0,25,130,0,30,130,0,30,130,0,130,40,25,0,75,130,84,0,131,61,132,84,0,130,40,0,130,84
,130,19,136,0,35,56,130,55,56,0,84,18,138,0,116,131,0,14,18,135,0,60,108,0,116,131,0,84,18,142,0,84,0,123,135,0,60,133,0,84,0,92,0,34,139,0,84,131,0,34,0,5,6,0,60,108,133,0,15,130,0,96,33,1,2,3,4,130,0,54,130,55,54,0,134,82,83,130,50,135,82
,136,0,21,143,0,20,131,0,21,139,0,20,131,0,20,136,0,84,70,0,20,130,0,131,19,134,0,130,84,70,0,20,130,0,17,13,18,133,0,131,84,70,0,20,0,19,131,16,19,131,0,132,84,70,0,20,0,17,13,14,13,18,130,0,133,84,70,0,20,0,17,13,76,13,18,0,144,82
};

//MORIA
const unsigned char world1_w = 4;
const unsigned char world1_h = 4;
const unsigned char start_scr1 = 0;
const unsigned char paper1[] = {PAPER_BLUE,PAPER_BLACK,PAPER_BLACK,PAPER_BLUE,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK};
const unsigned char lenght1[] = {45,38,36,60,49,50,41,27,60,36,30,43,56,28,25,38};
const unsigned char world1[] = {
 131,0,82,139,0,82,130,0,130,82,138,0,130,82,131,0,82,137,0,131,82,130,0,131,82,135,0,132,82,139,0,133,82,138,0,134,82,160,0,132,82,130,0,138,82
,133,82,138,0,78,132,82,139,0,78,131,82,140,0,78,130,82,141,0,78,82,142,0,78,82,0,110,173,0,131,78,130,0,136,78,70,130,78
,78,138,0,133,82,78,139,0,132,82,78,140,0,131,82,78,141,0,130,82,78,142,0,82,141,0,110,0,82,160,0,130,78,70,141,78
,82,143,0,130,82,137,0,21,132,0,131,82,136,0,20,132,0,132,82,134,0,131,19,131,0,133,82,133,0,17,13,18,131,0,133,82,132,0,19,131,16,19,139,0,17,13,14,13,18,139,0,17,13,76,13,18,130,0,144,82
,132,78,130,0,139,78,143,0,78,131,0,131,82,137,0,78,133,0,82,137,0,78,131,0,130,82,138,0,78,131,0,82,139,0,78,131,0,131,82,137,0,78,143,0,130,78,70,141,78
,131,78,0,137,78,70,130,78,141,0,70,0,78,141,0,70,0,78,141,0,70,0,78,141,0,70,0,78,131,0,110,137,0,70,0,78,143,0,78,141,0,78,0,130,78,70,78,0,140,78
,130,78,70,142,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,143,0,78,0,78,141,0,78,70,142,78
,144,78,143,0,78,143,0,78,143,0,78,143,0,78,143,0,78,143,0,78,143,0,130,78,70,142,78
,130,78,70,142,78,0,70,141,0,78,0,70,0,131,82,137,0,78,0,70,131,0,82,137,0,78,0,70,130,0,130,82,137,0,78,0,70,131,0,82,137,0,78,0,70,0,131,82,137,0,78,0,70,141,0,130,78,70,141,78
,78,70,130,78,0,139,78,0,70,143,0,70,143,0,70,143,0,70,143,0,70,143,0,70,143,0,70,142,0,78,70,130,78,0,139,78
,78,70,142,78,0,70,143,0,70,143,0,70,143,0,70,143,0,70,143,0,70,143,0,70,142,0,78,70,142,78
,78,70,142,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,130,78,70,142,78
,130,78,70,142,78,0,70,141,0,78,0,70,0,82,0,82,137,0,78,0,70,0,82,0,82,137,0,78,0,70,0,132,82,136,0,78,0,70,131,0,82,137,0,78,0,70,131,0,82,137,0,78,143,0,144,78
,78,70,142,78,0,70,143,0,70,143,0,70,143,0,70,143,0,70,143,0,70,158,0,133,78,0,138,78
,78,70,142,78,0,70,143,0,70,143,0,70,143,0,70,143,0,70,143,0,70,158,0,144,78
,78,70,142,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,0,70,141,0,78,143,0,145,78
};
