#ifndef NAG≥ÛWEK_H_INCLUDED
#define NAG≥ÛWEK_H_INCLUDED

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "Nag≥Ûwek.h"
#define POCZATEK_PLANSZY_X 100
#define POCZATEK_PLANSZY_Y 100
#define ROZMIAR_SRITE 30
#define WOLNY '0'
#define GRACZ '1'
#define SKRZYNIA '2'
#define DOCELOWE '3'
#define SCIANA '4'
#define DOCELOWE_I_ZAJETE '5'
#define LEWO 'a'
#define PRAWO 'b'
#define GORA 'c'
#define DOL 'd'
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
int znajdz_ludzika(char tablica[9][9], bool co, char czego_szukac);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
int inicjalizacja(char tablica[9][9], SDL_Surface *screen, SDL_Surface *charset,int level);
int WLASNA_PLANSZA(SDL_Renderer *renderer, SDL_Texture *scrtex, SDL_Surface *charset, SDL_Surface *screen, SDL_Surface *sprite0, SDL_Surface *sprite1, SDL_Surface *sprite2, SDL_Surface *sprite3, SDL_Surface *sprite4, SDL_Surface *sprite5, char tablica[9][9]);
int sprawdz(char tablica[9][9]);
void WYSWIETL_OPCJE(SDL_Renderer *renderer, SDL_Texture *scrtex, SDL_Surface *charset, SDL_Surface *screen, int *level);
bool warunki(char tablica[9][9], char kierunek, int kontrolka);
void DrawGame(SDL_Surface *screen, SDL_Surface *sprite0, SDL_Surface *sprite1, SDL_Surface *sprite2, SDL_Surface *sprite3, SDL_Surface *sprite4, SDL_Surface *sprite5, char tablica[9][9]);
//bool warunki(char tablica[9][9], int kierunek);
#endif // NAG≥ÛWEK_H_INCLUDED
