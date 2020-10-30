#include "include/player.h"
#include "include/image.h"
#include "include/astar.h"
#include "include/grid.h"
#include <SDL.h>
#include <cmath>


CPlayer::CPlayer(float a_X,float a_Y) : m_X(a_X),m_Y(a_Y),
                                        m_CurrentIdx(0),m_TargetIdx(0),
                                        m_Lifes(3),m_Side(0),m_Animation(0),m_AnimationCooldown(0),m_Flash(0),m_Points(0)
{
    //We initialize it once so we can access it at any time
    this->m_pPath = new CAStar();
    this->m_Address.host = 0;
    this->m_Address.port = 0;
}

//*********************************************************************//
//*********************************************************************//

CPlayer::~CPlayer()
{
    delete this->m_pPath;
}
//*********************************************************************//
//*********************************************************************//

unsigned int CPlayer::GetLastUpdate() const
{
    return this->m_LastUpdate;
}
//*********************************************************************//
//*********************************************************************//

void CPlayer::SetLastUpdate(unsigned int a_LastUpdate)
{
    this->m_LastUpdate = a_LastUpdate;
}
//*********************************************************************//
//*********************************************************************//

IPaddress CPlayer::GetIP() const
{
    return this->m_Address;
}
//*********************************************************************//
//*********************************************************************//

void CPlayer::SetIP(IPaddress a_Address)
{
    this->m_Address = a_Address;
}
//*********************************************************************//
//*********************************************************************//

int CPlayer::GetX() const
{
    return (int)this->m_X;
}

//*********************************************************************//
//*********************************************************************//

int CPlayer::GetY() const
{
    return (int)this->m_Y;
}

//*********************************************************************//
//*********************************************************************//

void CPlayer::Update(CGrid*,float a_DeltaTime)
{
        if(this->m_Flash > 0)
            this->m_Flash -= a_DeltaTime/1000.0f;
        else
            this->m_Flash = 0.0f;

    //only update if he moved
    if(this->m_CurrentIdx != this->m_TargetIdx)
    {
        int tx,ty;
        if(this->m_pPath->GetPath(this->m_CurrentIdx,tx,ty))
        {
            //Set speed based on deltatime
            float speed = a_DeltaTime*0.1f;
            //Set Animation Facing
            if(ty > this->m_Y)
                this->m_Side = 0;
            else if(ty < this->m_Y)
                this->m_Side = 3;
            else if(tx > this->m_X)
                this->m_Side = 2;
            else
                this->m_Side = 1;
            //Set Animation State
            if(this->m_AnimationCooldown <= 0)
            {
                this->m_Animation += a_DeltaTime*0.1f;

                while(this->m_Animation > 2)
                    this->m_Animation -= 2;
                this->m_AnimationCooldown = 5.0f;
            }
            else
                this->m_AnimationCooldown -= a_DeltaTime*0.1f;
            //Update Position
            //Move in X
            float mx = (float)tx;
            float my = (float)ty;
            if(tx > this->m_X)
            {
                this->m_X += speed;
                if(this->m_X > tx)
                    this->m_X = mx;
            }
            else if(tx < this->m_X)
            {
                this->m_X -= speed;
                if(this->m_X < tx)
                    this->m_X = mx;
            }
            //Move in Y
            if(ty > this->m_Y)
            {
                this->m_Y += speed;
                if(this->m_Y > ty)
                    this->m_Y = my;
            }
            else if(ty < this->m_Y)
            {
                this->m_Y -= speed;
                if(this->m_Y < ty)
                    this->m_Y = my;
            }

            if(this->m_X == mx&&this->m_Y == my)
                this->m_CurrentIdx++;
        }
    }
    else
    {
        this->m_Animation = 0;
        this->m_AnimationCooldown = 0;
    }
}

//*********************************************************************//
//*********************************************************************//

int CPlayer::GetLifes() const
{
    return this->m_Lifes;
}

//*********************************************************************//
//*********************************************************************//

int CPlayer::GetPoints() const
{
    return this->m_Points;
}

//*********************************************************************//
//*********************************************************************//

void CPlayer::Hit()
{
    this->m_Flash = 3.0f;
    this->m_Lifes--;
    if(this->m_Lifes < 0)
        this->m_Lifes = 0;

}

//*********************************************************************//
//*********************************************************************//

void CPlayer::Points()
{
    this->m_Points += 100;

}

//*********************************************************************//
//*********************************************************************//

void CPlayer::Move(CGrid* a_pGrid,int a_X,int a_Y)
{
    int start_x,start_y;
    //If he is already moving, we set his start/first point to the point he aims to, so he does not walk back
    if(this->m_CurrentIdx != this->m_TargetIdx)
        this->m_pPath->GetPath(this->m_CurrentIdx,start_x,start_y);
    else
    {
        //we start from the center so he doesn't bug anywhere
        float x = this->m_X+(TileWidth/2);
        float y = this->m_Y+(TileHeight/2);
        //move player to the tile, not exactly where he clicked
        start_x = ((int)(x/TileWidth))*TileWidth;
        start_y = ((int)(y/TileHeight))*TileHeight;
    }
    //start finding
    this->m_CurrentIdx = 0;
    this->m_TargetIdx = this->m_pPath->FindPath(a_pGrid,start_x,start_y,((int)(a_X/TileWidth))*TileWidth,((int)(a_Y/TileHeight))*TileHeight);
}

//*********************************************************************//
//*********************************************************************//

void CPlayer::Draw(SDL_Rect& a_pScreen, CImage* a_pImage,SDL_Renderer* a_pRenderer)
{
    SDL_Rect img;
    img.x = (int)this->m_Animation*TileWidth;
    img.y = this->m_Side*TileHeight;
    img.w = a_pScreen.w;
    img.h = a_pScreen.h;
    if(this->m_Flash == 0||(int)(this->m_Flash*10) % 2)
        a_pImage->Draw(&img,&a_pScreen,a_pRenderer);
}

//*********************************************************************//
//*********************************************************************//

void CPlayer::SetState(SPlayerData& a_pData)
{
    this->m_X = (float)a_pData.X;
    this->m_Y = (float)a_pData.Y;
    this->m_Animation = (float)a_pData.Animation;
    this->m_Side = a_pData.Side;
    this->m_Lifes = a_pData.Lifes;
    this->m_Points = a_pData.Points;
    this->m_Flash = a_pData.Flash/100.0f;
}

//*********************************************************************//
//*********************************************************************//

void CPlayer::GetState(SPlayerData& a_pData)
{
    a_pData.X = (unsigned int)this->m_X;
    a_pData.Y = (unsigned int)this->m_Y;
    a_pData.Animation = (char)((int)this->m_Animation);
    a_pData.Side = (char)this->m_Side;
    a_pData.Lifes = (char)this->m_Lifes;
    a_pData.Points = (unsigned int)this->m_Points;
    a_pData.Flash = (unsigned int)(this->m_Flash*100);
}