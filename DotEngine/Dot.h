#pragma once
#include "glm/glm.hpp"

class DotRenderer;

class Dot
{
public:

	Dot(glm::vec2& aPosition, float aRadius);
	void Render(DotRenderer* aRenderer, float dt);
	void TakeDamage(int someDamage);

	glm::vec2 m_Position;
	glm::vec2 m_Velocity;

	float m_TotalTime = 0;
	float m_Radius = 0;

	uint8_t m_Health = 3;
};

