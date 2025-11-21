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
int const MAX_columns = 100 ;
int const MAX_rows = 100 ;

/* using extern int cause the row and column constants would be decalred in .cpp file and woudl be share here */

extern int g_rows;
extern int g_columns;

extern char grid[MAX_columns][MAX_rows]; // THE MAXIMUM MAP

// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
const int Trains_max = 100;
const int TrainID = 0;   // using this to show the id now as it is not in the usual train code i would have to start from 1 when i am taking the data form the level file
const int TrainTICK = 1; // first value in the given data
const int T_x = 2;         // second value in the given data
const int T_y = 3;         // thired value in the given data
const int T_direction = 4; // fourth value in the given data
const int T_colour = 5;    // fifth value in the given data

// now taking destenation points to know where train needs to go so we can the use manhatan formula to evaluate the preority system
const int T_des_X = 6; // sixth value int would be caluculated 
const int T_des_Y = 7;

// TO check which trinas are active and which are not 

const int T_status = 8;

// Taking an other constant for the total properties so that i can make an array of total train x total properties

const int T_properties= 9;

// array 
extern int trains_data [Trains_max][T_properties];

// For number of trains 
extern int Number_Of_Trains;

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------

int const S_name = 0;
int const S_Mode = 1;
int const S_init = 2;
int const S_K_up = 3;
int const S_K_right = 3;
int const S_K_down = 4;
int const S_K_left = 5;
int const S_state0 = 6;
int const S_state1 = 7;


// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
int const weather;
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
