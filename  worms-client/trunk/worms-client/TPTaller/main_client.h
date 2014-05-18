#ifndef MAIN_CLIENT_H_
#define MAIN_CLIENT_H_

#include "Juego.h"
#include "Funciones.h"
#include "Cliente.h"
#include "Constantes.h"
#include "Paquete.h"
#include <string>

structEvento* crearPaqueteClick(int* click, Escalador* escalador, int cliente);// sino me tira error de que no esta definida aunque este el include



// argv[n]:
// 			n=0: Nombre del programa
// 			n=1: Nombre del cliente/jugador
// 			n=2: IP
// 			n=3: Puerto
int mainCliente(int argc, char* argv[]){

	if(checkCantParametros(argc) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	const char* name = argv[POS_NAME_USER];
	const char* ip_sv = argv[POS_IP];
	const char* puerto = argv[POS_PORT];

	Cliente* cliente = new Cliente(name, ip_sv, puerto);

	if(cliente->conectar() != EXIT_SUCCESS){
		return EXIT_FAILURE;
	}

	while (!cliente->getPaqueteInicial()){
	}

	structInicial* paqueteInicial = (structInicial*) cliente->getPaquete();

	Escalador* escalador = new Escalador(paqueteInicial->ancho_ventana , paqueteInicial->alto_ventana,
			paqueteInicial->ancho_unidades, paqueteInicial->alto_unidades,
			paqueteInicial->ancho_escenario,paqueteInicial->alto_escenario);

	Dibujador dibujador = Dibujador(NULL, escalador);
	dibujador.init();
	SDL_Event event;
	for(int i = 0; i < 322; i++) { // inicializa todas en falso
	   KEYS[i] = false;
	}
	string pathAgua = string(paqueteInicial->agua);
	string pathTierra = string(paqueteInicial->tierra);
	string pathCielo = string(paqueteInicial->cielo);
	Agua* agua = new Agua(paqueteInicial->nivel_agua, pathAgua);


	dibujador.iniciarFondo(agua, pathCielo, pathTierra);
	dibujador.dibujarFondo(agua);

	SDL_Delay(5000);


	//delete paqueteInicial;

	int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
	posicion_mouse_click[0] = -1;
	posicion_mouse_click[1] = -1;

	structPaquete* paquete = new structPaquete;

	while(KEYS[SDLK_ESCAPE] == false){

		dibujador.dibujarFondo(agua);
		keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);


		escalador->moverVentana(posicion_mouse_movimiento);
		escalador->hacerZoom(posicion_mouse_scroll);
		paquete = (structPaquete*) cliente->getPaquete();
		structFigura* vector = paquete->vector_figuras;
		int cantidad = paquete->cantidad_figuras;
		printf("CANTIDAD DE FIGURAS %d \n",cantidad);
		structFigura paqueteFigura = vector[0];
		b2Vec2 posicion = paqueteFigura.vector_vertices[2];
		printf("posicion de la figura : (%f,%f) \n",posicion.x,posicion.y);


		printf("CANTIDAD DE PERSONAJES %d \n",paquete->cantidad_personajes);
		structPersonaje* vector_personajes = paquete->vector_personajes;
		//structPersonaje vector_personajes = vector_personajes[0];
		b2Vec2 posicion2 = paqueteFigura.vector_vertices[2];
		printf("posicion del gusanito 1 es  : (%f,%f) \n",posicion2.x,posicion2.y);



		keyboard(event, posicion_mouse_movimiento, posicion_mouse_click, posicion_mouse_scroll);

		dibujador.dibujarPaquete(paquete);
		dibujador.actualizar();
		posicion_mouse_scroll[2]=0;
	}

	delete agua;
	delete escalador;
	delete paquete;
	free(posicion_mouse_click);
	free(posicion_mouse_scroll);
	free(posicion_mouse_movimiento);
	dibujador.close();

	//delete cliente;
	return 0;
}

#endif /* MAIN_CLIENT_H_ */
