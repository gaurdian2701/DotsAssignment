#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <SDL3/SDL_render.h>

class SDLRenderer;
class Dot;
class glm::vec2;

class QuadTree
{
public:
    QuadTree(glm::vec2 topLeftBoundary, glm::vec2 bottomRightBoundary, uint8_t currentDepth);
    ~QuadTree();
    void Insert(Dot* aDot);
    void QueryRange(Dot* aDot, std::vector<Dot*>& closestDots,
        glm::vec2& topLeftOfQueryBox, glm::vec2& bottomRightOfQueryBox);
    bool IntersectsTree(glm::vec2& topLeftOfQueryBox, glm::vec2& bottomRightOfQueryBox);
    void Visualize(SDL_Renderer* renderer);
    
private:
    QuadTree* m_TopLeftQuadTree = nullptr;
    QuadTree* m_TopRightQuadTree = nullptr;
    QuadTree* m_BottomLeftQuadTree = nullptr;
    QuadTree* m_BottomRightQuadTree = nullptr;
    
    glm::vec2 m_TopLeftBoundary = glm::vec2(0, 0);
    glm::vec2 m_BottomRightBoundary = glm::vec2(0, 0);
    std::vector<Dot*> m_DotsInTree;
    uint8_t m_MaxDepth = 5;
    uint8_t m_currentDepth = 0;
};
