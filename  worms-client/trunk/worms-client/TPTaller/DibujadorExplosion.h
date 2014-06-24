/*
 * DibujadorExplosion.h
 *
 *  Created on: 19/06/2014
 *      Author: mateo
 */

#ifndef DIBUJADOREXPLOSION_H_
#define DIBUJADOREXPLOSION_H_

#include "Dibujador.h"

class DibujadorExplosion {
public:
	DibujadorExplosion(SDL_Renderer* renderer, Escalador* escalador);
	virtual ~DibujadorExplosion();
	void actualizarExplosion();
	void actualizarExplosionBazooka();
	void actualizarExplosionGranada();
	void actualizarExplosionHoly();
	void actualizarExplosionPatada();
	void start(int tipo,b2Vec2 pos, float32 radio);

private:
	SDL_Renderer *renderizador;
	Escalador* escalador;
	char* pathExplosion;
	SDL_Surface* surfaceExplosion;
	SDL_Texture* textureExplosion;
	int actual;
	b2Vec2 pos;
	float32 radio;
	int tipo;
};


#endif /* DIBUJADOREXPLOSION_H_ */
