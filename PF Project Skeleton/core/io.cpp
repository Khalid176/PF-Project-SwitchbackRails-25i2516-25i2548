#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <iostream>  // adding this to use cout and cin
using namespace std; // adding this to use fstream and iostream without std ::

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
        cout << "Error: Could not open the file";
        return false;
    }

    initializeSimulationState();

    string key; // for reading under which heading the data is given for example rows would be given under rows heading
    // Now to fix
    bool FoundSwitchesInMap = false;

    // BASIC READING
    while (FoundSwitchesInMap || (file >> key))
    {
        if (FoundSwitchesInMap)
        {
            key = "SWITCHES:";
        }

        if (key == "ROWS:")
        {
            file >> grid_rows;
        }
        else if (key == "COLS:")
        {
            file >> grid_columns;
        }
        else if (key == "SEED:")
        {
            file >> seed;
            
        }
        else if (key == "WEATHER:")
        {
            string temp;

            file >> temp;
            if (temp == "NORMAL")
            {
                weather = 0;
            }
            else if (temp == "FOG")
            {
                weather = 2;
            }
            else if (temp == "RAIN")
            {
                weather = 1;
            }
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
                {
                    start++;
                }
                bool IsSwitchHeader = true;
                if ((int)line.length() - start < 8)
                {
                    IsSwitchHeader = false;
                }
                else
                {
                    if (line[start + 0] != 'S')
                    {
                        IsSwitchHeader = false;
                    }
                    if (line[start + 1] != 'W')
                    {
                        IsSwitchHeader = false;
                    }
                    if (line[start + 2] != 'I')
                    {
                        IsSwitchHeader = false;
                    }
                    if (line[start + 3] != 'T')
                    {
                        IsSwitchHeader = false;
                    }
                    if (line[start + 4] != 'C')
                    {
                        IsSwitchHeader = false;
                    }
                    if (line[start + 5] != 'H')
                    {
                        IsSwitchHeader = false;
                    }
                    if (line[start + 6] != 'E')
                    {
                        IsSwitchHeader = false;
                    }
                    if (line[start + 7] != 'S')
                    {
                        IsSwitchHeader = false;
                    }
                }
                if (IsSwitchHeader)
                {
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
            FoundSwitchesInMap = false; // doing this to make sure the bool doesnot result in it being always true

            while (file >> key)
            {

                bool isTrainHeader = true;
                if ((int)key.length() < 6)
                {
                     isTrainHeader = false;
                }
                   
                else
                {
                    if (key[0] != 'T')
                    {
                        isTrainHeader = false;
                    }
                    if (key[1] != 'R')
                    {
                        isTrainHeader = false;
                    }
                    if (key[2] != 'A')
                    {
                        isTrainHeader = false;
                    }
                    if (key[3] != 'I')
                    {
                        isTrainHeader = false;
                    }
                    if (key[4] != 'N')
                    {
                        isTrainHeader = false;
                    }
                    if (key[5] != 'S')
                    {
                        isTrainHeader = false;
                    }
                }
                if (isTrainHeader)
                {
                    key = "TRAINS:";
                    break;
                }

                int index = getSwitchIndex(key[0]); // the first switch would be called A so the value of row would be zero and for second switch the Alphabet would be B and value of the row would be 1 i.e using this to find out how many switches are there (through final value of it) and also using as if it is row number

                if (index >= 0)
                {
                    string mode, lable0, lable1;
                    int S_initial_Value, k_up, k_right, k_left, k_down; // using them just as local temps gave names other then temp to understand which is what
                    // Reading the switch lines now
                    file >> mode >> S_initial_Value >> k_up >> k_right >> k_down >> k_left >> lable0 >> lable1;

                    if (mode == "GLOBAL")
                    {
                        switch_data[index][S_Mode] = 1;
                    }
                    else
                    {
                        switch_data[index][S_Mode] = 0;
                    }

                    switch_data[index][S_State] = S_initial_Value;

                    switch_data[index][S_K_up] = k_up;
                    switch_data[index][S_K_right] = k_right;
                    switch_data[index][S_K_down] = k_down;
                    switch_data[index][S_K_left] = k_left;

                    switch_data[index][S_K_golbal] = k_up; // since the value would be same no matter what we use it would not effect the out come much

                    if (lable0 == "STRAIGHT")
                    {
                        switch_data[index][S_VisualType] = 0;
                    }
                    else
                    {
                        switch_data[index][S_VisualType] = 1;
                    }
                }
                
            }
        }
        if (key == "TRAINS:")                  // removing the else one because if the above loop breaks at the heading trains we would directly catch it here and would go in next segmant while else if would not allow us to do so
        {                                      // it would be different form the usual thing we were doing because in this case we donot know how many switches would be there so the loop would go on break at the headong = trains: and would then go for this if
            int tick, x, y, direction, colour; // just creating temporary variable for data storing so i can then send it into the array
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
                    trains_data[i][T_status] = 0; // showing that train hast spawed yet

                    //  For now we havent calculated the optimal destinations points for the train so using some place holders

                    trains_data[i][T_destination_X] = -1;
                    trains_data[i][T_destination_Y] = -1;

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
    ofstream trace("traintrace.csv");
    trace<<"Tick,TrainID,x,y,Direction,State\n";
    trace.close();

    ofstream Switchstate("switches.csv");
    trace<<"Tick,Switch,Mode,State";
    Switchstate.close();

    ofstream signals("signals.csv");
    signals<<"Tick,Switch,Scolor";
    signals.close();

    ofstream metrics("metrics.txt");
    // metrics<<"";
    metrics.out;
}

// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace()
{
    ofstream trace("traintrace.csv",ios::app);
    if(!trace.is_open()){
        cout<<"Error during loading of file";
        return ;
    }
    else{
        for(int i=0;i<Number_Of_Trains;i++)
     trace<<trains_data[i][TrainTICK]<<","<<trains_data[i][TrainID]<<","<<trains_data[i][T_x]<<","<<trains_data[i][T_y]<<","<<trains_data[i][T_direction]<<","<<trains_data[i][T_status];
    trace.close();}
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState()
{
    ofstream Switchstate("switches.csv",ios::app);
    if(!Switchstate.is_open()){
        cout<<"Error during file loading";
        return ;
    }
    else {
        for(int i=0;i<Number_Of_Trains;i++)
    Switchstate<<trains_data[i][TrainTICK]<<","<<char('A'+i)<<","<<switch_data[i][S_Mode]<<","<<switch_data[i][S_State];
    Switchstate.close();
}
}

// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState()
{
    ofstream signals("switches.csv",ios::app);
      if(!signals.is_open()){
        cout<<"Error during loading of file";
        return ;
    }
    else{
        for(int i=0;i<Number_Of_Trains;i++)
        signals<<trains_data[i][TrainTICK]<<","<<trains_data[i][TrainID]<<","<<char('A'+i)<<","<<trains_data[i][T_colour];
        signals.close();}
    
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics()
{
     ofstream metrics("metrics.txt",ios::app);
    // metrics<<"";
    metrics.out;
}