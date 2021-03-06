#include "Juego.h"

extern void loguear();

Juego::Juego(){
	mundo = NULL;

	cantidad_jugadores = 0;

	escalador = NULL;
	lector = NULL;

	figuras = NULL;
	cantidad_figuras = 0;

	cielo.clear();
	agua = NULL;
	inicial = NULL;

	this->cargar();
	manejador = new ManejadorPersonajes();

	for (int i =0; i< 4; i++){
		jugadores[i]=NULL;
	}
	jugador_actual = 0;
}

Juego::~Juego(){
	for(size_t i= 0; i < cantidad_figuras ; i++){
		if (figuras != NULL){
			if(figuras[i] != NULL){
				if(figuras[i]) delete figuras[i];
			}
		}
	}
	if(figuras) delete[] figuras;
	if(agua) delete agua;
	if(lector) delete lector;
	if(mundo) delete mundo;
	if(manejador) delete manejador;
	if(escalador) delete escalador;
	for(Uint8 i = 0; i < cantidad_jugadores; i++){
		if(jugadores[i] != NULL){
			if(jugadores[i]) delete jugadores[i];
		}
	}
}

Jugador** Juego::getJugadores(){
	return jugadores;
}

uint8 Juego::getCantidadJugadores(){
	return cantidad_jugadores;
}

Mundo* Juego::getMundo(){
	return mundo;
}

Figura** Juego::getFiguras(){
	return figuras;
}

LectorMascara* Juego::getLector(){
	return lector;
}

Agua* Juego::getAgua(){
	return agua;
}

Escalador* Juego::getEscalador(){
	return escalador;
}

string Juego::getCielo() {
	return cielo;
}

uint8 Juego::getCantidadFiguras(){
	return cantidad_figuras;
}

void Juego::cargar() {
	this->abrirLog();
	Cargador *cargador = new Cargador(pathEscTest.c_str());
	Node *nodo_escenario = this->cargaInicial(cargador);
	this->cargaPrincipal(cargador, *nodo_escenario);
	inicial = cargador->getPaqueteInicial();
	if(nodo_escenario) delete nodo_escenario;
	if(cargador) delete cargador;
}


// Funciones Privadas:

void Juego::abrirLog(){
	logFile.open("log.txt",ios::out);
	logFile << "Log File. Ejecución Worms" << endl;
	logFile << "   Fecha  | Hora |  Gravedad  | \t Mensaje " << endl;
}

Node* Juego::cargaInicial(Cargador* cargador){
	Node *nodo_escenario =  new Node();
	if(!((cargador->getNodo(cargador->getNodo(),*nodo_escenario,"esc"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenario"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"esce"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escen"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escena"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenar"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"escenari"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Esc"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"ESC"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"ESCENARIO"))||
			(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Escenario"))||(cargador->getNodo(cargador->getNodo(),*nodo_escenario,"Esc")))){
		loguear();
		logFile << "    Error   " << "\t No se encuentra el escenario. Se carga escenario por defecto."<<endl;
		if(cargador) delete cargador;
		cargador = new Cargador(pathDefEs.c_str());
		(*nodo_escenario) = cargador->getNodo()["escenario"];
	}
	return nodo_escenario;
}

void Juego::cargarEscalador(Cargador *cargador, Node nodo_escenario){
	this->escalador = cargador->loadEscalador(nodo_escenario);
	if(!this->escalador){
		loguear();
		logFile << "    Error   " << "\t  No se pudo escalar la información "<< endl;
	}
}

void Juego::cargarAgua(Cargador *cargador, Node nodo_escenario){
	string imagen_agua = pathAgua;
	this->agua = cargador->loadAgua(nodo_escenario, imagen_agua);
	if(!this->agua){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el agua. "<< endl;
	}
}

void Juego::cargarMundo(){
	b2Vec2 escalas = b2Vec2(this->escalador->getEscalaX(), this->escalador->getEscalaY());
	this->mundo = new Mundo(b2Vec2(GRAVEDAD_X,GRAVEDAD_Y), this->agua, escalas);
	if(!this->mundo){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el mundo."<< endl;
	}
}

string Juego::cargarTierra(Cargador *cargador, Node nodo_escenario){
	string tierra;
	if(!((cargador->loadPath(nodo_escenario, "imagen_tierra",tierra))||(cargador->loadPath(nodo_escenario, "tierra",tierra))||(cargador->loadPath(nodo_escenario, "imagen-tierra",tierra)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		tierra = pathDefMas;
	}
	return tierra;
}

void Juego::cargarCielo(Cargador *cargador, Node nodo_escenario){
	string cielo;
	if(!((cargador->loadPath(nodo_escenario, "imagen_cielo",cielo))||(cargador->loadPath(nodo_escenario, "cielo",cielo))||(cargador->loadPath(nodo_escenario, "imagen-cielo",cielo)))){
		loguear();
		logFile << "    Error  " <<"\t No se pudo cargar el path correspondiente a la clave buscada. " << endl;
		cielo = pathDefCielo;
	}
}

void Juego::cargarLector(string tierra){
	this->lector = new LectorMascara(tierra);
	if(!this->lector){
		loguear();
		logFile << "    Error   " << "\t  No se pudo crear el Lector de Máscara. " <<  SDL_GetError()<< endl;
	}
}

void Juego::generarTierra(){
	b2Vec2* vectorTierra = this->lector->LeerMascara(this->escalador);
	this->mundo->Crear_Chains(vectorTierra, this->lector->GetPixelAncho());
}

void Juego::cargarFiguras(Cargador *cargador, Node nodo_escenario){
	Node objetos;
	if((cargador->getNodo(nodo_escenario,objetos,"objetos"))||(cargador->getNodo(nodo_escenario,objetos,"obj"))||
			(cargador->getNodo(nodo_escenario,objetos,"o"))||(cargador->getNodo(nodo_escenario,objetos,"Objetos"))||(cargador->getNodo(nodo_escenario,objetos,"OBJETOS"))){
		this->cantidad_figuras = objetos.size();
		this->figuras = new Figura*[this->cantidad_figuras];
		for(size_t i = 0; i < this->cantidad_figuras; i++){
			this->figuras[i] = cargador->cargarFigura(objetos[i], this->mundo, this->escalador, i + 1);
		}
	}else{
		loguear();
		logFile << "    Warning " << "\t  No hay objetos para cargar "<< endl;
	}
}

//Retorna el mundo cargado y si hubo algún error terminante retorna NULL.
void Juego::cargaPrincipal(Cargador *cargador, Node nodo_escenario){
	cargarEscalador(cargador, nodo_escenario);
	cargarAgua(cargador, nodo_escenario);
	cargarMundo();
	string tierra = cargarTierra(cargador, nodo_escenario);
	cargarCielo(cargador, nodo_escenario);
	cargarLector(tierra);
	if(this->lector == NULL){
		if(this->mundo) delete this->mundo;
		this->mundo = NULL;
	}
	generarTierra();
	cargarFiguras(cargador, nodo_escenario);
}

structInicial* Juego::getPaqueteInicial(){
	return this->inicial;
}

void Juego::aplicarPaquete(structEvento* evento){
	if (evento == NULL) return;
	if (evento->click_mouse.x != -1){ // recibio un click
		manejador->seleccionarPersonaje(evento->click_mouse, evento->nro_jugador);
	}
	if (evento->direccion > 0){ // recibio un click
		manejador->moverPersonaje(evento->direccion , evento->nro_jugador);
		this->pasarTurno();
	}
	else return;
}

Jugador* Juego::agregarJugador(int id, char* nombre_cliente){
	this->jugadores[id] = new Jugador(mundo,id,manejador, nombre_cliente);
	Personaje** pers = this->jugadores[id]->getPersonajes();
	this->manejador->AgregarJugador(mundo,id, pers);
	return this->jugadores[id];
}

ManejadorPersonajes* Juego::getManejadorPersonajes(){
	return this->manejador;
}

int Juego::getJugadorActual(){
	return jugador_actual;
}

void Juego::pasarTurno(){
	jugador_actual++;
	if(jugador_actual == 5){
		jugador_actual =0;
	}
}
