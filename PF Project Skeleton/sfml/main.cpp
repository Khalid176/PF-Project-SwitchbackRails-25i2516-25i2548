#include <iostream>
#include <unistd.h>
#include <string>
#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
using namespace std;


// ============================================================================
// MAIN.CPP - Entry point of the application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// MAIN ENTRY POINT
// ----------------------------------------------------------------------------
// This function is the main entry point of the application. It handles command
// line arguments to specify the level file to load, loads the level file using
// loadLevelFile, initializes the simulation system, initializes the SFML
// application window, prints control instructions to the console, runs the
// main application loop, cleans up resources, and prints final simulation
// statistics. Returns 0 on success, 1 on error (e.g., failed to load level
// file or initialize application).
// ----------------------------------------------------------------------------
int main(int fileparts,char* filename[]) {
    
    std::cout<<"****************************************************************"<<endl;
    std::cout<<"               SWITCH BACK RAILS IN THE CRIB YOO!                 "<<endl;
    std::cout<<"****************************************************************"<<endl;

    if(fileparts<2)
    {
        std::cout<<"Enter path name in this format: ,/switchback_rails"<<endl;
        std::cout<<"Wrong path entered!"<<endl;
        return 1;
    }
    std::string level=filename[1];
    std::cout<<"Loading Files";
    for(int i = 1; i <= 5; i++) {
        for(int j = 0; j < i; j++) {
            std::cout << ".";
        }
        std::cout<<endl;
        usleep(500000); 
    }
    bool levelauth=loadLevelFile(level);
        if(!levelauth)
        {
            std::cout<<"Error loading the files";
            return 1;
        }
    std::cout<<"Level loaded successfully! Initializing graphics in";
    for(int p=3;p>0;p--)
    {
        std::cout<<endl<<p;
        usleep(500000); 

    }    
    bool appauth=initializeApp();
        if(!appauth)
        {
            std::cout<<"Could not initialize the SFML app file";
            return 1;
        }
        else
        {
            runApp();
        }
        writeMetrics();
        cleanupApp();
        


        }
