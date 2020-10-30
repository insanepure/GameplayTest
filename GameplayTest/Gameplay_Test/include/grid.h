#ifndef _GRID_H_
#define _GRID_H_

enum { GridWidth=64,GridHeight=64 };
enum { TileWidth=32,TileHeight=32 };
enum Tile { None,Trap };

class CImage;
class CCamera;
struct SDL_Renderer;

/// @brief Data of the Grid we send over Network
///
struct SGridData
{
    char Tiles[GridWidth*GridHeight];
};

/// @brief Grid we move on
///
class CGrid
{
private:
    char m_Tiles[GridWidth*GridHeight];

public:
	/// @brief Constructor to initialize a Grid
	////
	CGrid();
	/// @brief Destructor to destroy a Grid
	///
	~CGrid();
public:
	/// @brief Set the actual State of the Grid
    ///
    /// @param Data of the Enemy
    ///
    void SetState(SGridData& a_pData);
	/// @brief Get the actual State of the Grid
    ///
    /// @param Data of the Enemy
    ///
    void GetState(SGridData& a_pData);
	/// @brief Draw the Grid
	///
	/// @param Camera needed for culling
	/// @param Image to draw the player with
	/// @param Renderer to use
    ///
    void Draw(CCamera* a_pCamera,CImage* a_pImage,SDL_Renderer* a_pRenderer);
	/// @brief Get the Tile at Position X/Y
	///
	/// @param Position X
	/// @param Position Y
    ///
	/// @returns Tile at that Position
    ///
    Tile GetTile(int a_X,int a_Y);
	/// @brief Set the Tile at Position X/Y
	///
	/// @param Position X
	/// @param Position Y
    ///
    void SetTile(int a_X,int a_Y);
};

#endif /* _CAMERA_H_ */