#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL_net.h>

class CAStar;
class CImage;
class CGrid;
struct SDL_Renderer;
struct SDL_Rect;

/// @brief Data of the Player we send over Network
///
struct SPlayerData
{
    unsigned int X;
    unsigned int Y;
    char Side;
    char Animation;
    char Lifes;
    unsigned int Points;
    unsigned int Flash;
};

/// @brief Player controlled
///
class CPlayer
{
private:
    /// @brief Address for this player to know who he is (Server only)
    ///
    IPaddress m_Address;
    /// @brief Path Finding Algorithm
    ///
    CAStar* m_pPath;
    /// @brief X Position
    ///
    float m_X;
    /// @brief Y Position
    ///
    float m_Y;
    /// @brief Index of the Cell we are currently in
    ///
    int m_CurrentIdx;
    /// @brief Index of the Cell we are currently in
    ///
    int m_TargetIdx;
    /// @brief Which Animation is played
    ///
    float m_Animation;
    /// @brief Cooldown when the next animation is played
    ///
    float m_AnimationCooldown;
    /// @brief Which Side he is facing
    ///
    int m_Side;
    /// @brief Lifes the Player has
    ///
    int m_Lifes;
    /// @brief Points the Player has
    ///
    int m_Points;
    /// @brief Flash Time
    ///
    float m_Flash;
    /// @brief Time when the player got updated
    ///
    unsigned int m_LastUpdate;


public:
	/// @brief Constructor to initialize a Player
	///
	/// @param Starting X
	/// @param Starting Y
	////
	CPlayer(float a_X,float a_Y);
	/// @brief Destructor to destroy a Player
	///
	~CPlayer();
public:
	/// @brief Set the actual State of the Player
    ///
    /// @param Data of the Player
    ///
    void SetState(SPlayerData& a_pData);
	/// @brief Get the actual State of the Player
    ///
    /// @param Data of the Player
    ///
    void GetState(SPlayerData& a_pData);
	/// @brief Get the time the player last send something
    ///
    /// @returns Last Update
    ///
    unsigned int GetLastUpdate() const;
	/// @brief Set the time the player last send something
    ///
    /// @param Last Update
    ///
    void SetLastUpdate(unsigned int a_LastUpdate);
	/// @brief Get the IP Address
    ///
    /// @returns IP Address
    ///
    IPaddress GetIP() const;
	/// @brief Set the IP Address
    ///
    /// @param IP Address
    ///
    void SetIP(IPaddress a_Address);
	/// @brief Update the Player
    ///
    /// @param Gridd for Pathfinding
    /// @param DeltaTime in milliseconds
	///
    void Update(CGrid* a_pGrid,float a_DeltaTime);
	/// @brief Return the X Position of the Player
	///
    /// @returns X as Int
    ///
    int GetX() const;
	/// @brief Return the Y Position of the Player
	///
    /// @returns Y as Int
    ///
    int GetY() const;
	/// @brief Move the Player to Position X,Y
	///
    /// @param Gridd for Pathfinding
	/// @param Target X
	/// @param Target Y
    ///
    void Move(CGrid* a_pGrid,int a_X,int a_Y);
	/// @brief Draw the Player
	///
	/// @param ScreenRect, defined outside for culling
	/// @param Image to draw the player with
	/// @param Renderer to use
    ///
    void Draw(SDL_Rect& a_pScreen,CImage* a_pImage,SDL_Renderer* a_pRenderer);
	/// @brief If the Player is hit by an enemy
    ///
    void Hit();
	/// @brief Add Points to the Player
    ///
    void Points();
    
	/// @brief Return The Lifes a Player has
	///
    /// @returns lifes as int
    ///
    int GetLifes() const;
    
	/// @brief Return The Points a Player has
	///
    /// @returns Points as int
    ///
    int GetPoints() const;
};

#endif /* _CAMERA_H_ */