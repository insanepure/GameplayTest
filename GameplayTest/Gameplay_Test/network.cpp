#include "include/network.h"
#include <SDL.h>
#include <stdio.h>
#include <cstring>


CNetwork::CNetwork() : m_WaitTimer(0.0f),m_NType(NetworkType::Pending)
{
    //Initialize networking
    SDLNet_Init();
    //open port 13370 for broadcast
    if((this->m_bSocket = SDLNet_UDP_Open(BroadcastPort)) == NULL)
        printf("The Port is already in use!\n");
    //set broadcast packet size
    this->m_pbPacket = SDLNet_AllocPacket(512);
    //Game packet size
    this->m_pPacket = SDLNet_AllocPacket(sizeof(SSceneData)+512); //just in case

}
//*********************************************************************//
//*********************************************************************//


CNetwork::~CNetwork()
{
    SDLNet_UDP_Close(this->m_bSocket);
    SDLNet_Quit();

}
//*********************************************************************//
//*********************************************************************//


NetworkType CNetwork::GetType()
{
    return this->m_NType;
}
//*********************************************************************//
//*********************************************************************//


void CNetwork::Receive(std::vector<CPlayer*>& a_rPlayers,std::list<CEnemy*>& a_rEnemies,CGrid* a_pGrid,int& a_Index)
{
    while(SDLNet_UDP_Recv(this->m_bSocket, this->m_pPacket))
    {
        //we received something
        char* data = (char*)this->m_pPacket->data;
        SReceiveData* rData = (SReceiveData*)data;
        if(rData->head.P == Packet::Click)
        {
            SClickData* cData = (SClickData*)data;
            //find player and move him
            for(unsigned int i=1;i<a_rPlayers.size();i++)
            {
                if(a_rPlayers[i]->GetIP().host == this->m_pPacket->address.host
                 &&a_rPlayers[i]->GetIP().port == this->m_pPacket->address.port)
                 {
                    if(a_rPlayers[i]->GetLifes() > 0)
                        a_rPlayers[i]->Move(a_pGrid,cData->X,cData->Y);
                    break;
                 }
            }
        }
        else if(rData->head.P == Packet::Scene)
        {
            SSceneData* sData = (SSceneData*)data;
            a_Index = (int)sData->You;
            //get player size
            unsigned int players = sData->Players;
            //if the player size is different
            if(players > a_rPlayers.size())
            {
                //we need more players!
                while(players > a_rPlayers.size())
                    a_rPlayers.push_back(new CPlayer(TileWidth,TileHeight));

            }
            else if(players < a_rPlayers.size())
            {
                //we have to many players!! delete!
                while(players < a_rPlayers.size())
                {
                    delete a_rPlayers[0];
                    a_rPlayers.erase(a_rPlayers.begin());
                }
            }
            //now set the player's Data
            for(unsigned int i=0;i<a_rPlayers.size();i++)
                a_rPlayers[i]->SetState(sData->PData[i]);
            //get enemy size
            unsigned int enemies = sData->Enemies;
            //if the enemy size is different
            if(enemies > a_rEnemies.size())
            {
                //we need more enemy!
                while(enemies > a_rEnemies.size())
                    a_rEnemies.push_back(new CEnemy(TileWidth,TileHeight));

            }
            else if(enemies < a_rEnemies.size())
            {
                //we have to many enemy!! delete!
                while(enemies < a_rEnemies.size())
                {
                    delete *a_rEnemies.begin();
                    a_rEnemies.erase(a_rEnemies.begin());
                }
            }
            //now set the enemy's Data
            int i = 0;
            for(auto enemy : a_rEnemies)
            {
                enemy->SetState(sData->EData[i]);
                ++i;
            }
            //now set the grid
            a_pGrid->SetState(sData->Grid);
        }
        else if(rData->head.P == Packet::Alive)
        {
            //check if he exists, and if so, update his timeout
            bool exist = false;
            for(unsigned int i=1;i<a_rPlayers.size();i++)
            {
                if(a_rPlayers[i]->GetIP().host == this->m_pPacket->address.host
                 &&a_rPlayers[i]->GetIP().port == this->m_pPacket->address.port)
                 {
                    a_rPlayers[i]->SetLastUpdate(SDL_GetTicks());
                    exist = true;
                    break;
                 }
            }
            //he does not exist, so create him
            if(!exist&&a_rPlayers.size() < MaxClients)
            {
                a_rPlayers.push_back(new CPlayer(TileWidth,TileHeight));
                a_rPlayers[a_rPlayers.size()-1]->SetLastUpdate(SDL_GetTicks());
                a_rPlayers[a_rPlayers.size()-1]->SetIP(this->m_pPacket->address);
            }
            else if(exist)
            {
                //Send him the whole Game as State
                SSceneData sdata;
                sdata.head.P = Packet::Scene;
                sdata.Players = (char)a_rPlayers.size();
                sdata.Enemies = (char)a_rEnemies.size();
                //Save PlayerData
                for(unsigned int i=0;i<a_rPlayers.size();i++)
                {
                    if(a_rPlayers[i]->GetIP().host == this->m_pPacket->address.host
                     &&a_rPlayers[i]->GetIP().port == this->m_pPacket->address.port)
                        sdata.You = (char)i;
                    a_rPlayers[i]->GetState(sdata.PData[i]);
                }
                //Save Enemy Data
                int i = 0;
                for(auto enemy : a_rEnemies)
                {
                    enemy->GetState(sdata.EData[i]);
                    ++i;
                }
                //Save Grid Data
                a_pGrid->GetState(sdata.Grid);

                this->m_pbPacket->data = (Uint8*)&sdata;
                this->m_pbPacket->len = sizeof(SSceneData);
                this->m_pbPacket->address = this->m_pPacket->address;
                SDLNet_UDP_Send(this->m_bSocket, -1, this->m_pbPacket);
            }
        }
    }
}
//*********************************************************************//
//*********************************************************************//

void CNetwork::SendAlive()
{
    //Define ClickData
    Packet data;
    data = Packet::Alive;
    this->m_pbPacket->address = this->m_Server;
    this->m_pbPacket->data = (Uint8*)&data;
    this->m_pbPacket->len = sizeof(Packet);
    SDLNet_UDP_Send(this->m_bSocket, -1, this->m_pbPacket);
}
//*********************************************************************//
//*********************************************************************//


void CNetwork::SendClick(int a_X,int a_Y)
{
    //Define ClickData
    SClickData data;
    data.head.P = Packet::Click;
    data.X = a_X;
    data.Y = a_Y;
    //Send ClickData
    this->m_pbPacket->address = this->m_Server;
    this->m_pbPacket->data = (Uint8*)&data;
    this->m_pbPacket->len = sizeof(data);
    SDLNet_UDP_Send(this->m_bSocket, -1, this->m_pbPacket);
}
//*********************************************************************//
//*********************************************************************//


bool CNetwork::WaitForBroadcast(float a_DeltaTime)
{
    //We wait one and a half second
    if(this->m_WaitTimer < 2.0f)
    {
        this->m_WaitTimer += a_DeltaTime/1000.0f;

        if(SDLNet_UDP_Recv(this->m_bSocket, this->m_pbPacket))
        {
            //save server Data
            this->m_Server = this->m_pbPacket->address;
            printf("We are Client!\n");
            //bind ourself to a random port cause we don't need to listen for a host anymore because we know him
            SDLNet_UDP_Close(this->m_bSocket);
            if((this->m_bSocket = SDLNet_UDP_Open(0)) == NULL)
                printf("The Port is already in use!\n");

            this->m_NType = Client;
            return true;
        }
    }
    else
    {
        printf("We are Host!\n");
        //change port to gamePort which is the Port the messages go go
        SDLNet_UDP_Close(this->m_bSocket);
        if((this->m_bSocket = SDLNet_UDP_Open(GamePort)) == NULL)
            printf("The Port is already in use!\n");
        this->m_NType = Host;
    }
    return false;

}

//*********************************************************************//
//*********************************************************************//


void CNetwork::Broadcast()
{
    IPaddress address;
    //Broadcast on normal subgate
    SDLNet_ResolveHost(&address,"255.255.255.255",BroadcastPort);
    this->m_pbPacket->address = address;
    this->m_pbPacket->len = 0;
    SDLNet_UDP_Send(this->m_bSocket, -1, this->m_pbPacket);
    //now get the internal
    SDLNet_ResolveHost(&address, NULL, BroadcastPort);
    const char* ip = SDLNet_ResolveIP(&address);
    SDLNet_ResolveHost(&address,ip,BroadcastPort);
    Uint8* ip2 = (Uint8*)&address.host;
    ip2[3] = 255;
    address.host = (Uint32)ip2;
    //and send something to it
    this->m_pbPacket->address = address;
    this->m_pbPacket->len = 0;
    SDLNet_UDP_Send(this->m_bSocket, -1, this->m_pbPacket);

}
