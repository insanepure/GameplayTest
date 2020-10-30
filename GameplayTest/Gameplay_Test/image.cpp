#include "include/image.h"

#include <SDL.h>
#include <SDL_image.h>


CImage::CImage(const char* a_pPath,SDL_Renderer* a_pRenderer)
{
    this->m_pTexture = IMG_LoadTexture(a_pRenderer, a_pPath);
    SDL_QueryTexture(this->m_pTexture, NULL, NULL, &this->m_Width, &this->m_Height); // get the width and height of the texture
}

//*********************************************************************//
//*********************************************************************//

CImage::~CImage()
{
    SDL_DestroyTexture( this->m_pTexture);
}

//*********************************************************************//
//*********************************************************************//

int CImage::GetWidth() const
{
    return this->m_Width;
}

//*********************************************************************//
//*********************************************************************//

int CImage::GetHeight() const
{
    return this->m_Height;
}

//*********************************************************************//
//*********************************************************************//

void CImage::Draw(SDL_Rect* imageRect, SDL_Rect* screenRect,SDL_Renderer* a_pRenderer)
{
    // copy the texture to the rendering context
    SDL_RenderCopy(a_pRenderer, this->m_pTexture, imageRect, screenRect);
}