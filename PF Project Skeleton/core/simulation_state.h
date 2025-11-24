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
int const MAX_columns = 100;
int const MAX_rows = 100;

// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
const int Trains_max = 10000;
const int TrainID = 0;     // using this to show the id now as it is not in the usual train code i would have to start from 1 when i am taking the data form the level file
const int TrainTICK = 1;   // first value in the given data
const int T_x = 2;         // second value in the given data
const int T_y = 3;         // thired value in the given data
const int T_direction = 4; // fourth value in the given data
const int T_colour = 5;    // fifth value in the given data

// now taking destenation points to know where train needs to go so we can the use manhatan formula to evaluate the preority system
const int T_des_X = 6; // sixth value int would be caluculated
const int T_des_Y = 7;

const int T_status = 8; // TO check which trinas are active and which are not

// Taking an other constant for the total properties so that i can make an array of total train x total properties

const int T_properties = 9;

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------


int const S_Mode = 0; 
const int S_State = 1; // instead of state1 and state0 using this so that both can be checked  0 = straight and turn while 1 = left / right
const int S_VisualType = 2;

int const S_K_up = 3;
int const S_K_right = 4;
int const S_K_down = 5;
int const S_K_left = 6;

// current state of the switches is being told by the variable below
int const S_K_current_up = 7;
int const S_K_current_right = 8;
int const S_K_current_down = 9;
int const S_K_current_left = 10;

int const S_properties = 11;
int const MAX_switches = 26;

// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
extern int weather;
// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------
const int Signal_Green = 0;
const int Signal_Yellow = 1;
const int Signal_Red = 2;
// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------
/* using extern int cause the row and column constants would be decalred in .cpp file and woudl be share here */

extern int grid_rows;
extern int grid_columns;

extern char grid[MAX_rows][MAX_columns]; // THE MAXIMUM MAP
// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------

// array
extern int trains_data[Trains_max][T_properties];

// For number of trains
extern int Number_Of_Trains;
// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------
extern int switch_data[MAX_switches][S_properties];
// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
extern int tick;
extern unsigned int seed;
// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------
extern int trains_crashed;
extern int trains_arrived;
extern int total_switch_flips;
// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------
extern int emergency_halt_timer;
// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
