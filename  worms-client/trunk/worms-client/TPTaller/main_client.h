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
	printf("nivel del agua es %f \n", paqueteInicial->nivel_agua);
	//SDL_Delay(60000);

	Escalador* escalador = new Escalador(paqueteInicial->ancho_ventana , paqueteInicial->alto_ventana,
			paqueteInicial->ancho_unidades, paqueteInicial->alto_unidades,
			paqueteInicial->ancho_escenario,paqueteInicial->alto_escenario);
	crearVentana(escalador);
	Dibujador* dibujador = new Dibujador(gRenderer, escalador);
	SDL_Event event;
	for(int i = 0; i < 322; i++) { // inicializa todas en falso
	   KEYS[i] = false;
	}
	string pathAgua = string(paqueteInicial->agua);
	string pathTierra = string(paqueteInicial->tierra);
	string pathCielo = string(paqueteInicial->cielo);
	printf("path tierra %s \n",pathTierra.c_str());
	printf("path agua JUAN MANUEL HIJO DE UNA GRAN PUTA %s \n",pathAgua.c_str());
	printf("path cielo %s \n",pathCielo.c_str());
	Agua* agua = new Agua(paqueteInicial->nivel_agua, pathAgua);


	dibujador->iniciarFondo(agua, pathCielo, pathTierra);

	//SDL_Delay(20000);


	//delete paqueteInicial;

	int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
	posicion_mouse_click[0] = -1;
	posicion_mouse_click[1] = -1;

	structPaquete* paquete = new structPaquete;


	while(true){
		paquete = (structPaquete*) cliente->getPaquete();
		structFigura* vector = paquete->vector_figuras;
/*		int cantidad = paquete->cantidad_figuras;
		printf("CANTIDAD DE FIGURAS %d \n",cantidad);
		structFigura paqueteFigura = vector[0];
		b2Vec2 posicion = paqueteFigura.vector_vertices[2];
		printf("posicion de la figura : (%f,%f) \n",posicion.x,posicion.y);*/
		dibujador->dibujarFondo(agua);
		dibujador->dibujarPaquete(paquete);
		SDL_Delay(500);
	}

/*

	while (KEYS[SDLK_ESCAPE] == false){
		//structPaquete = cliente.recibir(sizeof(structPaquete),paquete);
		//
		keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
		escalador->moverVentana(posicion_mouse_movimiento);
		escalador->hacerZoom(posicion_mouse_scroll);
		//structEvento* evento = crearPaqueteClick(posicion_mouse_click, escalador, id_cliente);
		//cliente.enviar(evento, sizeof(structEvento));
		keyboard(event, posicion_mouse_movimiento, posicion_mouse_click, posicion_mouse_scroll);
		dibujador->dibujarFondo(agua);
		filledEllipseRGBA( gRenderer, 90 , 90,5, 5, 0, 0, 0, CIRC_OPACIDAD);
	}
	//cliente.desconectar()
*/
	delete agua;
	delete escalador;
	delete dibujador;
	delete paquete;
	free(posicion_mouse_click);
	free(posicion_mouse_scroll);
	free(posicion_mouse_movimiento);
	close();

	//delete cliente;
	return 0;
}


#endif /* MAIN_CLIENT_H_ */
