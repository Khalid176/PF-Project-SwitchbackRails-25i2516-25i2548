#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <ctime>

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation() {
 srand(seed);
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick() {
    spawnTrainsForTick();
    determineAllRoutes();
    queueSwitchFlips();
    moveAllTrains();
    applyDeferredFlips();
    checkArrivals();



    logTrainTrace();
    logSignalState();
    logSwitchState();

    tick++;
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() {
    if(trains_arrived+trains_crashed==Number_Of_Trains)
    {
        return true;
    }
    else
    return false;
}
