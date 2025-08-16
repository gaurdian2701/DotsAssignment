#pragma once

#include <vector>
class QuadTree;
struct SDL_Texture;
static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 800;

class Dot;
class DotRenderer;

const int m_DotAmount = 1000;

class Game
{
public:
	Game(DotRenderer* aRenderer);
	void Update(float aDeltaTime);
	void Render(float aDeltaTime);
	void CleanUp();

	std::vector<Dot*> m_Dots;

private:
	DotRenderer* m_Renderer = nullptr;
	SDL_Texture* m_ScreenTexture = nullptr;
	QuadTree* m_CollisionQuadTree = nullptr;
	unsigned int NumberOfThreads = 0;

	void RenderDotsPartition(int startingPixelBufferIndex,
		int endingPixelBufferIndex, float aDeltaTime);
};

