#include <gb/gb.h>
#include <stdint.h>
#include <gb/metasprites.h>

#include "graphics/tileset.h"
#include "graphics/tilemap.h"
#include "graphics/ball.h"
#include "common.h"

// Convert tilemap pixel dimensions to tile counts (divide by 8 using bit shift)
#define TILEMAP_WIDTH_IN_TILES (Tilemap_WIDTH>>3)
#define TILEMAP_HEIGHT_IN_TILES (Tilemap_HEIGHT>>3)

// Player position in world pixel coordinates
uint16_t playerX, playerY;

// Check if a world position collides with a solid tile
uint8_t WorldPositionIsSolid(uint16_t x, uint16_t y){
    // Convert pixel position to tile column
    uint16_t column = x / GRID_NODE_SIZE;

    // Out of bounds on the right is solid
    if(column >= TILEMAP_WIDTH_IN_TILES) return TRUE;

    // Convert pixel position to tile row
    uint16_t row = y / GRID_NODE_SIZE;

    // Out of bounds on the bottom is solid
    if(row >= TILEMAP_HEIGHT_IN_TILES) return TRUE;

    // Calculate 1D index from 2D tile coordinates
    uint16_t tilemapIndex = column + row * TILEMAP_WIDTH_IN_TILES;

    uint8_t tileIsSolid = FALSE;

    // Get the tile ID at this position
    uint8_t tilesetTile = Tilemap_map[tilemapIndex];

    // Tiles with ID < NUMBER_OF_SOLID_TILES are solid
    tileIsSolid = tilesetTile < NUMBER_OF_SOLID_TILES;

    return tileIsSolid;
}

// Read D-pad input and calculate the next player position
// Returns the proposed new position and movement direction via output parameters
void GetPlayerInput(uint16_t* nextPlayerX, uint16_t* nextPlayerY, int8_t* directionX, int8_t* directionY){
    // Read current joypad state
    uint8_t joypadCurrent = joypad();

    // Start with current position as default
    *nextPlayerX = playerX;
    *nextPlayerY = playerY;
    *directionX = 0;
    *directionY = 0;

    // Check each D-pad direction and update position/direction accordingly
    // Note: Multiple directions can be pressed simultaneously for diagonal movement
    if(joypadCurrent & J_RIGHT){
        *nextPlayerX += 1;
        *directionX = 1;
    }
    if(joypadCurrent & J_LEFT){
        *nextPlayerX -= 1;
        *directionX = -1;
    }
    if(joypadCurrent & J_DOWN){
        *nextPlayerY += 1;
        *directionY = 1;
    }
    if(joypadCurrent & J_UP){
        *nextPlayerY -= 1;
        *directionY = -1;
    }
}

// Initialize the Game Boy hardware and load graphics data
void SetupDemo(){
    // Enable rendering of sprites and background layers
    SHOW_SPRITES;
    SHOW_BKG;

    // Use 8x16 pixel sprites (two tiles stacked vertically)
    SPRITES_8x16;

    // Load sprite tile data into VRAM starting at tile 0
    set_sprite_data(0, Ball_TILE_COUNT, Ball_tiles);

    // Load background tile data into VRAM starting at tile 0
    set_bkg_data(0, Tileset_TILE_COUNT, Tileset_tiles);

    // Set up the background tilemap starting at position (0,0)
    set_bkg_tiles(0, 0, TILEMAP_WIDTH_IN_TILES, TILEMAP_HEIGHT_IN_TILES, Tilemap_map);

    // Set initial player position (in pixels)
    playerX = 80;
    playerY = 90;
}