#include "globals.h"

// PLACED IN BANK 06
// zsdcc can only place code and rodata into different banks at the moment
// anything else use assembly language and sections

// Compressed MAP
//TODO A SINGLE array
// FIRST ONE BYTE FOR EACH SCREEN SIZE, THEN THE DATA, ALSO THE PAPER BACKGROUND
const unsigned char start_scr0 = 0;
const unsigned char lenght0[] = {48,75,80,73,72,82,71,51,22,22,22,22,22,21,20,80};
const unsigned char world0[] = {
 142,0,7,8,133,0,107,136,0,9,10,161,0,120,121,122,0,61,139,0,131,92,146,0,70,132,61,70,0,40,5,6,134,0,70,132,0,70,106,1,2,3,135,84,132,48,133,84
,138,0,120,130,0,7,8,136,0,40,130,0,92,130,0,9,10,152,0,111,130,0,104,130,0,36,111,130,0,120,130,0,70,139,84,40,92,40,0,70,132,0,110,130,0,110,135,0,70,132,0,40,130,0,40,5,6,40,140,0,1,2,3,4,135,84,130,48,84,130,48,84,130,48,84
,131,0,120,136,0,7,8,133,0,92,130,0,40,133,0,9,10,147,0,111,0,105,130,0,111,137,0,137,84,70,130,0,120,134,0,110,130,0,110,130,0,70,0,40,92,40,130,0,40,130,0,40,130,0,40,130,0,70,130,0,5,6,141,0,1,2,3,4,0,84,130,48,84,130,48,84,130,48,135,84
,134,0,137,84,135,0,84,132,24,25,24,27,84,133,0,130,40,84,24,26,131,24,26,24,84,40,134,0,84,130,24,25,131,24,25,84,135,0,72,135,24,84,132,0,70,136,84,70,131,84,131,0,70,84,24,25,132,24,25,70,131,24,132,0,84,131,24,25,134,24,25,144,84
,138,0,7,8,139,0,108,130,0,9,10,0,108,130,0,40,134,0,40,130,0,40,130,0,40,0,37,138,0,108,138,0,40,0,84,130,0,84,130,0,84,130,0,84,143,0,24,132,0,84,133,0,5,6,131,0,24,136,0,111,1,2,3,4,111,0,132,84,132,48,136,84
,137,0,7,8,132,0,84,137,0,9,10,40,130,0,40,84,134,0,40,130,0,40,133,0,61,131,0,40,138,0,130,84,135,0,124,92,45,134,84,130,0,120,0,92,0,84,76,130,84,52,39,132,52,130,0,92,131,84,40,130,0,98,0,5,6,134,0,99,0,99,132,0,1,2,3,4,130,0,141,84,131,50
,130,84,134,0,7,8,134,0,130,84,40,0,40,130,0,40,9,10,135,0,39,135,0,40,130,0,40,131,0,133,84,138,0,40,137,84,45,0,92,132,0,135,52,39,52,132,84,130,0,92,137,0,98,5,6,40,131,84,137,0,1,2,3,4,99,0,99,133,50,139,84
,135,0,7,8,133,0,52,136,0,9,10,133,0,98,143,0,40,159,0,48,0,120,143,0,92,130,0,99,0,99,0,99,130,0,5,6,130,0,98,138,0,1,2,3,4,131,0,142,84,42,84
,134,0,7,8,142,0,9,10,209,0,5,6,141,0,1,2,3,4,132,0,144,84
,133,0,7,8,142,0,9,10,210,0,5,6,141,0,1,2,3,4,132,0,144,84
,132,0,7,8,142,0,9,10,210,0,5,6,141,0,1,2,3,4,133,0,144,84
,131,0,7,8,142,0,9,10,211,0,5,6,141,0,1,2,3,4,133,0,144,84
,130,0,7,8,142,0,9,10,211,0,5,6,141,0,1,2,3,4,134,0,144,84
,0,7,8,142,0,9,10,212,0,5,6,141,0,1,2,3,4,134,0,144,84
,7,8,142,0,9,10,212,0,5,6,141,0,1,2,3,4,135,0,144,84
,136,0,21,143,0,20,131,0,21,139,0,20,131,0,20,136,0,67,58,0,20,130,0,131,19,134,0,130,67,58,0,20,130,0,17,13,18,133,0,131,67,58,0,20,0,19,131,16,19,131,0,132,67,58,0,20,0,17,13,14,13,18,130,0,133,67,58,0,20,0,17,13,15,13,18,0,72,142,73,74
};
