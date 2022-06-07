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
			if (cb->GetTurn() == Color::WHITE)
			{

			}
			else
			{

			}

			movesEvaluated = true;
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
							//tutaj sprawdzamy poprawnoœæ ruchu, jesli jest OK to przenosimy figurke
							//na nowa pozycje. updatujemy jej pozycje na planszy

							// The piece has been returned to its default square

							if (PixelsToSquare(mouseX, mouseY) == selectedSquare)
							{
								*(selectedSquare->piece->GetPosition()) = SquareToPixels(selectedSquare);
								selectedSquare = nullptr;
								break;
							}
							// komment Arek


							// comment Marcin

							// Delete the oponent's piece if its on the target square

							if (PixelsToSquare(mouseX, mouseY)->piece)
							{
								cb->DestroyPiece(PixelsToSquare(mouseX, mouseY)->piece);
								PixelsToSquare(mouseX, mouseY)->piece = nullptr;
							}

							// Set the piece's position, assign pointer to the target square

							*(selectedSquare->piece->GetPosition()) = SquareToPixels(PixelsToSquare(mouseX, mouseY));
							PixelsToSquare(mouseX, mouseY)->piece = selectedSquare->piece;
							selectedSquare->piece = nullptr;



							// Add a new move to the move history

							cb->MakeMove(currentMove);

							turn = cb->GetTurn();
							currentMove = Move{};
							movesEvaluated = false;
						}
						else
						{
							// Returning to the default square

							*(selectedSquare->piece->GetPosition()) = SquareToPixels(selectedSquare);
							std::cout << "X: " << mouseX << " Y: " << mouseY << "\n";
							std::cout << "Square: " << PixelsToSquare(mouseX, mouseY)->position << "\n";
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

	cb = std::make_unique<Chessboard>();

	auto test = CreatePiece<Knight>(Color::WHITE, { 'a', 4 }, "pieces/nw.png");
	auto test2 = CreatePiece<Bishop>(Color::WHITE, { 'e', 4 }, "pieces/bw.png");
	auto test3 = CreatePiece<Queen>(Color::BLACK, { 'c', 7 }, "pieces/qb.png");
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
	int index = ((7 - (y / 100)) * 8) + (x / 100);
	return cb->GetSquare(index);
}

SDL_Rect Engine::SquareToPixels(Square* s)
{
	int index = s->position;
	SDL_Rect r{ 100 * (index % 8) + 10, 100 * (7 - (index / 8)) + 10, 80, 80 };

	return r;
}

std::vector<Move> Engine::FindMechanicalMoves(Chessboard* board, Color c)
{
	return board->FindMechanicalMoves(c);
}

