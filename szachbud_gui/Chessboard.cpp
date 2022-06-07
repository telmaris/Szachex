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

	GetSquare(m.src)->piece->pos = m.dest;
	GetSquare(m.src)->piece->moveCounter++;
	GetSquare(m.dest)->piece = GetSquare(m.src)->piece;
	GetSquare(m.src)->piece = nullptr;

	deleteIfEnPassant(m);

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

bool Chessboard::IsCheck(Move move)
{
	int kingPosition = 0;
	std::vector<Move> oponentsMove{};

	auto copy = Copy();

	copy->MakeMove(move);

	if (turn == Color::WHITE)
	{
		oponentsMove = copy->FindMechanicalMoves(Color::BLACK);

		for (auto p : copy->white)
		{
			if (p->type == KING) kingPosition = p->pos;
		}
	}
	else
	{
		oponentsMove = copy->FindMechanicalMoves(Color::WHITE);

		for (auto p : copy->black)
		{
			if (p->type == KING) kingPosition = p->pos;
		}
	}

	for (auto m : oponentsMove)
	{
		if (m.dest == kingPosition)
		{
			std::cout << "King is attacked!!\n";
			return true;
		}
	}

	return false;
}

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

			if (GetSquare(move.src)->piece->type == PAWN && abs(offset) == 8)
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
			//return false;
		}
		currPos += offset;
	}

	return false;
}

std::vector<Move> Chessboard::FindMechanicalMoves(Color c)
{
	std::cout << "Looking for mechancial moves! \n";
	std::vector<Move> buff{};
	if (c == Color::WHITE)
	{
		for (auto p : white)
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

	PawnEvents(c, buff);

	return buff;
}

bool Chessboard::IsMoveValid(const Move& m)
{
	for (auto& const move : legalMoves)
	{
		if (move.src == m.src && move.dest == m.dest)
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

std::unique_ptr<Chessboard> Chessboard::Copy()
{
	auto cpy = std::make_unique<Chessboard>(*this);

	cpy->white.clear();
	cpy->black.clear();

	for (auto& const p : white)
	{
		auto piece = p->CopyPiece();
		cpy->arr[p->pos].piece = piece;
		cpy->white.push_back(piece);
	}
	for (auto& const p : black)
	{
		auto piece = p->CopyPiece();
		cpy->arr[p->pos].piece = piece;
		cpy->black.push_back(piece);
	}

	return cpy;
}

std::vector<Move> Chessboard::PawnEvents(Color c, std::vector<Move>& moves)
{	// usuwa nadmierne bicie, dodaje bicie w przelocie. kolor bialy oznacza sprawdzenei czy biale moga bic
	
	std::vector<int> idxToErase{};

	int iter = 0;

	for (auto move : moves)
	{// dodawanie pozycji bicia w wektorze ruchow
		if (GetSquare(move.src)->piece->type == PAWN)
		{
			if (GetSquare(move.dest)->piece == nullptr && (abs(move.src - move.dest) == 7 || abs(move.src - move.dest) == 9))
			{
				idxToErase.push_back(iter);
			}
		}
		iter++;
	}
	// usuwanie bicia
	int idx = 0;
	int delIdx;
	if (!idxToErase.empty())
	{
		for (int i = 0; i < idxToErase.size(); i++)
		{
			delIdx = idxToErase.at(i) + idx;
			if (delIdx == moves.size()) { moves.pop_back(); }
			else { moves.erase(moves.begin() + delIdx); }
			idx -= 1;
		}
	}

	// en passant
	if (history.size() > 3)
	{
		Move m{ history.back() };
		// sprawdzanie czy ostatni ruch to skok o dwa pola pionem
		if (GetSquare(m.dest)->piece->type == PAWN && (abs(m.dest - m.src) == 16))
		{
			// sprawdzanie czy na lewo nie ma juz granicy planszy
			if (m.src % 8 == 0)
			{
				if (c == Color::BLACK)
				{
					// sprawdzanie czy na pozycji bojowej jest nasz pion 
					if (GetSquare(m.src + 17)->piece != nullptr) 
					{
						if (GetSquare(m.src + 17)->piece->type == PAWN && GetSquare(m.src + 17)->piece->color == c)
						{
							Move move{ m.src + 17, m.src + 8 };
							moves.push_back(move);
						}
					}
				}
				else
				{
					if (GetSquare(m.src - 15)->piece != nullptr)
					{
						if (GetSquare(m.src - 15)->piece->type == PAWN && GetSquare(m.src - 15)->piece->color == c)
						{
							Move move{ m.src - 15, m.src - 8 };
							moves.push_back(move);
						}
					}
				}
			}
			// sprawdzanie czy na lewo nie ma juz granicy planszy
			else if (m.src % 8 == 7)
			{
				if (c == Color::BLACK)
				{
					// sprawdzanie czy na pozycji bojowej jest nasz pion 
					if (GetSquare(m.src + 15)->piece != nullptr)
					{
						if (GetSquare(m.src + 15)->piece->type == PAWN && GetSquare(m.src + 15)->piece->color == c)
						{
							Move move{ m.src + 15, m.src + 8 };
							moves.push_back(move);
						}
					}
				}
				else
				{
					if (GetSquare(m.src - 17)->piece != nullptr)
					{
						if (GetSquare(m.src - 17)->piece->type == PAWN && GetSquare(m.src - 17)->piece->color == c)
						{
							Move move{ m.src - 17, m.src - 8 };
							moves.push_back(move);
						}
					}
				}
			}
			// srodkowe piony
			else
			{
				if (c == Color::BLACK)
				{
					// sprawdzanie czy na pozycji bojowej jest nasz pion 
					if (GetSquare(m.src + 17)->piece != nullptr)
					{
						if (GetSquare(m.src + 17)->piece->type == PAWN && GetSquare(m.src + 17)->piece->color == c)
						{
							Move move{ m.src + 17, m.src + 8 };
							moves.push_back(move);
						}
					}
					if (GetSquare(m.src + 15)->piece != nullptr)
					{
						if (GetSquare(m.src + 15)->piece->type == PAWN && GetSquare(m.src + 15)->piece->color == c)
						{
							Move move{ m.src + 15, m.src + 8 };
							moves.push_back(move);
						}
					}
				}
				else
				{
					if (GetSquare(m.src - 17)->piece != nullptr)
					{
						if (GetSquare(m.src - 17)->piece->type == PAWN && GetSquare(m.src - 17)->piece->color == c)
						{
							Move move{ m.src - 17, m.src - 8 };
							moves.push_back(move);
						}
					}
					if (GetSquare(m.src - 15)->piece != nullptr)
					{
						if (GetSquare(m.src - 15)->piece->type == PAWN && GetSquare(m.src - 15)->piece->color == c)
						{
							Move move{ m.src - 15, m.src - 8 };
							moves.push_back(move);
						}
					}
				}
			}
		}
	}
	return moves;
}

void Chessboard::deleteIfEnPassant(Move move)
{
	if (history.size() > 3)
	{
		Move m{ history.back() };
		if (GetSquare(m.dest)->piece->type == PAWN && (abs(m.dest - m.src) == 16))
		{
			if (abs(move.dest - m.dest) == 8 && GetSquare(move.dest)->piece->type == PAWN)
			{
				DestroyPiece(GetSquare(m.dest)->piece);
				GetSquare(m.dest)->piece = nullptr;
			}
		}
	}
}