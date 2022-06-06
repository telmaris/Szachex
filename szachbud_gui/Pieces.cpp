#include "Pieces.h"

Piece::Piece(Color c, Position p, const char* gfx, SDL_Renderer* rend)
{
	color = c;

	rect.x = p.x + 10;
	rect.y = p.y + 10;
	rect.w = 80;
	rect.h = 80;

	moveCounter = 0;

	texture = LoadTexture(rend, gfx);

	if (!texture)
	{
		std::cout << SDL_GetError() << std::endl;
	}
}

Piece::~Piece()
{
	std::cout << "Piece destroyed!" << std::endl;
}

SDL_Texture* Piece::GetTexture()
{
	return texture;
}

SDL_Rect* Piece::GetPosition()
{
	return &rect;
}

bool Pawn::MovePiece(Move m)
{
	return false;
}

bool Knight::MovePiece(Move m)
{
	return false;
}

bool Bishop::MovePiece(Move m)
{
	return false;
}

bool Rook::MovePiece(Move m)
{
	return false;
}

bool Queen::MovePiece(Move m)
{
	return false;
}

bool King::MovePiece(Move m)
{
	return false;
}

Pawn::Pawn(Color c, Position pos, const char* gfx, SDL_Renderer* rend) : Piece(c, pos, gfx, rend) {}
Knight::Knight(Color c, Position pos, const char* gfx, SDL_Renderer* rend) : Piece(c, pos, gfx, rend) {}
Bishop::Bishop(Color c, Position pos, const char* gfx, SDL_Renderer* rend) : Piece(c, pos, gfx, rend) {}
Rook::Rook(Color c, Position pos, const char* gfx, SDL_Renderer* rend) : Piece(c, pos, gfx, rend) {}
Queen::Queen(Color c, Position pos, const char* gfx, SDL_Renderer* rend) : Piece(c, pos, gfx, rend) {}
King::King(Color c, Position pos, const char* gfx, SDL_Renderer* rend) : Piece(c, pos, gfx, rend) {}

Piece* Pawn::CopyPiece() { return new Pawn{ *this }; }
Piece* Knight::CopyPiece() { return new Knight{ *this }; }
Piece* Bishop::CopyPiece() { return new Bishop{ *this }; }
Piece* Rook::CopyPiece() { return new Rook{ *this }; }
Piece* Queen::CopyPiece() { return new Queen{ *this }; }
Piece* King::CopyPiece() { return new King{ *this }; }
