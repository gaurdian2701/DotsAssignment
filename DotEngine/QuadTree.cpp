#include "QuadTree.h"

#include <SDL3/SDL_oldnames.h>

#include "Dot.h"

QuadTree::QuadTree(glm::vec2 topLeftBoundary, glm::vec2 bottomRightBoundary)
{
    m_TopLeftBoundary = topLeftBoundary;
    m_BottomRightBoundary = bottomRightBoundary;
}

QuadTree::~QuadTree()
{
    m_DotInTree = nullptr;
    delete m_TopLeftQuadTree;
    delete m_TopRightQuadTree;
    delete m_BottomLeftQuadTree;
    delete m_BottomRightQuadTree;
}

void QuadTree::Insert(Dot* aDot)
{
    if (m_DotInTree != nullptr)
        return;

    int xMidPoint = (m_TopLeftBoundary.x + m_BottomRightBoundary.x) / 2;
    int yMidPoint = (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2;

    //Quad boundaries are close to the size of the dot
    if (m_BottomRightBoundary.x - m_TopLeftBoundary.x - 2 * aDot->m_Radius <= 1 &&
        m_BottomRightBoundary.y - m_TopLeftBoundary.y - 2 * aDot->m_Radius <= 1) 
    {
        if (m_DotInTree == nullptr)
            m_DotInTree = aDot;
        return;
    }
        
    if (aDot->m_Position.x < xMidPoint) //Left Side
    {
        if (aDot->m_Position.y < yMidPoint)//Top Left
        {
            if (m_TopLeftQuadTree == nullptr)
                m_TopLeftQuadTree = new QuadTree(
                    m_TopLeftBoundary,
                    glm::vec2(xMidPoint, yMidPoint));
            m_TopLeftQuadTree->Insert(aDot);
        }
        else if (aDot->m_Position.y >= yMidPoint) //Bottom Left
        {
            if (m_BottomLeftQuadTree == nullptr)
                m_BottomLeftQuadTree = new QuadTree(
                    glm::vec2(m_TopLeftBoundary.x, yMidPoint),
                    glm::vec2(xMidPoint, m_BottomRightBoundary.y));
            m_BottomLeftQuadTree->Insert(aDot);
        }
    }
    else if (aDot->m_Position.x >= xMidPoint) //Right Side
    {
        if (aDot->m_Position.y < yMidPoint) //Top Right
        {
            if (m_TopRightQuadTree == nullptr)
                m_TopRightQuadTree = new QuadTree(
                    glm::vec2(xMidPoint, m_TopLeftBoundary.y),
                    glm::vec2(m_BottomRightBoundary.x, yMidPoint));
            m_TopRightQuadTree->Insert(aDot);
        }
        else if (aDot->m_Position.y >= yMidPoint) //Bottom Right
        {
            if (m_BottomRightQuadTree == nullptr)
                m_BottomRightQuadTree = new QuadTree(
                    glm::vec2(xMidPoint, yMidPoint),
                    m_BottomRightBoundary);
            m_BottomRightQuadTree->Insert(aDot);
        }
    }
}

void QuadTree::Visualize(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    SDL_RenderLine(renderer, m_TopLeftBoundary.x,
        m_TopLeftBoundary.y,
        m_BottomRightBoundary.x,
        m_TopLeftBoundary.y); //Top line

    SDL_RenderLine(renderer, m_TopLeftBoundary.x,
        m_BottomRightBoundary.y,
        m_BottomRightBoundary.x,
        m_BottomRightBoundary.y); //Bottom line

    SDL_RenderLine(renderer, m_TopLeftBoundary.x,
        m_TopLeftBoundary.y,
        m_TopLeftBoundary.x,
        m_BottomRightBoundary.y); //Left Line

    SDL_RenderLine(renderer, m_BottomRightBoundary.x,
        m_TopLeftBoundary.y,
        m_BottomRightBoundary.x,
        m_BottomRightBoundary.y); //RightLine
    
    if (m_TopLeftQuadTree != nullptr)
        m_TopLeftQuadTree->Visualize(renderer);
    if (m_TopRightQuadTree != nullptr)
        m_TopRightQuadTree->Visualize(renderer);
    if (m_BottomLeftQuadTree != nullptr)
        m_BottomLeftQuadTree->Visualize(renderer);
    if (m_BottomRightQuadTree != nullptr)
        m_BottomRightQuadTree->Visualize(renderer);
}

