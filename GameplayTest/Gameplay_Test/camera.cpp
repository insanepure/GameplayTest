#include "include/camera.h"

#include <SDL.h>


CCamera::CCamera(unsigned int a_Width,unsigned int a_Height,SDL_Rect& a_pBoundaries)
{
    this->m_pRect = new SDL_Rect();
    this->m_pBoundaries = new SDL_Rect();
    //define Camera's Rectangle
    this->m_pRect->x = 0;
    this->m_pRect->y = 0;
    this->m_pRect->w = a_Width;
    this->m_pRect->h = a_Height;
    //define Game's Boundaries
    this->m_pBoundaries->x = a_pBoundaries.x;
    this->m_pBoundaries->y = a_pBoundaries.y;
    this->m_pBoundaries->w = a_pBoundaries.w;
    this->m_pBoundaries->h = a_pBoundaries.h;

}

//*********************************************************************//
//*********************************************************************//

CCamera::~CCamera()
{
    delete this->m_pRect;
    delete this->m_pBoundaries;
}

//*********************************************************************//
//*********************************************************************//

bool CCamera::Intersect(SDL_Rect& a_rOther)
{
    //Rectangle Intersection
    return this->m_pRect->x < a_rOther.x+a_rOther.w && 
        this->m_pRect->x+this->m_pRect->w > a_rOther.x && 
        this->m_pRect->y < a_rOther.y+a_rOther.h &&
        this->m_pRect->y+this->m_pRect->h > a_rOther.y; 
}

//*********************************************************************//
//*********************************************************************//

int CCamera::GetX() const
{
    return this->m_pRect->x;
}

//*********************************************************************//
//*********************************************************************//

int CCamera::GetY() const
{
    return this->m_pRect->y;
}

//*********************************************************************//
//*********************************************************************//

void CCamera::Subtract(SDL_Rect& a_rOther)
{
    a_rOther.x -= this->m_pRect->x;
    a_rOther.y -= this->m_pRect->y;
}

//*********************************************************************//
//*********************************************************************//

void CCamera::Move(int a_Player_X,int a_Player_Y)
{
    
    //Set boundaries for the Camera
    int maxX = this->m_pRect->x+this->m_pRect->w*3/5;
    int minX = this->m_pRect->x+this->m_pRect->w*1/5;
    //if player went past the boundaries, move camera
    if(a_Player_X > maxX)
        this->m_pRect->x = a_Player_X-this->m_pRect->w*3/5;
    else if(a_Player_X < minX)
        this->m_pRect->x = a_Player_X-this->m_pRect->w*1/5;
        
    //set boundaries for the camera
    int maxY = this->m_pRect->y+this->m_pRect->h*3/5;
    int minY = this->m_pRect->y+this->m_pRect->h*1/5;
    
    //if player went past the boundaries, move camera
    if(a_Player_Y > maxY)
        this->m_pRect->y = a_Player_Y-this->m_pRect->h*3/5;
    else if(a_Player_Y < minY)
        this->m_pRect->y = a_Player_Y-this->m_pRect->h*1/5;

    //set map's MAX and MIN X, so he does not go past a specific area
    if(this->m_pRect->x < this->m_pBoundaries->x)
        this->m_pRect->x = this->m_pBoundaries->x;
    else if(this->m_pRect->x+this->m_pRect->w > this->m_pBoundaries->x+this->m_pBoundaries->w)
        this->m_pRect->x = (this->m_pBoundaries->x+this->m_pBoundaries->w)-this->m_pRect->w;
    //set map's MAX and MIN Y, so he does not go past a specific area
    if(this->m_pRect->y < this->m_pBoundaries->y)
        this->m_pRect->y = this->m_pBoundaries->y;
    else if(this->m_pRect->y+this->m_pRect->h > this->m_pBoundaries->y+this->m_pBoundaries->h)
        this->m_pRect->y = (this->m_pBoundaries->y+this->m_pBoundaries->h)-this->m_pRect->h;
}