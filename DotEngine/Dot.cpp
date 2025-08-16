#include "Dot.h"
#include "DotRenderer.h"
#include "Game.h"
#include <random>
#include <glm/gtc/constants.hpp>

const float DotVelocity = 50.0f;

Dot::Dot(glm::vec2& aPosition, float aRadius)
{
	m_Position = aPosition;
	m_StartPos = aPosition;
	m_Radius = aRadius;

	static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
	std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

	m_Velocity = glm::vec2(dist(rng), dist(rng));

	float angle = dist(rng) * glm::pi<float>() / 100.0f;
	m_Velocity = glm::vec2(cos(angle), sin(angle));
}

void Dot::Render(DotRenderer* aRenderer, float dt)
{
	m_TotalTime += dt;

	m_Position += m_Velocity * DotVelocity * dt;

	float redColor = (glm::cos((m_TotalTime + m_StartPos.x) * 0.1f) * 0.5f + 0.5f) * 255.0f;

	float greenColor = (glm::cos((m_TotalTime + m_StartPos.y) * 0.9f) * 0.5f + 0.5f) * 255.0f;

	float blueColor = (glm::cos(m_TotalTime * 0.4f) * 0.5f + 0.5f) * 255.0f;
	
	aRenderer->DrawFilledCircle(m_Position.x, m_Position.y, m_Radius, redColor, blueColor, greenColor);

	if (m_Position.x - m_Radius < 0.0f)
	{
		m_Position.x = m_Radius;
		m_Velocity.x *= -1;
	}
	else if (m_Position.x + m_Radius > SCREEN_WIDTH)
	{
		m_Position.x = SCREEN_WIDTH - m_Radius;
		m_Velocity.x *= -1;
	}

	if (m_Position.y - m_Radius < 0.0f)
	{
		m_Position.y = m_Radius;
		m_Velocity.y *= -1;
	}
	else if (m_Position.y + m_Radius > SCREEN_HEIGHT)
	{
		m_Position.y = SCREEN_HEIGHT - m_Radius;
		m_Velocity.y *= -1;
	}
}

void Dot::TakeDamage(int someDamage)
{
	m_Health -= someDamage;
}



