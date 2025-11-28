#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick()
{
    for (int i = 0; i < Number_Of_Trains; i++)
    {
        int spawnpoint_x = trains_data[i][T_x];
        int spawnpoint_y = trains_data[i][T_y];
        bool blocked_path = false;
        for (int j = 0; j < Number_Of_Trains; j++)
        {
            if (trains_data[i][T_status] == 1)
            {
                if (spawnpoint_x == trains_data[j][T_x] && spawnpoint_y == trains_data[j][T_x])
                {
                    blocked_path = true;
                    break;
                }
            }
        }
        if (blocked_path == false)
        {
            trains_data[i][T_status] = 1;
        }
    }
}

    // ----------------------------------------------------------------------------
    // DETERMINE NEXT POSITION for a train
    // ----------------------------------------------------------------------------
    // Compute next position/direction from current tile and rules.
    // ----------------------------------------------------------------------------
    bool determineNextPosition()
    {
    }

    // ----------------------------------------------------------------------------
    // GET NEXT DIRECTION based on current tile and direction
    // ----------------------------------------------------------------------------
    // Return new direction after entering the tile.
    // ----------------------------------------------------------------------------
    int getNextDirection()
    {
    }

    // ----------------------------------------------------------------------------
    // SMART ROUTING AT CROSSING - Route train to its matched destination
    // ----------------------------------------------------------------------------
    // Choose best direction at '+' toward destination.
    // ----------------------------------------------------------------------------
    int getSmartDirectionAtCrossing()
    {
    }

    // ----------------------------------------------------------------------------
    // DETERMINE ALL ROUTES (PHASE 2)
    // ----------------------------------------------------------------------------
    // Fill next positions/directions for all trains.
    // ----------------------------------------------------------------------------
    void determineAllRoutes()
    {
    }

    // ----------------------------------------------------------------------------
    // MOVE ALL TRAINS (PHASE 5)
    // ----------------------------------------------------------------------------
    // Move trains; resolve collisions and apply effects.
    // ----------------------------------------------------------------------------
    void moveAllTrains()
    {
        int moves [Trains_max][4]; // AS IN 2D THRE ARE ONLY 2 POSSIBLE DIRECTIONS IN WHICH A TRAINS CAN MOVE 

        for (int train = 0; train < Number_Of_Trains; train++)
        {
            moves[train][3] = 0;

            if (trains_data [train][T_status] == 1  ) // 1 == active status
            {
                int x = trains_data [train][T_x];
                int y = trains_data [train][T_y];

                int direction = trains_data [train][T_direction];

                char tile = grid[y][x];

                int upcomming_moves_direction = direction;

                // checking tile of the grid to find out the correct direction for upcomming move 

                if (tile == '/')
                {
                    if (direction == 0)
                    {
                        upcomming_moves_direction = 1;
                    }
                    else if (direction == 1)
                    {
                       upcomming_moves_direction = 0;
                    }
                    if (direction == 2)
                    {
                        upcomming_moves_direction = 3;
                    }
                    else if (direction == 3)
                    {
                       upcomming_moves_direction = 2;
                    }
                    
                }
                else if (tile == '\\')
                {
                    if (direction == 0)
                    {
                        upcomming_moves_direction = 3;
                    }
                    else if (direction == 3)
                    {
                       upcomming_moves_direction = 0;
                    }
                    if (direction == 2)
                    {
                        upcomming_moves_direction = 1;
                    }
                    else if (direction == 1)
                    {
                       upcomming_moves_direction = 2;
                    }
                }
                

                
            }
            
        }
        
    }

    // ----------------------------------------------------------------------------
    // DETECT COLLISIONS WITH PRIORITY SYSTEM
    // ----------------------------------------------------------------------------
    // Resolve same-tile, swap, and crossing conflicts.
    // ----------------------------------------------------------------------------
    void detectCollisions()
    {
    }

    // ----------------------------------------------------------------------------
    // CHECK ARRIVALS
    // ----------------------------------------------------------------------------
    // Mark trains that reached destinations.
    // ----------------------------------------------------------------------------
    void checkArrivals()
    {
    }

    // ----------------------------------------------------------------------------
    // APPLY EMERGENCY HALT
    // ----------------------------------------------------------------------------
    // Apply halt to trains in the active zone.
    // ----------------------------------------------------------------------------
    void applyEmergencyHalt()
    {
    }

    // ----------------------------------------------------------------------------
    // UPDATE EMERGENCY HALT
    // ----------------------------------------------------------------------------
    // Decrement timer and disable when done.
    // ----------------------------------------------------------------------------
    void updateEmergencyHalt()
    {
    }
