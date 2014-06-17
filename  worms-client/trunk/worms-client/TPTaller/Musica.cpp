/*
 * Musica.cpp
 *
 *  Created on: 27/05/2014
 *      Author: sami
 */

#include "Musica.h"
extern void loguear();
extern ofstream logFile;



Musica::Musica() {
	  this->musica = Mix_LoadMUS( "TPTaller/sonido/Worms.wav" );
	  if( this->musica == NULL ){
	         printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	     }
//	  this->playMusic();



}

Musica::~Musica() {
	Mix_FreeMusic( this->musica );
	this->musica = NULL;
	Mix_FreeChunk(this->s_bazooka);
	this->s_bazooka = NULL;
	Mix_FreeChunk(this->s_granada);
	this->s_granada = NULL;
	Mix_FreeChunk(this->s_select);
	this->s_select = NULL;
	Mix_FreeChunk(this->s_jump);
	this->s_jump = NULL;
	Mix_FreeChunk(this->s_explosion);
	this->s_explosion = NULL;
}

void Musica::playMusic(){
	  if( Mix_PlayingMusic() == 0 ) {
	     //Play the music
	     Mix_PlayMusic( this->musica, -1 );
	  }
}

void Musica::stopMusic(){
	if( Mix_PlayingMusic() != 0 ) {
		Mix_HaltMusic();
	  }

}

Mix_Music* Musica::getMusica(){
	return this->musica;
}

void Musica::playSonido(int id){

	switch(id){
		case SELECT: {
			this->s_select = Mix_LoadWAV("TPTaller/sonido/select.WAV");
			if(this->s_select == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;

				printf("No cargó WAV, %s", Mix_GetError());
			}
			Mix_PlayChannel(-1, this->s_select, 0);
			break;
		}
		case UP: {
			this->s_jump = Mix_LoadWAV("TPTaller/sonido/Jump1.wav");
			if(this->s_jump == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
			}
			Mix_PlayChannel(-1, this->s_jump, 0);
			break;
		}
		case TIME: {
			this->s_time = Mix_LoadWAV("TPTaller/sonido/clock-ticking-2.wav");
			if(this->s_time == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
			}
			Mix_PlayChannelTimed(-1, this->s_time, 0, 10000);
			//Mix_PlayChannel(-1, this->s_time, 0);
			break;
		}

		case BAZOOKA: {
			this->s_bazooka = Mix_LoadWAV("TPTaller/sonido/Bazooka.wav");
			if(this->s_bazooka == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				
			}
			Mix_PlayChannel(-1, this->s_bazooka, 0);
			break;

		}
		case GRANADA:{
			this->s_granada = Mix_LoadWAV("TPTaller/sonido/ExplosionGrenade.wav");
			if(this->s_granada == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
			}
			Mix_PlayChannel(-1, this->s_granada, 0);
			break;
		}
		case EXPLOSION:{
			this->s_explosion = Mix_LoadWAV("TPTaller/sonido/Explosion1.wav");
			if(this->s_explosion == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
			}
			Mix_PlayChannel(-1, this->s_explosion, 0);
			break;
		}
	}
}
