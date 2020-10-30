#ifndef _WINDOW_H_
#define _WINDOW_H_

///Forward declarations
struct SDL_Window;
struct SDL_Renderer;

/// @brief Class to display a Window
///
class CWindow
{
private:
	/// @brief SDL Window Handle
	///
	SDL_Window* m_pWindow;
	/// @brief SDL Renderer
	///
	SDL_Renderer* m_pRenderer;
	/// @brief Width of the Window
	///
	unsigned int m_Width;
	/// @brief Height of the Window
	///
	unsigned int m_Height;
public:
	/// @brief Constructor to initialize a Window
	///
	/// @param Width of the Window
	/// @param Height of the Window
	////
	CWindow(const unsigned int a_Width,const unsigned int a_Height);
	/// @brief Destructor to destroy a Window
	///
	~CWindow();
public:
	/// @brief Return the Width of the Window
	///
	/// @returns Width as unsigned int
	///
	unsigned int GetWidth() const;
	/// @brief Return the Height of the Window
	///
	/// @returns Height as unsigned int
	///
	unsigned int GetHeight() const;
	/// @brief Return the Renderer
	///
	/// @returns Pointer to the SDL_Renderer
	///
	SDL_Renderer* GetRenderer() const;
};

#endif /* _WINDOW_H_ */