#ifndef _ENEMY_H_
#define _ENEMY_H_

class CPlayer;
class CImage;
class CGrid;
struct SDL_Renderer;
struct SDL_Rect;

/// @brief Data of the Enemy we send over Network
///
struct SEnemyData
{
    unsigned int X;
    unsigned int Y;
    char Side;
    char Animation;
};

/// @brief Enemy that a player encounters
///
class CEnemy
{
private:
    /// @brief X Position
    ///
    float m_X;
    /// @brief Y Position
    ///
    float m_Y;
    /// @brief Which Animation is played
    ///
    float m_Animation;
    /// @brief Cooldown when the next animation is played
    ///
    float m_AnimationCooldown;
    /// @brief Which Side he is facing
    ///
    int m_Side;
    /// @brief True if alive, false if not
    ///
    bool m_Alive;

public:
	/// @brief Constructor to initialize an Enemy
	///
	/// @param Starting X
	/// @param Starting Y
	////
	CEnemy(float a_X,float a_Y);
	/// @brief Destructor to destroy an Enemy
	///
	~CEnemy();
public:
	/// @brief Set the actual State of the Enemy
    ///
    /// @param Data of the Enemy
    ///
    void SetState(SEnemyData& a_pData);
	/// @brief Get the actual State of the Enemy
    ///
    /// @param Data of the Enemy
    ///
    void GetState(SEnemyData& a_pData);
	/// @brief Update the Enemy
    ///
    /// @param Gridd for Traps
    /// @param DeltaTime in milliseconds
    /// @param Target Player, NULL of None
	///
    void Update(CGrid* a_pGrid,float a_DeltaTime,CPlayer* a_pPlayer);
	/// @brief Return the X Position of the Enemy
	///
    /// @returns X as Int
    ///
    int GetX() const;
	/// @brief Return the Y Position of the Enemy
	///
    /// @returns Y as Int
    ///
    int GetY() const;
	/// @brief Draw the Enemy
	///
	/// @param ScreenRect, defined outside for culling
	/// @param Image to draw the player with
	/// @param Renderer to use
    ///
    void Draw(SDL_Rect& a_pScreen,CImage* a_pImage,SDL_Renderer* a_pRenderer);
    
	/// @brief Check if the Enemy is alive or not
	///
    /// @returns true if alive, false if not
    ///
    bool IsAlive() const;
};

#endif /* _ENEMY_H_ */