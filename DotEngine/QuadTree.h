#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <SDL3/SDL_render.h>

class SDLRenderer;
class Dot;
class glm::vec2;

enum QuadTreePartitionArea
{
    TOPLEFT = 0,
    BOTTOMLEFT = 1,
    TOPRIGHT = 2,
    BOTTOMRIGHT = 3,
};

class QuadTree
{
public:
    QuadTree(glm::vec2 topLeftBoundary, glm::vec2 bottomRightBoundary, uint8_t currentDepth);
    ~QuadTree();
    void Insert(Dot* aDot);
    void QueryRange(Dot* aDot, std::vector<Dot*>& closestDots,
        glm::vec2& topLeftOfQueryBox, glm::vec2& bottomRightOfQueryBox);
    
    void GetDotsInQuadTreePartition(const QuadTreePartitionArea partitionArea,
        std::vector<Dot*>& dotsToBeFound);
    
    void GetAllDotsInQuadTree(std::vector<Dot*>& dotsToBeFound);
    
    bool IntersectsTree(glm::vec2& topLeftOfQueryBox, glm::vec2& bottomRightOfQueryBox);
    
    void Visualize(SDL_Renderer* renderer);

    QuadTree* m_TopLeftQuadTree = nullptr;
    QuadTree* m_TopRightQuadTree = nullptr;
    QuadTree* m_BottomLeftQuadTree = nullptr;
    QuadTree* m_BottomRightQuadTree = nullptr;
    
private:
    glm::vec2 m_TopLeftBoundary = glm::vec2(0, 0);
    glm::vec2 m_BottomRightBoundary = glm::vec2(0, 0);
    uint8_t m_MaxDepth = 5;
    uint8_t m_currentDepth = 0;
    std::vector<Dot*> m_DotsInTree;
};
