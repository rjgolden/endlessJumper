#pragma once

#include "raylib.h"

// Undefine conflicting raylib colors
#undef DARKBROWN
#undef BLACK
#undef RED
#undef ORANGE
#undef YELLOW
#undef GREEN
#undef BLUE
#undef WHITE
#undef LIGHTGRAY
#undef GRAY
#undef PURPLE
#undef MAGENTA
#undef PINK
#undef DARKPURPLE
#undef BROWN
#undef DARKGRAY
#undef SKYBLUE

// DawnBringer 32 (DB32) color palette
// Source: https://lospec.com/palette-list/dawnbringer-32
#define BLACK       CLITERAL(Color){   0,   0,   0, 255 } // #000000
#define DARKPURPLE  CLITERAL(Color){  34,  32,  52, 255 } // #222034
#define DARKBROWN   CLITERAL(Color){  69,  40,  60, 255 } // #45283C
#define BROWN       CLITERAL(Color){ 102,  57,  49, 255 } // #663931
#define BROWN2      CLITERAL(Color){ 143,  86,  59, 255 } // #8F563B
#define ORANGE      CLITERAL(Color){ 223, 113,  38, 255 } // #DF7126
#define TAN         CLITERAL(Color){ 217, 160, 102, 255 } // #D9A066
#define PEACH       CLITERAL(Color){ 238, 195, 154, 255 } // #EEC39A

#define YELLOW      CLITERAL(Color){ 251, 242,  54, 255 } // #FBF236
#define LIGHTGREEN  CLITERAL(Color){ 153, 229,  80, 255 } // #99E550
#define GREEN       CLITERAL(Color){ 106, 190,  48, 255 } // #6ABE30
#define TEAL        CLITERAL(Color){  55, 148, 110, 255 } // #37946E
#define OLIVEGREEN  CLITERAL(Color){  75, 105,  47, 255 } // #4B692F
#define OLIVE       CLITERAL(Color){  82,  75,  36, 255 } // #524B24
#define DARKGRAY    CLITERAL(Color){  50,  60,  57, 255 } // #323C39

#define NAVY        CLITERAL(Color){  63,  63, 116, 255 } // #3F3F74
#define BLUE        CLITERAL(Color){  48,  96, 130, 255 } // #306082
#define LIGHTBLUE   CLITERAL(Color){  91, 110, 225, 255 } // #5B6EE1
#define SKYBLUE     CLITERAL(Color){  99, 155, 255, 255 } // #639BFF
#define CYAN        CLITERAL(Color){  95, 205, 228, 255 } // #5FCDE4
#define LIGHTBLUE2  CLITERAL(Color){ 203, 219, 252, 255 } // #CBDBFC

#define WHITE       CLITERAL(Color){ 255, 255, 255, 255 } // #FFFFFF
#define LIGHTGRAY   CLITERAL(Color){ 155, 173, 183, 255 } // #9BADB7
#define GRAY        CLITERAL(Color){ 132, 126, 135, 255 } // #847E87
#define GRAY2       CLITERAL(Color){ 105, 106, 106, 255 } // #696A6A
#define DARKGRAY2   CLITERAL(Color){  89,  86,  82, 255 } // #595652

#define PURPLE      CLITERAL(Color){ 118,  66, 138, 255 } // #76428A
#define RED         CLITERAL(Color){ 172,  50,  50, 255 } // #AC3232
#define PINK        CLITERAL(Color){ 217,  87,  99, 255 } // #D95763
#define MAGENTA     CLITERAL(Color){ 215, 123, 186, 255 } // #D77BBA
#define OLIVE2      CLITERAL(Color){ 143, 151,  74, 255 } // #8F974A
#define BROWN3      CLITERAL(Color){ 138, 111,  48, 255 } // #8A6F30