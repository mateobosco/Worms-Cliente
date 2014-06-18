#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <SDL2/SDL_thread.h>
#include "Socket.h"
#include "Paquete.h"
#include "Jugador.h"
#include <queue>


extern ofstream logFile;
extern void loguear();

#define MAX_DATOS 100 // max number of bytes we can get at once

typedef struct communicatedThreads{
	SDL_Thread *enviar;
	SDL_Thread *recibir;
}comThreads;

class Cliente{
	private:
		char* name_client;
		Socket* socket_cl;
		SDL_mutex *mutex;
		char paquete_recibir[MAX_PACK];
		char paquete_enviar[MAX_PACK];
		int id;
		static int cant_clientes;
		structInicial* paqueteInicial;
		bool conectado;
		bool enviarpaquete;
		bool activo;
		bool servidor_conectado;
		comThreads hilos;
		Jugador* jugador;
		Dibujador* dibujador;




	public:
		Cliente(const char *name, const char *ip_sv, const char* puerto);
		Cliente(int fd);
		~Cliente();
		int conectar();
		int enviar(char *mensaje, size_t longData);
		int runRecibirInfo();
		int run();
		int runEnviarInfo();
		Socket* getSocket();
		int getID();
		void setID(int un_id);
		char* getPaquete();
		structInicial* getPaqueteInicial();
		void setConexion(bool estado);
		void actualizarPaquete(structEvento* evento);

		char* getNombre();
		void setNombre(char *name);
		int enviarNombre();
		void activar();
		void desactivar();
		bool getActivo();
		void setDibujador(Dibujador* dibujador);
		void dibujarMensajeDesconexion();
		bool getServidorConectado();
		void setServidorConectado(bool estado);
		Jugador* getJugador();

		void setHilos(comThreads hilos_server);
		comThreads getHilos();
		queue<structPaquete*> cola_explosiones;
		int recibirConfiguracion();
		void desencolarExplosion();
		SDL_mutex* getMutex();
		void setJugador(Jugador* jug);
		int getContadorCerrarse();
		size_t getTamanioColaExplosiones();
		structPaquete* getPaqueteColaExplosiones();
		bool resetearNivel;

};

#endif /* CLIENTE_H_ */
