#include "Chess.h"

Chessboard::Chessboard()
{
	for (int i = 0; i < 64; i++)
	{
		arr[i].position = i;
		arr[i].piece = nullptr;
	}

	white = std::vector<Piece*>{};
	black = std::vector<Piece*>{};
	history = std::vector<Move>{};

	turn = Color::WHITE;
}

Chessboard::~Chessboard()
{
	for (auto p : white)
	{
		DestroyPiece(p);
	}
	for (auto p : black)
	{
		DestroyPiece(p);
	}
}

void Chessboard::MakeMove(Move m)
{
	// todo
}

inline SDL_Texture* Chessboard::GetTexture(Piece* p)
{
	return p->GetTexture();
}

void Chessboard::DestroyPiece(Piece* p)
{
	for (auto b = white.begin(); b != white.end(); b++)
	{
		if (*b == p)
		{
			white.erase(b);
			std::cout << "Destroying white piece with address " << p << std::endl;
			delete p;
			return;
		}
	}
	for (auto b = black.begin(); b != black.end(); b++)
	{
		if (*b == p)
		{
			black.erase(b);
			std::cout << "Destroying black piece with address " << p << std::endl;
			delete p;
			return;
		}
	}
	//SDL_DestroyTexture(p->GetTexture());

}

void Chessboard::DrawPieces(SDL_Renderer* r)
{
	for (auto& const p : white)
	{
		SDL_RenderCopy(r, p->GetTexture(), NULL, p->GetPosition());
	}
	for (auto& const p : black)
	{
		SDL_RenderCopy(r, p->GetTexture(), NULL, p->GetPosition());
	}
}

Color& const Chessboard::GetTurn()
{
	return turn;
}

void Chessboard::ChangeTurn()
{
	if (turn == Color::WHITE)
	{
		turn = Color::BLACK;
	}
	else
	{
		turn = Color::WHITE;
	}
}

Square* Chessboard::GetSquare(int index)
{
	return &arr[index];
}

void Chessboard::AddMove(Move m)
{
	history.push_back(m);
}

std::vector<Move> Chessboard::FindMechanicalMoves(Color c)
{

}
