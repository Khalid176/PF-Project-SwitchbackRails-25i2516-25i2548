#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================

// ----------------------------------------------------------------------------
// GRID CONSTANTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
const int TrainID = 0;   // using this to show the id now as it is not in the usual train code i would have to start from 1 when i am taking the data form the level file
const int TrainTICK = 1; // first value in the given data
const int x = 2;         // second value in the given data
const int y = 3;         // thired value in the given data
const int direction = 4; // fourth value in the given data
const int colour = 5;    // fifth value in the given data

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------

    const int T_ID = 0; //using this to show the id now as it is not in the usual train code i would have to start from 1 when i am taking the data form the level file
    const int TrainTICK = 1; // first value in the given data
    const int x = 2; // second value in the given data
    const int y = 3; // thired value in the given data
    const int direction = 4; // fourth value in the given data
    const int colour = 5; // fifth value in the given data
// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------

const int weather ;
// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
