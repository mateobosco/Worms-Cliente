#ifndef MANEJADORPERSONAJES_H_
#define MANEJADORPERSONAJES_H_

#include "Personaje.h"

class ManejadorPersonajes {
public:
	ManejadorPersonajes();
	void seleccionarPersonaje(b2Vec2 posicion, int id_jugador);
	void AgregarJugador(Mundo* mundo, int id_jugador, Personaje** pers); //TODO hay que borrarlo
	Personaje** getPersonajes();
	int getCantidadPersonajes();
	void moverPersonaje(int direccion,int id_jugador);
	virtual ~ManejadorPersonajes();
	void agregarPersonaje(Personaje *personaje, uint8 numero_jugador);
	int getCantidadJugadores();

private:
	Personaje** vector_personajes;
	uint8 personajesMax;
	uint8 cantidad_jugadores;
	uint8 cantidad_actual_personajes;

};

#endif /* MANEJADORPERSONAJES_H_ */
