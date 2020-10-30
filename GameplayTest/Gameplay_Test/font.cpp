#include "include/font.h"

#include <SDL.h>


CFont::CFont(const char* a_pPath,int a_Size)
{
    this->m_pFont = TTF_OpenFont(a_pPath, a_Size);
}

//*********************************************************************//
//*********************************************************************//

CFont::~CFont()
{
    TTF_CloseFont(this->m_pFont);
}

//*********************************************************************//
//*********************************************************************//

void CFont::Draw(const char* a_pText,SDL_Renderer* a_pRenderer,int a_X,int a_Y,SDL_Color a_Color)
{
    //Create a Surface from which we create the texture to draw
	SDL_Surface* pSurface = TTF_RenderText_Blended(this->m_pFont, a_pText, a_Color);
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(a_pRenderer, pSurface);
    //define the drawing RECT
    SDL_Rect screen;
    screen.x = a_X;
    screen.y = a_Y;
    SDL_QueryTexture(pTexture, NULL, NULL, &screen.w, &screen.h);
    // copy the texture to the rendering context
    SDL_RenderCopy(a_pRenderer,pTexture, NULL, &screen);
	//Clean up the surface and Texture
	SDL_FreeSurface(pSurface);
    SDL_DestroyTexture(pTexture);
}