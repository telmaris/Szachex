#include "AI.h"

using PossiblePositions = std::vector<std::unique_ptr<Chessboard>>;

const int wKingTable[64] =
{
	-3, -4, -4, -5 , -5, -4, -4, -3,
	-3, -4, -4, -5 , -5, -4, -4, -3,
	-3, -4, -4, -5 , -5, -4, -4, -3,
	-3, -4, -4, -5 , -5, -4, -4, -3,
	-2, -3, -3, -4, -4, -3, -3, -2,
	-1, -2, -2, -2, -2, -2, -2, -1,
	2, 2, 0, 0, 0, 0, 2, 2,
	2, 3, 1, 0, 0, 1, 3, 2
};

const int bKingTable[64] =
{
	-2, -3, -1, 0 , 0, -1, -3, -2,
	-2, -2, 0, 0, 0, 0, -2, -2,
	1, 2, 2, 2, 2, 2, 2, 1,
	1, 2, 2, 2, 2, 2, 2, 1,
	3, 4, 4, 5 , 5, 4, 4, 3,
	3, 4, 4, 5 , 5, 4, 4, 3,
	3, 4, 4, 5 , 5, 4, 4, 3,
	3, 4, 4, 5 , 5, 4, 4, 3
};

const int wQueenTable[64] =
{
	-2, -1, -1, 0, 0, -1, -1, -2,
	-1, 0, 0, 0, 0, 0, 0, -1,
	-1, 0, 0, 1, 1, 0, 0, -1,
	0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 1, 2, 2, 1, 0, 0,
	-1, -1, 0, 1, 1, 0, 0, -1,
	-1, -1, 0, 0, 0, 0, 0, -1,
	-2, -1, -1, 0, 0, -1, -1, -2
};

const int bQueenTable[64] =
{
	2, 1, 1, 0, 0, 1, 1, 2,
	1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, -1, -1, 0, 0, 1,
	0, 0, -1, -2, -2, -1, 0, 0,
	0, 0, -1, -2, -2, -1, 0, 0,
	1, 0, 0, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	2, 1, 1, 0, 0, 1, 1, 2
};

const int wRookTable[64] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 2, 2, 2, 2, 1, 1,
	-1, 0, 0, 0, 0, 0, 0, -1,
	-1, 0, 0, 0, 0, 0, 0, -1,
	-1, 0, 0, 0, 0, 0, 0, -1,
	-1, 0, 0, 0, 0, 0, 0, -1,
	-1, 0, 0, 0, 0, 0, 0, -1,
	0, 0, 0, 1, 1, 0, 0, 0
};

const int bRookTable[64] =
{
	0, 0, 0, -1, -1, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	-1, -1, -2, -2, -2, -2, -1, -1,
	0, 0, 0, -1, -1, 0, 0, 0
};

const int wBishopTable[64] =
{
	-2, -1, -1, 0, 0, -1, -1, -2,
	-1, 0, 0, 0, 0, 0, 0, -1,
	-1, 0, 0, 1, 1, 0, 0, -1,
	0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 1, 2, 2, 1, 0, 0,
	-1, 1, 0, 1, 1, 0, 1, -1,
	-1, 1, 0, 0, 0, 0, 1, -1,
	-2, -1, -1, 0, 0, -1, -1, -2
};

const int bBishopTable[64] =
{
	2, 1, 1, 0, 0, 1, 1, 2,
	1, -1, 0, 0, 0, 0, -1, 1,
	1, -1, 0, -1, -1, 0, -1, 1,
	0, 0, -1, -2, -2, -1, 0, 0,
	0, 0, -1, -2,-2, -1, 0, 0,
	1, -1, 0, 1, 1, 0, -1, 1,
	1, -1, 0, 0, 0, 0, -1, 1,
	2, 1, 1, 0, 0, 1, 1, 2
};

const int wKnightTable[64] =
{
	-2, -1, -1, 0, 0, -1, -1, -2,
	-1, 0, 0, 0, 0, 0, 0, -1,
	-1, 0, 0, 1, 1, 0, 0, -1,
	0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 1, 2, 2, 1, 0, 0,
	-1, -1, 0, 1, 1, 0, -1, -1,
	-1, -1, 0, 1, 1, 0, -1, -1,
	-2, -1, -1, 0, 0, -1, -1, -2
};

const int bKnightTable[64] =
{
	2, 1, 1, 0, 0, 1, 1, 2,
	1, 0, 0, -1, -1, 0, 0, 1,
	1, 0, 0, -1, -1, 0, 0, 1,
	0, 0, -1, -2, -2, -1, 0, 0,
	0, 0, -1, -2, -2, -1, 0, 0,
	1, -1, 0, -1, -1, 0, -1, 1,
	1, -1, 0, -1, -1, 0, -1, 1,
	2, 1, 1, 0, 0, 1, 1, 2
};

const int wPawnTable[64] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 5, 5, 5, 5, 5, 5,
	1, 2, 3, 4, 4, 3, 2, 1,
	0, 1, 2, 3, 3, 2, 1, 0,
	0, 0, 0, 2, 2, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, -1, -1, 2, 2, 1,
	0, 0, 0, 0, 0, 0, 0, 0
};

const int bPawnTable[64] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	-1, -1, 0, 1, 1, 0, -1, -1,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, -2, -2, 0, 0, 0,
	0, -1, -2, -3, -3, -2, -1, 0,
	-1, -2, -3, -4, -4, -3, -2, -1,
	-5, -5, -5, -5, -5, -5, -5, -5,
	0, 0, 0, -1, -1, 0, 0, 0
};

AI::AI(int d)
{
	depth = d;
}

AI::~AI()
{

}

Move AI::RandomMove(std::vector<Move> v)
{
	int r = v.size();

	return v[Random(0, v.size() - 1)];
}

Move AI::ChooseMove(std::vector<Move> v, Chessboard* c)
{
	auto copy = c->Copy();

	int defaultPos = Evaluate(copy.get());

	auto moves = v;

	std::vector<std::vector<int>> results{};
	std::vector<PossiblePositions> boards{};

	boards.push_back(PossiblePositions{});
	results.push_back(std::vector<int>{});

	Move currentBestMove{};
	int bestMin = 0;
	int bestMax = 0;

	for (int i = 0; i < 1; i++)
	{
		results.clear();

		auto board = boards[i].begin();
		auto result = results[i].begin();

		for (auto& const m : moves)
		{
			boards[i].push_back(copy->Copy());
			

			(*board)->MakeMove(m);

			auto score = Evaluate((*board).get());

			results[i].push_back(score);

			if (score <= bestMin)
			{
				currentBestMove = m;
			}
		}

	}

	return currentBestMove;
}

int AI::Evaluate(Chessboard* c)
{
	int sum = 0;

	for (int i = 0; i < 64; i++)
	{
		if (c->GetSquare(i)->piece != nullptr)
		{
			if (c->GetSquare(i)->piece->color == Color::WHITE)
			{
				switch (c->GetSquare(i)->piece->type)
				{
				case PAWN:
					sum += (10 + wPawnTable[i]);
					break;
				case KNIGHT:
					sum += (30 + wKnightTable[i]);
					break;
				case BISHOP:
					sum += (30 + wBishopTable[i]);
					break;
				case ROOK:
					sum += (50 + wRookTable[i]);
					break;
				case QUEEN:
					sum += (90 + wQueenTable[i]);
					break;
				case KING:
					sum += (900 + wKingTable[i]);
					break;
				default:
					break;
				}
			}
			else
			{
				switch (c->GetSquare(i)->piece->type)
				{
				case PAWN:
					sum -= (10 - bPawnTable[i]);
					break;
				case KNIGHT:
					sum -= (30 - bKnightTable[i]);
					break;
				case BISHOP:
					sum -= (30 - bBishopTable[i]);
					break;
				case ROOK:
					sum -= (50 - bRookTable[i]);
					break;
				case QUEEN:
					sum -= (90 - bQueenTable[i]);
					break;
				case KING:
					sum -= (900 - bKingTable[i]);
					break;
				default:
					break;
				}
			}
		}
	}

	std::cout << "Position: " << sum << "\n";

	return sum;
}