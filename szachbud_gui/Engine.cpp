#include "Engine.h"


void Engine::MainLoop()
{
#ifdef MAX_FPS
	double delay = (1000.0 / MAX_FPS);
#endif

	while (running)
	{
		auto timeStart = SDL_GetPerformanceCounter();
		double elapsed = 0.0;

		static int mouseX = 0;
		static int mouseY = 0;

		SDL_PumpEvents();
		SDL_GetMouseState(&mouseX, &mouseY);

		static Move currentMove{};
		static Color turn = Color::WHITE;
		static bool movesEvaluated = false;

		if (!movesEvaluated)
		{

			std::vector<Move> mechMoves{};

			if (cb->GetTurn() == Color::WHITE)
			{
				mechMoves = FindMechanicalMoves(cb.get(), Color::WHITE);

			}
			else
			{
				mechMoves = FindMechanicalMoves(cb.get(), Color::BLACK);
			}

			cb->FindLegalMoves(mechMoves);

			movesEvaluated = true;

			if (cb->legalMoves.empty())
			{
				continue;
			}

			if (turn == Color::BLACK)
			{
				Move aiMove = ai->RandomMove(cb->legalMoves);
				//Move aiMove = ai->ChooseMove(cb->legalMoves, cb.get());

				cb->MakeMove(aiMove);

				turn = cb->GetTurn();
				currentMove = Move{};
				movesEvaluated = false;
				continue;
			}
		}

		if (SDL_PollEvent(&event) > 0)
		{
			static Square* selectedSquare = nullptr;
			static bool mouseButtonDown = false;

			static SDL_Rect clickOffset{};

			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				return;

			case SDL_MOUSEBUTTONDOWN:
				if (!mouseButtonDown)
				{
					mouseButtonDown = true;
					selectedSquare = PixelsToSquare(mouseX, mouseY);

					clickOffset.x = mouseX - SquareToPixels(selectedSquare).x;
					clickOffset.y = mouseY - SquareToPixels(selectedSquare).y;

					if ((selectedSquare != nullptr) && (selectedSquare->piece != nullptr) && (selectedSquare->piece->color == turn))
					{
						*(selectedSquare->piece->GetPosition()) = SDL_Rect{ mouseX - clickOffset.x, mouseY - clickOffset.y , 80, 80 };
					}
					else
					{
						selectedSquare = nullptr;
						break;
					}

					currentMove.src = selectedSquare->position;
				}
				break;

			case SDL_MOUSEMOTION:
				if (mouseButtonDown)
				{
					if ((selectedSquare != nullptr) && (selectedSquare->piece != nullptr))
					{
						*(selectedSquare->piece->GetPosition()) = SDL_Rect{ mouseX - clickOffset.x, mouseY - clickOffset.y , 80, 80 };
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (mouseButtonDown)
				{
					mouseButtonDown = false;

					if (selectedSquare != nullptr)
					{
						currentMove.dest = PixelsToSquare(mouseX, mouseY)->position;



						if (cb->IsMoveValid(currentMove))	//warunek 
						{
							if (PixelsToSquare(mouseX, mouseY) == selectedSquare)
							{
								*(selectedSquare->piece->GetPosition()) = SquareToPixels(selectedSquare);
								selectedSquare = nullptr;
								break;
							}

							cb->MakeMove(currentMove);

							turn = cb->GetTurn();
							currentMove = Move{};
							movesEvaluated = false;
						}
						else
						{
							*(selectedSquare->piece->GetPosition()) = SquareToPixels(selectedSquare);
						}

						selectedSquare = nullptr;
					}
				}
				break;
			default:
				break;
			}

		}

		Draw();


#ifdef MAX_FPS

		auto timeStop = SDL_GetPerformanceCounter();

		elapsed = ((timeStop - timeStart) / SDL_GetPerformanceFrequency()) * 1000.0;

		if (elapsed < delay)
		{
			SDL_Delay(floor(delay - elapsed));
	}
#endif
}
}

Engine::Engine(const char* gfx)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 800, 0);

	if (!window)
	{
		std::cout << SDL_GetError() << std::endl;
		return;


	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRelativeMouseMode(SDL_FALSE);

	if (!renderer)
	{
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	mainTexture = LoadTexture(renderer, gfx);

	running = true;

	graphics = std::make_unique<GFX>(renderer);

	cb = std::make_unique<Chessboard>();

	auto pw1 = CreatePiece<Pawn>(Color::WHITE, { 'a', 2 }, graphics->wp);
	auto pw2 = CreatePiece<Pawn>(Color::WHITE, { 'b', 2 }, graphics->wp);
	auto pw3 = CreatePiece<Pawn>(Color::WHITE, { 'c', 2 }, graphics->wp);
	auto pw4 = CreatePiece<Pawn>(Color::WHITE, { 'd', 2 }, graphics->wp);
	auto pw5 = CreatePiece<Pawn>(Color::WHITE, { 'e', 2 }, graphics->wp);
	auto pw6 = CreatePiece<Pawn>(Color::WHITE, { 'f', 2 }, graphics->wp);
	auto pw7 = CreatePiece<Pawn>(Color::WHITE, { 'g', 2 }, graphics->wp);
	auto pw8 = CreatePiece<Pawn>(Color::WHITE, { 'h', 2 }, graphics->wp);

	auto wr1 = CreatePiece<Rook>(Color::WHITE, { 'a', 1 }, graphics->wr);
	auto wr2 = CreatePiece<Rook>(Color::WHITE, { 'h', 1 }, graphics->wr);
	auto wk1 = CreatePiece<Knight>(Color::WHITE, { 'b', 1 }, graphics->wn);
	auto wk2 = CreatePiece<Knight>(Color::WHITE, { 'g', 1 }, graphics->wn);
	auto wb1 = CreatePiece<Bishop>(Color::WHITE, { 'c', 1 }, graphics->wb);
	auto wb2 = CreatePiece<Bishop>(Color::WHITE, { 'f', 1 }, graphics->wb);
	auto wq = CreatePiece<Queen>(Color::WHITE, { 'd', 1 }, graphics->wq);
	auto wk = CreatePiece<King>(Color::WHITE, { 'e', 1 }, graphics->wk);

	auto pb1 = CreatePiece<Pawn>(Color::BLACK, { 'a', 7 }, graphics->bp);
	auto pb2 = CreatePiece<Pawn>(Color::BLACK, { 'b', 7 }, graphics->bp);
	auto pb3 = CreatePiece<Pawn>(Color::BLACK, { 'c', 7 }, graphics->bp);
	auto pb4 = CreatePiece<Pawn>(Color::BLACK, { 'd', 7 }, graphics->bp);
	auto pb5 = CreatePiece<Pawn>(Color::BLACK, { 'e', 7 }, graphics->bp);
	auto pb6 = CreatePiece<Pawn>(Color::BLACK, { 'f', 7 }, graphics->bp);
	auto pb7 = CreatePiece<Pawn>(Color::BLACK, { 'g', 7 }, graphics->bp);
	auto pb8 = CreatePiece<Pawn>(Color::BLACK, { 'h', 7 }, graphics->bp);

	auto br1 = CreatePiece<Rook>(Color::BLACK, { 'a', 8 }, graphics->br);
	auto br2 = CreatePiece<Rook>(Color::BLACK, { 'h', 8 }, graphics->br);
	auto bk1 = CreatePiece<Knight>(Color::BLACK, { 'b', 8 }, graphics->bn);
	auto bk2 = CreatePiece<Knight>(Color::BLACK, { 'g', 8 }, graphics->bn);
	auto bb1 = CreatePiece<Bishop>(Color::BLACK, { 'c', 8 }, graphics->bb);
	auto bb2 = CreatePiece<Bishop>(Color::BLACK, { 'f', 8 }, graphics->bb);
	auto bq = CreatePiece<Queen>(Color::BLACK, { 'd', 8 }, graphics->bq);
	auto bk = CreatePiece<King>(Color::BLACK, { 'e', 8 }, graphics->bk);
}

Engine::~Engine()
{
	SDL_DestroyTexture(mainTexture);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Engine::Draw()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, mainTexture, NULL, NULL);
	cb->DrawPieces(renderer);
	SDL_RenderPresent(renderer);
}

Square* Engine::PixelsToSquare(int x, int y)
{
	return cb->PixelsToSquare(x,y);
}

SDL_Rect Engine::SquareToPixels(Square* s)
{
	return cb->SquareToPixels(s);
}

std::vector<Move> Engine::FindMechanicalMoves(Chessboard* board, Color c)
{
	board->legalMoves.clear();
	return board->FindMechanicalMoves(c);
}

