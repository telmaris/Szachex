#pragma once

#include "Config.h"
#include "Chess.h"
#include "AI.h"

struct GFX
{
	GFX() {}

	GFX(SDL_Renderer* r)
	{
		wp = LoadTexture(r, "pieces/pw.png");
		wn = LoadTexture(r, "pieces/nw.png");
		wb = LoadTexture(r, "pieces/bw.png");
		wr = LoadTexture(r, "pieces/rw.png");
		wq = LoadTexture(r, "pieces/qw.png");
		wk = LoadTexture(r, "pieces/kw.png");

		bp = LoadTexture(r, "pieces/pb.png");
		bn = LoadTexture(r, "pieces/nb.png");
		bb = LoadTexture(r, "pieces/bb.png");
		br = LoadTexture(r, "pieces/rb.png");
		bq = LoadTexture(r, "pieces/qb.png");
		bk = LoadTexture(r, "pieces/kb.png");
	}

	~GFX()
	{
		SDL_DestroyTexture(wp);
		SDL_DestroyTexture(wn);
		SDL_DestroyTexture(wb);
		SDL_DestroyTexture(wr);
		SDL_DestroyTexture(wq);
		SDL_DestroyTexture(wk);

		SDL_DestroyTexture(bp);
		SDL_DestroyTexture(bn);
		SDL_DestroyTexture(bb);
		SDL_DestroyTexture(br);
		SDL_DestroyTexture(bq);
		SDL_DestroyTexture(bk);
	}

	SDL_Texture* wp;
	SDL_Texture* wn;
	SDL_Texture* wb;
	SDL_Texture* wr;
	SDL_Texture* wq;
	SDL_Texture* wk;

	SDL_Texture* bp;
	SDL_Texture* bn;
	SDL_Texture* bb;
	SDL_Texture* br;
	SDL_Texture* bq;
	SDL_Texture* bk;
};

class Engine
{
public:
	Engine(const char* gfx);

	~Engine();

	void MainLoop();

private:

	void Draw();

	template <typename T> T* CreatePiece(Color c, ChessPosition pos, SDL_Texture* t)
	{
		return cb->CreatePiece<T>(c, pos, t);
	}

	Square* PixelsToSquare(int x, int y);
	SDL_Rect SquareToPixels(Square* s);

	std::vector<Move> FindMechanicalMoves(Chessboard* board, Color c);

	std::unique_ptr<Chessboard> cb;
	std::unique_ptr<AI> ai;

	bool running;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* mainTexture;
	SDL_Event event;
	SDL_Point clickOffset;

	std::unique_ptr<GFX> graphics;
};



