/*
 * Bazooka.h
 *
 *  Created on: 30/05/2014
 *      Author: juanmahidalgo
 */

#ifndef BAZOOKA_H_
#define BAZOOKA_H_


#include "Arma.h"

class Bazooka: public Arma {
public:
	Bazooka();

	virtual ~Bazooka();

protected:
	int jugador_duenio;
};

#endif /* BAZOOKA_H_ */
