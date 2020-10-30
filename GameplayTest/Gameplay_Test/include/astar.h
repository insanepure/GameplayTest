#ifndef _ASTAR_H_
#define _ASTAR_H_

#include <list>
#include <vector>

class CGrid;

/// @brief One Cell/Node
///
struct SNode
{
    /// @brief X Position
    ///
    int X;
    /// @brief Y Position
    ///
    int Y;
    /// @brief Total cost from start to end
    ///
    int fScore;
    /// @brief Cost from start to this
    ///
    int gScore;
    /// @brief Index in Closed List
    ///
    int From;
    /// @brief Overload operator to compare two Nodes
    ///
    inline bool operator==(SNode a_Rhs) 
    {
       if (a_Rhs.X == this->X && a_Rhs.Y == this->Y)
          return true;
       else
          return false;
    }
};

/// @brief A star for Pathfinding
///
class CAStar
{
private:
    /// @brief Open List
    ///
    std::vector<SNode> m_OpenList;
    /// @brief Closed List
    ///
    std::vector<SNode> m_ClosedList;
    /// @brief Final Path, vector because we need to access specific parts of the vectors
    ///
    std::vector<SNode> m_Path;
public:
    /// @brief Constructor to initialize a A Star
    ///
    CAStar();
    /// @brief Destructor to destroy a A Star
    ///
    ~CAStar();
private:
    /// @brief Reconstruct Path from ClosedList
    ///
    /// @param From where we reconstruct
    ///
    void ReconstructPath(SNode& current);
    /// @brief Calculate the Heuristic Costs
    ///
    /// @param Start from which we calculate
    /// @param End to which we calculate
    ///
    int HeuristicCost(SNode& a_Start, SNode& a_End);
public:
    /// @brief Start the path Finding
    ///
    /// @param Grid to search for traps
    /// @param where we start in X
    /// @param where we start in Y
    /// @param where we end in X
    /// @param where we end in Y
    ///
    /// @returns Amount of Cells, 0 if none
    ///
    int FindPath(CGrid* a_pGrid,int a_Start_X,int a_Start_Y,int a_Target_X,int a_Target_Y);
    /// @brief Return the Path at Index
    ///
    /// @param Index of the Cell
    /// @param X of the Cell
    /// @param Y of the Cell
    ///
    /// @returns true if exist, false if not
    ///
    bool GetPath(unsigned int a_Index,int& a_rX,int& a_rY);

};

#endif /* _ASTAR_H_ */