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

	legalMoves = std::vector<Move>{};

	turn = Color::WHITE;
}

Chessboard::~Chessboard()
{
	for (auto p : white)
	{
		delete p;
	}
	for (auto p : black)
	{
		delete p;
	}
}

void Chessboard::MakeMove(Move m)
{
	if (GetSquare(m.dest)->piece)
	{
		DestroyPiece(GetSquare(m.dest)->piece);
		GetSquare(m.dest)->piece = nullptr;
	}

	GetSquare(m.dest)->piece = GetSquare(m.src)->piece;
	GetSquare(m.src)->piece = nullptr;

	AddMove(m);
	ChangeTurn();
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

Color Chessboard::GetTurn()
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

// TODO: add body to these functions
bool Chessboard::IsCheck()
{
	return false;
}

bool Chessboard::IsCheck(Move move) { return false; }

bool Chessboard::IsCollision(Move move)
{
	int a = move.dest - move.src;
	int b = abs(floor(move.dest / 8) - floor(move.src / 8));
	int offset;

	if (b == 0) { offset = a / abs(a); }
	else { offset = a / b; }

	bool wasEnemySpotted = false;
	int currPos = move.src;

	if (GetSquare(move.src)->piece->type == KNIGHT)
	{
		offset = a;
	}

	while (currPos != move.dest)
	{
		if (wasEnemySpotted == true)
		{
			return true;
		}
		if (GetSquare((currPos + offset))->piece != nullptr)
		{

			if (GetSquare(currPos)->piece->type == PAWN && abs(offset) == 8)
			{
				return true;
			}
			else
			{
				// friendly piece
				if (GetSquare(currPos + offset)->piece->color == GetSquare(move.src)->piece->color)
				{
					return true;
				}
				// enemy piece
				if (GetSquare(currPos + offset)->piece->color != GetSquare(move.src)->piece->color)
				{
					wasEnemySpotted = true;
				}
			}

		}
		// No detection
		else
		{
			return false;
		}
		currPos += offset;
	}

	return false;
}

std::vector<Move> Chessboard::FindMechanicalMoves(Color c)
{
	std::cout << "Looking for mechancial moves! \n";
	std::vector<Move> buff{};
	if (turn == Color::WHITE)
	{
		for (auto p : white)
		{
			std::cout << "Looking for moves for: " << p->type << std::endl;

			auto b = p->GetPieceMoves();

			for (auto m : b)
			{
				if (!IsCollision(m))
				{
					buff.push_back(m);
				}
			}
		}
	}
	else
	{
		for (auto p : black)
		{
			auto b = p->GetPieceMoves();

			for (auto m : b)
			{
				if (!IsCollision(m))
				{
					buff.push_back(m);
				}
			}
		}
	}

	// Debugging
	for (auto m : buff)
	{
		std::cout << "Move: " << m.src << " " << m.dest << std::endl;
	}

	return buff;
}

bool Chessboard::IsMoveValid(Move m)
{
	for (auto& const move : legalMoves)
	{
		if (move == m)
		{
			return true;
		}
	}

	return false;
}

void Chessboard::FindLegalMoves(std::vector<Move> v)
{
	for (auto p : v)
	{
		if (!IsCheck(p))
		{
			legalMoves.push_back(p);
		}
	}
}


