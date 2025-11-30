#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------
bool loadLevelFile(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Could not open the file: " << filename << endl;
        return false;
    }

    initializeSimulationState();

    string key;
    bool FoundSwitchesInMap = false;

    // Check flag FIRST (Fixes order bug)
    while (FoundSwitchesInMap || (file >> key))
    {
        if (FoundSwitchesInMap)
        {
            key = "SWITCHES:";
        }

        // MANUAL CHECKS to avoid string modification issues
        if (key == "ROWS:")
            file >> grid_rows;
        else if (key == "COLS:")
            file >> grid_columns;
        else if (key == "SEED:")
        {
            file >> seed;
            srand(seed);
        }
        else if (key == "WEATHER:")
        {
            string temp;
            file >> temp;
            if (temp == "RAIN")
                weather = 1;
            else if (temp == "FOG")
                weather = 2;
            else
                weather = 0;
        }
        else if (key == "MAP:")
        {

            string temporary;
            getline(file, temporary);

            for (int i = 0; i < grid_rows; i++)
            {
                string line;
                getline(file, line);

                int start = 0;
                while (start < (int)line.length() && line[start] == ' ')
                    start++;

                bool isHeader = false;
                if ((int)line.length() - start >= 8)
                {
                    if (line[start] == 'S' && line[start + 1] == 'W' && line[start + 7] == 'S')
                        isHeader = true;
                }

                if (isHeader)
                {
                    cout << "DEBUG: Map ended early! Found SWITCHES." << endl;
                    FoundSwitchesInMap = true;
                    break;
                }

                for (int j = 0; j < grid_columns && j < (int)line.length(); j++)
                {
                    grid[i][j] = line[j];
                }
            }
        }
        else if (key == "SWITCHES:" || FoundSwitchesInMap)
        {
            FoundSwitchesInMap = false;

            while (file >> key)
            {

                bool isTrainHeader = true;
                if ((int)key.length() < 6)
                    isTrainHeader = false;
                else
                {
                    if (key[0] != 'T')
                        isTrainHeader = false;
                    if (key[1] != 'R')
                        isTrainHeader = false;
                    if (key[2] != 'A')
                        isTrainHeader = false;
                    if (key[3] != 'I')
                        isTrainHeader = false;
                    if (key[4] != 'N')
                        isTrainHeader = false;
                    // We don't check 'S' or ':' to be safe against typos/ghosts
                }

                if (isTrainHeader)
                {

                    key = "TRAINS:";
                    break;
                }

                int index = getSwitchIndex(key[0]);

                if (index >= 0)
                {
                    string mode, lable0, lable1;
                    int S_initial_Value, k_up, k_right, k_left, k_down;

                    file >> mode >> S_initial_Value >> k_up >> k_right >> k_down >> k_left >> lable0 >> lable1;

                    if (mode == "GLOBAL")
                        switch_data[index][S_Mode] = 1;
                    else
                        switch_data[index][S_Mode] = 0;

                    switch_data[index][S_State] = S_initial_Value;
                    switch_data[index][S_K_up] = k_up;
                    switch_data[index][S_K_right] = k_right;
                    switch_data[index][S_K_down] = k_down;
                    switch_data[index][S_K_left] = k_left;
                    switch_data[index][S_K_golbal] = k_up; // Global limit

                    if (lable0 == "STRAIGHT")
                        switch_data[index][S_VisualType] = 0;
                    else
                        switch_data[index][S_VisualType] = 1;
                }
            }
        }

        bool isTrainKey = false;
        if ((int)key.length() >= 6 && key[0] == 'T' && key[1] == 'R' && key[2] == 'A')
            isTrainKey = true;

        if (key == "TRAINS:" || isTrainKey)
        {

            int tick, x, y, direction, colour;
            while (file >> tick >> x >> y >> direction >> colour)
            {
                int i = Number_Of_Trains;
                if (i < Trains_max)
                {
                    trains_data[i][TrainID] = i;
                    trains_data[i][TrainTICK] = tick;
                    trains_data[i][T_x] = x;
                    trains_data[i][T_y] = y;
                    trains_data[i][T_direction] = direction;
                    trains_data[i][T_colour] = colour;
                    trains_data[i][T_status] = 0;
                    trains_data[i][T_des_X] = -1;
                    trains_data[i][T_des_Y] = -1;

                    Number_Of_Trains++;
                }
            }
        }
    }

    file.close();

    return true;
}

// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles()
{
    ofstream trace("out/trace.csv");
    trace << "Tick,TrainID,x,y,Direction,State\n";
    trace.close();

    ofstream Switchstate("out/switches.csv");
    Switchstate << "Tick,Switch,Mode,State\n";
    Switchstate.close();

    ofstream signals("out/signals.csv");
    signals << "Tick,Switch,Scolor\n";
    signals.close();

    ofstream metrics("out/metrics.txt");
    metrics.close();
}

// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace()
{
    ofstream trace("out/trace.csv", ios::app);
    if (!trace.is_open())
        return;

    for (int i = 0; i < Number_Of_Trains; i++)
        if (trains_data[i][T_status] > 0)
            trace << tick << "," << trains_data[i][TrainID] << "," << trains_data[i][T_x] << "," << trains_data[i][T_y] << "," << trains_data[i][T_direction] << "," << trains_data[i][T_status] << "\n";
    trace.close();
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState()
{
    ofstream Switchstate("out/switches.csv", ios::app);
    if (!Switchstate.is_open())
        return;

    for (int i = 0; i < MAX_switches; i++)
    {
        if (switch_data[i][S_K_up] > 0 || switch_data[i][S_Mode] == 1)
        {
            Switchstate << tick << "," << char('A' + i) << "," << switch_data[i][S_Mode] << "," << switch_data[i][S_State] << "\n";
        }
    }
    Switchstate.close();
}

// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState()
{
    ofstream signals("out/signals.csv", ios::app);
    if (!signals.is_open())
        return;

    for (int i = 0; i < MAX_switches; i++)
    {
        if (switch_data[i][S_K_up] > 0 || switch_data[i][S_Mode] == 1)
        {
            int colour = switch_data[i][S_signal]; // Fixed variable case
            signals << tick << "," << char('A' + i) << "," << colour << "\n";
        }
    }
    signals.close();
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics()
{
    ofstream metrics("out/metrics.txt", ios::app);
    if (metrics.is_open())
    {
        metrics << "Total Trains: " << Number_Of_Trains << "\n";
        metrics << "Arrived: " << trains_arrived << "\n";
        metrics << "Crashed: " << trains_crashed << "\n";
        metrics << "Flips: " << total_switch_flips << "\n";
        metrics.close();
    }
}