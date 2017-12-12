#include "globals.h"

// PLACED IN BANK 06
// zsdcc can only place code and rodata into different banks at the moment
// anything else use assembly language and sections

// Compressed MAP
//TODO A SINGLE array
// FIRST ONE BYTE FOR EACH SCREEN SIZE, THEN THE DATA, ALSO THE PAPER BACKGROUND
const unsigned char start_scr0 = 0;

const unsigned char lenght0[] = {71,85,85,76,91,93,83,81,97,75,87,49,22,21,20,78};
const unsigned char world0[] = {
 40,0,40,139,0,7,8,142,0,9,10,132,0,40,139,0,60,35,102,35,132,0,130,40,136,0,61,145,0,60,35,70,130,0,96,0,70,35,136,0,61,70,132,61,70,61,5,6,0,11,12,11,12,11,12,70,132,0,70,1,2,3,4,134,82,83,132,48,81,132,82
,141,0,7,8,136,0,40,130,0,121,0,40,9,10,139,0,92,137,0,35,11,12,111,11,12,104,11,12,36,11,12,132,0,70,81,138,82,131,40,0,70,135,0,110,135,0,70,0,40,130,0,40,130,0,40,5,6,40,0,11,12,0,70,96,131,0,110,130,0,1,2,3,4,134,82,83,130,48,82,130,48,82,130,48,81
,140,0,7,8,133,0,40,0,123,130,0,40,131,0,9,10,135,0,92,138,0,11,12,41,11,12,105,11,12,111,11,12,35,132,0,138,82,83,70,135,0,110,135,0,70,0,40,0,130,40,130,0,40,130,0,40,132,0,70,5,6,136,0,110,130,0,96,0,70,2,3,4,0,83,130,48,82,130,48,82,130,48,81,134,82
,134,0,137,19,135,0,137,84,40,132,0,130,40,84,135,52,84,40,134,0,84,0,108,133,0,84,40,132,0,11,12,72,130,0,130,40,0,108,11,84,12,0,40,0,70,136,84,70,131,84,131,0,70,84,131,0,108,0,108,0,70,132,0,11,12,11,84,12,46,130,0,108,134,0,144,82
,138,0,7,8,132,0,40,134,0,108,130,0,9,10,0,108,130,0,40,0,60,132,0,40,130,0,108,130,0,40,130,0,40,137,0,40,134,0,60,131,0,40,0,84,130,0,121,130,0,84,130,0,84,137,0,92,133,0,84,0,60,130,0,130,84,132,0,5,6,132,0,11,12,133,0,100,111,1,2,3,4,111,0,131,82,83,132,48,81,135,82
,137,0,7,8,132,0,84,137,0,9,10,40,130,0,40,84,134,0,40,130,0,40,133,0,75,131,0,40,134,0,11,12,0,11,130,84,134,0,11,12,0,11,81,133,82,130,0,120,11,12,11,81,130,82,83,52,39,132,52,130,0,92,81,82,83,40,132,0,5,6,131,0,11,12,0,99,0,99,0,11,12,98,1,2,3,4,130,0,140,82,83,131,50
,130,84,134,0,7,8,134,0,130,84,40,130,0,40,130,0,9,10,135,0,39,133,0,40,130,0,40,133,0,131,84,12,0,11,12,134,0,40,130,0,134,82,83,12,0,11,12,133,0,133,52,37,52,81,130,82,83,12,11,12,140,0,40,81,82,83,136,0,98,0,11,12,0,99,0,99,11,12,131,50,81,140,82
,135,0,7,8,142,0,9,10,140,0,35,130,0,102,0,35,136,0,130,40,0,132,61,0,130,40,138,0,108,40,137,0,40,0,98,40,0,40,108,0,40,0,99,40,131,0,70,132,60,70,130,60,70,132,60,70,130,0,70,132,0,70,130,0,70,132,0,70,0,82,83,132,48,81,130,82,83,132,48,81,82
,133,0,35,7,8,130,0,35,136,0,35,0,108,9,10,130,0,108,0,35,134,0,108,0,40,0,130,40,0,40,0,108,134,0,40,0,40,0,130,40,0,40,0,40,134,0,40,0,40,0,130,40,0,40,0,40,134,0,40,0,60,0,130,60,0,60,0,40,134,0,60,133,0,5,6,0,60,131,0,11,12,134,0,1,2,3,4,130,0,11,12,82,83,140,48,81,82
,131,0,40,0,7,8,140,0,40,0,9,10,0,40,131,0,40,134,0,40,143,0,71,130,0,112,35,11,12,102,11,12,134,0,71,81,139,82,131,0,71,81,82,83,140,0,71,81,82,83,0,40,5,6,40,130,0,108,132,0,71,81,82,83,12,1,2,3,4,47,44,130,0,144,82
,132,0,7,8,141,0,40,9,10,0,40,0,40,130,0,40,148,0,35,0,102,11,12,112,11,12,131,0,102,0,35,0,144,82,52,130,0,52,35,0,52,130,0,52,130,0,52,130,0,52,132,0,107,131,0,5,6,134,0,40,133,0,40,1,2,3,4,0,40,131,0,83,130,50,84,130,50,84,130,50,84,130,50,84,130,50,81
,131,0,7,8,142,0,9,10,149,0,37,135,0,11,12,112,11,12,137,0,135,82,83,140,0,131,77,141,0,131,77,0,5,6,135,0,11,12,0,131,77,1,2,3,4,133,0,144,82
,130,0,7,8,142,0,9,10,211,0,5,6,141,0,1,2,3,4,134,0,144,82
,0,7,8,142,0,9,10,212,0,5,6,141,0,1,2,3,4,134,0,144,82
,7,8,142,0,9,10,212,0,5,6,141,0,1,2,3,4,135,0,144,82
,136,0,21,143,0,20,131,0,21,139,0,20,131,0,20,136,0,84,70,0,20,130,0,131,19,134,0,130,84,70,0,20,130,0,17,13,18,133,0,131,84,70,0,20,0,19,131,16,19,131,0,132,84,70,0,20,0,17,13,14,13,18,130,0,133,84,70,0,20,0,17,13,76,13,18,0,144,82
};
