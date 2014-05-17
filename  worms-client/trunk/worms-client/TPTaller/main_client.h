#ifndef MAIN_CLIENT_H_
#define MAIN_CLIENT_H_

#include "Juego.h"
#include "Funciones.h"
#include "Cliente.h"
#include "Constantes.h"
#include "Paquete.h"

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

	while (!cliente->getComenzar()){
		printf(" Todavia no cmienza \n");
	}



	//char mensaje[sizeof(structInicial)];
	//cliente.recibir(mensaje, sizeof(mensaje) );
	//structInicial* paqueteInicial = new structInicial;
	//memcpy(paqueteInicial, mensaje, sizeof(structInicial));
	structInicial* paqueteInicial = cliente->getPaqueteInicial();
	printf (" EL ANCHO DEL ESCENARIO1 ES :  %f \n", paqueteInicial->ancho_escenario );

	Escalador* escalador = new Escalador(paqueteInicial->ancho_ventana , paqueteInicial->alto_ventana,
			paqueteInicial->ancho_unidades, paqueteInicial->alto_unidades,
			paqueteInicial->ancho_escenario,paqueteInicial->alto_escenario);
	crearVentana(escalador);
	/*
	Dibujador* dibujador = new Dibujador(gRenderer, escalador);

	SDL_Event event;
	for(int i = 0; i < 322; i++) { // inicializa todas en falso
	   KEYS[i] = false;
	}
	Agua* agua = new Agua(paqueteInicial->nivel_agua, paqueteInicial->agua);
	dibujador->iniciarFondo(agua, paqueteInicial->cielo, paqueteInicial->tierra);
	delete paqueteInicial;

	int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*2);
	int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
	posicion_mouse_click[0] = -1;
	posicion_mouse_click[1] = -1;

	structPaquete* paquete = new structPaquete;
	int id_cliente=1;
	while (KEYS[SDLK_ESCAPE] == false){
		//structPaquete = cliente.recibir(sizeof(structPaquete),paquete);
		dibujador->dibujarPaquete(paquete);
		keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
		escalador->moverVentana(posicion_mouse_movimiento);
		escalador->hacerZoom(posicion_mouse_scroll);
		structEvento* evento = crearPaqueteClick(posicion_mouse_click, escalador, id_cliente);
		//cliente.enviar(evento, sizeof(structEvento));
		keyboard(event, posicion_mouse_movimiento, posicion_mouse_click, posicion_mouse_scroll);
	}
	//cliente.desconectar()
	delete agua;
	delete escalador;
	delete dibujador;
	delete paquete;
	free(posicion_mouse_click);
	free(posicion_mouse_scroll);
	free(posicion_mouse_movimiento);


	close();

*/
	SDL_Delay(10000);

	//delete cliente;
	return 0;
}


#endif /* MAIN_CLIENT_H_ */