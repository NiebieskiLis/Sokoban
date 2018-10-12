#include "stdafx.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "NAGLOWEK.h"

extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv)
{
	int t1, t2, quit, frames, rc, level = 1, quit1 = 0, ilosc_leveli = 0;
	char tablica_sterujaca[9][9];
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *eti, *ludek = NULL, *pole, *skrzynia1, *skrzynia2, *sciana, *dostarcz;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	char tab[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			tab[i][j] = SCIANA;

		}
	}
	// okno konsoli nie jest widoczne, je¿eli chcemy zobaczyæ
	// komunikaty wypisywane printf-em trzeba w opcjach:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmieniæ na "Console"
	// console window is not visible, to see the printf output
	// the option:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// must be changed to "Console"
	printf("wyjscie printfa trafia do tego okienka\n");
	printf("printf output goes here\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe³noekranowy / fullscreen mode
	//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0)
	{
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Sokoban Aleksandra Rezetka s 171889");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	//SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazkow
	charset = SDL_LoadBMP("./cs8x8.bmp");
	sciana = SDL_LoadBMP("./sciana.bmp");
	ludek = SDL_LoadBMP("./ludek.bmp");
	//ustawienie ludzika
	int x, y;
	skrzynia1 = SDL_LoadBMP("./skrzynia.bmp");
	skrzynia2 = SDL_LoadBMP("./skrzynia2.bmp");
	dostarcz = SDL_LoadBMP("./dostarcz.bmp");
	pole = SDL_LoadBMP("./pole.bmp");
	if (charset == NULL)
	{
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	if (ludek == NULL)
	{
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);
	eti = SDL_LoadBMP("./eti.bmp");

	if (eti == NULL)
	{

		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int flaga = 1;

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	etiSpeed = 1;

	inicjalizacja(tablica_sterujaca, screen, charset, level);
	x = POCZATEK_PLANSZY_X + (znajdz_ludzika(tablica_sterujaca, false, GRACZ) + 1)*(ludek->h);
	y = POCZATEK_PLANSZY_Y + (znajdz_ludzika(tablica_sterujaca, true, GRACZ) + 1)*(ludek->w);
	while (quit != 1)
	{

		t2 = SDL_GetTicks();

		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna³ od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		// here t2-t1 is the time in milliseconds since
		// the last screen was drawn
		// delta is the same time in seconds
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;
		if (flaga == 0)
		{
			worldTime = 0;
			flaga = 1;
		}
		//distance += etiSpeed * delta;

		SDL_FillRect(screen, NULL, czarny);

		// tekst informacyjny / info text
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 56, czerwony, niebieski);
		//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
		sprintf(text, "poziom  %d czas trwania  %.1lf s", level, worldTime);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		//	      "Esc - exit, \030 - faster, \031 - slower"
		sprintf(text, " n - nowa gra , Esc - wyjscie, \030 - gora, \031 - dol , \032 - lewo , \033 - prawo");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
		sprintf(text, " r-rysuj plansze , w - wyswietl dostepne");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);

		DrawGame(screen, pole, ludek, skrzynia1, dostarcz, sciana, skrzynia2, tablica_sterujaca);

		DrawSurface(screen, ludek, x, y);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		//int co_robic;
		// obs³uga zdarzeñ (o ile jakieœ zasz³y) / handling of events (if there were any)
		if (sprawdz(tablica_sterujaca) == 0)
		{
			
			while (quit1 != 1)
			{
				// tekst informacyjny / info text
				DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 400, czerwony, niebieski);
				//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
				sprintf(text, "KONIEC GRY!", level, worldTime);
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 50, text, charset);
				sprintf(text, " n - graj dalej , Esc - wyjscie , w- wyswietl plansze");
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 66, text, charset);
				SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
				//		SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, scrtex, NULL, NULL);
				SDL_RenderPresent(renderer);
				while (SDL_PollEvent(&event))
				{
					switch (event.type)
					{
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE)
						{
							quit = 1;
							quit1 = 1;
						}
						else if (event.key.keysym.sym == SDLK_n)
						{
							level++;
							quit1 = 1;
							if (inicjalizacja(tablica_sterujaca, screen, charset, level) != 1)
							{
								inicjalizacja(tablica_sterujaca, screen, charset, level);
								x = POCZATEK_PLANSZY_X + (znajdz_ludzika(tablica_sterujaca, false, GRACZ) + 1)*(ludek->h);
								y = POCZATEK_PLANSZY_Y + (znajdz_ludzika(tablica_sterujaca, true, GRACZ) + 1)*(ludek->w);
								worldTime = 0;
							}
							else quit = 1;
						}
						else if (event.key.keysym.sym == SDLK_w)
						{
							level++;
							quit1 = 1;
							flaga = 0;
							WYSWIETL_OPCJE(renderer, scrtex, charset, screen, &level);
							inicjalizacja(tablica_sterujaca, screen, charset, level);
							x = POCZATEK_PLANSZY_X + (znajdz_ludzika(tablica_sterujaca, false, GRACZ) + 1)*(ludek->h);
							y = POCZATEK_PLANSZY_Y + (znajdz_ludzika(tablica_sterujaca, true, GRACZ) + 1)*(ludek->w);
						};
						break;
					};
				};
			}; quit1 = 0;
		};
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (warunki(tablica_sterujaca, DOL, 0) == true)y += ROZMIAR_SRITE;
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					if (warunki(tablica_sterujaca, GORA, 0) == true)y -= ROZMIAR_SRITE;
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					if (warunki(tablica_sterujaca, LEWO, 0) == true)x -= ROZMIAR_SRITE;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					if (warunki(tablica_sterujaca, PRAWO, 0) == true)x += ROZMIAR_SRITE;
				}
				else if (event.key.keysym.sym == SDLK_n)
				{
					if (inicjalizacja(tablica_sterujaca, screen, charset, level) != 1)
					{
						inicjalizacja(tablica_sterujaca, screen, charset, level);
						x = POCZATEK_PLANSZY_X + (znajdz_ludzika(tablica_sterujaca, false, GRACZ) + 1)*(ludek->h);
						y = POCZATEK_PLANSZY_Y + (znajdz_ludzika(tablica_sterujaca, true, GRACZ) + 1)*(ludek->w);
						worldTime = 0;
					}
					else quit = 1;
				}
				else if (event.key.keysym.sym == SDLK_r)
				{
					WLASNA_PLANSZA(renderer, scrtex, charset, screen, pole, ludek, skrzynia1, dostarcz, sciana, skrzynia2, tablica_sterujaca);
					x = POCZATEK_PLANSZY_X + (znajdz_ludzika(tablica_sterujaca, false, GRACZ) + 1)*(ludek->h);
					y = POCZATEK_PLANSZY_Y + (znajdz_ludzika(tablica_sterujaca, true, GRACZ) + 1)*(ludek->w);
					flaga = 0;
					inicjalizacja(tablica_sterujaca, screen, charset, level);
				}
				else if (event.key.keysym.sym == SDLK_w)
				{
					flaga = 0;
					WYSWIETL_OPCJE(renderer, scrtex, charset, screen, &level);
					inicjalizacja(tablica_sterujaca, screen, charset, level);
					x = POCZATEK_PLANSZY_X + (znajdz_ludzika(tablica_sterujaca, false, GRACZ) + 1)*(ludek->h);
					y = POCZATEK_PLANSZY_Y + (znajdz_ludzika(tablica_sterujaca, true, GRACZ) + 1)*(ludek->w);
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};

	};

	// zwolnienie powierzchni / freeing all surfaces
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(ludek);
	SDL_FreeSurface(skrzynia1);
	SDL_FreeSurface(skrzynia2);
	SDL_FreeSurface(sciana);
	SDL_FreeSurface(dostarcz);
	SDL_FreeSurface(pole);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	system("pause");
	return 0;
};
