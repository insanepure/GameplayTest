#ifndef _NETWORK_H_
#define _NETWORK_H_
#include "include/player.h"
#include "include/enemy.h"
#include "include/grid.h"
#include <SDL_net.h>
#include <vector>
#include <list>

class CGrid;

/// @brief Network Type we are
///
enum NetworkType { Pending,Client, Host };
/// @brief Settings
///
enum { BroadcastPort=13370,GamePort=13371, MaxClients=10,MaxEnemies=100 };
/// @brief Class to handle Networking
///
class CNetwork
{
private:
    /// @brief Types of packets
    ///
    enum Packet { Click,Alive,Scene };
    /// @brief HeadData in every data the head
    ///
    struct SHeadData
    {
        Packet P;
    };
    /// @brief State Data which contains the whole State
    ///
    struct SSceneData
    {
        SHeadData head;
        char You;
        char Players;
        char Enemies;
        SPlayerData PData[MaxClients];
        SEnemyData EData[MaxEnemies];
        SGridData Grid;
    };
    /// @brief Data send when someone clicked
    ///
    struct SClickData
    {
        SHeadData head;
        int X;
        int Y;
    };
    /// @brief Incoming Data format
    ///
    struct SReceiveData
    {
        SHeadData head;
        void* data;
    };

private:
    /// @brief Server's IpAddress we get after we received a broadcast
    ///
    IPaddress m_Server;
    /// @brief Socket to listen for broadcasts
    ///
    UDPsocket m_bSocket;
    /// @brief Broadcast Packet
    ///
    UDPpacket* m_pbPacket;
    /// @brief Normal Packet
    ///
    UDPpacket* m_pPacket;
    /// @brief Timer to Wait
    ///
    float m_WaitTimer;
    /// @brief Timer to Wait
    ///
    NetworkType m_NType;
public:
    /// @brief Constructor to initialize Network
    ///
    CNetwork();
    /// @brief Destructor to destroy Network
    ///
    ~CNetwork();
public:
    /// @brief Return the Type we are
    ///
    /// @returns NetworkType
    ///
    NetworkType GetType();
    /// @brief Wait for a Broadcast
    ///
    /// @param Delta Time
    ///
    /// @returns true if one happend, false if not
    ///
    bool WaitForBroadcast(float a_DeltaTime);
    /// @brief Broadcast that we exist
    ///
    void Broadcast();
    /// @brief Send that we clicked somewhere (Client only)
    ///
    /// @oaram X where the Click happend
    /// @param Y where the Click happend
    ///
    void SendClick(int a_X,int a_Y);
    /// @brief Send that we still are there (Client only)
    ///
    void SendAlive();
    /// @brief Receive something (Server only)
    ///
    /// @oaram players to change
    /// @oaram enemies to change
    /// @oaram Grid for movement
    /// @oaram Index of the Player
    ///
    void Receive(std::vector<CPlayer*>& a_rPlayers,std::list<CEnemy*>& a_rEnemies,CGrid* a_pGrid,int& a_Index);

};

#endif /* _NETWORK_H_ */