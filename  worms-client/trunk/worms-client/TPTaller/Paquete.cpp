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

structEvento* crearPaqueteClick(int* click, Escalador* escalador, int cliente){
	structEvento* paquete = new structEvento;
	b2Vec2 posicion(click[0],click[1]);
	b2Vec2 posicionEscalada = escalador->escalarPosicion(posicion);
	paquete->click_mouse = posicionEscalada;
	paquete->direccion = -9; //DIRECION NO VALIDA
	paquete->nro_jugador = cliente;
	paquete->aleatorio = random();
	return paquete;
}

structEvento* crearPaqueteMovimiento(bool* KEYS, int id_jugador){
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
		printf(" LE MANDO EN EL PAQUETE EL ANGULO con 5 \n");
		paquete->angulo_arma=5;
	}
	if(KEYS[SDLK_c]){
		paquete->angulo_arma=-5;
	}
	paquete->nro_jugador = id_jugador;
	paquete->click_mouse = b2Vec2( -1, -1 );
	paquete->aleatorio = random();
	return paquete;
}

structEvento* crearPaqueteVacio(){
	structEvento* paquete = new structEvento;
	paquete->click_mouse = b2Vec2 (-1,-1);
	paquete->direccion = -9;
	paquete->nro_jugador = MAX_CANT_JUGADORES;
	paquete->aleatorio = random();
	return paquete;
}

bool aceptarNuevaTecla(timeval act_time, timeval key_pressed_time){
	return ((((act_time.tv_sec - key_pressed_time.tv_sec)*1000000L
			           + act_time.tv_usec) - key_pressed_time.tv_usec) > 150000);
}

structEvento* crearPaqueteEvento(int* click, bool* KEYS, Escalador* escalador, int cliente, timeval &ultima_vez){
	structEvento* paquete;
	if ( KEYS[100] || KEYS[101] || KEYS[102] || KEYS[SDLK_SPACE] || KEYS[SDLK_x] || KEYS[SDLK_c]){ // no es un click, es un movimiento
		timeval tiempo_actual;
		gettimeofday(&tiempo_actual, 0x0);
		if(aceptarNuevaTecla(tiempo_actual, ultima_vez)){
			paquete = crearPaqueteMovimiento(KEYS, cliente);
			gettimeofday(&ultima_vez, 0x0);
		} else{
			return crearPaqueteVacio();
		}

	} else{
		if (click[0] != -1){
			paquete = crearPaqueteClick(click, escalador, cliente);
			printf(" LLEGA HSTA ACA \n");
			int ventanadoX = escalador->ventanarEnX(paquete->click_mouse.x);
			int ventanadoY = escalador->ventanarEnY(paquete->click_mouse.y);
			//printf(" MANDO UN MAQUETE CLICK con las posiciones : (%d, %d) \n", click[0], click[1]);
			//printf(" Los ventanado son %d , %d \n", ventanadoX, ventanadoY);
			if (click[0] > 600 &&  click[0] < 700 && click[1] > 100 && click[1] < 200){
				printf(" selecciona una bazooka \n");
				paquete->arma_seleccionada=1;
			}
			else if (click[0] > 700  && click[1] > 100 && click[1] <200){
				printf(" selecciona una granada \n");
				paquete->arma_seleccionada=2;
			}
			else if (click[0] > 600 &&  click[0] < 700 && click[1] > 200 && click[1] < 300){
				printf(" Selecciona una dinamita \n");
				paquete->arma_seleccionada=3;
			}
			else if (click[0] > 700 && click[1] > 200 && click[1] < 300){
				printf(" Selecciona una holy \n");
				paquete->arma_seleccionada=4;
			}
			else if (click[0] > 600  && click[0] < 700 && click[1] > 300 && click[1] < 400){
				printf(" Selecciona kamikaze \n");
				paquete->arma_seleccionada=5;
			}
			else if (click[0] > 700 && click[1] > 300 && click[1] <400){
				printf(" Selecciona patada \n");
				paquete->arma_seleccionada=6;
			}



		}

		else {
			paquete = crearPaqueteVacio();
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
