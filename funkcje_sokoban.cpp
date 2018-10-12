#include "stdafx.h"
#include <iostream>
#include<stdio.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <fstream>
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "NAGLOWEK.h"

extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}
using namespace std;
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset)
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text)
	{
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color)
{
	for (int i = 0; i < l; i++)
	{
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor)
{
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

int inicjalizacja(char tablica[9][9], SDL_Surface *screen, SDL_Surface *charset, int level)
{
	char tab[9][9];
	char numer[100];

	char nazwa[80];
	strcpy(nazwa, "plansza");
	_itoa(level, numer, 10);
	strcat(nazwa, numer);
	strcat(nazwa, ".txt");

	FILE *plansze;
	plansze = fopen(nazwa, "r");
	if (plansze == NULL)
	{
		printf("plansza error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		fread(tab, 1, 81, plansze);
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j <9; j++)
			{
				tablica[i][j] = tab[i][j];
			}
		}
		fclose(plansze);
	}
};
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w;
	dest.y = y - sprite->h;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};
void DrawGame(SDL_Surface *screen, SDL_Surface *sprite0, SDL_Surface *sprite1, SDL_Surface *sprite2, SDL_Surface *sprite3, SDL_Surface *sprite4, SDL_Surface *sprite5, char tablica[9][9])
{
	SDL_Rect dest;
	SDL_Surface *sprite;
	for (int a = 0; a < 9; a++)
	{
		for (int b = 0; b< 9; b++)
		{
			if ((tablica[a][b] == '0') || (tablica[a][b] == '1'))
			{
				DrawSurface(screen, sprite0,POCZATEK_PLANSZY_Y + sprite0->w*(b + 1), POCZATEK_PLANSZY_X + sprite0->w*(a + 1));
			}
			else if (tablica[a][b] == '2')
			{
				DrawSurface(screen, sprite2, POCZATEK_PLANSZY_Y + sprite2->w*(b + 1), POCZATEK_PLANSZY_X + sprite2->w*(a + 1));
			}
			else if (tablica[a][b] == '3')
			{
				DrawSurface(screen, sprite3, POCZATEK_PLANSZY_Y + sprite3->w*(b + 1), POCZATEK_PLANSZY_X + sprite3->w*(a + 1));
			}
			else if (tablica[a][b] == '4')
			{
				DrawSurface(screen, sprite4, POCZATEK_PLANSZY_Y + sprite4->w*(b + 1), POCZATEK_PLANSZY_X + sprite4->w*(a + 1));
			}
			else if (tablica[a][b] == '5')
			{
				DrawSurface(screen, sprite5, POCZATEK_PLANSZY_Y + sprite5->w*(b + 1), POCZATEK_PLANSZY_X + sprite5->w*(a + 1));
			};

		}
	}

};
void TWORZ_PLANSZA(char kto, SDL_Surface *KTORY_SPRITE, SDL_Renderer *renderer, SDL_Texture *scrtex, SDL_Surface *charset, SDL_Surface *screen, SDL_Surface *sprite0, SDL_Surface *sprite1, SDL_Surface *sprite2, SDL_Surface *sprite3, SDL_Surface *sprite4, SDL_Surface *sprite5, char tab[9][9], int x, int y)
{
	//tablica tab - nowa plansza do zapisu
	SDL_Event event;
	char text[128];
	int kto1;
	if (kto == SKRZYNIA)  kto1 = 2;
	if (kto == WOLNY)  kto1 = 1;
	if (kto == DOCELOWE)  kto1 = 2;
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int quit = 0, a, b;
		a = 0  ;
		b = 0;

	while ((!quit))
	{
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 56, czerwony,czerwony);
		sprintf(text, "RYSUJ WLASNA PLANSZE");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		sprintf(text, " k - koniec, a - zostaw tu element , kolejnosc POLE , SKRZYNIA , DOCELOWE");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
		sprintf(text, " teraz : %d" ,kto1);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 46, text, charset);
		DrawGame(screen, sprite0, sprite1,sprite2, sprite3, sprite4, sprite5,tab);
		DrawSurface(screen, sprite3, x, y);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		
	
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (b < 8)
					{
						y += ROZMIAR_SRITE;
						b++;
					}
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					if (b > 0)
					{
						y -= ROZMIAR_SRITE;
						b--;
					}
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					if (a > 0)
					{
						x -= ROZMIAR_SRITE;
						a--;
					}
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					
					if (a < 8)
					{
						x += ROZMIAR_SRITE;
						a++;
					}
				}
				else if (event.key.keysym.sym == SDLK_a)
				{
					if ((tab[b][a] == SCIANA)||(tab[b][a] ==WOLNY))	tab[b][a] = kto;
				}
				else if (event.key.keysym.sym == SDLK_k)
				{
					quit = 1;
				}

			};
			break;
		};
	};
};
int planszy_ilosc()
{
	//int flaga = 1;
	char tab[9][9];
	char numer[13];
	//int level = 1;
	char nazwa[10];
	int a = 0;
	FILE *plansze;
	plansze = fopen("ilosc.txt", "r");
	if (plansze == NULL)
	{
		printf("plansza error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		fread(numer, 1, 81, plansze);
		a = atoi(numer);
		//flaga++;


	}
	fclose(plansze);

	return a;

};
void WYSWIETL_OPCJE(SDL_Renderer *renderer, SDL_Texture *scrtex, SDL_Surface *charset, SDL_Surface *screen,int *level)
{
	//tablica tab - nowa plansza do zapisu
	SDL_Event event;
	char text[128];
	int ilosc_leveli = planszy_ilosc();
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int quit = 0;


	while ((!quit))
	{
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 400, czerwony, czerwony);
		//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
		sprintf(text, "DOSTEPNE PLANSZE TO");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		//	      "Esc - exit, \030 - faster, \031 - slower"
		for (int i = 1; i <= ilosc_leveli; i++) {
			sprintf(text, " plansza  %d" , i);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26+ i*16, text, charset);
		}
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);


		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_1)*level = 1;
				else if (event.key.keysym.sym == SDLK_2)*level = 2;
				else if (event.key.keysym.sym == SDLK_3)*level = 3;
				else if (event.key.keysym.sym == SDLK_4)*level = 4;
				else if (event.key.keysym.sym == SDLK_5)*level = 5;
				else if (event.key.keysym.sym == SDLK_6)*level = 6;
				else if (event.key.keysym.sym == SDLK_7)*level = 7;
				else if (event.key.keysym.sym == SDLK_8)*level = 8;
				else if (event.key.keysym.sym == SDLK_9)*level = 9;
				else if (event.key.keysym.sym == SDLK_k)
				{
					quit = 1;
				}
					case SDL_QUIT:
					quit = 1;
					break;
			}
		};
	};
};

int WLASNA_PLANSZA(SDL_Renderer *renderer, SDL_Texture *scrtex, SDL_Surface *charset, SDL_Surface *screen, SDL_Surface *sprite0, SDL_Surface *sprite1, SDL_Surface *sprite2, SDL_Surface *sprite3, SDL_Surface *sprite4, SDL_Surface *sprite5, char tablica[9][9])
{
	int level = planszy_ilosc()+1;
	int x = POCZATEK_PLANSZY_X+ROZMIAR_SRITE ;
	int y = POCZATEK_PLANSZY_Y+ROZMIAR_SRITE;
	char tablica1[9][9];
for (int i = 0; i < 9; i++)
		{
		for (int j = 0; j < 9; j++)
			{
			tablica1[i][j] = SCIANA;
			}
		}
	TWORZ_PLANSZA(WOLNY, sprite0, renderer, scrtex, charset, screen, sprite0, sprite1, sprite2, sprite3, sprite4, sprite5, tablica1, x, y);
	TWORZ_PLANSZA(SKRZYNIA, sprite2, renderer, scrtex, charset, screen, sprite0, sprite1, sprite2, sprite3, sprite4, sprite5, tablica1, x, y);
	TWORZ_PLANSZA(DOCELOWE, sprite3, renderer, scrtex, charset, screen, sprite0, sprite1, sprite2, sprite3, sprite4, sprite5, tablica1, x, y);
	int i = 0;
	int flaga1 = 0;
	while (flaga1!=1){
		for (int j = 1; j < 9; j++)
		{
			if (tablica1[i][j] == WOLNY)
			{
				flaga1 = 1;
				tablica1[i][j] = GRACZ;
				break;
			};
		}
		i++;
	}
	char tab[9][9];
	char numer[100];
	char nazwa[80];
	strcpy(nazwa, "plansza");
	_itoa(level, numer, 10);
	strcat(nazwa, numer);
	strcat(nazwa, ".txt");

	FILE *plansze;
	plansze = fopen(nazwa, "w");
	if (plansze == NULL)
	{
		printf("plansza error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		fprintf(plansze, "%s", tablica1);
		fclose(plansze);
	}
	int a = planszy_ilosc() + 1;
	FILE *plansze1;
	plansze1 = fopen("ilosc.txt", "w");
	if (plansze1 == NULL)
	{
		printf("plansza error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		_itoa(a, numer, 10);
		fprintf(plansze1, "%s", numer);
		fclose(plansze1);
	}
};
int sprawdz(char tablica[9][9])
{
	int a = 1, b = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if( (tablica[i][j] == SKRZYNIA)||(tablica[i][j] == DOCELOWE))
			{
				b ++;
				break;
			}
		}
	} 
	if (b == 0)a = 0;
	return a;
}
//true = wartosc na y , false = wartosc na x
int znajdz_ludzika(char tablica[9][9], bool co, char czego_szukac)
{
	int x = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (tablica[i][j] == czego_szukac)
			{
				if (co == true)
				{
					x = i;
				}
				else x = j;
				break;
			}
		}
	}
	return x;
};

// kontrolka mówi czy to pion(0) czy poziom (1)
bool warunki(char tablica[9][9], char kierunek, int kontrolka)
{
	bool zamiana = true;
	// a- pozycja na y , b - pozycja na x
	int a, b;
	static int znacznik;
	static int znacznika;
	if (znacznika == 0 )
	{
		znacznika = znacznik;
	}
	znacznik = 0;
	a = znajdz_ludzika(tablica, true, GRACZ);
	b = znajdz_ludzika(tablica, false, GRACZ);
	if (kierunek == PRAWO)
	{
		if (tablica[a][b + 1] == SCIANA)
		{
			zamiana = false;
		}
		else if (((tablica[a][b + 1] == SKRZYNIA) || (tablica[a][b + 1] == DOCELOWE_I_ZAJETE)) && ((tablica[a][b + 2] == SKRZYNIA) || (tablica[a][b + 2] == SCIANA) || (tablica[a][b + 2] == DOCELOWE_I_ZAJETE)))
		{
			zamiana = false;
		}
		else if ((tablica[a][b + 1] == SKRZYNIA) && ((tablica[a][b + 2] == WOLNY) || (tablica[a][b + 2] == DOCELOWE)))
		{
			tablica[a][b + 1] = GRACZ;
			if (znacznik == 1)	tablica[a][b] = DOCELOWE;
			else tablica[a][b] = WOLNY;
			tablica[a][b + 2] = tablica[a][b + 2] + 2;
		}
		else if (tablica[a][b + 1] == DOCELOWE)
		{
			znacznik = 1;
			tablica[a][b + 1] = GRACZ;
			tablica[a][b] = WOLNY;
		}
		else if ((tablica[a][b + 1] == DOCELOWE_I_ZAJETE) && ((tablica[a][b + 2] == WOLNY) || (tablica[a][b + 2] == DOCELOWE)))
		{
			znacznik = 1;
			tablica[a][b + 1] = GRACZ;
			tablica[a][b] = WOLNY;
			tablica[a][b + 2] = tablica[a][b + 2] + 2;
		}
		else
		{
			tablica[a][b + 1] = GRACZ;
			tablica[a][b] = WOLNY;
		};
	}
	else if (kierunek == LEWO)
	{
		if (tablica[a][b - 1] == SCIANA)
		{
			zamiana = false;
		}
		else if (((tablica[a][b - 1] == SKRZYNIA) || (tablica[a][b - 1] == DOCELOWE_I_ZAJETE)) && ((tablica[a][b - 2] == SKRZYNIA) || (tablica[a][b - 2] == SCIANA) || (tablica[a][b - 2] == DOCELOWE_I_ZAJETE)))
		{
			zamiana = false;
		}
		else if ((tablica[a][b - 1] == SKRZYNIA) && ((tablica[a][b - 2] == WOLNY) || ((tablica[a][b - 2] == DOCELOWE))))
		{
			tablica[a][b - 1] = GRACZ;
			if (znacznik == 1)	tablica[a][b] = DOCELOWE;
			else tablica[a][b] = WOLNY;
			tablica[a][b - 2] = tablica[a][b - 2] + 2;
		}
		else if (tablica[a][b - 1] == DOCELOWE)
		{
			znacznik = 1;
			tablica[a][b - 1] = GRACZ;
			tablica[a][b] = WOLNY;
		}
		else if ((tablica[a][b - 1] == DOCELOWE_I_ZAJETE) && ((tablica[a][b - 2] == WOLNY) || (tablica[a][b - 2] == DOCELOWE)))
		{
			znacznik = 1;
			tablica[a][b - 1] = GRACZ;
			tablica[a][b] = WOLNY;
			tablica[a][b - 2] = tablica[a][b - 2] + 2;
		}
		else
		{
			tablica[a][b - 1] = GRACZ;
			tablica[a][b] = WOLNY;
		};
	}
	else if (kierunek == GORA)
	{
		if (tablica[a - 1][b] == SCIANA)
		{
			zamiana = false;
		}
		else if (((tablica[a - 1][b] == SKRZYNIA) || (tablica[a - 1][b] == DOCELOWE_I_ZAJETE)) && ((tablica[a - 2][b] == SKRZYNIA) || (tablica[a - 2][b] == SCIANA) || (tablica[a - 2][b] == DOCELOWE_I_ZAJETE)))
		{
			zamiana = false;
		}
		else if (((tablica[a - 1][b] == SKRZYNIA) || (tablica[a - 1][b] == SKRZYNIA)) && ((tablica[a - 2][b] == WOLNY) || ((tablica[a - 2][b] == DOCELOWE))))
		{
			tablica[a - 1][b] = GRACZ;
			if (znacznik == 1)	tablica[a][b] = DOCELOWE;
			else tablica[a][b] = WOLNY;
			//ró¿nica w charach mmiedzy docelowe a zajete docelowe etc.
			tablica[a - 2][b] = tablica[a - 2][b] + 2;
		}
		else if (tablica[a - 1][b] == DOCELOWE)
		{
			znacznik = 1;
			tablica[a - 1][b] = GRACZ;
			tablica[a][b] = WOLNY;
		}
		else if ((tablica[a - 1][b] == DOCELOWE_I_ZAJETE) && ((tablica[a - 2][b] == WOLNY) || (tablica[a - 2][b] == DOCELOWE)))
		{
			znacznik = 1;
			tablica[a - 1][b] = GRACZ;
			tablica[a][b] = WOLNY;
			tablica[a - 2][b] = tablica[a - 2][b] + 2;
		}
		else
		{
			tablica[a - 1][b] = GRACZ;
			tablica[a][b] = WOLNY;
		};
	}
	else if (kierunek == DOL)
	{

		if (tablica[a + 1][b] == SCIANA)
		{
			zamiana = false;
		}
		else if (((tablica[a + 1][b] == SKRZYNIA) || (tablica[a + 1][b] == DOCELOWE_I_ZAJETE)) && ((tablica[a + 2][b] == SKRZYNIA) || (tablica[a + 2][b] == SCIANA) || (tablica[a + 2][b] == DOCELOWE_I_ZAJETE)))
		{
			zamiana = false;
		}
		else if ((tablica[a + 1][b] == SKRZYNIA) && ((tablica[a + 2][b] == WOLNY) || ((tablica[a + 2][b] == DOCELOWE))))
		{
			tablica[a + 1][b] = GRACZ;
			if (znacznik == 1)
			{
				tablica[a][b] = DOCELOWE;
				znacznik = 0;
			}
			else tablica[a][b] = WOLNY;
			//ró¿nica w charach mmiedzy docelowe a zajete docelowe etc.
			tablica[a + 2][b] = tablica[a + 2][b] + 2;
		}
		else if (tablica[a + 1][b] == DOCELOWE)
		{
			znacznik = 1;
			tablica[a + 1][b] = GRACZ;
			tablica[a][b] = WOLNY;
		}
		else if ((tablica[a + 1][b] == DOCELOWE_I_ZAJETE) && ((tablica[a + 2][b] == WOLNY) || (tablica[a + 2][b] == DOCELOWE)))
		{
			znacznik = 1;
			tablica[a + 1][b] = GRACZ;
			tablica[a][b] = WOLNY;
			tablica[a + 2][b] = tablica[a + 2][b] + 2;
		}
		else
		{
			tablica[a][b] = WOLNY;
			tablica[a + 1][b] = GRACZ;
		};

	};
	if ((znacznika == 1) && (zamiana == true))
	{
		znacznika = 0;
		tablica[a][b] = DOCELOWE;
	}
	//else tablica[a][b] = WOLNY;
	return zamiana;
};
