#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
int grid_rows = 0;
int grid_columns = 0;
char grid[MAX_rows][MAX_columns];

// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------

int Number_Of_Trains = 0;
int trains_data[Trains_max][T_properties];

// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------

int switch_data[MAX_switches][S_properties];
int switch_flip_queue[MAX_switches];

// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
int tick = 0;
int weather = 0;
unsigned int seed = 0;
// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------
int trains_crashed;
int trains_arrived;
int total_switch_flips;
// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------

int emergency_halt_timer;

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void initializeSimulationState()
{
    grid_columns = 0;
    grid_rows = 0;
    Number_Of_Trains = 0;
    tick = 0;
    weather = 0;
    seed = 0;
    trains_arrived = 0;
    trains_crashed = 0;
    total_switch_flips = 0;
    emergency_halt_timer = 0;

    for (int i = 0; i < MAX_rows; i++)
    {
        for (int j = 0; j < MAX_columns; j++)
        {
            grid[i][j] = ' ';
        }
    }
    for (int i = 0; i < Trains_max; i++)
    {
        for (int j = 0; j < T_properties; j++)
        {
            trains_data[i][j] = 0;
        }
    }
    for (int i = 0; i < MAX_switches; i++)
    {
        for (int j = 0; j < S_properties ; j++)
        {
            switch_data[i][j] = 0;
        }
    }
    for (int i = 0; i < MAX_switches; i++)
    {
        switch_flip_queue[i] = 0;
    }
    
}