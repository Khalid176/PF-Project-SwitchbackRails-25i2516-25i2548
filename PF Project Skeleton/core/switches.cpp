#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"
#include <iostream>
using namespace std;

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters(int switch_index, int train_dir)
{

    if (switch_index < 0 || switch_index >= MAX_switches) // ADDING WHIS CHECK IN EACH BOX TO MAKE SURE THAT THE VALUE THAT IS PROVIDED IS IN BETWEEN GIVE LIMITS I.E IS FROM A TO Z
    {
        return; 
    }

    int switch_mode = switch_data[switch_index][S_Mode]; // already used the if statments to make sure the string was converted to the need 0 1 bit formate

    if (switch_mode == 1)
    {
        switch_data[switch_index][S_K_current_golbal]++;

    }
    else
    {
        //  as per given instruction 0 = up direction of the train , 2 = down , 1 = right , 3 = left ;
        if (train_dir == 0)
        {
            switch_data[switch_index][S_K_current_up] = switch_data[switch_index][S_K_current_up] + 1;
        }
        else if (train_dir == 1)
        {
            switch_data[switch_index][S_K_current_right] = switch_data[switch_index][S_K_current_right] + 1;
        }
        else if (train_dir == 2)
        {
            switch_data[switch_index][S_K_current_down] = switch_data[switch_index][S_K_current_down] + 1;
        }
        else if (train_dir == 3)
        {
            switch_data[switch_index][S_K_current_left] = switch_data[switch_index][S_K_current_left] + 1;
        }

    }
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips()
{
    for (int i = 0; i < MAX_switches; i++)

    {
        switch_flip_queue[i] = 0; /// reseting the queue this would make sure that after each tick the values of the switches in the queue goes back to zero
    }
    // isoaddipowidopaiwpdiapwidpaipwoidpiawpdipawipdiwipdoiwpoidpoaiwpodipoaiwpdipawipdiapwidpaiwopidpoawipodiapwidpoiawpoidpoawipodipowipodiapwidpiwpo

    for (int i = 0; i < MAX_switches; i++)
    {
        int flip_checker = 0;
        int mode = switch_data[i][S_Mode];
        if (mode == 1)
        {
            int current_value = switch_data[i][S_K_current_golbal];
            int limit = switch_data[i][S_K_golbal];
            if ((current_value >= limit) && limit > 0)
            {
                flip_checker = 67;
            }
        }
        else if (mode == 0) // for per_direction
        {
            int current_value_0 = switch_data[i][S_K_current_up];
            int limit0 = switch_data[i][S_K_up];

            int current_value_1 = switch_data[i][S_K_current_right];
            int limit1 = switch_data[i][S_K_right];

            int current_value_2 = switch_data[i][S_K_current_down];
            int limit2 = switch_data[i][S_K_down];

            int current_value_3 = switch_data[i][S_K_current_left];
            int limit3 = switch_data[i][S_K_left];

            if ((current_value_0 >= limit0) && limit0 > 0)
            {
                flip_checker = 67;
            }
            if ((current_value_1 >= limit1) && limit1 > 0)
            {
                flip_checker = 67;
            }
            if ((current_value_2 >= limit2) && limit2 > 0)
            {
                flip_checker = 67;
            }
            if ((current_value_3 >= limit3) && limit3 > 0)
            {
                flip_checker = 67;
            }
        }
        if (flip_checker == 67)
        {
            switch_flip_queue[i] = 1;
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips()
{
    for (int i = 0; i < MAX_switches; i++)
    {
        if (switch_flip_queue[i] == 1)
        {
            int current_state = switch_data[i][S_State];
            if (current_state == 0)
            {
                switch_data[i][S_State] = 1;
            }
            else
            {
                switch_data[i][S_State] = 0;
            }

            switch_data[i][S_K_current_up] = 0;
            switch_data[i][S_K_current_right] = 0;
            switch_data[i][S_K_current_down] = 0;
            switch_data[i][S_K_current_left] = 0;

            switch_data[i][S_K_current_golbal] = 0;

            total_switch_flips++;

            switch_flip_queue[i] = 0;

        }
    }
}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights()
{
    for (int i = 0; i < MAX_switches; i++)
    {
        if (switch_data[i][S_K_up] > 0 || switch_data[i][S_Mode] == 1)
        {
            switch_data[i][S_signal] = Signal_Green;
        }
    }
    for (int r = 0; r < grid_rows; r++)
    {
        for (int c = 0; c < grid_columns; c++)
        {
            char tile = grid[r][c];
            if (isSwitchTile(tile))
            {
                int index = getSwitchIndex(tile);

                // using distance logic to check the distance to make the the signal red ot yellow
                for (int temp = 0; temp < Number_Of_Trains; temp++)
                {
                    if (trains_data[temp][T_status] == 1)
                    {
                        int train_X = trains_data[temp][T_x];
                        int train_Y = trains_data[temp][T_y];

                        // manhatan distance

                        int a = train_X - c;
                        int b = train_Y - r;

                        if (a < 0)
                        {
                            a = a * -1;
                        }
                        if (b < 0)
                        {
                            b = b * -1;
                        }

                        int distance = a + b;

                        if (distance <= 1)
                        {
                            switch_data[index][S_signal] = Signal_Red;
                        }
                        else if (distance == 2)
                        {
                            if (switch_data[index][S_signal] != Signal_Red)
                            {

                                switch_data[index][S_signal] = Signal_Yellow;
                            }
                        }
                    }
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState(int switch_index)
{
    if (switch_index < 0 || switch_index >= MAX_switches)
    {
        return; // idk why return 0 is not working Faseeh if you are readin this take a look
    }
    int currnet_Switch_State = switch_data[switch_index][S_State];
    if (currnet_Switch_State == 1)
    {
        currnet_Switch_State = 0;
    }
    else if (currnet_Switch_State == 0)
    {
        currnet_Switch_State = 1;
    }
    switch_data[switch_index][S_State] = currnet_Switch_State;
}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection(int switch_index)
{

    if (switch_index < 0 || switch_index >= MAX_switches)
    {
        return 0;
    }

    int temporary;

    temporary = switch_data[switch_index][S_State];

    return temporary;
}
