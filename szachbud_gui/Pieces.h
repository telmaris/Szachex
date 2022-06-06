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

	virtual bool MovePiece(Move m) = 0;

	SDL_Texture* GetTexture();
	SDL_Rect* GetPosition();

	virtual Piece* CopyPiece() = 0;

	Color color;
	int moveCounter;

protected:
	SDL_Texture* texture;
	SDL_Rect rect;
};

class Pawn : public Piece
{
public:
	Pawn(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Pawn();
	bool MovePiece(Move m) override;
	Piece* CopyPiece() override;

};

class Knight : public Piece
{
public:
	Knight(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Knight();
	bool MovePiece(Move m) override;
	Piece* CopyPiece() override;
};

class Bishop : public Piece
{
public:
	Bishop(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Bishop();
	bool MovePiece(Move m) override;
	Piece* CopyPiece() override;
};

class Rook : public Piece
{
public:
	Rook(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Rook();
	bool MovePiece(Move m) override;
	Piece* CopyPiece() override;
};

class Queen : public Piece
{
public:
	Queen(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~Queen();
	bool MovePiece(Move m) override;
	Piece* CopyPiece() override;
};

class King : public Piece
{
public:
	King(Color c, Position pos, const char* gfx, SDL_Renderer* rend);

	//~King();
	bool MovePiece(Move m) override;
	Piece* CopyPiece() override;
};
