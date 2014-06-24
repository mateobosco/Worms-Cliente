#include "Paquete.h"

//# define M_PI 3.1415

structFigura* crearPaqueteFigura(Figura* figura){
	structFigura* paquete = new structFigura;
	int tipo = figura->tipo;
	paquete->color= figura->getColor();

	if (tipo == 0){ //ES UN CIRCULO
		Circulo* circulo = (Circulo*) figura;
		paquete->vector_vertices[0] = circulo->obtenerPosicion();
		paquete->vector_vertices[1].x = circulo->getRadio();
		paquete->vector_vertices[1].y = circulo->getRadio();
		paquete->cantidad = 2;
	}
	if (tipo == 1){ //ES UN RECTANGULO
		Rectangulo* rectangulo = (Rectangulo*) figura;
		for (int i = 0 ; i<4; i++){
			paquete->vector_vertices[i] = rectangulo->getVertices()[i];
		}
		paquete->cantidad = 4;

	}
	if (tipo == 2){ //ES UN POLIGONO
		Poligono* poligono = (Poligono*) figura;
		int lados = poligono->getCantVertices();
		paquete->cantidad = lados;
		for (int i = 0 ; i<lados; i++){
			paquete->vector_vertices[i] = poligono->getVertices()[i];
		}
	}
	return paquete;
}

structPersonaje* crearPaquetePersonaje(Personaje* personaje){
	structPersonaje* paquete = new structPersonaje;
	b2Vec2 pos = personaje->getPosition();
//	int dir;
//	b2Body* body = personaje->getBody();
//	if (body->GetLinearVelocity().x > 0.5 ){
//		dir = 1;
//	}
//	else{
//		dir = -1;
//	}
	paquete->direccion = personaje->getOrientacion();
	paquete->posicion = pos;
	paquete->tamano.x = personaje->getAncho();
	paquete->tamano.y = personaje->getAlto();
	paquete->conectado = personaje->getConectado();
	paquete->id_jugador = personaje->getNrojugador();
	strcpy(paquete->nombre_cliente, personaje->getNombreCliente());
	bool* seleccion = personaje->getSeleccion();
	for (int i = 0 ; i<4; i++){
		if (seleccion[i]){
			paquete->seleccionado[i]=1;
		}
		if (!seleccion[i]){
			paquete->seleccionado[i]=0;
		}
	}
	return paquete;
}

void destruirPaqueteFigura(structFigura* paquete){
	delete paquete;
}

void destruirPaquetePersonaje(structPersonaje* paquete){
	delete paquete;
}

bool checkDisparando(structEvento *paquete, bool *KEYS, bool &disparando, bool &disparar){
	if(!KEYS[SDLK_v]){
		if(disparando){
			disparando = false;
			paquete->fuerza = 2;
			disparar = true;
			return true;
		} else{
			paquete->fuerza = 0;
			disparar = false;
		}
	}
	return false;
}

structEvento* crearPaqueteClick(int* click, Escalador* escalador, int cliente, bool *KEYS, bool &disparando, bool &disparar){
	structEvento* paquete = new structEvento;
	b2Vec2 posicion(click[0],click[1]);
	b2Vec2 posicionEscalada = escalador->escalarPosicion(posicion);
	paquete->click_mouse = posicionEscalada;
	paquete->direccion = -9; //DIRECION NO VALIDA
	paquete->nro_jugador = cliente;
	paquete->aleatorio = random();
	checkDisparando(paquete, KEYS, disparando,disparar);
	return paquete;
}

structEvento* crearPaqueteMovimiento(bool* KEYS, int id_jugador, bool &disparando, bool &disparar){
	structEvento* paquete = new structEvento;

	if ((KEYS[102] || KEYS[SDLK_SPACE])){ // para arriba
		paquete->direccion = 2;
	}
	if (KEYS[100]){ // para la derecha
		if(KEYS[102]) paquete->direccion=4;
		else{
			paquete->direccion = 3;
		}
	}
	if (KEYS[101]){ // para la izquierda
		if(KEYS[102]) paquete->direccion=5;
		else{
			paquete->direccion = 1;
		}
	}

	if(KEYS[SDLK_x]){
//		printf(" LE MANDO EN EL PAQUETE EL ANGULO con 5 \n");
		if((paquete->direccion == 3) || (paquete->direccion == 4))
			paquete->angulo_arma = 5;
		else
			paquete->angulo_arma -= 5;
	}
	if(KEYS[SDLK_c]){
		if((paquete->direccion == 3) || (paquete->direccion == 4))
			paquete->angulo_arma -= 5;
		else
			paquete->angulo_arma = 5;
	}
	if(KEYS[SDLK_v]){
		disparando = true;
		paquete->fuerza = 1;
	}
	checkDisparando(paquete, KEYS, disparando, disparar);
	paquete->nro_jugador = id_jugador;
	paquete->click_mouse = b2Vec2( -1, -1 );
	paquete->aleatorio = random();
	return paquete;
}

structEvento* crearPaqueteVacio(bool *KEYS, bool &disparando, bool &disparar){
	structEvento* paquete = new structEvento;
	paquete->click_mouse = b2Vec2 (-1,-1);
	paquete->direccion = -9;
	paquete->nro_jugador = MAX_CANT_JUGADORES;
	paquete->aleatorio = random();
	bool termino_disparando = checkDisparando(paquete, KEYS, disparando, disparar);
	if(termino_disparando)
		paquete->nro_jugador = -1;
	else
		paquete->nro_jugador = MAX_CANT_JUGADORES;
	return paquete;
}

bool aceptarNuevaTecla(timeval act_time, timeval key_pressed_time){
	return ((((act_time.tv_sec - key_pressed_time.tv_sec)*1000000L
			           + act_time.tv_usec) - key_pressed_time.tv_usec) > 150000);
}

structEvento* crearPaqueteEvento(int* click, bool* KEYS, Escalador* escalador, int cliente, timeval &ultima_vez, bool &disparando, bool &disparar, int& arma, Musica* music, int cantidad_granadas, int cantidad_dinamitas, int cantidad_holys){
	structEvento* paquete;
	//int arma = 0;
	if ( KEYS[100] || KEYS[101] || KEYS[102] || KEYS[SDLK_SPACE] || KEYS[SDLK_x] || KEYS[SDLK_c] || KEYS[SDLK_v]){ // no es un click, es un movimiento
		timeval tiempo_actual;
//		printf(" CREA UN PAQUETE MOVIMIENTO \n");
		gettimeofday(&tiempo_actual, 0x0);
		if(aceptarNuevaTecla(tiempo_actual, ultima_vez)){
			paquete = crearPaqueteMovimiento(KEYS, cliente, disparando, disparar);
			gettimeofday(&ultima_vez, 0x0);
		} else{
			paquete = crearPaqueteVacio(KEYS, disparando,disparar);
		}

	} else{
		if (click[0] != -1){
			paquete = crearPaqueteClick(click, escalador, cliente, KEYS, disparando,disparar);
//			printf(" CLICK 0 es: %d, CLICK 1 es : %d \n", click[0], click[1]);
			if(KEYS[SDLK_z]){
				if (click[0] > 600 &&  click[0] < 700 && click[1] > 100 && click[1] < 200){
//					printf(" SELECCIONA UN BAZOOKA\n");
					paquete->arma_seleccionada=1;
					arma = LANZAR_BAZOOKA;

				}
				else if (click[0] > 700  && click[1] > 100 && click[1] <200 && cantidad_granadas > 0){
					paquete->arma_seleccionada=2;
					arma = LANZAR;
				}
				else if (click[0] > 600 &&  click[0] < 700 && click[1] > 200 && click[1] < 300 && cantidad_dinamitas > 0){
					paquete->arma_seleccionada=3;
					arma = LANZAR_DINAMITA;
				}
				else if (click[0] > 700 && click[1] > 200 && click[1] < 300 && cantidad_holys > 0 ){
					paquete->arma_seleccionada=4;
					arma = LANZAR;
				}
				else if (click[0] > 600  && click[0] < 700 && click[1] > 300 && click[1] < 400){
					paquete->arma_seleccionada=5;
					arma = KAMIKAZE;
				}
				else if (click[0] > 700 && click[1] > 300 && click[1] <400){
					paquete->arma_seleccionada=6;
					arma = PATADA;
				}
				music->playSonido(SELECT);
			}
		}
		else {
			paquete = crearPaqueteVacio(KEYS, disparando,disparar);
		}
	}
//	if(disparando){
//		musica->playSonido(arma);
//	}
	if(!paquete){
		paquete = crearPaqueteVacio(KEYS, disparando,disparar);
		if(paquete->nro_jugador == -1){
			delete paquete;
			paquete = NULL;
		}
	}
	return paquete;
}

structPaquete* crearPaqueteCiclo(Mundo* mundo, char* mensaje, int nro_jugador){
	structPaquete* paquete = (structPaquete*) malloc(MAX_PACK);
	paquete->tipo = 1;

	Personaje** vector_personajes = mundo->getPersonajes();
	int cantidad_personajes = mundo->getCantidadPersonajes();
	for (int i=0 ; i<cantidad_personajes; i++){
		structPersonaje* paquetito = crearPaquetePersonaje(vector_personajes[i]);
		memcpy(&paquete->vector_personajes[i], paquetito, sizeof(structPersonaje));
		destruirPaquetePersonaje(paquetito);
	}
	Figura** vector_figuras = mundo->getFiguras();
	int cantidad_figuras = mundo->getCantidadFiguras();
	for (int i=0 ; i<cantidad_figuras; i++){
		structFigura* paquetito = crearPaqueteFigura(vector_figuras[i]);
		memcpy(&paquete->vector_figuras[i],paquetito,sizeof(structFigura));
		destruirPaqueteFigura(paquetito);
	}
	paquete->cantidad_figuras = cantidad_figuras;
	paquete->cantidad_personajes = cantidad_personajes;
	if (mensaje != NULL){
		strcpy(paquete->mensaje_mostrar, mensaje);
	}
	return paquete;
}

void destruirPaqueteCiclo(structPaquete* paquete){
	free(paquete);
}

bool estaVacio(structEvento* paquete){
	return (paquete->nro_jugador == MAX_CANT_JUGADORES);
}
