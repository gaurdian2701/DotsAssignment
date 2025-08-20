#include "QuadTree.h"
#include <iostream>
#include "Dot.h"

QuadTree::QuadTree(glm::vec2 topLeftBoundary, glm::vec2 bottomRightBoundary, uint8_t currentDepth)
{
    m_TopLeftBoundary = topLeftBoundary;
    m_BottomRightBoundary = bottomRightBoundary;
    m_currentDepth = currentDepth;
}

QuadTree::~QuadTree()
{
    m_DotsInTree.clear();
    delete m_TopLeftQuadTree;
    delete m_TopRightQuadTree;
    delete m_BottomLeftQuadTree;
    delete m_BottomRightQuadTree;
}

void QuadTree::Insert(Dot* aDot)
{
    // Quad boundaries are close to the size of the dot
    if (m_currentDepth == m_MaxDepth)
    {
        m_DotsInTree.push_back(aDot);
        return;
    }

    if (aDot->m_Position.x < (m_TopLeftBoundary.x + m_BottomRightBoundary.x) / 2) //Left Side
    {
        if (aDot->m_Position.y < (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2) //Top Left
        {
            if (m_TopLeftQuadTree == nullptr && m_currentDepth < m_MaxDepth)
            {
                m_TopLeftQuadTree = new QuadTree(
                    m_TopLeftBoundary,
                    glm::vec2((m_TopLeftBoundary.x + m_BottomRightBoundary.x) / 2,
                              (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2),
                    m_currentDepth + 1);
            }
            m_TopLeftQuadTree->Insert(aDot);
        }
        else if (aDot->m_Position.y >= (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2) //Bottom Left
        {
            if (m_BottomLeftQuadTree == nullptr && m_currentDepth < m_MaxDepth)
            {
                m_BottomLeftQuadTree = new QuadTree(
                    glm::vec2(m_TopLeftBoundary.x,
                              (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2),
                    glm::vec2((m_TopLeftBoundary.x + m_BottomRightBoundary.x) / 2,
                              m_BottomRightBoundary.y),
                    m_currentDepth + 1);
            }
            m_BottomLeftQuadTree->Insert(aDot);
        }
    }
    else if (aDot->m_Position.x >= (m_TopLeftBoundary.x + m_BottomRightBoundary.x) / 2) //Right Side
    {
        if (aDot->m_Position.y < (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2) //Top Right
        {
            if (m_TopRightQuadTree == nullptr && m_currentDepth < m_MaxDepth)
            {
                m_TopRightQuadTree = new QuadTree(
                    glm::vec2((m_TopLeftBoundary.x + m_BottomRightBoundary.x) / 2,
                              m_TopLeftBoundary.y),
                    glm::vec2(m_BottomRightBoundary.x,
                              (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2),
                    m_currentDepth + 1);
            }
            m_TopRightQuadTree->Insert(aDot);
        }
        else if (aDot->m_Position.y >= (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2) //Bottom Right
        {
            if (m_BottomRightQuadTree == nullptr && m_currentDepth < m_MaxDepth)
            {
                m_BottomRightQuadTree = new QuadTree(
                    glm::vec2((m_TopLeftBoundary.x + m_BottomRightBoundary.x) / 2,
                              (m_TopLeftBoundary.y + m_BottomRightBoundary.y) / 2),
                    m_BottomRightBoundary,
                    m_currentDepth + 1);
            }
            m_BottomRightQuadTree->Insert(aDot);
        }
    }
}

void QuadTree::QueryRange(Dot* aDot, std::vector<Dot*>& closestDots,
                          glm::vec2& topLeftOfQueryBox, glm::vec2& bottomRightOfQueryBox)
{
    if (m_currentDepth < m_MaxDepth)
    {
        if (m_TopLeftQuadTree != nullptr &&
            m_TopLeftQuadTree->IntersectsTree(topLeftOfQueryBox, bottomRightOfQueryBox))
        {
            m_TopLeftQuadTree->QueryRange(aDot, closestDots,
                                          topLeftOfQueryBox, bottomRightOfQueryBox);
        }
        if (m_BottomLeftQuadTree != nullptr &&
            m_BottomLeftQuadTree->IntersectsTree(topLeftOfQueryBox, bottomRightOfQueryBox))
        {
            m_BottomLeftQuadTree->QueryRange(aDot, closestDots,
                                             topLeftOfQueryBox, bottomRightOfQueryBox);
        }
        if (m_TopRightQuadTree != nullptr &&
            m_TopRightQuadTree->IntersectsTree(topLeftOfQueryBox, bottomRightOfQueryBox))
        {
            m_TopRightQuadTree->QueryRange(aDot, closestDots,
                                           topLeftOfQueryBox, bottomRightOfQueryBox);
        }
        if (m_BottomRightQuadTree != nullptr &&
            m_BottomRightQuadTree->IntersectsTree(topLeftOfQueryBox, bottomRightOfQueryBox))
        {
            m_BottomRightQuadTree->QueryRange(aDot, closestDots,
                                              topLeftOfQueryBox, bottomRightOfQueryBox);
        }
    }
    else
    {
        for (int i = 0; i < m_DotsInTree.size(); i++)
        {
            if (m_DotsInTree[i] != nullptr)
            {
                closestDots.push_back(m_DotsInTree[i]);
            }
        }
    }
}

void QuadTree::GetDotsInQuadTreePartition(const QuadTreePartitionArea partitionArea,
                                          std::vector<Dot*>& dotsToBeFound)
{
    switch (partitionArea)
    {
    case TOPLEFT:
        m_TopLeftQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        break;
    case TOPRIGHT:
        m_TopRightQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        break;
    case BOTTOMLEFT:
        m_BottomLeftQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        break;
    case BOTTOMRIGHT:
        m_BottomRightQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        break;
    }

    
    std::cout << dotsToBeFound.size() << std::endl;
}

void QuadTree::GetAllDotsInQuadTree(std::vector<Dot*>& dotsToBeFound)
{
    if (m_currentDepth == m_MaxDepth)
    {
        for (int i = 0; i < m_DotsInTree.size(); i++)
        {
            if (m_DotsInTree[i] != nullptr)
            {
                dotsToBeFound.push_back(m_DotsInTree[i]);
            }
        }
    }
    else
    {
        if (m_TopLeftQuadTree != nullptr)
        {
            m_TopLeftQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        }
        if (m_BottomLeftQuadTree != nullptr)
        {
            m_BottomLeftQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        }
        if (m_TopRightQuadTree != nullptr)
        {
            m_TopRightQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        }
        if (m_BottomRightQuadTree != nullptr)
        {
            m_BottomRightQuadTree->GetAllDotsInQuadTree(dotsToBeFound);
        }
    }
}

bool QuadTree::IntersectsTree(glm::vec2& topLeftOfQueryBox, glm::vec2& bottomRightOfQueryBox)
{
    return !(bottomRightOfQueryBox.x < m_TopLeftBoundary.x ||
        topLeftOfQueryBox.x > m_BottomRightBoundary.x ||
        bottomRightOfQueryBox.y < m_TopLeftBoundary.y ||
        topLeftOfQueryBox.y > m_BottomRightBoundary.y);
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
