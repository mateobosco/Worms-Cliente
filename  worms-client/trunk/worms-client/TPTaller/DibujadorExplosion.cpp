/*
 * DibujadorExplosion.cpp
 *
 *  Created on: 19/06/2014
 *      Author: mateo
 */

#include "DibujadorExplosion.h"
#include "Dibujador.h"


DibujadorExplosion::DibujadorExplosion(SDL_Renderer* renderer, Escalador* escalador){
	this->renderizador = renderer;
	this->escalador = escalador;
	this->actual = -1;
	this->pos = b2Vec2(-1,-1);
	this->radio = 0;
	this->tipo = -1;
	this->textureExplosion = NULL;
	this->surfaceExplosion = NULL;
//	this->pathExplosion = '\0';
}

DibujadorExplosion::~DibujadorExplosion() {
	if (this->textureExplosion) SDL_DestroyTexture(this->textureExplosion);
	if (this->textureExplosion) SDL_FreeSurface(this->surfaceExplosion);
}

void DibujadorExplosion::actualizarExplosion(){
	if (this->tipo == 1) this->actualizarExplosionBazooka();
	if (this->tipo == 4) this->actualizarExplosionHoly();
	if (this->tipo == 2 || this->tipo == 3 || this->tipo == 5) this->actualizarExplosionGranada();
	if (this->tipo == 6) this->actualizarExplosionPatada();
}

void DibujadorExplosion::actualizarExplosionHoly(){
	if (this->actual < 0) return;

	SDL_Rect puntos;
	SDL_Rect recta;

	b2Vec2* posPix = escalador->aplicarZoomPosicion(pos);
	recta.x = (int) posPix->x;
	recta.y = (int) posPix->y;

	int cantidadMov = 4;

	int aux = 2;
	if ((this->actual >= 1) && (this->actual <= aux)) {
		puntos.x = 0;
		puntos.y = 0;
		puntos.w = 264;
		puntos.h = 220;

		recta.x -= 130;
		recta.y -= 100;
		escalador->moverDerecha(cantidadMov);
	}
	if ((this->actual >= 1+aux) && (this->actual <= aux*2)) {
		puntos.x = 264;
		puntos.y = 0;
		puntos.w = 286;
		puntos.h = 220;

		recta.x -= (410-264);
		recta.y -= 100;
		escalador->moverIzquierda(cantidadMov);
	}
	if ((this->actual >= 1+aux*2) && (this->actual <= aux*3)) {
		puntos.x = 550;
		puntos.y = 0;
		puntos.w = 300;
		puntos.h = 220;

		recta.x -= (680-550);
		recta.y -= 100;
		escalador->moverDerecha(cantidadMov-1);
	}
	if ((this->actual >= 1+aux*3) && (this->actual <= aux*4)) {
		puntos.x = 0;
		puntos.y = 220;
		puntos.w = 286;
		puntos.h = 240;

		recta.x -= 130;
		recta.y -= (410-220);
		escalador->moverIzquierda(cantidadMov-1);
	}
	if ((this->actual >= 1+aux*4) && (this->actual <= aux*5)) {
		puntos.x = 264;
		puntos.y = 220;
		puntos.w = 264;
		puntos.h = 240;

		recta.x -= (410-264);
		recta.y -= (410-220);
		escalador->moverDerecha(cantidadMov-1);
	}
	if ((this->actual >= 1+aux*5) && (this->actual <= aux*6)) {
		puntos.x = 550;
		puntos.y = 220;
		puntos.w = 300;
		puntos.h = 240;

		recta.x -= (680-550);
		recta.y -= (410-220);
		escalador->moverIzquierda(cantidadMov-1);
	}
	if ((this->actual >= 1+aux*6) && (this->actual <= aux*7)) {
		puntos.x = 0;
		puntos.y = 460;
		puntos.w = 264;
		puntos.h = 270;

		recta.x -= 130;
		recta.y -= (680-460);
		escalador->moverDerecha(cantidadMov-4);
	}
	if ((this->actual >= 1+aux*7) && (this->actual <= aux*8)) {
		puntos.x = 264;
		puntos.y = 460;
		puntos.w = 286;
		puntos.h = 270;

		recta.x -= (410-264);
		recta.y -= (680-460);
		escalador->moverIzquierda(cantidadMov-4);
	}
	if ((this->actual >= 1+aux*8) && (this->actual <= aux*9)) {
		puntos.x = 550;
		puntos.y = 460;
		puntos.w = 300;
		puntos.h = 270;

		recta.x -= (680-550);
		recta.y -= (680-460);
		escalador->moverDerecha(cantidadMov-4);
	}
	if ((this->actual >= 1+aux*9) && (this->actual <= aux*10)) {
		puntos.x = 264;
		puntos.y = 730;
		puntos.w = 286;
		puntos.h = 220;

		recta.x -= (410-264);
		recta.y -= (880-730);
		this->actual = -5;
		this->tipo = -1;
	}

	recta.w = puntos.w;
	recta.h = puntos.h;
	SDL_RenderCopy(this->renderizador,this->textureExplosion,&puntos,&recta);

	delete posPix;
	this->actual++;
}

void DibujadorExplosion::start(int tipo, b2Vec2 pos, float32 radio){
	this->tipo = tipo;
	this->pos = pos;
	this->radio = radio;
	this->actual = 1;

	if (this->surfaceExplosion) SDL_FreeSurface(this->surfaceExplosion);
	if (this->textureExplosion) SDL_DestroyTexture(this->textureExplosion);

	if (this->tipo == 1){
		this->pathExplosion = "TPTaller/imagenes/explosionBazooka.png";
		this->surfaceExplosion = IMG_Load(this->pathExplosion);
		this->textureExplosion = SDL_CreateTextureFromSurface(this->renderizador,this->surfaceExplosion);
		printf("LLega acá %d \n", this->tipo);
	}
	if (this->tipo == 4){
		this->pathExplosion = "TPTaller/imagenes/explosionHoly.png";
		this->surfaceExplosion = IMG_Load(this->pathExplosion);
		this->textureExplosion = SDL_CreateTextureFromSurface(this->renderizador,this->surfaceExplosion);
		printf("LLega acá %d \n", this->tipo);
	}
	if (this->tipo == 2 || this->tipo == 3 || this->tipo == 5){
		this->pathExplosion = "TPTaller/imagenes/explosionGranada.png";
		this->surfaceExplosion = IMG_Load(this->pathExplosion);
		this->textureExplosion = SDL_CreateTextureFromSurface(this->renderizador,this->surfaceExplosion);
		printf("LLega acá %d \n", this->tipo);
	}
	if (this->tipo == 6 ){
		this->pathExplosion = "TPTaller/imagenes/patadaKapow.png";
		this->surfaceExplosion = IMG_Load(this->pathExplosion);
		this->textureExplosion = SDL_CreateTextureFromSurface(this->renderizador,this->surfaceExplosion);
	}

}

void DibujadorExplosion::actualizarExplosionBazooka(){
	if (this->actual < 0) return;
	if (this->actual > 40){
		this->actual = -1;
		this->tipo = -1;
		return;
	}
	int numero = this->actual/2 -1;

	int indiceX = numero % 5 ;
	int indiceY = (numero - indiceX) / 5;

	int limiteX = indiceX * 96;
	int limiteY = indiceY * 96;

	SDL_Rect src;
	src.x = limiteX;
	src.y = limiteY;
	src.w = 96;
	src.h = 96;

	SDL_Rect posicion;
	b2Vec2* posPix = escalador->aplicarZoomPosicion(pos);
	posicion.w = 96 * escalador->getZoom()/100;
	posicion.h = 96 * escalador->getZoom()/100;
	posicion.x = (int) posPix->x - posicion.w/2;
	posicion.y = (int) posPix->y - posicion.h/2;

	int cantidadMov = 20;
	if (this->actual > 20) cantidadMov = 0;
	if (this->actual > 12) cantidadMov = 10;
	if (this->actual % 2 == 1) escalador->moverDerecha(cantidadMov);
	if (this->actual % 2 == 0) escalador->moverIzquierda(cantidadMov);



	SDL_RenderCopy(this->renderizador,this->textureExplosion,&src,&posicion);

	this->actual ++;
	delete posPix;
}

void DibujadorExplosion::actualizarExplosionGranada(){
	if (this->actual < 0) return;
	if (this->actual > 40){
		this->actual = -1;
		this->tipo = -1;
		return;
	}
	int numero = this->actual -1;

	int indiceX = numero % 8 ;
	int indiceY = (numero - indiceX) / 8;

	int limiteX = indiceX * 128;
	int limiteY = indiceY * 128;

	SDL_Rect src;
	src.x = limiteX;
	src.y = limiteY;
	src.w = 128;
	src.h = 128;

	SDL_Rect posicion;
	b2Vec2* posPix = escalador->aplicarZoomPosicion(pos);
	posicion.w = 128 * escalador->getZoom()/100;
	posicion.h = 128 * escalador->getZoom()/100;
	posicion.x = (int) posPix->x - posicion.w/2;
	posicion.y = (int) posPix->y - posicion.h/2;

	int cantidadMov = 20;
	if (this->actual > 17) cantidadMov = 0;
	if (this->actual > 10) cantidadMov = 3;
	if (this->actual > 5) cantidadMov = 10;
	if (this->actual % 2 == 1) escalador->moverDerecha(cantidadMov);
	if (this->actual % 2 == 0) escalador->moverIzquierda(cantidadMov);

	SDL_RenderCopy(this->renderizador,this->textureExplosion,&src,&posicion);

	this->actual ++;
	delete posPix;

}


void DibujadorExplosion::actualizarExplosionPatada(){
	if (this->actual < 0) return;
	if (this->actual > 15){
		this->actual = -1;
		this->tipo = -1;
		return;
	}

	if (this->actual == 1 ||this->actual == 3) escalador->moverDerecha(5);
	if (this->actual == 2 ||this->actual == 4) escalador->moverIzquierda(5);
	SDL_Rect posicion;
	b2Vec2* posPix = escalador->aplicarZoomPosicion(pos);
	posicion.w = this->surfaceExplosion->w * escalador->getZoom()/100;
	posicion.h = this->surfaceExplosion->h * escalador->getZoom()/100;
	posicion.x = (int) posPix->x - posicion.w/2;
	posicion.y = (int) posPix->y - posicion.h/2;
	delete posPix;

	SDL_RenderCopy(this->renderizador,this->textureExplosion,NULL,&posicion);

	this->actual ++;
}

