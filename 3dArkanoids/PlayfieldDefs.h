#pragma once
/*
   - arkanoid block plane is 13 by 29 blocks
   - arkanoid block sprite is 16 by 8 pixels

   208 232
*/
#define PLAYFIELD_WIDTH_BLOCKS  13
#define PLAYFIELD_HEIGHT_BLOCKS 29
#define PLAYFIELD_DEPTH_BLOCKS  5
#define MAX_POSSIBLE_BLOCKS     PLAYFIELD_WIDTH_BLOCKS * PLAYFIELD_HEIGHT_BLOCKS * PLAYFIELD_DEPTH_BLOCKS

#define BLOCK_WIDTH_UNITS  16
#define BLOCK_HEIGHT_UNITS 8
#define BLOCK_DEPTH_UNITS  8

#define MAX_INSTANCES 500
#define MAX_INSTANCES_STRING "500"
