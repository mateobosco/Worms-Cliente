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
	Escalador* escalador_inicio = new Escalador(800,600, 80,60,800,600);
	Dibujador* dibujador_inicio = new Dibujador(NULL, escalador_inicio);
	dibujador_inicio->init();
	Musica* musica_inicio = new Musica();
	musica_inicio->playSonido(START);
	dibujador_inicio->mostrarImagenPrincipal();

	char nombre[MAX_NAME_USER];
	strcpy(nombre, dibujador_inicio->mostrarPantallaPrincial());

	printf(" NOBMRE QUEDO COMO %s \n", nombre);
	dibujador_inicio->mostrarImagenPrincipal2();
	char* ip_sv = new char[20];
	memset(ip_sv,0,20);
	char* ip = argv[POS_IP];
	strcpy(ip_sv,ip);

	char* puerto = new char[10];
	memset(puerto,0,10);
	char* port = argv[POS_PORT];
	strcpy(puerto,port);

	Cliente* cliente = new Cliente(nombre, ip_sv, puerto);
	delete dibujador_inicio;
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
		float32 viento = paqueteInicial->viento;
		Dibujador* dibujador = new Dibujador(NULL, escalador);
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
		dibujador->aplicarExplosiones(paqueteInicial);
		dibujador->dibujarFondo();
//		int contador_aceptar=0;
//		while(contador_aceptar<2000){
//			dibujador->dibujarLoading();
//			contador_aceptar++;
//		}
		//SDL_Delay(2000);
		int* posicion_mouse_click = (int*)malloc (sizeof(int)*2);
		memset(posicion_mouse_click,'\0',2);
		int* posicion_mouse_scroll = (int*)malloc (sizeof(int)*3);
		memset(posicion_mouse_scroll,'\0',3);
		int* posicion_mouse_movimiento = (int*)malloc (sizeof(int)*2);
		memset(posicion_mouse_movimiento,'\0',2);
		posicion_mouse_click[0] = -1;
		posicion_mouse_click[1] = -1;
		posicion_mouse_movimiento[0] = 0;//para q valgrind no joda
		posicion_mouse_movimiento[1] = 0;//para q valgrind no joda
		posicion_mouse_scroll[0] = 0;//para q valgrind no joda
		posicion_mouse_scroll[1] = 0;//para q valgrind no joda
		posicion_mouse_scroll[2] = 0;//para q valgrind no joda
		structPaquete* paquete;
		float aux2=0;

		timeval ultima_vez;
		gettimeofday(&ultima_vez, 0x0);
 		bool disparar = false;
		int arma  = 0;
		bool disparando = false;
		while(KEYS[SDLK_ESCAPE] == false){

			posicion_mouse_click[0] = -1;
			posicion_mouse_click[1] = -1;

			keyboard(event, posicion_mouse_movimiento,posicion_mouse_click,posicion_mouse_scroll);
			if(!KEYS[SDLK_z]){
				escalador->moverVentana(posicion_mouse_movimiento);
			}
			dibujador->dibujarFondo();
			escalador->hacerZoom(posicion_mouse_scroll);
			paquete = (structPaquete*) cliente->getPaquete();
			if(paquete->cantidad_personajes==0){
				dibujador->mostrarLoading();
			}
			if(paquete->show_proyectil && paquete->tipo_proyectil != 3)	bloquearMovimientos();

			if((KEYS[102]==true)){ //agregar condicion cuando esta seleccionado
				music->playSonido(UP);
			}
			cliente->setID(paquete->id);


			structEvento* evento = crearPaqueteEvento(posicion_mouse_click, KEYS, escalador, cliente->getID(), ultima_vez, disparando,disparar,arma, music);

//			structEvento* evento = crearPaqueteEvento(posicion_mouse_click, KEYS, escalador, cliente->getID(), ultima_vez, disparando);
			if ((evento->direccion == 3)||(evento->direccion == 1)) music->playSonido(WALK);
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
				music->playMusic();
				if (paquete->turno_jugador == cliente->getID()){
					strcpy(mensaje, "Es tu turno");
					dibujador->mostrarCartel(mensaje, 300 ,0,300, 50);
				}
				else{
					dibujador->mostrarCartelTurno(paquete->turno_jugador, paquete->nombre_jugador_actual);

				}
				if((paquete->reloj >= 50*1000) && (paquete->reloj < 60*1000)){
//					music->stopMusic(); todo para mí queda mejor que la música siga sonando [Nahue]
					music->playSonido(TIME);
				}
			}
			else{
				strcpy(mensaje, "Esperando a que se conecten jugadores");
				dibujador->mostrarCartel(mensaje, -1, 0, 500,50);
			}
			//Cuando termina partida. La idea es que cuando haya un ganador se pueda resetear el nivel o pasar al siguiente
			if(paquete->cantidad_personajes == 1 ){
				printf("Cantidad de personajes = 1 \n");
				char mensaje[90];
				strcpy(mensaje, "Partida finalizada. Presione R para volver a empezar.");
				dibujador->mostrarCartel(mensaje, 30 ,70,600, 50);

			}
			if(KEYS[SDLK_z]){
				dibujador->mostrarMenuArmas(escalador->getVentanaX()-100,100);
			}
			dibujador->mostrarReloj(paquete->reloj);
			dibujador->dibujarViento(viento);

			bool play_explotar = false;
			if(cliente->getTamanioColaExplosiones() >=1){
				structPaquete* paquetecola = cliente->getPaqueteColaExplosiones();
				if(paquetecola->posicion_proyectil.y > 0){
					if (paquetecola->tipo_proyectil != 6) dibujador->borrarExplosion(paquetecola->posicion_proyectil, paquetecola->radio_explosion);
					dibujador->setPosicionExplosion(paquetecola->posicion_proyectil, paquetecola->radio_explosion);
				}
				play_explotar = true;
				cliente->desencolarExplosion();
//				free(paquetecola);
			}
			if(disparar) {
//				printf("Disparar...arma seleccionada %d\n", arma);
				music->playSonido(arma);
//				if(paquete->tipo_proyectil == 1) music->playSonido(DISPARO);
//				else if((paquete->tipo_proyectil == 2)&&(paquete->tipo_proyectil == 3)&&(paquete->tipo_proyectil == 4)){
//					music->playSonido(LANZAR);
//				}
			}
			if(dibujador->dibujar_explosion() == true){
				if (paquete->tipo_proyectil != 0) dibujador->tipo_explosion = paquete->tipo_proyectil;
				if (dibujador->tipo_explosion == 4){
					dibujador->dibujarExplosion(4);
				}
				if (dibujador->tipo_explosion != 4) {
					dibujador->dibujarExplosion(dibujador->tipo_explosion);
				}
				if (play_explotar) {
					//Ver cual arma está seleccionada y según que arma es el sonido
					if(paquete->tipo_proyectil == 1) music->playSonido(EXPLOSION_BAZOOKA);
					else if(paquete->tipo_proyectil == 2) music->playSonido(EXPLOSION_GRANADA);
					else if(paquete->tipo_proyectil == 3) music->playSonido(EXPLOSION_DINAMITA);
					else if(paquete->tipo_proyectil == 4) music->playSonido(EXPLOSION_HOLY);
					else if(paquete->tipo_proyectil == 5) music->playSonido(KAMIKAZE);
					else if(paquete->tipo_proyectil == 6) music->playSonido(PATADA);
					//music->playSonido(EXPLOSION);
					play_explotar = false;
				}
			}

			if(((structPaquete*) cliente->getPaquete())->ganador[0] != '\0'){
				char mensaje_ganador[200];
				if(((structPaquete*) cliente->getPaquete())->resultado == 0){
					int cant_ganadores = ((structPaquete*) cliente->getPaquete())->cant_ganadores;
					sprintf(mensaje_ganador, "Empate entre:");
					while(cant_ganadores > 0){
						sprintf(mensaje_ganador, " %s,", ((structPaquete*) cliente->getPaquete())->ganador);
						sprintf(mensaje_ganador, ",");
					}
					mensaje_ganador[strlen(mensaje_ganador) - 1] = '\0';
					dibujador->mostrarCartel(mensaje_ganador, 250, 250, 300, 100);

				}
				sprintf(mensaje_ganador, "El ganador es: %s", ((structPaquete*) cliente->getPaquete())->ganador);
				dibujador->mostrarCartel(mensaje_ganador, 250, 250, 300, 100);
			}


			if (cliente->resetearNivel){
				char mensaje[100];
				strcpy(mensaje,"reseteando el nivel hijo de puta");
				dibujador->mostrarCartel(mensaje, 250,250,300,100);
//				SDL_Delay(3000);
				dibujador->resetearEscenario(pathTierra);
				cliente->resetearNivel = false;
			}
			dibujador->actualizar(); //todo si todos los personajes mueren queda trabado aca

			posicion_mouse_scroll[2] = 0;
			delete[] paquete;
		}
		if(KEYS[SDLK_ESCAPE] == true){
			music->stopMusic();
			music->playSonido(BYE);
		}
		SDL_Delay(1500);
		delete music;
		delete paqueteInicial;
		delete agua;
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
