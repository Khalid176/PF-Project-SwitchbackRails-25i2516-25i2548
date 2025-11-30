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

void initializeSimulation()
{
    srand(seed);
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick()
{

    updateEmergencyHalt();
    spawnTrainsForTick();

    determineAllRoutes();
    detectCollisions();

    // Adding the COMPLEX WEATHER LOGIC :(
    int skip_movment = 0;
    if ((weather == 1) && (tick % 5 == 0))
    {
        skip_movment = 67;
    }

    queueSwitchFlips();
    if (skip_movment != 67)
    {
        moveAllTrains();
    }
    queueSwitchFlips();
    applyDeferredFlips();

    checkArrivals();
    updateSignalLights();

    logTrainTrace();
    logSignalState();
    logSwitchState();

    tick++;
    
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete()
{
    if (trains_arrived + trains_crashed == Number_Of_Trains)
    {
        return true;
    }
    else
        return false;
}