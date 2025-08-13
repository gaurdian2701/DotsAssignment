#include "Game.h"
#include "DotRenderer.h"
#include "Dot.h"
#include "glm/glm.hpp"
#include <algorithm>



Game::Game(DotRenderer* aRenderer)
{
	m_Renderer = aRenderer;
	glm::vec2 startPos = { 0.0f, 0.0f };

	for (size_t i = 0; i < m_DotAmount; i++)
	{
		startPos = { std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT};
		Dot* dot = new Dot(startPos, 3);
		m_Dots.push_back(dot);
	}
}

void Game::Update(float aDeltaTime)
{
	glm::vec2 NewDotStartPos = { 0.0f, 0.0f };
	Dot* dot1 = nullptr;
	Dot* dot2 = nullptr;

	for (size_t i = 0; i < m_Dots.size(); i++)
	{
		dot1 = m_Dots[i];
		for(size_t j = i + 1; j < m_Dots.size(); j++)
		{
			dot2 = m_Dots[j];

			if (dot1 != nullptr && dot2 != nullptr)
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
					dot1->m_Position = { std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT };
					dot1->m_Health = 3;
					dot1->m_Radius = 3;
				}
			}
		}
	}

	for (Dot* d : m_Dots)
	{
		if (d != nullptr)
		{
			d->Render(m_Renderer, aDeltaTime);
		}
	}
}



void Game::CleanUp()
{
	for(Dot* d : m_Dots)
	{
		if (d != nullptr)
		{
			delete d;
			d = nullptr;
		}
	}
}
