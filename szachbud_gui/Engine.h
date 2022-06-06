#pragma once

#include "Config.h"
#include "Chess.h"
#include "AI.h"

class Engine
{
public:
	Engine(const char* gfx);

	~Engine();

	void MainLoop();

private:

	void Draw();

	template <typename T> T* CreatePiece(Color c, ChessPosition pos, const char* gfx)
	{
		return cb->CreatePiece<T>(c, pos, gfx, renderer);
	}

	Square* PixelsToSquare(int x, int y);
	SDL_Rect SquareToPixels(Square* s);

	std::unique_ptr<Chessboard> cb;
	std::unique_ptr<AI> ai;

	bool running;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* mainTexture;
	SDL_Event event;
	SDL_Point clickOffset;
};
