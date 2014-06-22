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
	if( this->musica == NULL ) this->musica = Mix_LoadMUS( "TPTaller/sonido/Worms.wav" );
	if( this->musica == NULL ){
		loguear();
		logFile << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << endl;
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}
}

Musica::~Musica() {
	Mix_FreeMusic( this->musica );
	this->musica = NULL;

	Mix_FreeChunk(this->s_eBazooka);
	this->s_eBazooka = NULL;
	Mix_FreeChunk(this->s_eGranada);
	this->s_eGranada = NULL;
	Mix_FreeChunk(this->s_eDinamita);
	this->s_eDinamita = NULL;
	Mix_FreeChunk(this->s_eHoly);
	this->s_eHoly = NULL;
	Mix_FreeChunk(this->s_kamikaze);
	this->s_kamikaze = NULL;
	Mix_FreeChunk(this->s_patada);
	this->s_patada = NULL;

	Mix_FreeChunk(this->s_select);
	this->s_select = NULL;
	Mix_FreeChunk(this->s_jump);
	this->s_jump = NULL;
	Mix_FreeChunk(this->s_explosion);
	this->s_explosion = NULL;
	Mix_FreeChunk(this->s_start);
	this->s_start = NULL;
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

			if( !this->s_select ) this->s_select = Mix_LoadWAV("TPTaller/sonido/MINETICK.WAV");
			if(this->s_select == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_select, 0);
			break;
		}
		case UP: {
			if( !this->s_jump ) this->s_jump = Mix_LoadWAV("TPTaller/sonido/Jump1.wav");
			if(this->s_jump == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s\n", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_jump, 0);
			break;
		}
		case TIME: {
			if( !this->s_time ) this->s_time = Mix_LoadWAV("TPTaller/sonido/clock-ticking-2.wav");
			if(this->s_time == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s\n", Mix_GetError());
				return;
			}
			Mix_PlayChannelTimed(-1, this->s_time, 0, 10000);
			//Mix_PlayChannel(-1, this->s_time, 0);
			break;
		}
		case START: {
			if( !this->s_start ) this->s_start = Mix_LoadWAV("TPTaller/sonido/StartRound.wav");
			if(this->s_start == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_start, 0);
			break;
		}

		//Caso Lanzar TIpo disparo
		case LANZAR_BAZOOKA: {
			if( !this->s_disparo ) this->s_disparo = Mix_LoadWAV("TPTaller/sonido/disparo.wav");
			if(this->s_disparo == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_disparo, 0);
		}
		case LANZAR_DINAMITA: {
			if( !this->s_dinamita ) this->s_dinamita = Mix_LoadWAV("TPTaller/sonido/dinamita.wav");
			if(this->s_dinamita == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_dinamita, 0);
		}

		//Lanzar no disparo
		case LANZAR:{
			if( !this->s_lanzar ) this->s_lanzar = Mix_LoadWAV("TPTaller/sonido/lanzar4.wav");
			if(this->s_lanzar == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_lanzar, 0);

		}
		//EXPLOSIONES
		case EXPLOSION_BAZOOKA: {
			if( !this->s_eBazooka ) this->s_eBazooka = Mix_LoadWAV("TPTaller/sonido/Explosion2.wav");
			if(this->s_eBazooka == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_eBazooka, 0);
			break;

		}
		case EXPLOSION_GRANADA:{
			if( !this->s_eGranada ) this->s_eGranada = Mix_LoadWAV("TPTaller/sonido/Explosion2.wav");
			if(this->s_eGranada == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_eGranada, 0);
			break;
		}
		case EXPLOSION_DINAMITA:{
			if( !this->s_eDinamita ) this->s_eDinamita = Mix_LoadWAV("TPTaller/sonido/Explosion3.WAV");
			if(this->s_eDinamita == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_eDinamita, 0);
			break;
		}
		case EXPLOSION_HOLY:{
			if( !this->s_eHoly ) this->s_eHoly = Mix_LoadWAV("TPTaller/sonido/holy.WAV");
			if(this->s_eHoly == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_eHoly, 0);
			break;
		}
		case KAMIKAZE:{
			if( !this->s_kamikaze ) this->s_kamikaze = Mix_LoadWAV("TPTaller/sonido/KAMIKAZERELEASE.WAV");
			if(this->s_kamikaze == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_kamikaze, 0);
			break;
		}
		case PATADA:{
			if( !this->s_patada ) this->s_patada = Mix_LoadWAV("TPTaller/sonido/patada3.wav");
			if(this->s_patada == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_patada, 0);
			break;
		}
		case EXPLOSION:{
			if( !this->s_explosion ) this->s_explosion = Mix_LoadWAV("TPTaller/sonido/Explosion2.wav");
			if(this->s_explosion == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s\n", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_explosion, 0);
			break;
		}
		case BYE:{
			if( !this->s_bye ) this->s_bye = Mix_LoadWAV("TPTaller/sonido/BYEBYE.WAV");
			if(this->s_bye == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s\n", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_bye, 0);
			break;
		}
		case WALK:{
			if( !this->s_walk ) this->s_walk = Mix_LoadWAV("TPTaller/sonido/walk-expand.wav");
			if(this->s_walk == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				printf("No cargó WAV, %s\n", Mix_GetError());
				return;
			}
			Mix_PlayChannel(-1, this->s_walk, 0);
			break;
		}

	}
}
