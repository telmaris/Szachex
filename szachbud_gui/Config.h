#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <unordered_set>

#include <SDL.h>
#include <SDL_image.h>

#define SDL_MOUSE_RELATIVE 0

#define WINDOW_X 800
#define WINDOW_Y 800

//#define MAX_FPS 200

enum class Color
{
	WHITE,
	BLACK
};

enum Type
{
	DEFAULT = 1,
	PAWN = 2,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

struct Position
{
	int x, y;
};

struct ChessPosition
{
	char col;
	int row;
};

inline SDL_Texture* LoadTexture(SDL_Renderer* r, const char* gfx)
{
	SDL_Surface* surf = IMG_Load(gfx);

	if (!surf)
	{
		std::cout << SDL_GetError() << std::endl;
		return nullptr;
	}

	auto text = SDL_CreateTextureFromSurface(r, surf);

	SDL_FreeSurface(surf);

	if (!text)
	{
		std::cout << SDL_GetError() << std::endl;
		return nullptr;
	}

	return text;
}

inline void MoveToChessPosition(int m)
{
	std::cout << static_cast<char>(97 + m % 8) << static_cast<int>(1 + m / 8);
}

