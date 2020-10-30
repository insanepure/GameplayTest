#include "include/astar.h"
#include "include/grid.h"

#include <stdio.h>


CAStar::CAStar()
{


}

//*********************************************************************//
//*********************************************************************//

CAStar::~CAStar()
{


}


//*********************************************************************//
//*********************************************************************//

int CAStar::HeuristicCost(SNode& a_Start, SNode& a_End)
{
    //Manhattan Method
    int x = std::abs(a_Start.X-a_End.X);
    int y = std::abs(a_Start.Y-a_End.Y);
    return x+y;
}


//*********************************************************************//
//*********************************************************************//

void CAStar::ReconstructPath(SNode& current)
{
    this->m_Path.push_back(current);
    if(current.From != -1)
        this->ReconstructPath(this->m_ClosedList[current.From]); //reconstruct recursively
}


//*********************************************************************//
//*********************************************************************//

int CAStar::FindPath(CGrid* a_pGrid,int a_Start_X,int a_Start_Y,int a_Target_X,int a_Target_Y)
{
    if(a_Start_X == a_Target_X&&a_Start_Y==a_Target_Y)
        return 0; // Trivial

    if(a_pGrid->GetTile(a_Target_X,a_Target_Y) == Tile::Trap)
        return 0; //Trivial

    if(a_pGrid->GetTile(a_Start_X,a_Start_Y) == Tile::Trap)
        return 0; //Trivial


    //clear old paths
    this->m_Path.clear();
    
    //define start
    SNode start;
    start.X = a_Start_X;
    start.Y = a_Start_Y;
    start.gScore = 0;
    start.From = -1;
    //define end
    SNode end;
    end.X = a_Target_X;
    end.Y = a_Target_Y;
    end.gScore = 0;
    end.From = -1;
    //define start fCost
    start.fScore = start.gScore + this->HeuristicCost(start,end);
    //Add start
    this->m_OpenList.push_back(start);
    //Go through list
    while(this->m_OpenList.size() != 0)
    {
        //stop at half looked objects because to much
        if(this->m_ClosedList.size() > GridWidth*GridHeight/2)
            break;
        int oIdx = 0;
        //get the lowest
        SNode current = this->m_OpenList[0];
        for(unsigned int i=0;i<this->m_OpenList.size();i++)
        {
            if(this->m_OpenList[i].fScore < current.fScore)
            {
                oIdx = i;
                current = this->m_OpenList[oIdx];
            }
        }
        if(end == current)
        {
            //reconstruct path
            this->ReconstructPath(current);
            break; //we found the way
        }
        //remove current from open list
        this->m_OpenList.erase(this->m_OpenList.begin()+oIdx);
        //add current to closed list
        int from = (int)this->m_ClosedList.size();
        this->m_ClosedList.push_back(current);
        //now go through the neighbours
        for(int y=-1;y<2;y++)
        {
            for(int x=-1;x<2;x++)
            {
                if(x == 0&&y == 0)
                    continue; //skip same
                    
                //define neighbour
                SNode neighbour;
                neighbour.X = current.X+(x*TileWidth);
                neighbour.Y = current.Y+(y*TileHeight);
                neighbour.gScore = 0;
                neighbour.fScore = 0;

                //if the neighbour is in range
                if(neighbour.X >= 0&&neighbour.X < TileWidth*GridWidth&&neighbour.Y >= 0&&neighbour.Y < TileHeight*GridHeight)
                { 
                    
                    //Dont count traps
                    if(a_pGrid->GetTile(neighbour.X,neighbour.Y) == Tile::Trap)
                        continue;

                    bool inClosed = false;
                    //check if the neighbour is already in list
                    for(unsigned int i=0;i<this->m_ClosedList.size();i++)
                    {
                        if(this->m_ClosedList[i] == neighbour)
                        {
                            inClosed = true;
                            break;
                        }
                    }
                    if(inClosed)
                        continue;
                    int inOpen = -1;
                    int gScore = current.gScore + this->HeuristicCost(current,neighbour);
                    //check if the neighbour is already in list
                    for(unsigned int i=0;i<this->m_OpenList.size();i++)
                    {
                        if(this->m_OpenList[i] == neighbour)
                        {
                            inOpen = i;
                            break;
                        }
                    }
                    //add him or change him
                    if(inOpen == -1||gScore < neighbour.gScore)
                    {
                        neighbour.From = from;
                        neighbour.gScore = gScore;
                        neighbour.fScore = gScore + this->HeuristicCost(neighbour,end);
                        
                        if(inOpen == -1)
                            this->m_OpenList.push_back(neighbour); // add new
                        else
                            this->m_OpenList[inOpen] = neighbour; //reset old
                    }
                }
                else
                    continue; //outside of grid
            }
        }
    }
       //clear old lists
    this->m_OpenList.clear();
    this->m_ClosedList.clear();

    return (int)m_Path.size();
}



//*********************************************************************//
//*********************************************************************//

bool CAStar::GetPath(unsigned int a_Index,int& a_X,int& a_Y)
{
    if(a_Index == this->m_Path.size())
        return false;

    a_Index = (unsigned int)(this->m_Path.size()-a_Index)-1; //we start from the back because we added it from there
    a_X = this->m_Path[a_Index].X;
    a_Y = this->m_Path[a_Index].Y;
    //path found, return that we found sth
    return true;
}
