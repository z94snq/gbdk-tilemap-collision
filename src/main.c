#include <gb/gb.h>
#include <stdint.h>
#include <gb/metasprites.h>

#include "graphics/ball.h"
#include "common.h"

// Main entry point
void main(void){
    // Initialize hardware and load graphics
    SetupDemo();

    // Main game loop - runs forever
    while(1){
        // Variables to store proposed new position and movement direction
        uint16_t nextPlayerX = playerX, nextPlayerY = playerY;
        int8_t directionY = 0, directionX = 0;

        // Read D-pad input and calculate proposed movement
        GetPlayerInput(&nextPlayerX, &nextPlayerY, &directionX, &directionY);

        // Check if the player is attempting to move
        uint8_t playerIsMoving = directionX != 0 || directionY != 0;

        if(playerIsMoving){
            // Handle horizontal movement
            if(directionX != 0){
                // Check collision at three points along the leading edge (top, middle, bottom)
                // Subtract 1 when moving right to get actual edge pixel
                uint8_t solid =
                    WorldPositionIsSolid(nextPlayerX + directionX * PLAYER_HALF_WIDTH - (directionX > 0), playerY - PLAYER_HALF_HEIGHT) ||
                    WorldPositionIsSolid(nextPlayerX + directionX * PLAYER_HALF_WIDTH - (directionX > 0), playerY) ||
                    WorldPositionIsSolid(nextPlayerX + directionX * PLAYER_HALF_WIDTH - (directionX > 0), playerY + PLAYER_HALF_HEIGHT - 1);

                if(!solid){
                    playerX = nextPlayerX;
                }
            }

            // Handle vertical movement
            if(directionY != 0){
                // Check collision at three points along the leading edge (left, center, right)
                // Subtract 1 when moving down to get actual edge pixel
                uint8_t solid =
                    WorldPositionIsSolid(playerX + PLAYER_HALF_WIDTH - 1, nextPlayerY + directionY * PLAYER_HALF_HEIGHT - (directionY > 0)) ||
                    WorldPositionIsSolid(playerX, nextPlayerY + directionY * PLAYER_HALF_HEIGHT - (directionY > 0)) ||
                    WorldPositionIsSolid(playerX - PLAYER_HALF_WIDTH, nextPlayerY + directionY * PLAYER_HALF_HEIGHT - (directionY > 0));

                if(!solid){
                    playerY = nextPlayerY;
                }
            }
        }

        // Update sprite position on screen
        // Offset by +8 and +16 to convert from center coordinates to top-left corner
        // (required by Game Boy sprite coordinate system)
        move_metasprite(Ball_metasprites[0], 0, 0, playerX + 8, playerY + 16);

        // Wait for vertical blank
        wait_vbl_done();
    }
}