#ifndef _CAMERA_H_
#define _CAMERA_H_

struct SDL_Rect;

/// @brief Camera used to display a specific area of the game
///
class CCamera
{
private:
    /// @brief View of the Camera
    ///
    /// @remarks pointer because we define SDL_Rect in the cpp
    SDL_Rect* m_pRect;
    /// @brief Boundaries of the Game
    ///
    SDL_Rect* m_pBoundaries;
public:
	/// @brief Constructor to initialize a Camera
	///
	/// @param Width of the Camera
	/// @param Height of the Camera
	/// @param Boundaries of the Game
	////
	CCamera(unsigned int a_Width,unsigned int a_Height,SDL_Rect& a_pBoundaries);
	/// @brief Destructor to destroy a Camera
	///
	~CCamera();
public:
	/// @brief Return the X Position of the Camera
	///
    /// @returns X as Int
    ///
    int GetX() const;
	/// @brief Return the Y Position of the Camera
	///
    /// @returns Y as Int
    ///
    int GetY() const;
	/// @brief Check for Intersection between the Camera and the other Rect
    ///
    /// @param Other Rect to check
    ///
    /// @returns bool if intersect, false if not
	///
    bool Intersect(SDL_Rect& a_rOther);
	/// @brief Subtract the Rect so its relative to the Camera
    ///
    /// @param Other Rect to subtract
	///
    void Subtract(SDL_Rect& a_rOther);
	/// @brief Move the Camera
    ///
    /// @param X of the Player
    /// @param Y of the Player
	///
    void Move(int a_Player_X,int a_Player_Y);
};

#endif /* _CAMERA_H_ */