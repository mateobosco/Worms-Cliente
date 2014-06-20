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

#define BAZOOKA 1
#define GRANADA 2
#define DINAMITA 3
#define HOLY 4
#define KAMIKAZE 5
#define PATADA 6
#define EXPLOSION 7

using namespace std;
class Musica {
private:
	Mix_Music *musica;
	Mix_Chunk *s_select;
	Mix_Chunk *s_jump;
	Mix_Chunk *s_granada;
	Mix_Chunk *s_bazooka;
	Mix_Chunk* s_time;
	Mix_Chunk* s_explosion;


public:
	Musica();

	 ~Musica();
	Mix_Music* getMusica();
	void playMusic();
	void stopMusic();
	void playSonido(int id);
};

#endif /* MUSICA_H_ */
