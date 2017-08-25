#include "globals.h"

// PLACED IN BANK 06
// zsdcc can only place code and rodata into different banks at the moment
// anything else use assembly language and sections

// Compressed MAP
//TODO A SINGLE array
// FIRST ONE BYTE FOR EACH SCREEN SIZE, THEN THE DATA, ALSO THE PAPER BACKGROUND
const unsigned char scr_0[] = 	{0,7,8,142,0,9,10,151,0,132,70,56,70,142,0,56,143,0,56,0,65,66,67,68,69,70,71,72,73,74,75,76,130,0,56,141,0,5,6,137,0,93,0,90,0,1,2,3,4,0,144,70}	;
const unsigned char scr_1[] = 	{141,0,7,8,130,0,32,0,32,0,32,0,32,133,0,9,10,148,0,99,87,0,11,12,133,0,84,130,0,139,70,0,130,70,58,70,0,68,131,32,130,44,130,0,130,44,131,0,58,132,0,99,134,0,5,6,130,0,58,130,0,130,32,68,32,132,0,1,2,3,4,87,58,0,135,70,130,42,135,70}	;
const unsigned char scr_2[] = 	{176,32,0,93,130,0,92,139,0,131,70,56,140,70,131,0,56,143,0,56,134,0,5,6,133,0,11,12,56,133,0,1,2,3,4,0,88,0,72,142,73,74}	;
const unsigned char scr_3[] = 	{130,0,7,8,131,0,137,71,130,0,9,10,131,0,133,71,0,130,32,136,0,133,71,142,0,92,139,0,57,134,71,130,42,71,134,0,57,137,71,134,0,57,137,71,0,130,32,132,0,137,71,72,131,73,74,130,42,72,135,73,74}	;
const unsigned char scr_4[] = 	{143,71,131,0,32,0,71,130,0,32,0,71,132,0,71,133,0,59,132,0,44,132,0,44,133,0,59,135,0,92,131,0,71,130,42,71,59,71,130,42,134,71,130,0,132,71,59,133,71,131,0,71,130,0,71,0,130,32,59,130,32,130,0,71,5,6,0,71,130,0,71,0,130,32,59,130,32,0,92,1,2,3,4,131,0,72,142,73,74}	;
const unsigned char scr_5[] = 	{0,7,8,142,0,9,10,137,0,32,137,0,32,133,0,32,132,0,32,132,0,32,131,0,32,0,99,92,32,130,0,32,130,0,32,0,99,92,32,0,53,131,54,55,0,32,99,32,0,53,131,54,55,131,0,23,131,0,53,54,55,131,0,23,131,0,5,6,23,132,0,23,131,0,88,22,0,88,1,2,3,22,130,0,88,72,142,73,74}	;
const unsigned char scr_6[] = 	{0,44,32,44,32,44,32,44,32,44,32,44,32,44,32,133,0,92,134,0,86,133,0,142,48,134,0,87,136,0,32,130,0,130,48,0,136,48,0,130,48,138,0,92,135,0,130,48,0,135,48,130,0,130,48,145,0,144,70}	;
const unsigned char scr_7[] = 	{136,0,21,132,0,7,8,137,0,20,131,0,21,9,10,137,0,20,131,0,20,136,0,67,58,0,20,130,0,131,19,134,0,130,67,58,0,20,130,0,17,13,18,133,0,131,67,58,0,20,0,19,131,16,19,131,0,132,67,58,0,20,0,17,13,14,13,18,130,0,133,67,58,0,20,0,17,13,15,13,18,0,72,142,73,74}	;
