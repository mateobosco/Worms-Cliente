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
	if(this->musica) Mix_FreeMusic( this->musica );
	this->musica = NULL;

	if(this->s_eBazooka) Mix_FreeChunk(this->s_eBazooka);
	this->s_eBazooka = NULL;
	if(this->s_eGranada)Mix_FreeChunk(this->s_eGranada);
	this->s_eGranada = NULL;
	if(this->s_eDinamita)Mix_FreeChunk(this->s_eDinamita);
	this->s_eDinamita = NULL;
	if(this->s_eHoly)Mix_FreeChunk(this->s_eHoly);
	this->s_eHoly = NULL;
	if(this->s_kamikaze) Mix_FreeChunk(this->s_kamikaze);
	this->s_kamikaze = NULL;
	if(this->s_patada) Mix_FreeChunk(this->s_patada);
	this->s_patada = NULL;

	if(this->s_select) Mix_FreeChunk(this->s_select);
	this->s_select = NULL;
	if(this->s_jump) Mix_FreeChunk(this->s_jump);
	this->s_jump = NULL;
	if(this->s_fuego) Mix_FreeChunk(this->s_fuego);
	this->s_fuego = NULL;
	if(this->s_start)Mix_FreeChunk(this->s_start);
	this->s_start = NULL;
	if(this->s_lanzar) Mix_FreeChunk(this->s_lanzar);
	this->s_lanzar = NULL;
	if(this->s_disparo) Mix_FreeChunk(this->s_disparo);
	this->s_disparo = NULL;
	if(this->s_dinamita) Mix_FreeChunk(this->s_dinamita);
	this->s_dinamita = NULL;
	if(this->s_mecha) Mix_FreeChunk(this->s_mecha);
	this->s_mecha = NULL;
	if(this->s_time) Mix_FreeChunk(this->s_time);
	this->s_time = NULL;
	if(this->s_bye) Mix_FreeChunk(this->s_bye);
	this->s_bye = NULL;
	if(this->s_walk) Mix_FreeChunk(this->s_walk);
	this->s_walk = NULL;
	if(this->s_tick) Mix_FreeChunk(this->s_tick);
	this->s_tick = NULL;
	if(this->s_vida) Mix_FreeChunk(this->s_vida);
	this->s_vida = NULL;
}






void Musica::playMusic(){
	  if( Mix_PlayingMusic() == 0 ) {
		Mix_VolumeMusic(50);
	    Mix_PlayMusic( this->musica, -1 );
	  }
}


void Musica::playMusicaInicio(){
	if( this->inicio == NULL ) this->inicio = Mix_LoadMUS( "TPTaller/sonido/inicio.wav" );
	  if( Mix_PlayingMusic() == 0 ) {
	     //Play the music
	     Mix_PlayMusic( this->inicio, -1 );
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
				return;
			}
			Mix_PlayChannel(-1, this->s_select, 0);
			break;
		}
		case UP: {
			if( !this->s_jump ) this->s_jump = Mix_LoadWAV("TPTaller/sonido/JUMP1.WAV");
			if(this->s_jump == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			//Mix_VolumeChunk(this->s_jump, 100);
			Mix_PlayChannelTimed(-1, this->s_jump, 0, 1000);
			//Mix_PlayChannel(-1, this->s_jump, 0);
			break;
		}
		case TIME: {
			if( !this->s_time ) this->s_time = Mix_LoadWAV("TPTaller/sonido/clock-ticking-2.wav");
			if(this->s_time == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			Mix_VolumeChunk(this->s_time, 128);
			Mix_PlayChannelTimed(-1, this->s_time, -1, 10000);
			//Mix_PlayChannel(-1, this->s_time, 0);
			break;
		}
		case START: {
			if( !this->s_start ) this->s_start = Mix_LoadWAV("TPTaller/sonido/StartRound.wav");
			if(this->s_start == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			Mix_PlayChannel(-1, this->s_start, 0);
			break;
		}

		//Caso Lanzar TIpo disparo
		case DISPARO: {
			if( !this->s_disparo ) this->s_disparo = Mix_LoadWAV("TPTaller/sonido/gunFire.wav");
			if(this->s_disparo == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			//printf("disparo\n");
			Mix_VolumeChunk(this->s_disparo, 100);
			//Mix_PlayChannelTimed(-1, this->s_disparo, 0, 500);
			Mix_PlayChannel(-1, this->s_disparo, 0);
			break;
		}
		case LANZAR_DINAMITA: {
			if( !this->s_dinamita ) this->s_dinamita = Mix_LoadWAV("TPTaller/sonido/export (1).wav");//dinamita.wav
			if(this->s_dinamita == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			//printf("Sonido Lanzar dinamita\n");
			Mix_VolumeChunk(this->s_dinamita, 80);
			Mix_PlayChannel(1, this->s_dinamita, 0);
			//Mix_PlayChannelTimed(-1, this->s_dinamita, -1, 4500);
			break;
		}

		case MECHA_DINAMITA: {
			if( !this->s_mecha ) this->s_mecha = Mix_LoadWAV("TPTaller/sonido/mechadinamita.WAV");
			if(this->s_mecha == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			//printf("sonido mecha dinamita\n");
			Mix_PlayChannelTimed(-1, this->s_mecha, -1, 4500);
			break;
		}
		//Lanzar no disparo
		case LANZAR:{
			if( !this->s_lanzar ) this->s_lanzar = Mix_LoadWAV("TPTaller/sonido/lanzar4.wav");
			if(this->s_lanzar == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			Mix_PlayChannel(-1, this->s_lanzar, 0);
			break;

		}
		//EXPLOSIONES
		case EXPLOSION_BAZOOKA: {
			if( !this->s_eBazooka ) this->s_eBazooka = Mix_LoadWAV("TPTaller/sonido/Explosion2.wav");
			if(this->s_eBazooka == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
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
				return;
			}
			Mix_PlayChannel(-1, this->s_patada, 0);
			break;
		}
		case FUEGO:{
			if( !this->s_fuego ) this->s_fuego = Mix_LoadWAV("TPTaller/sonido/fuego.WAV");
			if(this->s_fuego == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			Mix_PlayChannel(-1, this->s_fuego, 0);
			break;
		}
		case BYE:{
			if( !this->s_bye ) this->s_bye = Mix_LoadWAV("TPTaller/sonido/BYEBYE.WAV");
			if(this->s_bye == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
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
				return;
			}
			//Mix_PlayChannelTimed(1, this->s_walk, 0, 500);
			Mix_PlayChannel(-1, this->s_walk, 0);
			break;
		}
		case TICK: {
			if( !this->s_tick ) this->s_tick = Mix_LoadWAV("TPTaller/sonido/tick.WAV");
			if(this->s_tick == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			Mix_VolumeChunk(this->s_tick, 128);
			Mix_PlayChannelTimed(-1, this->s_tick, 0, 1000);

			//Mix_PlayChannel(0, this->s_tick, 0);
			break;
		}
		case VIDA:{
			if( !this->s_vida ) this->s_vida = Mix_LoadWAV("TPTaller/sonido/OUCH.WAV");
			if(this->s_vida == NULL){
				loguear();
				logFile << "No se pudo cargar sonido: %s" << Mix_GetError() << endl;
				return;
			}
			//printf("Pierde vida\n");
			Mix_PlayChannel(-1, this->s_vida, 0);
			break;
		}
	}
}
