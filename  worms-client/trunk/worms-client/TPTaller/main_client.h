#ifndef MAIN_CLIENT_H_
#define MAIN_CLIENT_H_

#include "Juego.h"
#include "Funciones.h"
#include "Cliente.h"
#include "Constantes.h"
#include "Paquete.h"
#include <string>

//structEvento* crearPaqueteClick(int* click, Escalador* escalador, int cliente);// sino me tira error de que no esta definida aunque este el include



// argv[n]:
// 			n=0: Nombre del programa
// 			n=1: Nombre del cliente/jugador
// 			n=2: IP
// 			n=3: Puerto
int mainCliente(int argc, char* argv[]){
	int id = -1;

	if(checkCantParametros(argc) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	char* name = new char[MAX_NAME_USER];
	memset(name,0,MAX_NAME_USER);
	char* nombre = argv[POS_NAME_USER]; //inicializar todo
	strcpy(name,nombre);

	//const char* ip_sv = argv[POS_IP]; //inicializar todo
	char* ip_sv = new char[20];
	memset(ip_sv,0,20);
	char* ip = argv[POS_IP]; //inicializar todo
	strcpy(ip_sv,ip);

	char* puerto = new char[10];
	memset(puerto,0,10);
	char* port = argv[POS_PORT];
	strcpy(puerto,port);
	//const char* puerto = argv[POS_PORT]; //inicializar todo

	Cliente* cliente = new Cliente(name, ip_sv, puerto);

	if(cliente->conectar() != EXIT_SUCCESS){
		return EXIT_FAILURE;
	}

	while (!cliente->getPaqueteInicial()){
	}

	structInicial* paqueteInicial = (structInicial*) cliente->getPaqueteInicial();

	Escalador* escalador = new Escalador(paqueteInicial->ancho_ventana , paqueteInicial->alto_ventana,
			paqueteInicial->ancho_unidades, paqueteInicial->alto_unidades,
			paqueteInicial->ancho_escenario,paqueteInicial->alto_escenario);

	Dibujador* dibujador =new Dibujador(NULL, escalador);
	dibujador->init();
	SDL_Event event;
	for(int i = 0; i < 322; i++) { // inicializa todas en falso
	   KEYS[i] = false;
	}
	string pathAgua = string(paqueteInicial->agua);
	string pathTierra = string(paqueteInicial->tierra);
	string pathCielo = string(paqueteInicial->cielo);
	Agua* agua = new Agua(paqueteInicial->nivel_agua, pathAgua);


	dibujador->iniciarFondo(agua, pathCielo, pathTierra);
	dibujador->dibujarFondo(agua);

	SDL_Delay(5000);


	//delete paqueteInicial;

	int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
	memset(posicion_mouse_click,'\0',2);
	int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*3);
	memset(posicion_mouse_scroll,'\0',3);
	int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
	memset(posicion_mouse_movimiento,'\0',2);
	posicion_mouse_click[0] = 0;
	posicion_mouse_click[1] = 0;

	//char* paquete_devuelto;
	structPaquete* paquete;
	while(KEYS[SDLK_ESCAPE] == false){

		dibujador->dibujarFondo(agua);
		keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
		escalador->moverVentana(posicion_mouse_movimiento);
		escalador->hacerZoom(posicion_mouse_scroll);
		paquete = (structPaquete*) cliente->getPaquete();
		cliente->setID(paquete->id);
		keyboard(event, posicion_mouse_movimiento, posicion_mouse_click, posicion_mouse_scroll);
		structEvento* evento = crearPaqueteEvento(posicion_mouse_click, KEYS, escalador, cliente->getID() );
		if (evento){
			//printf(" ----- PAQUETE LISTOOO ---------- \n");
			cliente->actualizarPaquete(evento);
		}
		if(evento){
			delete evento;
		}
		dibujador->dibujarPaquete(paquete, cliente->getNombre(), cliente->getID());
		dibujador->actualizar();
		posicion_mouse_scroll[2] = 0;
		delete[] paquete;
	}

	delete paqueteInicial; //ver si hay que hacer casteo a char*
	delete agua;
	delete[] name;
	delete[] ip_sv;
	delete[] puerto;
	//delete paquete;
	free(posicion_mouse_click);
	free(posicion_mouse_scroll);
	free(posicion_mouse_movimiento);
	delete dibujador;

	//delete cliente;
	return 0;
}

#endif /* MAIN_CLIENT_H_ */
