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
	SDL_DestroyTexture(texture);
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
// Queen
bool Queen::IsValidPosition(int posit, int offset)
{
	int position = posit + offset;
	bool horizontalCheck = true;
	if ((abs(floor(posit / 8) - floor(position / 8)) == 1) && (abs(offset) == 1)) { horizontalCheck = false; }

	bool diagonalCheck = true;
	if (((floor(posit / 8) == floor(position / 8)) && (abs(offset) == 7)) || (abs(floor(posit / 8) - floor(position / 8)) > 1) && (abs(offset) == 9)) { diagonalCheck = false; }

	return (position >= 0 && position < 64) && horizontalCheck == true && diagonalCheck == true;
}

std::vector<Move> Queen::GetPieceMoves()
{
	std::array<int, 8> Offset{ 1, -1, 7, -7, 8, -8, 9, -9 };
	std::vector<Move> moves;
	for (int off : Offset)
	{
		int currentPos = pos;
		while (IsValidPosition(currentPos, off))
		{
			currentPos += off;
			auto m = Move{ pos, currentPos };
			moves.push_back(m);
		}
	}
	return moves;
}

// King
bool King::IsValidPosition(int posit, int offset)
{
	int position = posit + offset;
	bool horizontalCheck = true;
	if ((abs(floor(posit / 8) - floor(position / 8)) == 1) && (abs(offset) == 1)) { horizontalCheck = false; }

	bool diagonalCheck = true;
	if (((floor(posit / 8) == floor(position / 8)) && (abs(offset) == 7)) || (abs(floor(posit / 8) - floor(position / 8)) > 1) && (abs(offset) == 9)) { diagonalCheck = false; }

	return (position >= 0 && position < 64) && (horizontalCheck == true && diagonalCheck == true);
}

std::vector<Move> King::GetPieceMoves()
{
	std::array<int, 8> Offset{ 1, -1, 7, -7, 8, -8, 9, -9 };
	std::vector<Move> moves;
	for (int off : Offset)
	{
		int currentPos = pos;
		if (IsValidPosition(currentPos, off))
		{
			currentPos += off;
			auto m = Move{ pos, currentPos };
			moves.push_back(m);
		}
	}
	return moves;
}

// Rook
bool Rook::IsValidPosition(int posit, int offset)
{
	int position = posit + offset;
	bool horizontalCheck = true;
	if ((abs(floor(posit / 8) - floor(position / 8)) == 1) && (abs(offset) == 1)) { horizontalCheck = false; }

	return (position >= 0 && position < 64) && horizontalCheck == true;
}

std::vector<Move> Rook::GetPieceMoves()
{
	std::array<int, 4> Offset{ 1, -1, 8, -8};
	std::vector<Move> moves;
	for (int off : Offset)
	{
		int currentPos = pos;
		while (IsValidPosition(currentPos, off))
		{
			currentPos += off;
			auto m = Move{ pos, currentPos };
			moves.push_back(m);
		}
	}
	return moves;
}

// Bishop
bool Bishop::IsValidPosition(int posit, int offset)
{
	int position = posit + offset;
	bool diagonalCheck = true;
	if (((floor(posit / 8) == floor(position / 8)) && (abs(offset) == 7)) || (abs(floor(posit / 8) - floor(position / 8)) > 1) && (abs(offset) == 9)) { diagonalCheck = false; }

	return (position >= 0 && position < 64) && diagonalCheck == true;
}

std::vector<Move> Bishop::GetPieceMoves()
{
	std::array<int, 8> Offset{ 7, -7, 9, -9 };
	std::vector<Move> moves;
	for (int off : Offset)
	{
		int currentPos = pos;
		while (IsValidPosition(currentPos, off))
		{
			currentPos += off;
			auto m = Move{ pos, currentPos };
			moves.push_back(m);
		}
	}
	return moves;
}

// Knight




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
