#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
#include<iostream>
#include<string>
using namespace std;

// ============================================================================
// APP.CPP - Implementation of SFML application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES FOR APP STATE
// ----------------------------------------------------------------------------
static sf::RenderWindow* g_window = nullptr;
static sf::Font g_font;
static sf::Text g_text;
static sf::Sprite g_sastaspr;
static sf::Sprite g_background;
static sf::Texture bgtext;
static sf::Texture strtile;
static sf::Texture uptile;
static sf::Texture junction;
static sf::Texture lefttrain;
static sf::Texture righttrain;
static sf::Texture uptrain;
static sf::Texture downtrain;
static sf::Texture Spawn;
static sf::Texture Dest;
static sf::Texture sft;
static sf::Texture swich;
static sf::Sprite g_tile;

// View for camera (panning/zoom)
static sf::View g_camera;

// Simulation state
static bool g_isPaused = false;
static bool g_isStepMode = false;
static float g_tickrate= 0.5; 

// Mouse state
static bool g_isDragging = false;
static int g_lastMouseX = 0;
static int g_lastMouseY = 0;

// Grid rendering parameters
static float g_cellSize = 40.0f;
static float g_gridOffsetX = 50.0f;
static float g_gridOffsetY = 50.0f;

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// This function will initialize the SFML application window and resources.
// It creates a render window with a specified size and title, sets the
// framerate limit, attempts to load a font file for text rendering, and
// initializes the camera view. Returns true on success, false on failure.
// This should be called once at the start of the application before entering
// the main loop.
// ----------------------------------------------------------------------------
bool initializeApp() {
    g_window=new sf::RenderWindow(sf::VideoMode(800,600),"Switchback Rails");
    
    if(!bgtext.loadFromFile("data/WhatsApp Image 2025-11-30 at 3.48.17 PM.jpeg"))
    {
        return 1;
    }
     g_background.setTexture(bgtext);   
        sf::Vector2f size = g_camera.getSize();

    g_background.setScale(
    size.x / bgtext.getSize().x,
    size.y / bgtext.getSize().y
    );

    if(!(strtile.loadFromFile("Sprites/seedhitile-removebg-preview.png")&&
    uptile.loadFromFile("Sprites/uptile-removebg-preview.png")&&
    junction.loadFromFile("Sprites/junction-removebg-preview.png")
    &&lefttrain.loadFromFile("Sprites/lefttrain-removebg-preview.png")&&
    righttrain.loadFromFile("Sprites/right-removebg-preview.png")&&
    downtrain.loadFromFile("Sprites/downtrain-removebg-preview.png")&&
    uptrain.loadFromFile("Sprites/up-removebg-preview.png")&&
    Spawn.loadFromFile("Sprites/S-removebg-preview.png")&&
    Dest.loadFromFile("Sprites/D-removebg-preview.png")&&
    sft.loadFromFile("Sprites/safety-removebg-preview.png")&&
    swich.loadFromFile("Sprites/switch-removebg-preview.png")))
    {
        cout<<"Error Loading someshi"<<endl;
        return 1;
    }
    g_text.setFont(g_font);
    g_text.setCharacterSize(24);
    g_text.setFillColor(sf::Color::White);
     sf::Vector2u texSize = uptile.getSize(); 
    float scaleX = g_cellSize / texSize.x;
    float scaleY = g_cellSize / texSize.y;

    g_tile.setScale(scaleX, scaleY);
    g_tile.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);

    sf::Vector2u trainSize = uptrain.getSize(); 
    float tScaleX = g_cellSize / trainSize.x;
    float tScaleY = g_cellSize / trainSize.y;

    g_sastaspr.setScale(tScaleX, tScaleY);

    g_sastaspr.setOrigin(trainSize.x / 2.0f, trainSize.y / 2.0f);
    g_camera.setSize(800, 600);
     g_camera.setCenter(800, 350);
    g_window->setFramerateLimit(60);


    if(g_window==nullptr)
    {
        cout<<"Error opening the window. Aborting program...."<<endl;
        return false;
    }
    if(!(g_font.loadFromFile("data/levels/BitBlox_Monospaced.otf")))
    {
        cout<<"Error Loading Fonts!!! Aborting Program....."<<endl;
        return false;
    }

    return true;
}
void drawgrid()
{   
    
    for(int y=0;y<grid_rows;y++)
    {   
        g_tile.setColor(sf::Color::White);
        for(int x=0;x<grid_columns;x++)
        {   
        char tile=grid[y][x];
            if(isTrackTile(tile)){
                float pixelX = (x * g_cellSize) + g_gridOffsetX;
                float pixelY = (y * g_cellSize) + g_gridOffsetY;
                if(isSwitchTile(grid[y][x]))
                { 
                    g_tile.setTexture(Spawn);
            }
            else if(tile=='S')
                g_tile.setTexture(swich);
            else if(tile=='D')
                g_tile.setTexture(Dest);
            else if(tile=='-')
                g_tile.setTexture(strtile);
            else if(tile == '=')
                g_tile.setTexture(sft);
            else if(tile == '+')
                g_tile.setTexture(junction);
            else if(tile == '|')
                g_tile.setTexture(uptile);        
          

            g_tile.setPosition(pixelX,pixelY);
            g_window->draw(g_tile);
            
           }
        }

        }
    }
    void drawtrains()
    {
        float P_x,P_y;
        for(int i=0;i<Number_Of_Trains;i++){
            switch(trains_data[i][T_direction])
            {
                case 0:
                g_sastaspr.setTexture(uptrain);
                break;
                case 1:
                g_sastaspr.setTexture(righttrain);
                break;
                case 2:
                g_sastaspr.setTexture(downtrain);
                break;
                case 3:
                g_sastaspr.setTexture(lefttrain);
                break;
                default:
                break;

            }
            

            
            if(trains_data[i][T_status]==0||trains_data[i][T_status]>2)
            {
                continue;
            }
            else if(trains_data[i][T_status]==1)
            {
                P_x=(trains_data[i][T_x]*g_cellSize)+g_gridOffsetX+20.0f;
                P_y=(trains_data[i][T_y]*g_cellSize)+g_gridOffsetY+20.0f;
                switch(trains_data[i][T_colour]%3)
                {
                    case 0:
                    g_sastaspr.setColor(sf::Color::Blue);
                    break;
                    case 1:
                    g_sastaspr.setColor(sf::Color::Red);
                    break;
                    case 2:
                    g_sastaspr.setColor(sf::Color::Green);
                    break;
                    default:
                    g_sastaspr.setColor(sf::Color::Blue);
                    break; 

                }
                
                g_sastaspr.setPosition(P_x,P_y);
                g_window->draw(g_sastaspr);
            }
            else if(trains_data[i][T_status]==2)
            {
                P_x=(trains_data[i][T_x]*g_cellSize)+g_gridOffsetX+20.0f;
                P_y=(trains_data[i][T_y]*g_cellSize)+g_gridOffsetY+20.0f;
                g_sastaspr.setColor(sf::Color::White);
                g_sastaspr.setPosition(P_x,P_y);
                g_window->draw(g_sastaspr);

            }

        }

    }



// ----------------------------------------------------------------------------
// MAIN RUN LOOP
// ----------------------------------------------------------------------------
// This function will run the main application loop. It handles event processing,
// simulation updates, and rendering. The loop continues while the window is open.
// It processes SFML events (window close, keyboard input, mouse input), updates
// the simulation at a fixed interval (2 ticks per second) when not paused,
// checks if the simulation is complete, and renders the current frame. Keyboard
// controls: SPACE to pause/resume, PERIOD to step one tick, ESC to exit. The
// loop exits when the window is closed or ESC is pressed.
// ----------------------------------------------------------------------------
void runApp() {
    // sf::CircleShape sastaspr(10);
    // sastaspr.setFillColor(sf::Color::Red);
    float time_counter=0.0f;


    sf::Clock clock;
    while (g_window->isOpen())
    {
        float dt = clock.restart().asSeconds();
        time_counter +=dt;
        if((time_counter>=g_tickrate&&!g_isPaused)||g_isStepMode)
        {
            simulateOneTick();
            time_counter=0.0;
            if(g_isStepMode)
            {
                g_isStepMode=false;
            }
        }
        sf::Event event;
        while (g_window->pollEvent(event))
        {
            // if(time_counter>=g_tickrate&&!g_isPaused)
            // {
            //     simulateOneTick();
            //     time_counter=0.0;
            // }    
           if(event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code){
                    case sf::Keyboard::Space:
            
                        if(g_isPaused)
                        g_isPaused=false;
                        else
                        g_isPaused=true;
                        break;
                    
                    case sf::Keyboard::Period:
                        g_isStepMode=true;
                        break;
                    case sf::Keyboard::Escape:
                        g_window->close();
                        break;
                    default:
                    break;            
                }
            }
            else if(event.type==sf::Event::MouseWheelScrolled){
               if(event.mouseWheelScroll.delta > 0)
                 g_camera.zoom(0.5);
                 else
                 g_camera.zoom(2.00f);
                 
                }
            else if(event.type==sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePixelPos= {event.mouseButton.x, event.mouseButton.y};
                 sf::Vector2f worldpos= g_window->mapPixelToCoords(mousePixelPos, g_camera);
                 int tile_x=(worldpos.x-g_gridOffsetX)/g_cellSize;
                 int tile_y=(worldpos.y-g_gridOffsetY)/g_cellSize;
                 if(isInBounds(tile_x,tile_y)){
                     char tl=grid[tile_y][tile_x];
                     if(event.mouseButton.button==sf::Mouse::Left)
                {   
                    if(tl == '-' || tl == '=')
                    {
                        toggleSafetyTile(tile_x,tile_y);
                        

                    }
                }
                else if(event.mouseButton.button==sf::Mouse::Right)
                {
                    
                    int ind=getSwitchIndex(tl);
                    toggleSwitchState(ind);

                }

            }
        }
                if (event.type == sf::Event::Closed)
                g_window->close();
                
            }
            string counter=to_string(tick);
            g_text.setString(counter);
            
            g_window->clear();
            g_window->setView(g_window->getDefaultView());
            g_window->draw(g_background);
            g_window->setView(g_camera);
            drawgrid();
            drawtrains();
            g_window->setView(g_window->getDefaultView());
            g_window->draw(g_text);
            g_window->display();
    }
}

// ----------------------------------------------------------------------------
// CLEANUP
// ----------------------------------------------------------------------------
// This function will clean up all resources and close the application window.
// It deletes the render window object and sets the pointer to nullptr. This
// should be called once at the end of the application before exiting to ensure
// proper resource cleanup.
// ----------------------------------------------------------------------------
void cleanupApp() {
    if(g_window)
    {
        delete g_window;
        g_window=nullptr;

    }
}

