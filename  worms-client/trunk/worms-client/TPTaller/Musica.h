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
#include <string>

#define UP 1
#define SELECT 2
#define TIME 3
#define BAZOOKA 4
#define GRANADA 5

using namespace std;
class Musica {
private:
	Mix_Music *musica = NULL;
	Mix_Chunk *s_select = NULL;
	Mix_Chunk *s_jump = NULL;
	Mix_Chunk *s_granada = NULL;
	Mix_Chunk *s_bazooka = NULL;
	Mix_Chunk* s_time = NULL;


public:
	Musica();

	 ~Musica();
	Mix_Music* getMusica();
	void playMusic();
	void stopMusic();
	void playSonido(int id);
};

#endif /* MUSICA_H_ */
