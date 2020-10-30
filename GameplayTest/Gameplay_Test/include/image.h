#ifndef _IMAGE_H_
#define _IMAGE_H_

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

/// @brief Class to display an image
///
class CImage
{
private:
	/// @brief SDL Texture Handle
	///
	SDL_Texture* m_pTexture;
	/// @brief Width of the Image
	///
	int m_Width;
	/// @brief Height of the Image
	///
	int m_Height;
public:
	/// @brief Constructor to initialize an Image
	///
	/// @param Path to the Image
	/// @param Renderer needed to initialize
	////
	CImage(const char* a_pPath,SDL_Renderer* a_pRenderer);
	/// @brief Destructor to destroy an Image
	///
	~CImage();
public:
	/// @brief Return the Width of the Window
	///
	/// @returns Width as unsigned int
	///
	int GetWidth() const;
	/// @brief Return the Height of the Window
	///
	/// @returns Height as unsigned int
	///
	int GetHeight() const;
    /// @brief Draw the Image at Position
    ///
    /// @param Rect for the Image itself, NULL if FULL
    /// @param Rect for the Image onScreen
	/// @param Renderer needed to draw
    ///
    void Draw(SDL_Rect* imageRect, SDL_Rect* screenRect,SDL_Renderer* a_pRenderer); 
};

#endif /* _IMAGE_H_ */