#include "grid.h"
#include "simulation_state.h"

// ============================================================================
// GRID.CPP - Grid utilities
// ============================================================================

// ----------------------------------------------------------------------------
// Check if a position is inside the grid.
// ----------------------------------------------------------------------------
// Returns true if x,y are within bounds.
// ----------------------------------------------------------------------------
bool isInBounds(int x, int y)
{

    if ((y < grid_rows && x >= 0) && (x < grid_columns && y >= 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
// Check if a tile is a track tile.
// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile(char c)
{
    
    if ((c == '-') || (c == '|') || (c == '\\') || (c == '/') || (c == '=') || (c == '+'))
    {
        return true;
    }
    if (c >= 'A' && c <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
// Check if a tile is a switch.
// ----------------------------------------------------------------------------
// Returns true if the tile is 'A'..'Z'.
// ----------------------------------------------------------------------------
bool isSwitchTile(char c)
{
    if ((c >= 'A' && c <= 'Z') && (c != 'D' && c != 'S'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(char c)
{
    if (c >= 'A' && c <= 'Z') // insted of making another chek using the above used function here
    {
        return c - 'A';
    }
    else
    {
        return -1; // error if it is out of A to Z limit
    }
}

// ----------------------------------------------------------------------------
// Check if a position is a spawn point.
// ----------------------------------------------------------------------------
// Returns true if x,y is a spawn.
// ----------------------------------------------------------------------------
bool isSpawnPoint(int x, int y)
{


    if (!isInBounds(x , y))
    {
        return false;
    }
    
    if (grid[y][x] == 'S')
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
// Check if a position is a destination.
// ----------------------------------------------------------------------------
// Returns true if x,y is a destination.
// ----------------------------------------------------------------------------
bool isDestinationPoint(int x, int y)
{
    if (!isInBounds(x , y))
    {
        return false;
    }

    if (grid[y][x] == 'D')
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
// Toggle a safety tile.
// ----------------------------------------------------------------------------
// Returns true if toggled successfully.
// ----------------------------------------------------------------------------
bool toggleSafetyTile(int x, int y)
{
    if (!isInBounds(x , y))
    {
        return false;
    }
    char current = grid[y][x];

    if (current == '-')
    {
        grid[y][x] = '=';
        return true;
    }
    else if (current == '=')
    {
        grid[y][x] = '-';
        return true;
    }
    else
    {
        return false;
    }
}