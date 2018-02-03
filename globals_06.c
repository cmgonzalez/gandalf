#include "globals.h"
#include <arch/zx.h>
/* This Data is placed on Bank #6
/ zsdcc can only place code and rodata into different banks at the moment
/ anything else use assembly language and sections
/ Compressed MAP Data for game paging
*/
const unsigned char world0_w[]   = {16, 4, 8,  1};
const unsigned char world0_h[]   = { 1, 4, 2, 16};
const unsigned char start_scr0[] = {0,  0, 0, 15};
const unsigned char paper0[] = {
PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLACK,PAPER_BLUE ,PAPER_BLUE ,
PAPER_BLUE ,PAPER_BLACK,PAPER_BLACK,PAPER_BLUE ,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,
PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,
PAPER_BLUE ,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_RED  ,PAPER_RED  ,PAPER_RED  ,PAPER_RED  ,PAPER_RED  ,PAPER_RED
};
const unsigned char lenght0[] = {
83,95,97,83,94,99,96,101,98,96,93,91,113,111,88,93
,94,93,77,96,92,80,92,106,98,88,108,102,98,95,107,98
,103,111,92,54,106,95,81,101,96,103,106,93,99,94,95,97
,102,90,106,127,107,97,103,91,102,93,106,89,105,107,74,58

};
const unsigned char world0[] = {
 142,0,7,8,0,7,8,139,0,9,10,0,9,10,131,0,7,8,118,141,0,9,10,132,0,131,40,130,0,40,138,0,131,60,130,0,84,77,130,0,35,70,40,0,96,40,70,35,134,0,32,77,61,70,132,61,70,61,5,6,130,0,130,30,33,77,30,70,132,0,70,1,2,3,4,134,82,83,132,48,81,132,82
,135,0,40,130,0,121,0,40,7,8,130,0,7,8,132,0,84,130,0,90,0,84,9,10,130,0,9,10,130,0,58,142,0,35,11,12,111,11,12,96,11,12,36,11,12,131,0,40,0,81,138,82,130,0,40,80,70,86,134,29,40,29,118,29,130,0,81,86,70,139,29,11,12,131,29,96,131,29,40,130,29,40,130,29,41,134,82,83,130,48,78,48,81,132,82
,131,0,40,0,124,130,0,40,131,0,7,8,133,0,84,0,90,130,0,84,131,0,9,10,140,0,58,130,0,126,130,0,11,12,0,11,12,96,11,12,111,11,12,35,0,93,130,0,138,82,83,0,40,131,0,29,118,29,40,134,29,86,70,80,40,130,0,139,29,70,86,83,130,0,131,29,40,130,29,40,130,29,96,132,29,11,12,132,82,83,48,78,130,48,81,134,82
,132,0,140,19,0,7,8,0,17,139,84,0,9,10,130,0,27,84,135,52,84,28,132,0,130,40,84,135,29,84,40,0,40,130,0,130,40,72,29,35,29,113,131,29,84,40,0,84,0,70,136,84,70,131,84,131,0,70,84,131,29,108,29,108,29,70,130,29,84,0,21,0,30,84,29,43,134,29,40,130,29,144,82
,19,134,0,7,8,135,0,18,131,0,7,8,0,9,10,35,0,35,135,0,40,9,10,131,0,108,134,0,40,130,0,58,134,0,44,133,0,40,58,133,0,58,130,0,58,108,132,0,84,0,40,130,0,130,40,130,0,35,122,35,132,0,84,0,58,0,132,58,130,0,90,5,6,131,0,29,11,12,135,0,1,2,3,4,53,0,131,82,83,132,48,81,135,82
,137,0,7,8,0,35,0,130,19,0,7,8,134,0,9,10,40,130,0,17,84,0,9,10,131,0,40,130,0,40,130,0,110,130,0,84,131,0,40,32,133,0,11,12,40,30,0,75,132,0,32,0,11,12,30,11,81,133,82,130,0,123,30,33,30,81,130,82,83,52,39,52,29,130,52,130,0,90,81,82,83,29,40,136,29,11,12,132,29,99,29,98,135,29,140,82,83,131,48
,130,19,130,0,35,131,0,7,8,134,0,84,18,131,0,40,130,0,9,10,134,0,84,130,0,110,130,0,40,131,0,40,135,0,30,40,11,12,133,0,32,0,40,130,0,134,82,12,30,11,12,0,32,132,0,130,52,29,52,29,52,81,130,82,83,30,33,30,122,130,0,136,29,40,29,81,82,83,90,130,0,135,29,99,29,98,131,29,0,11,12,131,48,81,140,82
,135,0,7,8,137,0,7,8,131,0,9,10,131,0,7,8,132,0,9,10,0,35,0,11,12,102,35,0,9,10,132,0,40,0,40,0,132,61,0,40,0,40,136,0,40,130,0,40,135,0,99,40,11,12,40,0,130,40,0,40,11,12,40,98,130,0,70,132,60,70,130,60,70,132,60,70,0,21,70,132,0,70,11,12,70,132,0,70,0,82,83,132,48,81,130,82,83,132,48,81,82
,136,0,35,135,0,7,8,130,0,35,0,40,130,0,40,0,35,132,0,9,10,130,0,108,130,40,130,0,130,40,108,130,0,7,8,131,0,40,0,40,130,0,108,0,40,0,40,0,9,10,131,0,40,130,0,60,0,35,60,130,0,40,136,0,60,132,0,60,136,0,60,35,132,0,5,6,35,60,131,0,11,12,134,0,1,2,3,4,130,0,11,12,82,83,140,48,81,82
,133,0,7,8,142,0,9,10,32,131,0,32,0,7,8,130,0,108,130,0,130,40,0,32,131,0,32,0,9,10,131,0,123,0,130,40,112,33,30,40,102,33,30,40,132,0,90,0,81,139,82,131,0,11,131,86,52,86,52,130,86,52,86,52,86,130,0,11,131,86,135,29,108,29,86,40,21,132,86,29,130,40,29,40,29,45,29,42,40,136,82,83,48,81,133,82
,132,0,7,8,138,0,7,8,34,40,9,10,0,40,0,40,130,0,40,34,130,0,9,10,34,138,0,34,131,0,35,33,100,11,12,35,11,12,35,0,30,100,33,30,35,144,82,86,52,29,86,130,29,52,130,29,52,130,29,52,131,29,86,130,29,108,140,29,73,133,29,40,133,29,40,131,29,82,83,48,86,130,48,86,130,48,86,130,48,86,130,48,81
,131,0,7,8,140,0,123,0,9,10,130,0,7,8,131,0,7,8,131,0,90,32,132,0,9,10,0,37,0,9,10,132,0,33,30,112,11,12,137,0,133,82,83,70,80,136,0,29,86,130,29,130,77,70,130,0,122,134,0,29,108,130,29,130,77,70,0,5,90,0,32,0,32,0,32,132,29,130,77,21,1,2,3,4,33,96,33,96,33,144,82
,130,0,7,8,134,0,35,0,35,131,19,130,0,9,10,0,7,8,134,0,17,130,84,133,0,9,10,0,63,130,0,63,0,17,84,14,132,0,35,130,0,40,133,0,47,130,84,130,0,35,132,0,58,132,0,130,61,130,84,0,34,108,32,108,34,131,0,34,108,32,108,34,130,84,0,34,0,32,0,34,0,58,0,34,0,32,0,34,130,84,0,33,40,33,40,33,131,0,33,40,33,40,33,130,29,133,82,83,131,48,81,134,82
,84,134,24,52,130,24,52,130,24,52,27,130,84,131,24,40,30,40,131,24,30,130,24,30,38,130,84,130,24,133,61,130,63,61,130,63,130,61,130,84,24,130,61,139,24,130,84,46,24,26,24,25,130,24,25,131,24,26,130,24,130,84,61,130,24,100,130,24,78,24,30,132,24,61,130,84,130,24,136,61,131,24,108,84,130,24,25,130,24,30,130,24,30,132,24,25,24,74,130,84,24,131,61,132,84,24,130,40,24,130,84
,130,19,0,7,8,0,56,130,55,56,134,0,84,18,0,9,10,139,0,14,18,130,0,130,61,132,0,130,61,130,0,7,8,84,18,133,0,120,134,0,9,10,84,0,123,130,0,61,0,120,130,0,61,133,0,84,0,90,141,0,84,132,0,61,132,0,61,130,0,5,6,0,21,131,0,35,130,0,96,131,0,35,1,2,3,4,144,82
,136,0,21,136,0,7,8,133,0,20,131,0,21,132,0,9,10,130,0,40,130,0,20,131,0,20,135,0,40,84,70,0,20,130,0,131,19,133,0,40,130,84,70,0,20,130,0,17,13,18,132,0,40,131,84,70,0,20,0,19,131,16,19,130,0,40,132,84,70,0,20,0,17,13,14,13,18,130,0,133,84,70,0,20,0,17,13,57,13,18,0,144,82
,142,0,11,81,0,7,8,138,0,11,81,86,0,9,10,122,131,0,7,8,131,0,11,81,130,86,131,0,90,131,0,9,10,130,0,11,81,130,86,84,138,0,11,81,130,86,130,84,0,40,0,40,0,40,130,0,36,0,81,130,86,131,84,0,60,0,60,0,60,132,0,52,110,131,29,84,0,21,133,0,105,130,0,133,29,72,131,82,83,131,48,81,136,82
,148,84,28,131,52,130,0,131,52,27,78,132,84,28,0,35,0,40,130,0,40,131,0,27,131,84,28,131,0,35,40,113,0,40,35,131,0,130,84,28,35,26,0,108,0,58,130,0,58,0,26,25,0,130,84,0,108,130,0,40,0,26,130,0,26,0,25,130,0,84,133,0,58,136,0,112,84,131,0,40,132,0,30,40,0,30,0,98,143,84,70,130,84
,148,84,28,0,56,130,55,56,0,27,135,84,28,131,0,120,132,0,27,133,84,28,26,131,0,120,131,0,26,0,27,131,84,28,0,124,0,58,132,0,58,0,123,0,27,130,84,130,0,90,136,0,90,130,0,130,84,132,0,58,132,0,58,132,0,130,84,0,21,140,0,29,130,84,70,141,78
,82,12,140,0,7,8,86,82,12,136,0,21,130,0,9,10,130,86,82,12,130,0,7,8,131,0,20,132,0,84,130,86,82,12,0,9,10,130,0,131,19,131,0,130,84,130,86,82,133,0,17,13,18,131,0,130,84,130,86,82,132,0,19,131,16,19,130,0,84,132,29,0,131,40,17,13,14,13,18,130,0,133,29,0,131,40,17,13,57,13,18,130,0,143,82,84
,145,84,28,136,0,132,77,27,130,84,131,0,26,130,0,96,40,97,0,130,77,130,0,130,84,26,35,132,0,131,40,0,77,0,40,26,130,84,0,108,121,131,0,131,85,0,77,25,77,40,130,84,0,25,90,131,0,131,52,0,77,40,77,131,84,131,0,25,0,25,132,0,133,77,84,12,35,130,0,35,133,0,35,0,21,130,0,130,84,70,141,84
,141,84,70,131,84,28,139,52,70,27,130,84,26,139,0,70,26,130,84,131,0,26,130,0,43,130,0,26,130,0,70,0,130,84,40,25,35,130,40,0,58,0,130,40,25,35,70,0,131,84,0,110,130,58,0,26,0,130,58,0,110,70,0,84,77,142,0,84,134,0,99,131,0,40,130,0,78,0,145,84
,130,84,70,142,84,28,70,141,52,84,35,70,132,0,35,135,0,25,84,113,70,135,0,25,0,25,131,0,84,106,70,131,0,25,130,0,125,131,0,124,130,0,84,130,0,130,40,132,0,93,131,0,90,130,0,84,130,0,130,63,135,0,25,0,25,0,84,35,132,0,25,130,0,112,99,0,21,130,0,40,84,131,0,130,58,131,0,131,63,70,131,63
,144,84,130,0,52,0,52,0,27,84,28,133,0,27,84,0,25,131,0,25,0,84,135,0,84,130,0,40,0,44,0,40,84,0,40,0,108,40,130,0,84,130,0,58,0,58,0,58,84,0,59,130,0,59,0,25,84,40,130,0,26,0,26,0,84,131,0,40,131,0,130,84,134,0,84,25,130,0,59,131,0,84,0,40,0,40,130,0,104,74,0,40,0,40,0,99,37,84,130,63,130,78,70,138,78,84
,130,84,70,142,84,28,70,135,0,121,130,0,52,68,130,84,0,70,135,0,90,131,0,68,130,84,0,70,130,0,102,136,0,68,130,84,0,70,0,65,66,67,131,0,103,131,0,68,75,84,0,70,130,0,23,131,0,65,66,67,130,0,68,130,84,0,70,130,0,22,132,0,23,131,0,68,130,84,12,70,0,40,22,132,40,22,40,0,53,68,130,84,80,70,81,139,82,83
,145,84,28,130,0,35,52,130,0,52,35,52,130,0,52,27,130,84,131,0,108,40,130,0,40,108,40,130,0,130,40,130,84,131,0,130,40,130,0,40,43,40,130,0,40,47,84,0,131,40,130,78,130,0,131,78,130,0,130,78,130,84,58,130,63,84,138,48,130,84,131,0,84,138,49,130,84,131,53,84,138,49,84,81,141,82,83,84
,84,35,40,132,0,35,132,0,70,0,27,0,84,0,130,58,130,0,40,133,0,70,131,0,84,132,0,130,58,133,0,70,0,25,0,84,130,0,58,134,0,40,0,70,0,40,0,84,130,0,58,0,25,0,25,0,26,58,26,70,0,58,0,84,0,25,130,0,40,134,0,70,131,0,84,131,0,130,58,0,107,132,0,70,131,0,84,38,134,0,112,0,40,0,70,0,40,0,84,130,78,130,58,130,0,35,136,78
,0,27,130,78,70,138,78,84,25,131,0,70,133,0,68,0,25,0,27,84,130,0,35,113,70,0,35,131,0,68,132,0,84,40,131,0,70,0,26,40,130,0,68,0,26,45,0,84,0,25,130,0,70,130,0,58,130,0,68,130,0,58,0,84,130,0,35,113,70,0,35,136,0,84,132,0,70,130,0,25,135,0,84,132,0,70,99,0,35,53,0,53,0,53,0,53,84,78,70,141,78,84
,130,84,70,142,84,28,70,130,52,0,131,52,0,130,52,132,0,84,0,70,132,0,118,136,0,84,0,70,0,40,133,0,40,133,0,84,26,70,0,84,0,131,40,0,84,130,0,26,130,0,84,0,70,0,52,0,131,84,0,52,0,25,131,0,84,0,70,131,0,52,40,52,132,0,35,25,0,84,12,0,130,40,130,0,96,130,0,130,40,0,114,130,0,141,84,50,130,84
,144,84,0,68,0,130,52,0,52,0,130,52,132,0,27,84,26,68,0,122,130,0,40,132,0,40,130,0,26,84,0,68,0,90,35,26,58,35,131,0,58,0,25,130,0,25,68,132,0,99,0,130,40,131,0,26,0,40,0,68,84,134,58,84,131,0,119,40,84,0,68,138,0,132,84,0,68,21,0,114,40,41,114,133,0,134,84,130,50,130,84,134,50,131,84
,84,130,78,28,131,0,35,136,78,84,78,28,26,0,58,130,0,27,28,27,28,27,28,27,28,84,28,134,0,35,139,0,58,0,40,132,0,26,130,0,26,0,40,133,0,58,0,107,130,0,123,25,124,25,0,84,58,130,0,119,0,35,0,40,130,0,90,0,90,130,0,84,130,0,130,58,131,0,58,0,25,0,25,131,0,84,130,53,130,0,58,114,35,0,119,0,130,40,0,100,0,131,84,132,50,137,84
,78,70,141,78,84,27,70,27,28,27,28,27,28,27,84,28,131,0,27,84,0,70,131,0,25,131,0,84,26,25,117,26,0,84,0,70,131,0,39,131,0,84,133,0,84,0,70,0,26,0,58,0,26,112,73,131,40,42,40,84,0,70,130,0,25,0,25,40,58,84,133,63,84,0,70,132,0,40,58,0,84,133,50,84,0,21,130,0,130,40,58,130,0,84,133,51,145,84
,7,8,130,0,7,8,0,7,8,0,7,8,132,0,9,10,7,8,9,10,116,9,10,0,9,10,7,8,132,0,9,10,133,0,130,34,0,9,10,0,7,131,0,34,0,35,0,35,132,34,131,0,9,130,0,130,34,130,0,122,0,132,34,130,0,35,131,0,131,34,0,90,0,132,34,130,0,108,131,0,131,34,108,0,108,132,34,133,0,21,131,33,40,0,40,132,33,130,40,130,0,144,82
,0,7,8,0,7,8,81,137,82,0,9,10,0,9,10,0,131,24,110,131,24,130,86,8,40,102,40,0,40,0,132,24,40,24,41,130,86,10,133,63,81,136,82,83,131,0,123,130,0,86,131,24,86,131,24,130,86,131,0,90,130,0,86,130,24,81,83,24,122,130,24,86,133,0,80,86,24,81,82,83,24,90,130,24,72,11,12,0,11,12,30,86,81,130,82,83,24,96,24,130,86,82,83,69,81,136,82,83,69,130,86
,81,134,82,83,131,0,40,35,0,40,35,136,86,133,0,40,0,115,86,0,96,35,40,96,0,40,21,35,40,130,0,59,130,0,86,130,0,69,81,83,69,81,83,0,59,0,115,131,0,86,130,40,69,86,40,69,86,136,0,133,86,69,130,86,136,48,132,0,96,69,130,86,136,49,86,40,130,0,40,69,131,86,135,49,130,86,69,134,86,81,134,82
,0,40,35,0,35,134,0,35,0,35,40,132,0,40,136,0,40,131,0,40,130,0,59,0,131,40,43,130,40,0,59,130,0,40,59,0,115,0,115,134,59,115,0,115,0,59,144,0,144,48,160,49,144,82
,35,0,40,133,0,52,132,24,131,77,137,0,133,24,130,77,0,40,0,21,130,40,105,31,0,130,24,110,131,24,77,115,59,0,131,85,130,69,130,85,131,24,110,69,85,132,0,111,40,130,69,0,88,24,40,31,40,69,88,132,48,88,48,130,69,48,88,133,85,88,132,49,88,49,130,69,48,130,88,0,88,130,0,88,131,49,131,88,130,69,131,88,40,73,40,46,88,130,82,83,131,88,130,69,136,88
,131,77,135,24,52,133,0,130,77,136,24,71,133,0,77,134,24,102,130,24,71,34,130,0,34,0,85,69,136,85,71,34,130,0,34,0,88,69,0,110,132,88,110,88,71,34,130,0,34,0,88,69,88,0,132,52,39,88,71,34,0,122,34,0,88,69,88,134,69,88,71,34,0,90,34,0,88,69,88,134,53,88,71,33,40,31,33,35,88,69,136,88,81,133,82
,116,138,0,56,130,55,56,141,0,120,132,0,34,0,34,130,0,34,131,0,59,0,120,132,0,34,0,34,130,0,34,0,59,136,0,34,0,34,130,0,34,138,0,34,0,34,130,0,34,0,123,0,59,134,0,34,0,34,130,0,34,0,90,135,0,35,33,21,33,130,40,33,132,35,54,130,55,54,35,144,82
,136,0,19,133,16,19,138,0,13,14,13,14,13,132,0,34,130,0,34,0,34,0,133,13,132,0,34,130,0,34,0,34,0,130,13,14,130,13,132,0,34,130,0,34,0,34,0,133,13,132,0,34,130,0,34,0,34,19,133,16,19,131,0,34,130,0,34,0,34,0,130,13,14,130,13,130,0,130,35,33,130,35,33,31,33,31,130,13,57,130,13,130,31,130,82,83,130,50,81,138,82
,145,78,28,131,0,35,132,0,35,133,0,78,130,0,25,132,0,25,132,0,25,130,0,78,26,106,131,0,26,132,0,26,131,0,26,78,35,0,36,0,108,130,0,130,40,108,35,130,0,25,0,78,25,0,58,130,0,40,0,130,58,130,0,40,0,40,25,78,58,132,0,58,131,0,25,0,58,0,58,0,78,131,40,0,35,132,0,35,132,0,21,132,78,139,50,78
,130,78,69,138,78,69,130,78,130,0,69,137,0,113,69,27,78,130,0,69,131,0,35,0,35,131,0,40,69,0,78,26,0,69,130,0,26,131,0,26,40,0,131,58,78,130,0,69,131,0,108,25,108,0,80,132,0,78,130,0,69,0,25,0,40,0,40,131,0,40,130,0,78,130,0,69,131,0,63,0,63,130,0,25,63,130,0,78,130,0,69,0,119,138,0,134,78,133,50,78,132,50,78
,130,85,69,141,85,88,28,69,0,52,130,0,52,130,0,52,130,0,52,130,0,88,0,69,0,25,0,26,130,0,25,0,35,26,130,0,35,88,0,69,132,0,35,0,26,133,0,26,88,0,69,132,0,25,132,0,130,40,130,0,88,0,69,130,0,40,130,0,130,40,130,0,130,59,130,0,88,0,69,130,0,59,130,0,130,59,134,0,88,0,69,21,119,130,0,114,131,0,114,131,0,114,133,85,139,50
,144,85,52,0,52,0,123,122,0,52,0,52,130,0,52,130,0,52,0,25,35,0,130,90,0,25,35,130,0,25,134,0,26,133,0,26,0,35,130,0,26,133,0,130,40,37,40,130,0,42,131,0,35,130,0,40,130,0,132,59,130,0,59,0,40,130,0,25,0,59,138,0,59,130,0,59,25,130,0,114,133,0,114,0,114,130,0,114,0,119,143,50,85
,134,85,130,69,136,85,133,0,125,130,69,108,52,0,52,0,52,108,88,130,0,26,130,0,93,130,69,130,40,130,69,130,40,69,88,26,131,0,26,0,69,40,131,69,40,132,69,131,0,25,130,0,69,40,131,69,40,130,69,40,69,132,0,25,0,130,69,130,40,130,69,130,40,69,88,0,25,133,0,136,69,88,0,31,21,31,0,31,131,0,53,108,53,108,53,11,88,144,85
,85,69,88,141,85,88,69,134,52,130,0,25,0,118,0,118,0,88,69,40,131,69,40,130,69,132,0,26,130,0,69,40,69,40,69,40,69,40,130,69,0,25,132,0,69,131,40,69,131,40,69,135,85,88,40,69,40,69,40,69,40,130,69,134,0,88,136,69,0,35,130,0,35,25,0,88,135,53,119,0,114,130,0,114,0,31,138,85,50,130,85,50,130,85
,144,85,52,0,52,0,52,0,52,25,52,0,52,0,52,0,52,130,0,26,131,0,26,131,0,26,131,0,26,133,0,25,131,0,96,97,0,31,25,131,0,31,144,85,130,0,35,131,0,35,130,0,35,130,0,35,134,0,40,119,40,0,25,130,0,130,40,131,0,25,31,21,114,131,59,114,130,0,114,130,59,114,130,0,31,130,85,133,50,130,85,132,50,131,85
,144,85,52,0,52,0,52,0,52,0,52,0,27,88,131,24,88,0,26,0,25,0,26,131,0,26,0,88,24,26,24,88,132,0,96,0,96,0,96,0,96,75,44,38,24,88,144,85,0,123,132,0,35,130,0,35,27,88,131,24,88,0,90,25,132,0,130,40,130,0,88,24,26,24,88,0,21,0,31,130,0,114,130,59,114,0,74,45,47,24,88,134,85,132,50,134,85
,130,0,7,8,0,134,19,0,7,8,130,0,7,8,9,10,0,17,132,13,18,0,9,10,7,8,9,10,131,0,17,14,130,13,14,18,131,0,9,10,133,0,17,132,13,18,138,0,17,132,13,18,133,0,32,0,32,0,19,134,16,19,0,32,0,130,32,0,32,0,17,130,13,130,14,130,13,18,0,32,0,32,33,30,33,30,17,130,13,130,15,130,13,18,30,33,76,33,81,142,82,83
,0,7,8,131,0,16,0,117,16,131,0,7,8,130,0,9,10,131,0,16,130,0,16,131,0,9,10,135,0,16,130,0,16,140,0,16,120,0,16,135,0,124,131,0,58,16,120,0,16,58,131,0,124,130,0,90,131,0,119,16,130,0,16,119,131,0,90,134,0,58,16,130,0,16,58,133,0,130,16,21,138,16,0,130,16,130,84,68,141,84
,130,78,68,141,78,84,24,68,122,24,68,24,68,24,68,131,24,122,24,130,84,24,68,90,24,68,24,68,24,68,131,24,90,24,130,84,24,68,130,24,68,24,68,24,68,133,24,130,84,24,68,130,24,68,24,68,24,68,24,40,24,40,24,131,84,68,142,84,52,68,130,52,68,52,68,52,68,133,52,130,84,0,68,130,0,68,0,68,0,68,133,0,130,84,0,68,130,0,68,0,68,0,68,133,0,84
,84,0,68,130,0,68,0,68,0,68,132,0,40,130,84,0,68,130,0,68,0,68,0,68,130,0,130,40,77,130,84,106,68,130,0,68,0,68,0,68,0,40,131,77,130,84,0,68,0,25,68,0,113,0,68,40,132,77,130,84,0,40,130,0,68,131,0,68,77,134,84,0,40,130,0,68,131,0,68,77,84,28,40,27,130,84,0,40,0,26,68,25,130,0,68,77,84,131,40,130,84,0,40,130,0,68,130,0,21,68,77,75,45,40,46,130,84,0,40,132,0,40,77,68,77,133,84
,84,0,39,132,0,130,77,68,77,134,84,0,40,35,130,0,131,84,68,135,84,0,40,131,0,68,130,0,68,131,0,68,0,130,84,0,40,131,0,68,130,0,68,131,0,68,0,130,84,0,40,131,0,68,130,0,68,130,0,25,68,0,130,84,0,40,130,0,99,0,98,0,68,133,0,130,84,0,40,0,68,131,58,0,26,131,0,26,0,130,84,0,40,0,68,138,0,130,84,25,40,0,68,137,0,107,84
,84,131,0,68,136,0,114,37,130,84,0,40,0,68,21,132,0,114,132,0,130,84,0,62,0,68,84,58,114,59,130,0,59,130,0,59,130,84,131,0,68,84,137,50,130,84,130,0,25,68,84,137,51,130,84,131,0,68,140,84,131,53,68,136,0,68,27,131,84,130,58,68,0,40,130,0,130,40,130,0,68,0,130,84,28,130,0,68,0,62,0,132,58,25,68,0,84
,84,106,126,0,68,136,0,68,0,130,84,0,93,132,0,25,132,0,40,0,104,130,84,131,0,25,0,113,130,0,130,40,0,131,58,130,84,133,0,130,40,0,130,58,131,0,27,130,84,130,58,84,130,0,130,62,0,118,133,0,130,84,43,44,73,135,0,25,0,122,0,131,84,131,58,0,40,134,0,90,0,130,84,28,132,0,62,0,131,40,0,131,40,130,84,135,0,131,58,0,131,58,84
,84,106,26,0,26,130,40,131,0,118,0,40,0,104,130,84,132,0,130,58,133,0,131,58,130,84,0,25,133,0,131,40,131,0,27,130,84,35,130,0,25,130,40,0,131,58,130,0,25,0,130,84,40,21,40,0,130,62,133,0,131,40,130,84,71,130,58,136,0,130,58,71,130,84,71,140,0,71,130,84,71,140,55,71,130,84,71,140,84,71,84
,84,71,131,0,132,77,133,0,71,130,84,71,130,0,135,77,0,25,0,71,130,84,71,130,0,132,40,79,133,0,71,130,84,71,0,131,40,130,79,0,131,79,130,0,71,130,84,71,0,130,79,130,40,130,79,0,130,79,130,0,71,130,84,71,130,0,132,79,134,0,71,130,84,71,131,0,134,79,131,0,71,130,84,71,0,25,130,0,131,79,131,0,25,0,71,130,84,71,140,0,71,84
,84,71,140,0,71,130,84,71,130,0,26,130,0,130,122,130,0,26,130,0,71,130,84,71,133,0,130,90,132,0,47,71,130,84,71,130,59,136,0,130,59,71,130,84,71,133,0,21,134,0,71,130,84,71,130,0,59,0,132,77,132,0,71,130,84,71,132,0,77,130,69,77,132,0,71,130,84,71,132,53,77,130,69,77,132,53,71,136,84,130,69,135,84
,132,13,131,78,130,69,131,78,132,13,14,13,14,131,13,77,130,69,77,131,13,14,13,14,13,14,131,13,40,77,130,69,77,40,131,13,14,13,14,13,14,130,13,78,132,77,78,130,13,14,13,14,131,13,124,131,13,130,69,131,13,124,134,13,90,131,13,130,69,131,13,90,132,13,109,131,13,40,13,130,69,13,40,131,13,116,134,13,78,13,130,69,13,78,136,13,78,131,13,130,69,131,13,78,131,13
,135,13,130,69,135,13,14,13,14,131,13,40,102,13,40,131,13,14,13,14,13,123,13,40,108,13,132,84,13,108,40,13,124,13,14,90,14,130,84,40,13,130,84,13,40,130,84,14,90,14,133,13,84,132,13,84,136,13,21,131,13,113,132,13,21,132,13,130,84,68,136,84,68,130,84,132,13,68,136,13,68,134,13,68,136,13,68,131,13
,131,13,68,13,40,13,130,40,13,40,13,68,131,13,130,85,78,68,13,87,13,130,78,13,87,13,68,78,131,85,42,74,68,13,40,105,130,40,104,40,13,68,72,41,131,85,78,68,87,134,85,87,68,78,130,85,13,117,13,68,136,13,68,13,117,132,13,68,136,13,68,132,13,130,53,68,136,53,68,130,53,130,13,78,87,68,87,78,87,130,78,87,78,87,68,87,78,132,13,68,136,13,68,131,13
,131,13,68,130,13,118,130,13,118,130,13,68,131,13,14,13,14,68,136,13,68,14,13,14,13,14,13,68,130,13,36,102,13,38,130,13,68,13,14,132,13,68,130,13,87,130,85,87,130,13,68,134,13,40,13,40,132,69,40,13,40,132,13,106,13,78,13,78,13,130,69,13,78,13,78,13,117,130,13,133,53,13,130,69,13,133,53,130,13,87,131,85,87,13,130,69,13,87,131,85,87,136,13,130,69,135,13
,134,13,40,139,13,14,13,14,13,78,13,40,143,13,78,130,13,131,40,102,139,13,87,85,68,85,87,132,13,124,131,13,130,40,131,13,68,134,13,90,131,13,130,85,131,13,68,132,13,40,105,13,40,133,13,40,13,68,132,13,87,130,85,87,133,13,87,13,68,143,13,68,131,13
,140,13,68,142,13,26,68,26,133,13,14,133,13,35,40,131,13,40,137,13,40,13,87,131,85,87,132,13,122,131,13,35,87,138,13,90,130,13,40,143,13,87,138,13,21,133,13,115,13,115,134,13,81,131,82,83,139,48
};
