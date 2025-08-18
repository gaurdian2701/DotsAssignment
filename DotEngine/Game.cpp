#include "Game.h"
#include "DotRenderer.h"
#include "Dot.h"
#include "QuadTree.h"
#include "future"
#include "glm/glm.hpp"
#include <algorithm>

Game::Game(DotRenderer* aRenderer)
{
    m_Renderer = aRenderer;
    m_ScreenTexture = SDL_CreateTexture(m_Renderer->GetSDLRenderer(),
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT);

    NumberOfThreads = std::thread::hardware_concurrency();

    glm::vec2 startPos = {0.0f, 0.0f};

    for (size_t i = 0; i < m_DotAmount; i++)
    {
        startPos = {std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT};
        Dot* dot = new Dot(startPos, 2);
        m_Dots.push_back(dot);
    }
}

void Game::Update(float aDeltaTime)
{
    InitQuadTree();
    CalculateCollisions();
    if (DrawQuadTreeVisualization)
    {
        m_CollisionQuadTree->Visualize(m_Renderer->GetSDLRenderer());
    }
    ClearQuadTree();
}

void Game::InitQuadTree()
{
    m_CollisionQuadTree = new QuadTree(glm::vec2(0, 0),
                                       glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 0);

    for (size_t i = 0; i < m_DotAmount; i++)
        m_CollisionQuadTree->Insert(m_Dots[i]);
}

void Game::CalculateCollisions()
{
    glm::vec2 NewDotStartPos = {0.0f, 0.0f};

    Dot* dot1 = nullptr;
    Dot* dot2 = nullptr;
    std::vector<Dot*> dotsToCollideWith;
    glm::vec2 topLeftOfQueryBox = {0.0f, 0.0f};
    glm::vec2 bottomRightOfQueryBox = {0.0f, 0.0f};
    for (size_t i = 0; i < m_DotAmount; i++)
    {
        dot1 = m_Dots[i];
        topLeftOfQueryBox = {dot1->m_Position.x - 2 * dot1->m_Radius,
                dot1->m_Position.y - 2 * dot1->m_Radius};
        bottomRightOfQueryBox = {dot1->m_Position.x + 2 * dot1->m_Radius,
                    dot1->m_Position.y + 2 * dot1->m_Radius};
        m_CollisionQuadTree->QueryRange(dot1, dotsToCollideWith,
            topLeftOfQueryBox, bottomRightOfQueryBox);
        for (size_t j = 0; j < dotsToCollideWith.size(); j++)
        {
            dot2 = dotsToCollideWith[j];
            if (dot1 != nullptr && dot2 != nullptr && dot1 != dot2)
            {
                float dist = glm::distance(dot1->m_Position, dot2->m_Position);
                float minDist = dot1->m_Radius + dot2->m_Radius;
        
                if (dist < minDist)
                {
                    glm::vec2 normal = glm::normalize(dot2->m_Position - dot1->m_Position);
        
                    dot1->m_Velocity = glm::reflect(dot1->m_Velocity, normal);
                    dot2->m_Velocity = glm::reflect(dot2->m_Velocity, -normal);
        
                    float overlap1 = 1.5f * ((minDist + 1) - dist);
                    float overlap2 = 1.5f * (minDist - dist);
                    dot1->m_Position -= normal * overlap1;
                    dot2->m_Position += normal * overlap2;
                    dot1->TakeDamage(1);
                    dot1->m_Radius++;
                }
                if (dot1->m_Health <= 0)
                {
                    dot1->m_Position = {std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT};
                    dot1->m_Health = 3;
                    dot1->m_Radius = 3;
                }
            }
        }
        dotsToCollideWith.clear();
    }
}

inline void Game::ClearQuadTree()
{
    delete m_CollisionQuadTree;
}

void Game::Render(float aDeltaTime)
{
    std::fill(m_Renderer->m_PixelBuffer.begin(),
              m_Renderer->m_PixelBuffer.end(),
              0x00000000);

    std::vector<std::future<void>> futures;
    int dotsPerThread = m_Dots.size() / NumberOfThreads;

    for (int i = 0; i < NumberOfThreads; i++)
    {
        int startingIndex = i * dotsPerThread;
        int endingIndex = i == NumberOfThreads - 1 ? m_Dots.size() : startingIndex + dotsPerThread;
        futures.push_back(std::async(std::launch::async, &Game::RenderDotsPartition, this,
                                     startingIndex, endingIndex, aDeltaTime));
    }

    for (auto& f : futures)
    {
        f.get();
    }

    SDL_UpdateTexture(m_ScreenTexture,
                      nullptr,
                      m_Renderer->m_PixelBuffer.data(),
                      SCREEN_WIDTH * sizeof(uint32_t));

    SDL_RenderTexture(m_Renderer->GetSDLRenderer(), m_ScreenTexture, nullptr, nullptr);
}

void Game::RenderDotsPartition(int startingPixelBufferIndex,
                               int endingPixelBufferIndex, float aDeltaTime)
{
    for (int i = startingPixelBufferIndex; i < endingPixelBufferIndex; i++)
    {
        if (m_Dots[i] != nullptr)
        {
            m_Dots[i]->Render(m_Renderer, aDeltaTime);
        }
    }
}


void Game::CleanUp()
{
    for (Dot* d : m_Dots)
    {
        if (d != nullptr)
        {
            delete d;
            d = nullptr;
        }
    }

    delete m_Renderer;
    delete m_CollisionQuadTree;
    SDL_DestroyTexture(m_ScreenTexture);
}
