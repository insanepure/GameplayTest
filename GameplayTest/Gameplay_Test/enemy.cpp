#include "include/enemy.h"
#include "include/image.h"
#include "include/grid.h"
#include "include/player.h"
#include <SDL.h>


CEnemy::CEnemy(float a_X,float a_Y) : m_X(a_X),m_Y(a_Y),m_Side(0),m_Animation(0),m_AnimationCooldown(0),m_Alive(true)
{
}

//*********************************************************************//
//*********************************************************************//

CEnemy::~CEnemy()
{
}

//*********************************************************************//
//*********************************************************************//

int CEnemy::GetX() const
{
    return (int)this->m_X;
}

//*********************************************************************//
//*********************************************************************//

int CEnemy::GetY() const
{
    return (int)this->m_Y;
}

//*********************************************************************//
//*********************************************************************//

bool CEnemy::IsAlive() const
{
    return this->m_Alive;
}

//*********************************************************************//
//*********************************************************************//

void CEnemy::Update(CGrid* a_pGrid,float a_DeltaTime,CPlayer* a_pPlayer)
{
    //only update if he moved
    if(a_pPlayer != NULL)
    {
        int pX = a_pPlayer->GetX();
        int pY = a_pPlayer->GetY();
        //Set speed based on deltatime
        float speed = a_DeltaTime*0.05f;
        //Set Animation Facing
        if(pY > this->m_Y)
            this->m_Side = 0;
        else if(pY < this->m_Y)
            this->m_Side = 3;
        else if(pX > this->m_X)
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

        //Move in X
        if(pX > this->m_X)
        {
            this->m_X += speed;
            if(this->m_X > pX)
                this->m_X = (float)pX;
        }
        else if(pX < this->m_X)
        {
            this->m_X -= speed;
            if(this->m_X < pX)
                this->m_X = (float)pX;
        }
        //Move in Y
        if(pY > this->m_Y)
        {
            this->m_Y += speed;
            if(this->m_Y > pY)
                this->m_Y = (float)pY;
        }
        else if(pY < this->m_Y)
        {
            this->m_Y -= speed;
            if(this->m_Y < pY)
                this->m_Y = (float)pY;
        }

        if(this->m_X < pX+TileWidth && 
        this->m_X+TileWidth > pX && 
        this->m_Y < pY+TileHeight &&
        this->m_Y+TileHeight > pY)
        {
            //he hit him!
            a_pPlayer->Hit();
            this->m_Alive = false;
        }

        int offset = 5;
        int mx = (int)this->m_X;
        int my = (int)this->m_Y;
        if(a_pGrid->GetTile(mx+offset,my+offset) == Tile::Trap
         ||a_pGrid->GetTile(mx+TileWidth-offset,my+offset) == Tile::Trap
         ||a_pGrid->GetTile(mx+offset,my+TileHeight-offset) == Tile::Trap
         ||a_pGrid->GetTile(mx+TileWidth-offset,my+TileHeight-offset) == Tile::Trap)
        {
            a_pPlayer->Points();
            this->m_Alive = false;
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

void CEnemy::Draw(SDL_Rect& a_pScreen, CImage* a_pImage,SDL_Renderer* a_pRenderer)
{
    SDL_Rect img;
    img.x = (int)this->m_Animation*TileWidth;
    img.y = this->m_Side*TileHeight;
    img.w = a_pScreen.w;
    img.h = a_pScreen.h;
    a_pImage->Draw(&img,&a_pScreen,a_pRenderer);
}

//*********************************************************************//
//*********************************************************************//

void CEnemy::SetState(SEnemyData& a_pData)
{
    this->m_X = (float)a_pData.X;
    this->m_Y = (float)a_pData.Y;
    this->m_Animation = (float)a_pData.Animation;
    this->m_Side = a_pData.Side;
}

//*********************************************************************//
//*********************************************************************//

void CEnemy::GetState(SEnemyData& a_pData)
{
    a_pData.X = (unsigned int)this->m_X;
    a_pData.Y = (unsigned int)this->m_Y;
    a_pData.Animation = (char)((int)this->m_Animation);
    a_pData.Side = (char)this->m_Side;
}   