#ifndef _SCENE_H_
#define _SCENE_H_

#include "window.h"
#include "network.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "image.h"
#include "font.h"
#include "grid.h"
#include <list>
#include <vector>

/// @brief Modes there are, eg Game or Editor
///
enum Mode { Game, TrapEditor, EnemyEditor };
/// @brief Mouse click, whether Left or Right
///
enum Click { Left,Right };
/// @brief Scene that holds everything
///
class CScene
{
private:
    /// @brief Window Height and Width defined here
    ///
    enum { WindowWidth=1024,WindowHeight=578 };
private:
    /// @brief What Type of Network we have
    ///
    NetworkType m_NType;
    /// @brief Network Handle
    ///
    CNetwork* m_pNetwork;
    /// @brief Font Size
    ///
    int m_FontSize;
    /// @brief Which Mode we are in
    ///
    Mode m_Mode;
    /// @brief Window Handle
    ///
    CWindow* m_pWindow;
    /// @brief Camera to see only part of the map
    ///
    CCamera* m_pCamera;
    /// @brief IMG for tiles
    ///
    CImage* m_pTileImg;
    /// @brief IMG for the player
    ///
    CImage* m_pPlayerImg;
    /// @brief IMG for the enemies
    ///
    CImage* m_pEnemyImg;
    /// @brief Font to draw text
    ///
    CFont* m_pFont;
    /// @brief Grid we play on
    ///
    CGrid* m_pGrid;
    /// @brief player we play
    ///
    std::vector<CPlayer*> m_Players;
    /// @brief Enemies we play against
    ///
    std::list<CEnemy*> m_Enemies;
    /// @brief Index where the player is in the vector
    ///
    int m_Index;
public:
    /// @brief Constructor to initialize the Scene
    ///
    CScene();
    /// @brief Destructor to destroy the Scene
    ///
    ~CScene();
public:
    /// @brief Mouse is clicked
    ///
    /// @param Which Side was clicked
    /// @param X where the click happend
    /// @param Y where the click happend
    ///
    void MouseClick(Click a_Click,int a_X,int a_Y);
    /// @brief Change the Mode of the Game
    ///
    /// @param New Mode
    ///
    void ChangeMode(Mode a_Mode);
    /// @brief Update the Scene
    ///
    void Update(float a_DeltaTime);
    /// @brief Render the Scene
    ///
    void Render();
    /// @brief Draw the GUI
    ///
    void DrawGUI();
};

#endif /* _SCENE_H_ */