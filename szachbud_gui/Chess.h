#pragma once

#include "Config.h"
#include "Pieces.h"

struct Square
{
	int position;
	Piece* piece;
};

class Chessboard
{
public:
	Chessboard();
	~Chessboard();

	void MakeMove(Move m);
	void AddMove(Move m);

	bool IsMoveValid(const Move& m);

	std::vector<Move> FindMechanicalMoves(Color c);
	void FindLegalMoves(std::vector<Move> v);

	template <typename T> T* CreatePiece(Color c, ChessPosition pos, SDL_Texture* t)
	{
		Position position{ (static_cast<uint8_t>(pos.col) - 97) * 100, 800 - 100 * pos.row };

		T* piece = new T{ c, position, t };

		if (c == Color::WHITE)
		{
			white.push_back(static_cast<Piece*>(piece));
		}
		else
		{
			black.push_back(static_cast<Piece*>(piece));
		}

		arr[8 * (pos.row - 1) + (static_cast<uint8_t>(pos.col) - 97)].piece = piece;

		return piece;
	}

	void DestroyPiece(Piece* p);

	inline SDL_Texture* GetTexture(Piece* p);
	Color GetTurn();
	void ChangeTurn();
	Square* GetSquare(int index);

	void DrawPieces(SDL_Renderer* r);

	std::unique_ptr<Chessboard> Copy();

	std::vector<Move> legalMoves;

private:

	bool IsCheck(Move m);

	bool IsCollision(Move m);

	std::vector<Move> PawnEvents(Color c, std::vector<Move>& moves);

	void deleteIfEnPassant(Move move);

	std::array<Square, 64> arr;

	std::vector<Piece*> white;
	std::vector<Piece*> black;

	Color turn;
	int whiteKingPosition, blackKingPosition;

	std::vector<Move> history;

};
