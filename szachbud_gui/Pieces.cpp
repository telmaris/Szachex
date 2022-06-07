#include "Pieces.h"

Piece::Piece(Color c, Position p, const char* gfx, SDL_Renderer* rend)
{
	color = c;

	rect.x = p.x + 10;
	rect.y = p.y + 10;
	rect.w = 80;
	rect.h = 80;

	pos = ((7 - (p.y / 100)) * 8) + (p.x / 100);
	std::cout << pos << std::endl;

	moveCounter = 0;

	texture = LoadTexture(rend, gfx);

	if (!texture)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	LegalMoves = std::vector<Move>{};
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
	std::array<int, 4> Offset{ 1, -1, 8, -8 };
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
bool Knight::IsValidPosition(int posit, int offset)
{
	int position = posit + offset;
	bool knightCheck = true;

	if ((((abs(offset) == 15) || abs(offset) == 17) && (abs(floor(posit / 8) - floor(position / 8)) != 2))) { knightCheck = false; }
	if ((((abs(offset) == 6) || (abs(offset) == 10)) && (abs(floor(posit / 8) - floor(position / 8)) != 1))) { knightCheck = false; }

	return (position >= 0 && position < 64) && knightCheck == true;
}

std::vector<Move> Knight::GetPieceMoves()
{
	std::array<int, 8> Offset{ 17, -17, 15, -15, 10, -10, 6, -6 };
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

// Pawn
bool Pawn::IsValidPosition(int posit, int offset)
{
	bool cond = true;
	int position = posit + offset;
	if ((abs(offset) == 7) && (floor(posit / 8) == floor(position / 8))) { cond = false; }
	if ((abs(offset) == 9) && (abs(floor(posit / 8) - floor(position / 8)) > 1)) { cond = false; }
	return (position >= 0 && position < 64) && cond == true;
}

std::vector<Move> Pawn::GetPieceMoves()
{
	std::vector<Move> moves;
	if (color == Color::WHITE)
	{
		if (moveCounter == 0)
		{
			Move m{ pos, pos + 16 };
			moves.push_back(m);
		}
		Move m{ pos, pos + 8 };
		moves.push_back(m);
		if (IsValidPosition(pos, 7))
		{
			Move m{ pos, pos + 7 };
			moves.push_back(m);
		}
		if (IsValidPosition(pos, 9))
		{
			Move m{ pos, pos + 9 };
			moves.push_back(m);
		}
	}
	else
	{
		if (moveCounter == 0)
		{
			Move m{ pos, pos - 16 };
			moves.push_back(m);
		}
		Move m{ pos, pos - 8 };
		moves.push_back(m);
		if (IsValidPosition(pos, -7))
		{
			Move m{ pos, pos - 7 };
			moves.push_back(m);
		}
		if (IsValidPosition(pos, -9))
		{
			Move m{ pos, pos - 9 };
			moves.push_back(m);
		}
	}
	return moves;
}




Pawn::Pawn(Color c, Position pos, const char* gfx, SDL_Renderer* rend) : Piece(c, pos, gfx, rend) { type = PAWN; }
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
