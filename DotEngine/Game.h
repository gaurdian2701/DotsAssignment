#pragma once

#include <random>
#include <vector>
#include "Dot.h"
#include "Config.h"

class QuadTree;
struct SDL_Texture;

class Dot;
class DotRenderer;


class Game
{
public:
	Game(DotRenderer* aRenderer);
	void Update(float aDeltaTime);
	void CalculateCollisions();
	void DoCollisionsAsync(std::vector<Dot*>& dotsToCalculateCollisionsFor);
	void CalculateCollisionsAsync();
	void Render(float aDeltaTime);
	void CleanUp();

	std::vector<Dot> m_Dots;
	std::random_device m_randomGenerator;
	std::uniform_real_distribution<float> m_widthDistribution;
	std::uniform_real_distribution<float> m_heightDistribution;

private:
	DotRenderer* m_Renderer = nullptr;
	SDL_Texture* m_ScreenTexture = nullptr;
	QuadTree* m_CollisionQuadTree = nullptr;
	unsigned int NumberOfThreads = 0;

	void RenderDotsPartition(int startingPixelBufferIndex,
		int endingPixelBufferIndex, float aDeltaTime);
	void InitQuadTree();
	void ClearQuadTree();
};

