#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

 static bool temporary_queue [MAX_switches]; // as said in the instruction manual that we cannot just update a switch on the spot we would update the switches after a tick has passed this array i am creating would be responsiable for hold ing all the data pf the switches that have been updateed
// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters(int switch_index ,int train_dir) {

    if (switch_index < 0 || switch_index > MAX_switches)
    {
        return ; // idk why return 0 is not working Faseeh if you are readin this take a look
    }

    int switch_mode = switch_data [switch_index][S_Mode];  // already used the if statments to make sure the string was converted to the need 0 1 bit formate

    if (switch_mode == 1)
    {
        switch_data [switch_index][S_K_current_up] ++ ;          
    }
    
    
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState() {
}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection() {
}
