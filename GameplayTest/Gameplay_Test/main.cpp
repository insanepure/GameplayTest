#include "include/scene.h"
#include <SDL.h>
#undef main
#include <crtdbg.h>

/// @brief Scene that contains everything
///
CScene* pScene;
/// @brief Processes Input
///
/// @returns true if still running, false if end
///
inline bool ProcessInput()
{
    SDL_Event event;
    memset(&event, 0, sizeof(event)); //NULL event so we don't have garbage
    SDL_PollEvent(&event); //Fill event with the events from SDL

    //go through events
    switch (event.type)
    {
    case SDL_QUIT: //SDL calls Quit
        return false;
    case SDL_MOUSEBUTTONDOWN:
        int x;
        int y;
        //get point where he clicked
        SDL_GetMouseState(&x,&y);
        if (event.button.button == SDL_BUTTON_LEFT)
            pScene->MouseClick(Click::Left,x,y);
        else if (event.button.button == SDL_BUTTON_RIGHT)
            pScene->MouseClick(Click::Right,x,y);
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) //get keyboard input
        {
        case SDLK_ESCAPE: //ESC Pressed
            return false;
        case SDLK_F1: //Game Mode
            pScene->ChangeMode(Mode::Game);
            break;
        case SDLK_F2: //Trap Editor Mode
            pScene->ChangeMode(Mode::TrapEditor);
            break;
        case SDLK_F3: //Enemy Editor Mode
            pScene->ChangeMode(Mode::EnemyEditor);
            break;
        }
        break;

    default:
        break;
    }
    return true;
}


/// @brief Main Entry
///
int main()
{
    pScene = new CScene();

    //How often we want to update
    float DeltaTime = 1000.0f/60.0f;
    //Last Update Timer
    unsigned int LastUpdate = 0;
    /// @brief Time Difference
    unsigned int TimeDifference = 0;
    /// @brief Added Time
    ///
    float TimeAccumulation = 0;
    ////////////// MAIN LOOP ///////////
    while(ProcessInput())
    {

        //Calculate DeltaTime
        TimeDifference = (SDL_GetTicks() - LastUpdate);
        LastUpdate = SDL_GetTicks();

        TimeAccumulation += (float)TimeDifference;
        //we update only 60 times a second
        while(TimeAccumulation >= DeltaTime)
        {
            LastUpdate = SDL_GetTicks();
            TimeAccumulation -= DeltaTime;
            pScene->Update(DeltaTime);
        }
            pScene->Render();
    }


    delete pScene;

    ////////////// CHECK LEAKS ///////////
    int leaks = _CrtDumpMemoryLeaks();
    if(leaks != 0)
        printf("Memory Leaks detected!\n");

    return 0;
}