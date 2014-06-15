#ifndef MAIN_CLIENT_H_
#define MAIN_CLIENT_H_

#include "Juego.h"
#include "Funciones.h"
#include "Cliente.h"
#include "Constantes.h"
#include "Paquete.h"
#include <string>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "Musica.h"

extern void abrirLog();

// argv[n]:
// 			n=0: Nombre del programa
// 			n=1: Nombre del cliente/jugador
// 			n=2: IP
// 			n=3: Puerto
int mainCliente(int argc, char* argv[]){
	abrirLog();

	if(checkCantParametros(argc) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	char* name = new char[MAX_NAME_USER];
	memset(name,0,MAX_NAME_USER);
	char* nombre = argv[POS_NAME_USER];
	strcpy(name,nombre);

	char* ip_sv = new char[20];
	memset(ip_sv,0,20);
	char* ip = argv[POS_IP];
	strcpy(ip_sv,ip);

	char* puerto = new char[10];
	memset(puerto,0,10);
	char* port = argv[POS_PORT];
	strcpy(puerto,port);

	Cliente* cliente = new Cliente(name, ip_sv, puerto);

	if(cliente->conectar() != EXIT_SUCCESS){
		printf("No se ha podido realizar la conexion\n"
				"El programa se cerrará en 5 segundos.\n");
		SDL_Delay(5000);
		return EXIT_FAILURE;
	}

	while (!cliente->getPaqueteInicial());

	structInicial* paqueteInicial = (structInicial*) cliente->getPaqueteInicial();
	if(paqueteInicial->cliente_aceptado){
		Escalador* escalador = new Escalador(paqueteInicial->ancho_ventana , paqueteInicial->alto_ventana,
				paqueteInicial->ancho_unidades, paqueteInicial->alto_unidades,
				paqueteInicial->ancho_escenario,paqueteInicial->alto_escenario);
		Dibujador* dibujador =new Dibujador(NULL, escalador);
		dibujador->init();
		Musica* music = new Musica();
		dibujador->setMusica(music);
		SDL_Event event;
		for(int i = 0; i < 322; i++) { // inicializa todas en falso
		   KEYS[i] = false;
		}

		cliente->setDibujador(dibujador);
		string pathAgua = string(paqueteInicial->agua);
		string pathTierra = string(paqueteInicial->tierra);
		string pathCielo = string(paqueteInicial->cielo);
		Agua* agua = new Agua(paqueteInicial->nivel_agua, pathAgua);
		dibujador->iniciarFondo(agua, pathCielo, pathTierra);
		dibujador->dibujarFondo();
		SDL_Delay(2000);
		int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
		memset(posicion_mouse_click,'\0',2);
		int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*3);
		memset(posicion_mouse_scroll,'\0',3);
		int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
		memset(posicion_mouse_movimiento,'\0',2);
		posicion_mouse_click[0] = -1;
		posicion_mouse_click[1] = -1;

		structPaquete* paquete;
		float aux2=0;

		timeval ultima_vez;
		gettimeofday(&ultima_vez, 0x0);
//		int* aux = (int*)malloc (sizeof(int)*3);
//		aux[2]=1;
//		aux[0]=escalador->getVentanaX()/2;
//		aux[1]=escalador->getVentanaY()/2;
//		for(int i=0; i <20; i++){
//			escalador->hacerZoom(aux);
//		}
//		int aux3 = 0 ;

		bool disparando = false;
		while(KEYS[SDLK_ESCAPE] == false){
			posicion_mouse_click[0] = -1;
			posicion_mouse_click[1] = -1;
			dibujador->dibujarFondo();
			keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
			if(KEYS[102]==true){
				music->playSonido(UP);
			}
			if((posicion_mouse_click[0]!=-1)&&(posicion_mouse_click[1]!=-1)){
				music->playSonido(SELECT);
			}
			escalador->moverVentana(posicion_mouse_movimiento);
			escalador->hacerZoom(posicion_mouse_scroll);
			paquete = (structPaquete*) cliente->getPaquete();
			cliente->setID(paquete->id);
			structEvento* evento = crearPaqueteEvento(posicion_mouse_click, KEYS, escalador, cliente->getID(), ultima_vez, disparando);

//			if(evento->click_mouse.x != -1 && evento->click_mouse.x != 0){
//							printf("entra aca\n");
//							dibujador->borrarExplosion(evento->click_mouse, 3);
//						}
			if ((evento)){
				cliente->actualizarPaquete(evento);
			}
			if(evento){
				delete evento;
			}
			float aux=cos(aux2);
			aux2+=0.1;
			if (aux2==360) aux2=0;
			dibujador->dibujarPaquete(paquete, cliente->getNombre(), cliente->getID(), aux);
			if(!cliente->getServidorConectado()){
				cliente->dibujarMensajeDesconexion();
				if(cliente->getContadorCerrarse() == -1){
					break;
				}
			}
			char mensaje[90];
			dibujador->dibujar_agua(agua);
			if (paquete->comenzar == 1){
				if (paquete->turno_jugador == cliente->getID()){
					strcpy(mensaje, "Es tu turno");
					dibujador->mostrarCartel(mensaje, 300 ,0,300, 50);
				}
				else{
					//printf(" RECIBE EL NOMBRE %s \n", paquete->nombre_jugador_actual);
					dibujador->mostrarCartelTurno(paquete->turno_jugador, paquete->nombre_jugador_actual);

				}
				if(paquete->reloj < 10){
					music->playSonido(TIME);
				}
			}
			else{
				strcpy(mensaje, "Esperando a que se conecten jugadores");
				dibujador->mostrarCartel(mensaje, -1, 0, 500,50);
			}
			if(KEYS[SDLK_z]){
				dibujador->mostrarMenuArmas(escalador->getVentanaX()-100,100);
			}
			dibujador->mostrarReloj(paquete->reloj);



			//printf(" RECIBO %d \n", paquete->radio_explosion);
//			if(paquete->radio_explosion != -1 && paquete->radio_explosion != 0){
//				printf(" ENTRA EN BORRAR EXPLOSION\n");
//				dibujador->borrarExplosion(paquete->posicion_proyectil, paquete->radio_explosion);
//				dibujador->setPosicionExplosion(paquete->posicion_proyectil);
//				//dibujador->dibujarExplosion(paquete->posicion_proyectil, paquete->radio_explosion);
//			}
			if(cliente->getTamanioColaExplosiones() ==1){
				printf(" LO LEE DESDE LA COLA \n");
				structPaquete* paquetecola = cliente->getPaqueteColaExplosiones();
				printf(" //////////// RECIBE como radio %d y posiciones %f ,  %f \n", paquetecola->radio_explosion, paquetecola->posicion_proyectil.x, paquetecola->posicion_proyectil.y);
				dibujador->borrarExplosion(paquetecola->posicion_proyectil, paquetecola->radio_explosion);
				dibujador->setPosicionExplosion(paquetecola->posicion_proyectil);
				cliente->desencolarExplosion();

			}
			if(dibujador->dibujar_explosion() == true){
				printf(" SE ROMPE ACA \n");
				dibujador->dibujarExplosion();

			}


			dibujador->actualizar();
			posicion_mouse_scroll[2] = 0;

			delete[] paquete;
		}

		delete paqueteInicial;
		delete agua;
		delete[] name;
		delete[] ip_sv;
		delete[] puerto;
		free(posicion_mouse_click);
		free(posicion_mouse_scroll);
		free(posicion_mouse_movimiento);
		delete dibujador;
	}
	logFile.close();
	delete cliente;
	return 0;
}

#endif /* MAIN_CLIENT_H_ */
