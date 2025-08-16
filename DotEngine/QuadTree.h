#pragma once
#include <glm/vec2.hpp>
class Dot;
class glm::vec2;

class QuadTree
{
public:
    QuadTree(glm::vec2 topLeftBoundary, glm::vec2 bottomRightBoundary);
    ~QuadTree();
    void Insert(Dot* aDot);
    
private:
    Dot* m_DotInTree = nullptr;
    QuadTree* m_TopLeftQuadTree = nullptr;
    QuadTree* m_TopRightQuadTree = nullptr;
    QuadTree* m_BottomLeftQuadTree = nullptr;
    QuadTree* m_BottomRightQuadTree = nullptr;

    glm::vec2 m_TopLeftBoundary = glm::vec2(0, 0);
    glm::vec2 m_BottomRightBoundary = glm::vec2(0, 0);
};
