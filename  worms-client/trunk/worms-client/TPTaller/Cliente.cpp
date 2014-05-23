#include "Cliente.h"

int Cliente::cant_clientes = 0;

Cliente::Cliente(int fd){
	this->name_client = new char[MAX_NAME_USER];
	memset(this->name_client,0,MAX_NAME_USER);
	this->enviarpaquete = false;
	this->socket_cl = new Socket(PUERTO,fd);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
	this->id = Cliente::cant_clientes;
	Cliente::cant_clientes++;
	this->paqueteInicial = new structInicial();
	this->conectado = false;
	this->activo = false;
	this->hilos.enviar = NULL;
	this->hilos.recibir = NULL;
	jugador=NULL;
}



Cliente::Cliente(const char *name, const char *ip_sv, const char *puerto){
	this->name_client = new char[MAX_NAME_USER];
	memset(this->name_client,0,MAX_NAME_USER);
	strncpy(this->name_client, name, MAX_NAME_USER - 1);
	this->socket_cl = new Socket(ip_sv, puerto);
	memset(paquete_enviar, 0, MAX_PACK);
	memset(paquete_recibir, 0, MAX_PACK);
	this->mutex = SDL_CreateMutex();
	this->id = Cliente::cant_clientes;
	Cliente::cant_clientes++;
	this->paqueteInicial = new structInicial();
	this->conectado = false;
	this->activo = false;
	this->hilos.enviar = NULL;
	this->hilos.recibir = NULL;
	this->enviarpaquete = true;
}

Cliente::~Cliente(){
	delete paqueteInicial;
	this->activo = false;
	SDL_WaitThread(this->hilos.enviar, 0);
	SDL_WaitThread(this->hilos.recibir, 0);
	delete this->socket_cl;
	SDL_DestroyMutex(mutex);
	Cliente::cant_clientes--;
}

Socket* Cliente::getSocket(){
	return this->socket_cl;
}


int Cliente::getID(){
	return this->id;
}

void Cliente::setID(int un_id){
	this->id = un_id;
}

char* Cliente::getPaquete(){
	//SDL_LockMutex(this->mutex);
	char* buffer = new char[MAX_PACK];
	memcpy(buffer, this->paquete_recibir, MAX_PACK);
	//SDL_UnlockMutex(this->mutex);
	return buffer;
}

structInicial* Cliente::getPaqueteInicial(){
	return this->paqueteInicial;
}

void Cliente::setConexion(bool estado){
	this->conectado = estado;
}

int runSendInfoCliente(void* cliente){
	Cliente* clien = (Cliente*) cliente;
	clien->runEnviarInfo();
	return EXIT_SUCCESS;
}

int runRecvInfoCliente(void* cliente){
	Cliente* client = (Cliente*) cliente;
	client->runRecibirInfo();
	return EXIT_SUCCESS;
}

int Cliente::conectar(){
	if ( this->socket_cl->conectar() != EXIT_SUCCESS) {
		loguear();
		logFile << "Cliente: " <<this->name_client << " \t No se pudo conectar con servidor" << endl;
		return EXIT_FAILURE;
	}
	bool envio_nombre = false;
	while (!envio_nombre){
		int bytes = this->enviarNombre();
		if(bytes > 0 ) envio_nombre = true;
		if(bytes == 0) break;// TODO Verificar qué pasa si la # de Bytes es -1 o 0;
	}
	if(this->recibirConfiguracion() <= 0) return EXIT_FAILURE;
	else{
		if(!this->paqueteInicial->cliente_aceptado) return EXIT_FAILURE;
		this->activar();

//		this->conectado = true; //todo
		hilos.recibir = SDL_CreateThread(runRecvInfoCliente, "recibirServidor",(void*)this);
		if(hilos.recibir == NULL){
			loguear();
			logFile << "No se puede crear hilo para recibir información del servidor" << endl;
			return EXIT_FAILURE;
		}
		hilos.enviar = SDL_CreateThread(runSendInfoCliente,"enviarServidor",(void*)this);
		if(hilos.enviar == NULL){
			loguear();
			logFile << "No se puede crear hilo para enviar información al servidor" << endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
}

int Cliente::runEnviarInfo(){
	while(this->activo){
		if ( enviarpaquete == true){
			SDL_Delay(25);
			char buffer[MAX_PACK];
			memset(buffer,0,MAX_PACK);
			//SDL_LockMutex(this->mutex);
			memcpy(buffer, this->paquete_enviar, MAX_PACK);
			int enviados = this->enviar(buffer, MAX_PACK); //todo
			//SDL_UnlockMutex(this->mutex);
			if (enviados > 0){
				//printf(" ENVIE ALGOO \n");
				enviarpaquete = false;
			}
			else if(enviados == -1){
				loguear();
				logFile << "Error al enviar información del cliente "<< this->name_client <<" al servidor \n" << endl;
				//break;
			}
			if(enviados == 0) {
			loguear();
			logFile << "Error \t Se enviaron 0 bytes de información al servidor." << endl;
			this->desactivar();
			}
		}
		}
	return EXIT_SUCCESS;
}


//solo envia info al servidos a través del thread
int Cliente::enviar(char* mensaje, size_t longData){
	return this->socket_cl->enviar(mensaje, longData);
}


int Cliente::runRecibirInfo(){
	while(this->activo){
		SDL_Delay(25);
		char buffer[MAX_PACK];
		//char* buffer = (char*) malloc(sizeof(char) * MAX_PACK);
		memset(buffer, 0, MAX_PACK);
		int recibidos = this->socket_cl->recibir(buffer, MAX_PACK);
		if (recibidos > 0){
			//SDL_LockMutex(this->mutex);
			memcpy(this->paquete_recibir, buffer, MAX_PACK);

			//SDL_UnlockMutex(this->mutex);
		}
		else if(recibidos ==0){
			//this->dibujarMensajeDesconexion();
			loguear();
			logFile << "Error \t Servidor desconectado, no se puede recibir información " << endl;
			this->desactivar();
		}
		else if (recibidos == -1){
			loguear();
			logFile << "Error al recibir información" << endl;
		}
	}
	return EXIT_SUCCESS;
}


void Cliente::actualizarPaquete(structEvento* evento){
	if(evento != NULL){
		this->enviarpaquete=true;
		memcpy( this->paquete_enviar, evento, sizeof(structEvento));
	}
}

char* Cliente::getNombre(){
	return this->name_client;
}

void Cliente::setNombre(char *name){
	//SDL_LockMutex(this->mutex);
	strncpy(this->name_client, name, MAX_NAME_USER-1);
	//SDL_UnlockMutex(this->mutex);
}

int Cliente::enviarNombre(){
	return this->socket_cl->enviar(this->name_client, MAX_NAME_USER);
}

void Cliente::activar(){
	this->activo = true;
}

void Cliente::desactivar(){
	this->activo = false;
	jugador->desconectar();
}

bool Cliente::getActivo(){
	return this->activo;
}

void Cliente::setHilos(comThreads hilos_server){
	this->hilos.enviar = hilos_server.enviar;
	this->hilos.recibir = hilos_server.recibir;
}

comThreads Cliente::getHilos(){
	return this->hilos;
}

int Cliente::recibirConfiguracion(){
	char buffer[MAX_PACK];
	memset(buffer, 0, MAX_PACK);
	int bytes_recibidos = this->socket_cl->recibir(buffer, MAX_PACK);
	memcpy(this->paqueteInicial, buffer, sizeof(structInicial));
	return bytes_recibidos;
}

SDL_mutex* Cliente::getMutex(){
	return this->mutex;
}

void Cliente::setJugador(Jugador* jug){
	this->jugador=jug;
}

Jugador* Cliente::getJugador(){
	return this->jugador;
}


void Cliente::setDibujador(Dibujador* dibu){
	this->dibujador = dibu;
}
void Cliente::dibujarMensajeDesconexion(){
	char* mensaje;
	this->dibujador->dibujarMensaje();

}



