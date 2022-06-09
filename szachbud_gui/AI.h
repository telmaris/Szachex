#pragma once

#include "Config.h"
#include "Chess.h"

class AI
{
public:

	AI(int d);
	~AI();

	Move ChooseMove(std::vector<Move> v, Chessboard* c);
	Move RandomMove(std::vector<Move> v);

private:
	int Evaluate(Chessboard* c);

	int depth;
};