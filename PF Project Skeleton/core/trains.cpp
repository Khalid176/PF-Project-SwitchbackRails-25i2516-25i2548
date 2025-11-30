#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

static int moves[Trains_max][4]; // Storage for planned moves.

int calculateDistance(int train_idx) // extra function for manhatan distance
{
    int CurrentX = trains_data[train_idx][T_x];
    int CurrentY = trains_data[train_idx][T_y];

    if (trains_data[train_idx][T_des_Y] == 9999 || trains_data[train_idx][T_des_Y] == -9999) // using this to make sure that the distacne is the maximun preority
    {
        return 9999;
    }

    int destX = trains_data[train_idx][T_des_X];
    int destY = trains_data[train_idx][T_des_Y];

    if (destX != -1 && destY != -1)
    {
        int temp = abs(CurrentX - destX) + abs(CurrentY - destY);

        return temp;
    }
    return 0;
}

void assignTargetDestination(int train_idx)
{
    int InitialX = trains_data[train_idx][T_x];
    int InitialY = trains_data[train_idx][T_y];
    int bestX = -1;
    int bestY = -1;
    int MinimumDistance = 999999;

    for (int r = 0; r < grid_rows; r++)
    {
        for (int c = 0; c < grid_columns; c++)
        {
            if (grid[r][c] == 'D')
            {
                int distance = abs(InitialX - c) + abs(InitialY - r);

                int traffic = 0;
                for (int j = 0; j < Number_Of_Trains; j++)
                {
                    if (j == train_idx)
                        continue;
                    if (trains_data[j][T_status] == 1 && trains_data[j][T_des_X] == c && trains_data[j][T_des_Y] == r)
                    {
                        traffic++;
                    }
                }

                distance += (traffic * 500);

                if (distance < MinimumDistance)
                {
                    MinimumDistance = distance;
                    bestX = c;
                    bestY = r;
                }
            }
        }
    }
    trains_data[train_idx][T_des_X] = bestX;
    trains_data[train_idx][T_des_Y] = bestY;
}

bool canMoveImmediate(int x, int y, int direction)
{
    if (direction == 0)
    {
        y--;
    }

    else if (direction == 1)
    {
        x++;
    }

    else if (direction == 2)
    {
        y++;
    }

    else if (direction == 3)
    {
        x--;
    }

    int thakgaya = (isInBounds(x, y) && isTrackTile(grid[y][x]));
    return thakgaya;
}

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick()
{

    for (int i = 0; i < Number_Of_Trains; i++)
    {
        if (trains_data[i][T_status] == 0)
        {
            if (tick >= trains_data[i][TrainTICK])
            {
                int XRN = trains_data[i][T_x]; // x postion right now
                int YRN = trains_data[i][T_y]; // y position right now
                int finalY = YRN;

                if (isInBounds(XRN, YRN) && isTrackTile(grid[YRN][XRN]))
                {
                    finalY = YRN;
                }

                else if (isInBounds(XRN, YRN - 1) && isTrackTile(grid[YRN - 1][XRN]))
                {
                    finalY = YRN - 1;
                }

                else if (isInBounds(XRN, YRN - 2) && isTrackTile(grid[YRN - 2][XRN]))
                {
                    finalY = YRN - 2;
                }

                trains_data[i][T_x] = XRN;
                trains_data[i][T_y] = finalY;

                bool blocked = false;
                for (int j = 0; j < Number_Of_Trains; j++)
                {
                    if (trains_data[j][T_status] == 1 && trains_data[j][T_x] == XRN && trains_data[j][T_y] == finalY)
                    {
                        blocked = true;
                        break;
                    }
                }

                if (!blocked)
                {
                    trains_data[i][T_status] = 1;
                    assignTargetDestination(i);
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
    int dir = trains_data[train_index][T_direction];

    outDir = getNextDirection(dir, grid[y][x], train_index);

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

    for (int k = 1; k <= 4; k++)
    {
        int scanX = x;
        int scanY = y;
        if (outDir == 0)
        {
            scanY = scanY - k;
        }

        else if (outDir == 1)
        {
            scanX = scanX + k;
        }

        else if (outDir == 2)
        {
            scanY = scanY + k;
        }
        else if (outDir == 3)
        {
            scanX = scanX - k;
        }

        if (isInBounds(scanX, scanY) && isTrackTile(grid[scanY][scanX]))
        {
            outX = scanX;
            outY = scanY;
            return true;
        }
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
    int next = direction;
    int CurrentX = trains_data[train_index][T_x];
    int CurrentY = trains_data[train_index][T_y];
    int destY = trains_data[train_index][T_des_Y];

    if (tile == '/')
    {
        if (direction == 0)
        {
            next = 1;
        }

        else if (direction == 1)
        {
            next = 0;
        }

        else if (direction == 2)
        {
            next = 3;
        }

        else if (direction == 3)
        {
            next = 2;
        }
    }
    else if (tile == '\\')
    {
        if (direction == 0)
        {
            next = 3;
        }

        else if (direction == 3)
        {
            next = 0;
        }

        else if (direction == 1)
        {
            next = 2;
        }

        else if (direction == 2)
        {
            next = 1;
        }
    }
    else if (tile == '+')
    {
        next = getSmartDirectionAtCrossing(train_index, direction);
    }

    else if (isSwitchTile(tile))
    {
        int idx = getSwitchIndex(tile);
        int state = getSwitchStateForDirection(idx);
        int type = switch_data[idx][S_VisualType];

        int DirectionProposed = next;

        if (type == 0 && state == 1)
        {
            if (direction == 0)
            {
                DirectionProposed = 1;
            }

            else if (direction == 1)
            {
                DirectionProposed = 2;
            }

            else if (direction == 2)
            {
                DirectionProposed = 3;
            }

            else if (direction == 3)
            {
                DirectionProposed = 0;
            }
        }
        else if (type == 1)
        {
            if (state == 0)
            {
                if (direction == 0)
                {
                    DirectionProposed = 3;
                }

                else if (direction == 1)
                {
                    DirectionProposed = 0;
                }

                else if (direction == 2)
                {
                    DirectionProposed = 1;
                }

                else if (direction == 3)
                {
                    DirectionProposed = 2;
                }
            }
            else
            {
                if (direction == 0)
                {
                    DirectionProposed = 1;
                }

                else if (direction == 1)
                {
                    DirectionProposed = 2;
                }

                else if (direction == 2)
                {
                    DirectionProposed = 3;
                }

                else if (direction == 3)
                {
                    DirectionProposed = 0;
                }
            }
        }

        if (canMoveImmediate(CurrentX, CurrentY, DirectionProposed))
        {
            next = DirectionProposed;
        }
        else
        {

            next = direction;
        }
    }

    else
    {
        if (direction == 1 || direction == 3)
        {

            for (int k = 1; k <= 4; k++)
            {
                if (isInBounds(CurrentX, CurrentY + k) && grid[CurrentY + k][CurrentX] == '|')
                {

                    if (destY == 9999 || destY > CurrentY)
                    {
                        return 2;
                    }
                }
            }

            for (int k = 1; k <= 4; k++)
            {
                if (isInBounds(CurrentX, CurrentY - k) && grid[CurrentY - k][CurrentX] == '|')
                {
                    if (destY == -9999 || destY < CurrentY)
                    {
                        return 0;
                    }
                }
            }
        }
    }

    return next;
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int index, int current_direction)
{

    int CurrentX = trains_data[index][T_x];
    int CurrentY = trains_data[index][T_y];
    int targetX = trains_data[index][T_des_X];
    int targetY = trains_data[index][T_des_Y];

    // if check is 9999 would prefer turning
    if (targetY == 9999 || targetY == -9999)
    {
        // Scan for a pipe
        for (int d = 0; d < 4; d++)
        {
            if (abs(d - current_direction) == 2)
            {
                continue;
            }

            int nx = CurrentX, ny = CurrentY;
            if (d == 0)
            {
                ny--;
            }

            else if (d == 1)
            {
                nx++;
            }

            else if (d == 2)
            {
                ny++;
            }

            else if (d == 3)
            {
                nx--;
            }

            if (isInBounds(nx, ny) && isTrackTile(grid[ny][nx]))
            {
                return d;
            }
        }
    }

    if (targetX == -1)
    {
        return current_direction;
    }
        

    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    int bestDir = current_direction;
    int MinimumDistance = 999999;

    for (int d = 0; d < 4; d++)
    {
        if (abs(d - current_direction) == 2)
        {
             continue;
        }
           
        int nx = CurrentX + dx[d];
        int ny = CurrentY + dy[d];

        bool validPath = false;
        for (int k = 1; k <= 4; k++)
        {
            if (isInBounds(CurrentX + dx[d] * k, CurrentY + dy[d] * k) &&
                isTrackTile(grid[CurrentY + dy[d] * k][CurrentX + dx[d] * k]))
            {
                validPath = true;
                break;
            }
        }

        if (validPath)
        {
            int dist = abs(nx - targetX) + abs(ny - targetY);
            if (dist < MinimumDistance)
            {
                MinimumDistance = dist;
                bestDir = d;
            }
        }
    }
    return bestDir;
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
            int nx, ny, nd;
            if (determineNextPosition(i, nx, ny, nd))
            {
                moves[i][0] = nx;
                moves[i][1] = ny;
                moves[i][2] = nd;
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

            int oldX = trains_data[i][T_x];
            int oldY = trains_data[i][T_y];
            int OldDestination = trains_data[i][T_direction];

            // Update Position
            trains_data[i][T_x] = moves[i][0];
            trains_data[i][T_y] = moves[i][1];
            trains_data[i][T_direction] = moves[i][2];

            // DYNAMIC REROUTING: If direction changed, find new best target
            if (OldDestination != moves[i][2])
            {
                assignTargetDestination(i); // Reset logic now that we turned
            }

            // EXIT TRIGGER for Switches
            char tile = grid[oldY][oldX];
            if (isSwitchTile(tile))
            {
                int idx = getSwitchIndex(tile);
                updateSwitchCounters(idx, moves[i][2]);

                
                int state = getSwitchStateForDirection(idx);
                int type = switch_data[idx][S_VisualType];
                if (type == 0 && state == 1)
                {
                    trains_data[i][T_des_Y] = 9999; // Force Turn Down
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
    for (int i = 0; i < Number_Of_Trains; i++)
    {
        if (moves[i][3] == 0)
        {
              continue;
        }
          
        for (int k = 0; k < Number_Of_Trains; k++)
        {
            if (i == k)
            {
                continue;
            }
                
            if (trains_data[k][T_status] == 1 && moves[k][3] == 0)
            {
                if (moves[i][0] == trains_data[k][T_x] && moves[i][1] == trains_data[k][T_y])
                {
                    moves[i][3] = 0;
                }
            }
        }

        if (moves[i][3] == 0)
        {
            continue;
        }

        for (int j = i + 1; j < Number_Of_Trains; j++)
        {
            if (moves[j][3] == 0)
            {
                continue;
            }

            bool crash = false;
            if (moves[i][0] == moves[j][0] && moves[i][1] == moves[j][1])
            {
                crash = true;
            }

            if (moves[i][0] == trains_data[j][T_x] && moves[i][1] == trains_data[j][T_y] && moves[j][0] == trains_data[i][T_x] && moves[j][1] == trains_data[i][T_y])
            {
                crash = true;
            }

            if (crash)
            {
                int d1 = calculateDistance(i);
                int d2 = calculateDistance(j);
                if (d1 > d2)
                {
                    moves[j][3] = 0;
                }

                else if (d2 > d1)
                {
                    moves[i][3] = 0;
                }

                else
                {
                    if (i < j)
                    {
                        moves[j][3] = 0;
                    }

                    else
                    {
                        moves[i][3] = 0;
                    }
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
            if (grid[trains_data[i][T_y]][trains_data[i][T_x]] == 'D')
            {
                trains_data[i][T_status] = 2;
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
          emergency_halt_timer = 3;
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