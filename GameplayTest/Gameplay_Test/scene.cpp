#include "include/scene.h"
#include <SDL.h>
#include <string>

CScene::CScene() : m_FontSize(24),m_Mode(Mode::Game),m_Index(0)
{
    ////////////// INITIALIZE ///////////
    m_pWindow = new CWindow(WindowWidth,WindowHeight);
    SDL_Renderer* pRenderer = m_pWindow->GetRenderer();
    m_pTileImg = new CImage("../Data/Images/tile.png",pRenderer);
    m_pPlayerImg = new CImage("../Data/Images/player.png",pRenderer);
    m_pEnemyImg = new CImage("../Data/Images/enemy.png",pRenderer);
    int m_FontSize = 24;
    m_pFont = new CFont("../Data/Font/Roboto-Black.ttf",m_FontSize);

    SDL_Rect bound;
    bound.x = 0;
    bound.y = 0;
    bound.w = GridWidth*TileWidth;
    bound.h = GridHeight*TileHeight;
    m_pCamera = new CCamera(WindowWidth,WindowHeight,bound);

    this->m_pNetwork = new CNetwork();
    m_pGrid = new CGrid();

}
//*********************************************************************//
//*********************************************************************//

CScene::~CScene()
{

    ////////////// EXIT ///////////
    for(auto enemy : m_Enemies)
        delete enemy;
    for(auto player : m_Players)
        delete player;
    delete this->m_pNetwork;
    delete m_pGrid;
    delete m_pFont;
    delete m_pTileImg;
    delete m_pPlayerImg;
    delete m_pEnemyImg;
    delete m_pCamera;
    delete m_pWindow;

}
//*********************************************************************//
//*********************************************************************//


void CScene::MouseClick(Click a_Click,int a_X,int a_Y)
{
    //add camera offset to it
    a_X += m_pCamera->GetX();
    a_Y += m_pCamera->GetY();
    //Client only, send click
    if(this->m_pNetwork->GetType() == NetworkType::Client)
        this->m_pNetwork->SendClick(a_X,a_Y);
    //only do something if the game started
    if(m_Players.size() > 0)
    {
        //check for button
        if (a_Click == Click::Left)
            m_Players[m_Index]->Move(this->m_pGrid,a_X,a_Y);
        else if(m_Mode == Mode::TrapEditor&&a_Click == Click::Right)
            m_pGrid->SetTile(a_X,a_Y);
        else if(m_Mode==Mode::EnemyEditor&&a_Click == Click::Right)
        {
            //Check if we clicked on an Enemy
            bool enemyClick = false;
            auto it=m_Enemies.begin();
            while(it != m_Enemies.end())
            {
                CEnemy* enemy = *it;
                int eX = enemy->GetX();
                int eY = enemy->GetY();
                if(a_X < eX+TileWidth && 
                    a_X+TileWidth >eX && 
                    a_Y < eY+TileHeight &&
                    a_Y+TileHeight > eY)
                {
                    delete enemy;
                    m_Enemies.erase(it++); //erase last one and increment afterwards
                    enemyClick = true;
                }
                else
                    ++it;
            }
            //if not clicked on enemy, add one
            if(!enemyClick&&m_Enemies.size() < MaxEnemies)
            {
                //limit the clicked position to the Tile
                float EX = (float) ((int)(a_X/TileWidth))*TileWidth;
                float EY = (float) ((int)(a_Y/TileHeight))*TileHeight;
                m_Enemies.push_back(new CEnemy(EX,EY));
            }
        }
    }
}
//*********************************************************************//
//*********************************************************************//


void CScene::ChangeMode(Mode a_Mode)
{
    if(this->m_pNetwork->GetType() == NetworkType::Host)
        this->m_Mode = a_Mode;
}
//*********************************************************************//
//*********************************************************************//


void CScene::Update(float a_DeltaTime)
{
    if(this->m_pNetwork->GetType() == NetworkType::Pending)
    {
        //At the start, we wait a little bit and see if there are games in our subnetwork
        if(!this->m_pNetwork->WaitForBroadcast(a_DeltaTime))
        {
            if(this->m_pNetwork->GetType() == NetworkType::Host)
            {
                //timeover, we are the host
                this->m_Players.push_back(new CPlayer(TileWidth,TileHeight));

                //add Enemies
                for(int y=0;y<4;y++)
                {
                    for(int x=0;x<2;x++)
                    {
                        m_Enemies.push_back(new CEnemy((float)(23+x*4)*TileWidth,(float)(10+y*4)*TileHeight));
                    }
                }
            } //networktype == host
        } // !waitforbroadcast

    }
    else
    {
        //Receive Data either from clients or from host
        this->m_pNetwork->Receive(this->m_Players,this->m_Enemies,this->m_pGrid,this->m_Index);
        if(this->m_pNetwork->GetType() == NetworkType::Host)
        {
            //we are the host, so broadcast for new players
            this->m_pNetwork->Broadcast();
            //check when a player send something and delete him if he timed out
            unsigned int timeout = 1000*3; //three seconds
            //go through players
            for(unsigned int i=1;i<this->m_Players.size();i++)
            {
                //compare time to last update
                if(SDL_GetTicks()-this->m_Players[i]->GetLastUpdate() > timeout)
                {
                    //delete player
                    delete this->m_Players[i];
                    this->m_Players.erase(this->m_Players.begin()+i);
                    --i;
                }
            }

            //Update player
            for(auto player : m_Players)
                player->Update(m_pGrid,a_DeltaTime);

            //Update Enemies
            auto it=m_Enemies.begin();
            while(it != m_Enemies.end())
            {
                CEnemy* enemy = *it;
                int eX = enemy->GetX()/TileWidth;
                int eY = enemy->GetY()/TileHeight;
                CPlayer* nearestPlayer = nullptr;
                int playerDiff = 9999;
                for(auto player : m_Players)
                {
                    //Update 
                    int pX = player->GetX()/TileWidth;
                    int pY = player->GetY()/TileHeight;
                    int diff = std::abs(eX-pX)+std::abs(eY-pY);
                    //if the player is near 5 blocks
                    if(diff < 5&&diff < playerDiff)
                    {
                        nearestPlayer = player;
                        playerDiff = diff;
                    }

                }
                //if the player is near 5 blocks
                if(nearestPlayer != nullptr)
                {
                    enemy->Update(m_pGrid,a_DeltaTime,nearestPlayer);
                    if(!enemy->IsAlive())
                    {
                        delete enemy;
                        m_Enemies.erase(it++); //erase last one and increment afterwards
                    }
                    else
                        ++it;
                }
                else
                    ++it;
            }
        }
        else if(this->m_pNetwork->GetType() == NetworkType::Client)
            this->m_pNetwork->SendAlive(); //Tell the host that we are there!

        /// Update Camera based on Player's Location
        int playerX = 0;
        int playerY = 0;
        if(m_Players.size() > 0)
        {
            playerX = m_Players[m_Index]->GetX();
            playerY = m_Players[m_Index]->GetY();
        }
        m_pCamera->Move(playerX,playerY);
    }

}
//*********************************************************************//
//*********************************************************************//

void CScene::DrawGUI()
{
    SDL_Renderer* pRenderer = m_pWindow->GetRenderer();
    //Render GUI
    SDL_Color blue = {0,100,200,255};
    SDL_Color black = {0,0,0,255};
    //Simple Shadow Trick
    int x = 2;
    int y = 2;
    int shadow_offset = 3;
    std::string text;
    if(m_Players.size() == 0)
    {
            text = "Checking for Host ...";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
    }
    else if(m_Players.size() > 0)
    {
        if(this->m_pNetwork->GetType() == NetworkType::Host)
        {
            text = "Press F1 for Game Mode";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            y += m_FontSize;
            text = "Press F2 for Trap Editor Mode";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            y += m_FontSize;
            text = "Press F3 for Enemy Editor Mode";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
        }
        y += m_FontSize*2;
        switch(m_Mode)
        {
        case Mode::Game:
            text = "Mode: Game";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            y += m_FontSize*2;
            text = "Lifes: "+std::to_string(m_Players[m_Index]->GetLifes());
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            y += m_FontSize;
            text = "Points: "+std::to_string(m_Players[m_Index]->GetPoints());
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            y += m_FontSize;
            x += WindowWidth/2;
            if(m_Players[m_Index]->GetLifes() == 0)
            {
                text = "Game Over!";
                SDL_Color red = {255,0,0,255};
                m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
                m_pFont->Draw(text.c_str(),pRenderer,x,y,red);
            }
            else if(m_Enemies.size() == 0)
            {
                text = "You won!";
                SDL_Color white = {255,255,255,255};
                m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
                m_pFont->Draw(text.c_str(),pRenderer,x,y,white);
            }
            break;
        case Mode::TrapEditor:
            text = "Mode: Trap Editor";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            y += m_FontSize*2;
            text = "Right-Click to Add/Delete Trap";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            break;
        case Mode::EnemyEditor:
            text = "Mode: Enemy Editor";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            y += m_FontSize*2;
            text = "Right-Click to Add/Delete Enemy";
            m_pFont->Draw(text.c_str(),pRenderer,x+shadow_offset,y+shadow_offset,black);
            m_pFont->Draw(text.c_str(),pRenderer,x,y,blue);
            break;
        }
    }
}
//*********************************************************************//
//*********************************************************************//

void CScene::Render()
{
    SDL_Renderer* pRenderer = m_pWindow->GetRenderer();
    SDL_Rect screen;
    screen.w = TileWidth;
    screen.h = TileHeight;
    //Clear Renderer
    SDL_RenderClear(pRenderer);

    //Render Map
    m_pGrid->Draw(m_pCamera,m_pTileImg,pRenderer);

    //Render Players
    //check if he is inside the camera
    for(auto player : m_Players)
    {
        screen.x = player->GetX();
        screen.y = player->GetY();
        if(player->GetLifes() > 0&&m_pCamera->Intersect(screen))
        {
            //add offset
            m_pCamera->Subtract(screen);
            //draw player
            player->Draw(screen,m_pPlayerImg,pRenderer);
        }
    }
    //Render Enemies
    for(auto enemy : m_Enemies)
    {
        screen.x = enemy->GetX();
        screen.y = enemy->GetY();
        //check if he is inside the camera
        if(m_pCamera->Intersect(screen))
        {
            //add offset
            m_pCamera->Subtract(screen);
            //draw player
            enemy->Draw(screen,m_pEnemyImg,pRenderer);
        }
    }
    //Render GUI
    this->DrawGUI();

    //Update Renderer
    SDL_RenderPresent(pRenderer);

}
//*********************************************************************//
//*********************************************************************//
