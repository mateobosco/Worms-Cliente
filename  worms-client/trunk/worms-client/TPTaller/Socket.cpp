#include "Socket.h"

// Para Servidor debe pasarse el IP como NULL, Que luego se cargará la ip propia del servidor,
// al cargar el flag AI_PASSIVE.
// Para cliente debe pasarse la IP del servidor al que se desea conectar.
Socket::Socket(const char *ip, const char *puerto) {
	this->sockFD = 0;
	this->puerto = puerto;
	struct addrinfo hints;
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (!ip) hints.ai_flags = AI_PASSIVE;
	int estado = getaddrinfo(ip,this->puerto,&hints,&this->info);
	if(estado !=0){
		//loguear error
		exit(1);
	}
	if ((this->sockFD = socket(info->ai_family, info->ai_socktype,info->ai_protocol)) == -1) {
		//log error
		perror("socket");
	}
	this->activo = true;

}
Socket::Socket(const char* puerto, int sockfd) {
	this->puerto = puerto;
	this->sockFD = sockfd;
	this->activo = true;
	this->info = NULL;
}

Socket::~Socket() {
	close(this->sockFD);
}

int Socket::conectar(){
	if(connect(this->sockFD, this->info->ai_addr, this->info->ai_addrlen) == -1) {
		close(this->sockFD);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Socket::EnlazarYEscuchar(int cantMaxCon){
	int yes = 1;
	setsockopt(this->sockFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		//log errot todo
	if (bind(this->sockFD, this->info->ai_addr, this->info->ai_addrlen) == -1) {
		//log error
		close(this->sockFD);
		perror("bind");
	}
	return this->escuchar(cantMaxCon);
}

int Socket::escuchar(int cantMaxCon){
	int l = listen(this->sockFD, cantMaxCon);
	if (l == -1) {
		//loguear error todo
		return -1;
	}
	return l;
}

// Aceptar devuelve un FD (file descriptor)
int Socket::aceptar(){
	socklen_t len;
	int nuevoFD;
	struct sockaddr_storage infoCliente;
	len = sizeof infoCliente;

	//Se acepta la conexión y se el asigna un fd
	nuevoFD = accept(this->sockFD,(struct sockaddr *)&infoCliente,&len);
	if (nuevoFD == -1) {
		//loguear error todo
		close(nuevoFD);
	}
	return nuevoFD;

}

void Socket::setInfo(struct sockaddr* sock ){
	memcpy(this->info->ai_addr->sa_data,sock->sa_data,14);
	this->info->ai_addr->sa_family = sock->sa_family;
}

struct addrinfo* Socket::getInfo(){
	return this->info;
}

int Socket::enviar(const void* dato, size_t longDato){
	return send(this->sockFD, dato, longDato, 0);
}

int Socket::recibir(char* buffer, int longBuffer){
	return recv(this->sockFD, buffer, longBuffer, 0);
}

int Socket::getFD(){
	return this->sockFD;
}
