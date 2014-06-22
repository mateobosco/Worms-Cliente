/*
 * Musica.h
 *
 *  Created on: 27/05/2014
 *      Author: sami
 */

#ifndef MUSICA_H_
#define MUSICA_H_


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

#define UP 10
#define SELECT 11
#define TIME 12
#define START 13
#define LANZAR_BAZOOKA 14
#define LANZAR_DINAMITA 16
#define LANZAR 15
#define BYE 17
#define WALK 18

#define EXPLOSION_BAZOOKA 1
#define EXPLOSION_GRANADA 2
#define EXPLOSION_DINAMITA 3
#define EXPLOSION_HOLY 4
#define KAMIKAZE 5
#define PATADA 6
#define EXPLOSION 7


using namespace std;
class Musica {
private:
	Mix_Music *musica = NULL;

	Mix_Chunk *s_select = NULL;
	Mix_Chunk *s_jump = NULL;

	Mix_Chunk *s_lanzar = NULL;
	Mix_Chunk *s_disparo = NULL;
	Mix_Chunk *s_dinamita = NULL;

	Mix_Chunk *s_eGranada = NULL;
	Mix_Chunk *s_eBazooka = NULL;
	Mix_Chunk* s_eHoly = NULL;
	Mix_Chunk* s_eDinamita = NULL;
	Mix_Chunk* s_kamikaze = NULL;
	Mix_Chunk* s_patada = NULL;

	Mix_Chunk* s_time = NULL;
	Mix_Chunk* s_explosion = NULL;
	Mix_Chunk* s_start = NULL;
	Mix_Chunk* s_bye = NULL;
	Mix_Chunk* s_walk = NULL;


public:
	Musica();

	 ~Musica();
	Mix_Music* getMusica();
	void playMusic();
	void stopMusic();
	void playSonido(int id);
};

#endif /* MUSICA_H_ */
