#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Storage for planned moves (for collisions).
static int moves[Trains_max][4]; // AS IN 2D THRE ARE ONLY 2 POSSIBLE DIRECTIONS IN WHICH A TRAINS CAN MOVE

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
        if (trains_data[i][T_status] == 0) // pending
        {
            if (trains_data[i][TrainTICK] <= tick)
            {
                int x = trains_data[i][T_x];
                int y = trains_data[i][T_y];

                int alreadythere = 0;

                for (int i = 0; i < Number_Of_Trains; i++)
                {
                    if ((trains_data[i][T_status] == 1) && (trains_data[i][T_x] == x) && (trains_data[i][T_y] == y))
                    {
                        alreadythere = 1;
                        break;
                    }
                }
                if (!alreadythere)
                {
                    trains_data[i][T_status] = 1;
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int train_index, int &outX, int &outY, int &outDir)
{
    int x = trains_data[train_index][T_x];
    int y = trains_data[train_index][T_y];
    int direction = trains_data[train_index][T_direction];

    char tile = grid[y][x];

    outDir = getNextDirection(direction , tile , train_index);
    outX = x;
    outY = y;
    if (outDir == 0)
    {
        outY--;
    }
    if (outDir == 1)
    {
        outX++;
    }
    if (outDir == 2)
    {
        outY++;
    }
    if (outDir == 3)
    {
        outX--;
    }
    if (isInBounds(outX, outY) && isTrackTile(grid[outY][outX]))
    {
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int direction, char tile, int train_index)
{
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
        else if (direction == 2)
        {
            upcomming_moves_direction = 1;
        }
        else if (direction == 1)
        {
            upcomming_moves_direction = 2;
        }
    }
    else if (tile == '+')
    {
        upcomming_moves_direction = getSmartDirectionAtCrossing(train_index, direction);
    }

    // checking for the switches tiles

    else if (isSwitchTile(tile)) // return true or false
    {
        int switch_index = getSwitchIndex(tile);
        int state_of_switch = getSwitchStateForDirection(switch_index);
        int type = switch_data[switch_index][S_VisualType];

        // changing direction
        if (type == 0)
        {
            if (state_of_switch == 1)
            {
                if (direction == 0)
                {
                    upcomming_moves_direction = 1;
                }
                else if (direction == 1)
                {
                    upcomming_moves_direction = 2;
                }
                else if (direction == 2)
                {
                    upcomming_moves_direction = 3;
                }
                else if (direction == 3)
                {
                    upcomming_moves_direction = 0;
                }
            }
        }

        else
        {
            if (state_of_switch == 0)
            {
                if (direction == 0)
                {
                    upcomming_moves_direction = 3;
                }
                else if (direction == 1)
                {
                    upcomming_moves_direction = 0;
                }
                else if (direction == 2)
                {
                    upcomming_moves_direction = 1;
                }
                else if (direction == 3)
                {
                    upcomming_moves_direction = 2;
                }
            }
            else
            {
                if (direction == 0)
                {
                    upcomming_moves_direction = 1;
                }
                else if (direction == 1)
                {
                    upcomming_moves_direction = 2;
                }
                else if (direction == 2)
                {
                    upcomming_moves_direction = 3;
                }
                else if (direction == 3)
                {
                    upcomming_moves_direction = 0;
                }
            }
        }
    }
    return upcomming_moves_direction;
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int index, int current_direction)
{
    int X = -1;
    int Y = -1;
    int maxDistnace = -1;

    int current_X = trains_data[index][T_x];
    int current_Y = trains_data[index][T_y];

    // finding farthest destination
    for (int r = 0; r < grid_rows; r++)
    {
        for (int c = 0; c < grid_columns; c++)
        {
            if (grid[r][c] == 'D')
            {
                int temp1 = current_X - c;
                int temp2 = current_Y - r;

                if (temp1 < 0)
                {
                    temp1 = temp1 * -1;
                }
                if (temp2 < 0)
                {
                    temp2 = temp2 * -1;
                }
                if ((temp1 + temp2) > maxDistnace) // shouldnt this always be true
                {
                    maxDistnace = temp1 + temp2;
                    X = c;
                    Y = r;
                }
            }
        }
    }
    if (X > current_X && current_direction != 3)
    {
        return 1;
    }
    if (X < current_X && current_direction != 1)
    {
        return 3;
    }
    if (Y > current_X && current_direction != 0)
    {
        return 2;
    }
    if (Y < current_Y && current_direction != 2)
    {
        return 0;
    }

    return current_direction;
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes()
{
    if (emergency_halt_timer > 0)
    {
        for (int i = 0; i < Number_Of_Trains; i++)
        {
            moves[i][3] = 0;
        }
        return;
    }
    for (int i = 0; i < Number_Of_Trains; i++)
    {
        moves[i][3] = 0;

        if (trains_data[i][T_status] == 1)
        {
            int x1, y1, d1;
            if (determineNextPosition(i, x1, y1, d1))
            {
                moves[i][0] = x1;
                moves[i][1] = y1;
                moves[i][2] = d1;
                moves[i][3] = 1;
            }
            else
            {
                trains_data[i][T_status] = 3;
                trains_crashed++;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains()
{
    for (int i = 0; i < Number_Of_Trains; i++)
    {
        if (trains_data[i][T_status] == 1 && moves[i][3] == 1)
        {
            trains_data[i][T_x] = moves[i][0];
            trains_data[i][T_y] = moves[i][1];
            trains_data[i][T_direction] = moves[i][2];

            char upcomming_tile = grid[moves[i][1]][moves[i][0]];
            if (isSwitchTile(upcomming_tile))
            {
                int index = getSwitchIndex(upcomming_tile);
                updateSwitchCounters(index, moves[i][2]);
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
    for (int train1 = 0; train1 < Number_Of_Trains; train1++)
    {
        if (moves[train1][3] == 0)
        {
            continue;
        }
        for (int train2 = train1 + 1; train2 < Number_Of_Trains; train2++)
        {
            if (moves[train2][3] == 0)
            {
                continue;
            }
            if ((moves[train1][0] == moves[train2][0]) && moves[train1][1] == moves[train2][1])
            {
                //  if this if condition is true a colission can occor now calculate the distance for train 1 and train 2
                int distance1 = 0;
                int distance2 = 0;

                // FOR TRAIN 1
                for (int r = 0; r < grid_rows; r++)
                {
                    for (int c = 0; c < grid_columns; c++)
                    {
                        if (grid[r][c] == 'D')
                        {
                            int a = trains_data[train1][T_x] - c;
                            int b = trains_data[train1][T_y] - r;

                            if (a < 0)
                            {
                                a = a * (-1);
                            }
                            if (b < 0)
                            {
                                b = b * (-1);
                            }
                            if (a + b > distance1)
                            {
                                distance1 = a + b;
                            }
                        }
                    }
                }
                // For train 2
                for (int r = 0; r < grid_rows; r++)
                {
                    for (int c = 0; c < grid_columns; c++)
                    {
                        if (grid[r][c] == 'D')
                        {
                            int a = trains_data[train2][T_x] - c;
                            int b = trains_data[train2][T_y] - r;

                            if (a < 0)
                            {
                                a = a * (-1);
                            }
                            if (b < 0)
                            {
                                b = b * (-1);
                            }
                            if (a + b > distance2)
                            {
                                distance2 = a + b;
                            }
                        }
                    }
                }
                if (distance1 > distance2)
                {
                    moves[train2][3] = 0; // train2 waits
                }
                else if (distance1 < distance2)
                {
                    moves[train1][3] = 0; // train1 waits
                }
                else // same distnace crash would occour
                {
                    trains_data[train1][T_status] = 3;
                    trains_data[train2][T_status] = 3;

                    moves[train1][3] = 0;
                    moves[train2][3] = 0;

                    trains_crashed = trains_crashed + 2;
                }
            }
        }
    }
}
// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals()
{
    for (int i = 0; i < Number_Of_Trains; i++)
    {
        if (trains_data[i][T_status] == 1)
        {
            int x = trains_data[i][T_x];
            int y = trains_data[i][T_y];

            if (grid[y][x] == 'D')
            {
                int x = trains_data[i][T_status] = 2; // arived as 'D' is the destination block
                trains_arrived++;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt()
{
    if (emergency_halt_timer == 0)
    {
        emergency_halt_timer = 3; // starts the emergency halt
    }
}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt()
{
    if (emergency_halt_timer > 0)
    {
        emergency_halt_timer--;
    }
}
