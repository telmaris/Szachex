#pragma once

#include "Config.h"

struct Move
{
	int src, dest;
};

class Piece
{
public:

	Piece(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	~Piece();

	virtual std::vector<Move> GetPieceMoves() = 0;

	SDL_Texture* GetTexture();
	SDL_Rect* GetPosition();
	std::vector<Move> LegalMoves;

	virtual Piece* CopyPiece() = 0;

	Color color;
	int moveCounter;
	int pos;
	Type type;
	


protected:
	SDL_Texture* texture;
	SDL_Rect rect;
	virtual bool IsValidPosition(int position, int offset) = 0;
};

class Pawn : public Piece
{
public:
	Pawn(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Pawn();
	Piece* CopyPiece() override;
	std::vector<Move> GetPieceMoves() override;
private:
	bool IsValidPosition(int position, int offset) override;
	Type type = PAWN;

};

class Knight : public Piece
{
public:
	Knight(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Knight();
	Piece* CopyPiece() override;
	std::vector<Move> GetPieceMoves() override;
private:
	bool IsValidPosition(int position, int offset) override;
	Type type = KNIGHT;

};

class Bishop : public Piece
{
public:
	Bishop(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Bishop();
	Piece* CopyPiece() override;
	std::vector<Move> GetPieceMoves() override;
private:
	bool IsValidPosition(int position, int offset) override;
	Type type = BISHOP;
};

class Rook : public Piece
{
public:
	Rook(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Rook();
	Piece* CopyPiece() override;
	std::vector<Move> GetPieceMoves() override;
private:
	bool IsValidPosition(int position, int offset) override;
	Type type = ROOK;
};

class Queen : public Piece
{
public:
	Queen(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Queen();
	Piece* CopyPiece() override;
	std::vector<Move> GetPieceMoves() override;
private:
	bool IsValidPosition(int position, int offset) override;
	Type type = QUEEN;
};

class King : public Piece
{
public:
	King(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~King();
	Piece* CopyPiece() override;
	std::vector<Move> GetPieceMoves() override;
private:
	bool IsValidPosition(int position, int offset) override;
	Type type = KING;
};
