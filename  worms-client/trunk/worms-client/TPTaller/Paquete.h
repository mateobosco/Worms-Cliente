#ifndef PAQUETE_H_
#define PAQUETE_H_

#include "Personaje.h"
#include "figura.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"
#include "Musica.h"
#include <sys/time.h>

typedef struct structFigura {
	b2Vec2 vector_vertices[6] ;
	int cantidad = 0;
	SDL_Color color;
} structFigura;

typedef struct structExplosion{
	b2Vec2 posicion = b2Vec2(-1,-1);
	float32 radio = 0;
}structExplosion;

typedef struct structPersonaje {
	char nombre_cliente[20];
	int id_jugador;
	int seleccionado[4];
	int conectado;
	b2Vec2 posicion;
	b2Vec2 tamano;
	int direccion; // 0 derecha, 1 izquierda
	int energia;
	int arma_seleccionada =-1;
	int angulo_arma;
	int perdioVida = 0;
	int movio = 0;
	int salto = 0;
}structPersonaje;

typedef struct structPaquete{
	int comenzar;
	int tipo = 0;
//	structFigura vector_figuras[10];
//	int cantidad_figuras = 0;
	structPersonaje vector_personajes[20];
	int cantidad_personajes = 0;
	int id;
	char mensaje_mostrar[MAX_MENSAJE];
	int turno_jugador;
	int reloj;
	char nombre_jugador_actual[MAX_MENSAJE];

	int tipo_proyectil = 0;
	bool show_proyectil = false;
	b2Vec2 posicion_proyectil;
	b2Vec2 direccion_proyectil;
	b2Vec2 tamanio_proyectil;
	int radio_explosion = -1;
	int contador_segundos = 0;
	int potencia = -1;
	double angulo = 0;
	char ganador[LONGITUD_WINNERS];
	int resultado = -1; //-1 == no terminó, 0 == empate, 1 == hay un ganador
	int cant_ganadores = 0; //0 == no hay ganadores, 1 == un ganador, >1 == empate de cant_ganadores
	bool resetear = false;
}structPaquete;

typedef struct structInicial{
	bool cliente_aceptado = false;
	int tipo = 0;
	char tierra[50];
	char cielo[50];
	char agua[50];
	int ancho_ventana = 0;
	int alto_ventana = 0;
	float32 nivel_agua = 0.0;
	int ancho_unidades = 0;
	int alto_unidades = 0;
	float32 ancho_escenario = 0.0;
	float32 alto_escenario = 0.0;
	float32 viento = 0.0;
	structExplosion explosiones[50];
	int cantidadExplosiones = 0;
} structInicial;

typedef struct structEvento{
	b2Vec2 click_mouse; // posicion escalada del mouse
	int direccion; // -1 izquierda, 0 arriba, 1 derecha //todo
	int nro_jugador = 0; //Identificador del cliente
	int aleatorio = 0;
	int arma_seleccionada = 0;
	int angulo_arma = 0;
	int fuerza = 0;

} structEvento;

structFigura* crearPaqueteFigura(Figura* figura);
structPersonaje* crearPaquetePersonaje(Personaje* personaje, int nro_arma);
structPaquete* crearPaqueteCiclo(Mundo* mundo, char* mensaje, int turno_jugador, int empezar_juego, int tiempo_ronda, char* nombre, char* winner, int cant_winners, bool resetear);
structEvento* crearPaqueteEvento(int* click, bool* KEYS, Escalador* escalador, int cliente, timeval &ultima_vez, bool &disparando, bool &disparar, int& arma, Musica* music, int cantidad_granadas, int cantidad_dinamitas, int cantidad_holys);

void destruirPaqueteFigura(structFigura* paquete);
void destruirPaquetePersonaje(structPersonaje* paquete);
void destruirPaqueteCiclo(structPaquete* paquete);
bool estaVacio(structEvento* paquete);
void resetBoolPersonaje(Personaje* personaje);
bool contieneSonido(structPaquete* paqueteCiclo);
bool paqueteValido(structPaquete* paquete);

#endif /* PAQUETE_H_ */
