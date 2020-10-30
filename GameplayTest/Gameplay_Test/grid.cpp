#include "include/grid.h"
#include "include/image.h"
#include "include/camera.h"

#include <SDL.h>
#include <cstring>


CGrid::CGrid()
{
    for(int y=0;y<GridHeight;y++)
    {
        for(int x=0;x<GridWidth;x++)
        {
            int idx = (GridWidth * y)+x;
            this->m_Tiles[idx] = Tile::None;
            if(x % 4 == 0&&y % 4 == 0)
                this->m_Tiles[idx] = Tile::Trap;
        }
    }
}

//*********************************************************************//
//*********************************************************************//

CGrid::~CGrid()
{
}
//*********************************************************************//
//*********************************************************************//

Tile CGrid::GetTile(int a_X,int a_Y)
{
    a_Y = a_Y/TileHeight;
    a_X = a_X/TileWidth;
    int idx = (GridWidth*a_Y)+a_X;
    return (Tile)this->m_Tiles[idx];
}
//*********************************************************************//
//*********************************************************************//

void CGrid::SetTile(int a_X,int a_Y)
{
    a_Y = a_Y/TileHeight;
    a_X = a_X/TileWidth;
    int idx = (GridWidth*a_Y)+a_X;
    if(this->m_Tiles[idx] == Tile::None)
        this->m_Tiles[idx] = Tile::Trap;
    else
        this->m_Tiles[idx] = Tile::None;
}
//*********************************************************************//
//*********************************************************************//

void CGrid::Draw(CCamera* a_pCamera,CImage* a_pImage,SDL_Renderer* a_pRenderer)
{

    SDL_Rect screen;
    screen.w = TileWidth;
    screen.h = TileHeight;
    SDL_Rect tile;
    tile.x = 0;
    tile.y = 0;
    tile.w = TileWidth;
    tile.h = TileHeight;
    //Render Map
    for(int y=0;y<GridHeight;y++)
    {
        for(int x=0;x<GridWidth;x++)
        {
            screen.x = x*TileWidth;
            screen.y = y*TileHeight;
            int idx = (GridWidth * y)+x;
            if(this->m_Tiles[idx] == Tile::Trap)
            {
                tile.x = TileWidth;
                tile.y = 0;
            }
            else
            {
                if(x % 2||y % 2)
                {
                    tile.x = 0;
                    tile.y = 0;
                }
                else if(x % 3)
                {
                    tile.x = 0;
                    tile.y = TileHeight;
                }
                else
                {
                    tile.x = TileWidth;
                    tile.y = TileHeight;
                }
            }
            if(a_pCamera->Intersect(screen))
            {
                a_pCamera->Subtract(screen);
                a_pImage->Draw(&tile,&screen,a_pRenderer);
            }
        }
    }
}

//*********************************************************************//
//*********************************************************************//

void CGrid::SetState(SGridData& a_pData)
{
    memcpy(this->m_Tiles,a_pData.Tiles,sizeof(SGridData));
}

//*********************************************************************//
//*********************************************************************//

void CGrid::GetState(SGridData& a_pData)
{
    memcpy(a_pData.Tiles,this->m_Tiles,sizeof(SGridData));
}   