#include "SDL2/SDL.h"
#include <complex>
#include <iostream>

/*
	AUTHOR: EvilKarut, https://github.com/EvilKarut

	CONTROLS:
	+/-			to Zoom in/out.
	Arrow Keys	to move the viewport.
	M			to permanently execute 10 more iterations.
	N			to permanently execute 10 less iterations.
	F			to duplicate the iterations on the next frame (foto mode).
	Quit or Q	to Exit.
*/

struct Color {
	int R = 0;
	int G = 0;
	int B = 0;
	
	Color(int Step) {
		Step = Step % 4;
		switch(Step) {
			case 0 : break;
			case 1 : R = 255;
					 G = 0;
					 B = 0;
					 break;
			case 2 : R = 0;
					 G = 255;
					 B = 0;
					 break;
			case 3 : R = 0;
					 G = 0;
					 B = 255;
					 break;
		}
	}
};

int MandelBrotIterations(const std::complex<double>& Point, const int MaxIterations) {
	int Iteration = 0;
	std::complex<double> LastIt = Point;
	while (Iteration < MaxIterations) {
		LastIt = (LastIt * LastIt) + Point;
		Iteration += 1;
		if (abs(LastIt.real() - Point.real()) < 0.0005 and abs(LastIt.imag() - Point.imag()) < 0.0005) {
			return Iteration;
		}
	}
	return 0;
}

int main() {
	int SizeX = 2048;
	int SizeY = 1024;
	double Definition = 0.0022;
	int Iterations = 25;
	std::complex<double> PivotPoint(-2.8, -1.2);

	bool Draw = true;
	bool FotoMode = false;
	bool quit = false;
	SDL_Event event;
	SDL_Window* Window;
	SDL_Renderer* Renderer;

	SDL_CreateWindowAndRenderer(SizeX, SizeY, 0, &Window, &Renderer);
	
	while (!quit) {
		if (Draw) {
			// Check all pixels
			std::complex<double> Point = PivotPoint;
			for (int y = 0; y < SizeY; ++y) { 
				for (int x = 0; x < SizeX; ++x) {
					int Iterated = MandelBrotIterations(Point, Iterations);
					Color C(Iterated);	
					SDL_SetRenderDrawColor(Renderer, C.R, C.G, C.B, Uint8(256));

  					SDL_RenderDrawPoint(Renderer, x, y);
				
					Point.real(Point.real() + Definition);
				}
				Point.real(PivotPoint.real());
				Point.imag(Point.imag() + Definition);
			}
			
			// Render
			SDL_RenderPresent(Renderer);

			Draw = false;
			if (FotoMode) {
				Iterations = Iterations / 2;
				FotoMode = false;
			}
		}

		// Handle Events
		if (SDL_PollEvent(&event) == 1) {
			if (event.type == SDL_QUIT)
				quit = true;
			if (event.type == SDL_KEYDOWN) {
				double Displacement = (SizeY * Definition) / 4;
				switch (((event.key).keysym).sym) {
					case SDLK_q:
						quit = true;
						break;
					case SDLK_PLUS:
						Draw = true;
						Definition = Definition * 0.80;;
						break;
					case SDLK_MINUS:
						Draw = true;
						Definition = Definition * 1.20;
						break;	
					case SDLK_UP:
						Draw = true;
						PivotPoint.imag(PivotPoint.imag() - Displacement);
						break;
					case SDLK_DOWN:
						Draw = true;
						PivotPoint.imag(PivotPoint.imag() + Displacement);
						break;
					case SDLK_LEFT:
						Draw = true;
						PivotPoint.real(PivotPoint.real() - Displacement);
						break;
					case SDLK_RIGHT:
						Draw = true;
						PivotPoint.real(PivotPoint.real() + Displacement);
						break;
					case SDLK_m:
						Draw = true;
						Iterations += 10;
						break;
					case SDLK_n:
						Draw = true;
						Iterations -= 10;
					case SDLK_f:
						Draw = true;
						Iterations = Iterations * 2;
						FotoMode = true;
				}
			}
		}
	}
}
