#ifndef _FONT_H_
#define _FONT_H_

#include <SDL_ttf.h>

struct SDL_Renderer;
struct SDL_Color;

/// @brief Class to display text
///
class CFont
{
private:
	/// @brief SDL Font Handle
	///
	TTF_Font* m_pFont;
public:
	/// @brief Constructor to initialize a Font
	///
	/// @param Path to the Image
	/// @param Renderer needed to initialize
	/// @param Size
	////
	CFont(const char* a_pPath,int a_Size);
	/// @brief Destructor to destroy a Font
	///
	~CFont();
public:
    /// @brief Draw a Text at the Position X/Y
    ///
    /// @param Text to be drawn
	/// @param Renderer needed to draw
    /// @param X Position
    /// @param Y Position
    /// @param Color
    ///
    void Draw(const char* a_pText,SDL_Renderer* a_pRenderer,int a_X,int a_Y,SDL_Color a_Color); 
};

#endif /* _FONT_H_ */