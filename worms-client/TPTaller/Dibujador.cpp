#include "Dibujador.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"
#include "Personaje.h"
#include "Paquete.h"

#include "Juego.h"


#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string>
#include <sstream>

extern void loguear();

Dibujador::Dibujador(){
	this->escalador = NULL;
	this->renderizador = NULL;
	this->escalado_x = 1;
	this->escalado_y = 1;
	this->textureCielo = NULL;
	this->textureAgua = NULL;
	this->textureTierra = NULL;
	this->texturederecha = NULL;
	this->textureizquierda = NULL;
	this->texturederechaNEGRO = NULL;
	this->textureizquierdaNEGRO = NULL;
	this->flechitaroja = NULL;
	this->window = NULL;
	this->contador_cerrarse = 10;
	this->oscilaragua = 0;
	this->escalaZoom = 100;
	bazooka = loadTexture("TPTaller/imagenes/bazooka", this->renderizador);
	granada = loadTexture("TPTaller/imagenes/granada", this->renderizador);
	dinamita = loadTexture("TPTaller/imagenes/dinamita", this->renderizador);
	holy = loadTexture("TPTaller/imagenes/holy", this->renderizador);
	kamikaze = loadTexture("TPTaller/imagenes/kamikaze", this->renderizador);
	patada = loadTexture("TPTaller/imagenes/patada", this->renderizador);
}

Dibujador::Dibujador(SDL_Renderer* renderer, Escalador* esc){
	this->escalador = esc;
	this->window=NULL;
	this->renderizador = renderer;
	this->escalado_x = esc->getEscalaX();
	this->escalado_y = esc->getEscalaY();
	this->textureCielo = NULL;
	this->textureAgua = NULL;
	this->textureTierra = NULL;
	this->texturederecha = NULL;
	this->textureizquierda = NULL;
	this->texturederechaNEGRO = NULL;
	this->textureizquierdaNEGRO = NULL;
	this->flechitaroja = NULL;
	this->contador_cerrarse = 10;
	this->oscilaragua = 0;
	this->escalaZoom = 100;

}

Dibujador::~Dibujador(){
	delete this->escalador;
	if(this->textureAgua) SDL_DestroyTexture(this->textureAgua);
	if(this->textureCielo) SDL_DestroyTexture(this->textureCielo);
	if (this->textureTierra) SDL_DestroyTexture(textureTierra);
	if (this->texturederecha) SDL_DestroyTexture(texturederecha);
	if (this->textureizquierda) SDL_DestroyTexture(textureizquierda);
	if (this->texturederechaNEGRO) SDL_DestroyTexture(texturederechaNEGRO);
	if (this->textureizquierdaNEGRO) SDL_DestroyTexture(textureizquierdaNEGRO);

	if (this->surfaceTierra) SDL_FreeSurface(this->surfaceTierra);
	this->close();
}

// Retorna: 0- Exito. Negativo- Fracaso.
int Dibujador::limpiarFondo(){
	// Setea el fondo en blanco.
	SDL_SetRenderDrawColor(this->renderizador, 0xFF, 0xFF, 0xFF, 0xFF);
	return SDL_RenderClear(this->renderizador);
}

void Dibujador::dibujarFiguras(Figura** figuras, int cantidad){
	for (int32 i = 0; i < cantidad; ++i){
		if (figuras[i] != NULL) figuras[i]->dibujar(this);
	}
	this->actualizar();
}

void Dibujador::dibujarPersonajes(Personaje** personajes, int cantidad){
	for (int32 i = 0; i < cantidad; ++i){
		if (personajes[i] != NULL) personajes[i]->dibujar(this);
	}
}

int Dibujador::dibujarCirculo(Circulo* circulo){
	SDL_Color color = circulo->getColor();
	float32 radio = circulo->getRadio();
	Uint16 rad_pix_x = (Uint16) escalador->aplicarZoomX(radio);
	Uint16 rad_pix_y = (Uint16) escalador->aplicarZoomY(radio);
	b2Vec2 posicion = circulo->obtenerPosicion();
	b2Vec2* posicionVentanada = this->escalador->aplicarZoomPosicion(posicion);

	int retorno = filledEllipseRGBA( renderizador, posicionVentanada->x, posicionVentanada->y,
								rad_pix_x, rad_pix_y, color.r, color.g, color.b, CIRC_OPACIDAD);
	delete posicionVentanada;
	return retorno;
}

int Dibujador::dibujarPoligono(Poligono* poligono){
	SDL_Color color = poligono->getColor();
	int cantidad_lados = poligono->getCantVertices();
	float32* vecX = poligono->getVecX();
	float32* vecY = poligono->getVecY();
	Sint16* vecXventanado = this->escalador->aplicarZoomXVector(vecX, cantidad_lados);
	Sint16* vecYventanado = this->escalador->aplicarZoomYVector(vecY,cantidad_lados);
	int retorno =  filledPolygonRGBA(this->renderizador, vecXventanado, vecYventanado, cantidad_lados,
									color.r, color.g, color.b, POLI_OPACIDAD);
	delete[] vecX;
	delete[] vecY;
	delete[] vecYventanado;
	delete[] vecXventanado;
	return retorno;
}

int Dibujador::dibujarRectangulo(Rectangulo* rectangulo){
	SDL_Color color = rectangulo->getColor();
	float32* vecX = rectangulo->getVecX();
	float32* vecY = rectangulo->getVecY();
	Sint16* vecXventanado = this->escalador->aplicarZoomXVector(vecX , 4);
	Sint16* vecYventanado = this->escalador->aplicarZoomYVector(vecY , 4);
	int retorno = filledPolygonRGBA(this->renderizador, vecXventanado, vecYventanado, 4,
									color.r, color.g, color.b, RECT_OPACIDAD);
	delete[] vecX;
	delete[] vecY;
	delete[] vecYventanado;
	delete[] vecXventanado;
	return retorno;
}

SDL_Texture* Dibujador::RenderText(std::string message, std::string fontFile,
                        SDL_Color color, int fontSize){
	if (TTF_Init() == -1){
	    std::cout << TTF_GetError() << std::endl;
	}
	TTF_Font *font = nullptr;
	font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr)
        throw std::runtime_error("Error al cargar font: " + fontFile + TTF_GetError());
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderizador, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

SDL_Texture* Dibujador::dibujarPersonaje2(Personaje* personaje){
	SDL_Texture *gusanito = loadTexture(personaje->getDirImagen(), this->renderizador);
	b2Vec2 posicion = personaje->getPosition();
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
	int anchoPX = escalador->aplicarZoomX( personaje->getAncho());
	int altoPX = escalador->aplicarZoomY(personaje->getAlto());
	int x = posicionVentanada->x - anchoPX/2;
	int y = posicionVentanada->y - altoPX/2;
	int w = anchoPX;
	int h = altoPX;
	SDL_Texture *image;
	std::string nombre_jugador;
	SDL_Color color = { 0, 0, 0 };

	if (personaje->getSeleccionado()){
		nombre_jugador = "Juan";

		image = RenderText( nombre_jugador, "TPTaller/imagenes/abecedarionegrita.ttf", color, 52); // despues preguntar el nombre de cada uno

		renderTexture2(image, this->renderizador, x - 30 ,y - 70 , 80, 80 );
		if (image) SDL_DestroyTexture(image);
	}
	renderTexture2(gusanito, this->renderizador, x ,y ,w , h );
	delete[] posicionVentanada;
	if (gusanito) SDL_DestroyTexture(gusanito);
	return gusanito;
}

SDL_Renderer* Dibujador::getRenderizador(){
	return this->renderizador;
}

void Dibujador::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	SDL_Rect dst;
	int zoom =  escalador->getZoom();
	dst.x = escalador->aplicarZoomXaPix(x) ;
	dst.y = escalador->aplicarZoomYaPix(y) ;
	dst.w = w*((float32)zoom/100);
	dst.h = h*((float32)zoom/100);

	if(SDL_RenderCopy(ren, tex, NULL, &dst)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

void Dibujador::renderTexture2(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;

	if(SDL_RenderCopy(ren, tex, NULL, &dst)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

void Dibujador::renderTexture3(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo, int punto1, int punto2){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_Point point;
	point.x=punto1;
	point.y=punto2;
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, angulo, &point,SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

void Dibujador::renderTextureCenter(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, angulo-45, NULL, SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

void Dibujador::renderTexture5(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo, int punto1, int punto2){
	SDL_Rect dst;
	dst.x = x + cos(angulo * PI/180) * this->escalaZoom;
	dst.y = y + sin(angulo * PI/180) * this->escalaZoom;
	dst.w = w;
	dst.h = h;
	SDL_Point point;
	point.x=punto1;
	point.y=punto2;
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, angulo, &point,SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

void Dibujador::renderTexture6(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo, int punto1, int punto2){
	SDL_Rect dst;
	dst.x = x - cos(angulo * PI/180) * this->escalaZoom;
	dst.y = y - sin(angulo * PI/180) * this->escalaZoom;
	dst.w = w;
	dst.h = h;
	SDL_Point point;
	point.x=punto1;
	point.y=punto2;
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, angulo, &point,SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}

void Dibujador::renderTexture4(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_Point point;
	point.x=-w/2;
	point.y=h/2;
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, angulo, &point,SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló " << endl;
	}
}



SDL_Texture* Dibujador::dibujar_cielo(Escalador* escalador, std::string path, int hasta){
	SDL_Texture *background = loadTexture(path.c_str(), this->renderizador);
	if(background == NULL){
		background = loadTexture(pathDefCielo.c_str(),this->renderizador);
		loguear();
		logFile <<"    Error    " <<"\t No se pudo cargar textura de fondo del cielo. Se carga por defecto el cielo de ubicación '"<<pathDefCielo<<"'." << endl;
	}
	textureCielo = background;
	renderTexture(background, this->renderizador, 0-escalador->getOffsetX() , 0-escalador->getOffsetY(), escalador->getPixelX(), hasta);
	return background;
}

SDL_Texture* Dibujador::dibujarAgua(Escalador* escalador, Agua* agua ){
	SDL_Texture *background = loadTexture(pathAgua, this->renderizador);
	if(background == NULL){
		background = loadTexture(pathDefAgua,this->renderizador);
		loguear();
		logFile << "    Error    " <<"\t No se pudo cargar textura de agua. Se carga por defecto agua de ubicación '"<<pathDefAgua<<"'." << endl;
	}
	SDL_SetTextureAlphaMod(background, 150);
	textureAgua = background;
	int pixelY = escalador->aplicarZoomY(agua->GetNivel()) + escalador->getOffsetY();
	renderTexture(background, this->renderizador,0- escalador->getOffsetX(), pixelY, escalador->getPixelX(), escalador->getPixelY());
	return background;
}

SDL_Texture* Dibujador::dibujar_tierra(Escalador* escalador, std::string path){
	SDL_Texture *background = loadTextureTierra(path.c_str(), this->renderizador);
	if(background == NULL){
		loguear();
		logFile << "    Error    " <<"\t No se pudo cargar textura de fondo " << endl;
	}
	textureTierra = background;
	renderTexture(background, this->renderizador, 0-escalador->getOffsetX() , 0-escalador->getOffsetY(), escalador->getPixelX(), escalador->getPixelY());
	return background;
}

void Dibujador::actualizar(){
	SDL_RenderPresent(this->renderizador);
}

SDL_Texture* Dibujador::loadTexture(const std::string &path, SDL_Renderer *ren){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        loguear();
        logFile << "No se pudo cargar la imagen: " << path.c_str() << "! SDL_image Error: " <<  IMG_GetError() << endl;
    } else{
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if(newTexture == NULL){
        	loguear();
        	logFile << "No se pudo crear textura de: " << path.c_str() << "! SDL Error: " <<  SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

SDL_Texture* Dibujador::loadTextureTierra(const std::string &path, SDL_Renderer *ren){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    this->surfaceTierra = loadedSurface;

    if(loadedSurface == NULL){
        loguear();
        logFile << "No se pudo cargar la imagen: " << path.c_str() << "! SDL_image Error: " <<  IMG_GetError() << endl;
    } else{
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if(newTexture == NULL){
        	loguear();
        	logFile << "No se pudo crear textura de: " << path.c_str() << "! SDL Error: " <<  SDL_GetError() << endl;
        }
    }
    return newTexture;
}

void Dibujador::iniciarFondo(Agua* agua, std::string pathCielo, std::string pathTierra){
	bazooka = loadTexture("TPTaller/imagenes/bazooka.png", this->renderizador);
	granada = loadTexture("TPTaller/imagenes/granada.png", this->renderizador);
	dinamita = loadTexture("TPTaller/imagenes/dinamita.png", this->renderizador);
	holy = loadTexture("TPTaller/imagenes/holy.png", this->renderizador);
	kamikaze = loadTexture("TPTaller/imagenes/kamikaze.png", this->renderizador);
	patada = loadTexture("TPTaller/imagenes/patada3.png", this->renderizador);
	mira = loadTexture("TPTaller/imagenes/mira.png", this->renderizador);
	float nivelAgua = escalador->aplicarZoomY(agua->GetNivel()) - escalador->getOffsetY();
	this->dibujar_tierra(escalador, pathTierra);
	this->dibujarAgua(escalador, agua);
	this->dibujar_cielo(escalador, pathCielo, nivelAgua);

}

void Dibujador::dibujarFondo(){
	this->renderTexture(textureCielo, renderizador,0  , 0 , escalador->getPixelX(), escalador->getPixelY() );
	this->renderTexture(textureTierra, renderizador, 0 , 0 , escalador->getPixelX() , escalador->getPixelY() );
}

void Dibujador::dibujar_agua(Agua* agua){
	this->oscilaragua+=0.03;
	float32 aux=cos(oscilaragua);
	if(oscilaragua > 360){
		this->oscilaragua=0;
	}

	float nivel = (agua->GetNivel()*(escalador->getPixelY()/escalador->getEscalaY()));
	this->renderTexture(textureAgua, renderizador, 0 , ((nivel*aux/100 + (nivel))) , escalador->getPixelX(), (escalador->getPixelY()-(agua->GetNivel() *(escalador->getPixelY()/escalador->getEscalaY()))+50));
}

int Dibujador::dibujarPaqueteFigura(structFigura figura){
	int cantidad = figura.cantidad;
	SDL_Color color = figura.color;
	int retorno;

	if ( cantidad == 2){
		float32 radio = figura.vector_vertices[1].x;
		Uint16 rad_pix_x = (Uint16) escalador->aplicarZoomX(radio);
		Uint16 rad_pix_y = (Uint16) escalador->aplicarZoomY(radio);
		b2Vec2 posicion = figura.vector_vertices[0];
		b2Vec2* posicionVentanada = this->escalador->aplicarZoomPosicion(posicion);
		retorno = filledEllipseRGBA( renderizador, posicionVentanada->x, posicionVentanada->y,
										rad_pix_x, rad_pix_y, color.r, color.g, color.b, CIRC_OPACIDAD);
		delete posicionVentanada;

	}
	if (cantidad > 2){
		int cantidad_lados = cantidad;
		float32* vecX = new float32[cantidad];
		float32* vecY = new float32[cantidad];
		for (int i=0; i < cantidad_lados; i++){
			vecX[i] = figura.vector_vertices[i].x;
			vecY[i] = figura.vector_vertices[i].y;
		}

		Sint16* vecXventanado = this->escalador->aplicarZoomXVector(vecX, cantidad_lados);
		Sint16* vecYventanado = this->escalador->aplicarZoomYVector(vecY,cantidad_lados);
		retorno =  filledPolygonRGBA(this->renderizador, vecXventanado, vecYventanado, cantidad_lados,
										color.r, color.g, color.b, POLI_OPACIDAD);

		delete[] vecX;
		delete[] vecY;
		delete[] vecYventanado;
		delete[] vecXventanado;
	}
	return retorno;
}

void Dibujador::dibujarPaquetePersonaje(structPersonaje paquete, char* nombre_jugador, bool duenio, int cliente_id, float aux){

	int dir = paquete.direccion;
	b2Vec2 tam = paquete.tamano;

	SDL_Texture* gusanito;
	if (dir ==1){
		if(paquete.arma_seleccionada == 3) gusanito = loadTexture("TPTaller/imagenes/gusano_tnt_der.png", this->renderizador);
		else if(paquete.arma_seleccionada == 5) gusanito = loadTexture("TPTaller/imagenes/suicidader.png", this->renderizador);
		else if(paquete.arma_seleccionada == 6) gusanito = loadTexture("TPTaller/imagenes/patadaDER.png", this->renderizador);
		else gusanito = this->texturederecha;

	}
	if (dir == -1){
		if(paquete.arma_seleccionada == 3) gusanito = loadTexture("TPTaller/imagenes/gusano_tnt_izq.png", this->renderizador);
		else if(paquete.arma_seleccionada == 5) gusanito = loadTexture("TPTaller/imagenes/suicidaizq.png", this->renderizador);
		else if(paquete.arma_seleccionada == 6) gusanito = loadTexture("TPTaller/imagenes/patadaIZQ.png", this->renderizador);
		else gusanito = this->textureizquierda;
	}
	if (dir ==1 && paquete.conectado == 0){
		gusanito = this->texturederechaNEGRO;
	}
	if (dir == -1 && paquete.conectado == 0){
		gusanito = this->textureizquierdaNEGRO;
	}

	b2Vec2 posicion = paquete.posicion;
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
	int anchoPX = escalador->aplicarZoomX( tam.x);
	int altoPX = escalador->aplicarZoomY(tam.y);
	int x = posicionVentanada->x - anchoPX/2;
	int y = posicionVentanada->y - altoPX/2;
	int w = anchoPX;
	int h = altoPX;

	SDL_Texture *image;
	SDL_Color vectorcolores[4];
	vectorcolores[0] = { 0, 0, 0 };
	vectorcolores[1]= { 255, 0, 0 };
	vectorcolores[2] = { 0, 255, 0 };
	vectorcolores[3] = { 0, 0, 255 };

	std::string nombre_a_imprimir = string(paquete.nombre_cliente);
	image = RenderText(paquete.nombre_cliente, "TPTaller/imagenes/Hilarious.ttf", vectorcolores[paquete.id_jugador], 20); // despues preguntar el nombre de cada uno

	renderTexture2(image, this->renderizador, x - anchoPX/((float32)escalador->getZoom()/100) ,y - 50 , 80 , 30 );
	if (image) SDL_DestroyTexture(image);
	if(paquete.seleccionado[cliente_id] == 1){
		this->
		renderTexture2(flechitaroja, this->renderizador, x - anchoPX/((float32)escalador->getZoom()/100), ((y)*aux/100)+(y-140), 80, 80);
	}

	char energia[10];
	sprintf(energia,"%d", paquete.energia);
//	Sint16* vecX = new Sint16();
//	Sint16* vecY = new Sint16();
//	Sint16* vecXventanado = this->escalador->aplicarZoomXVector(vecX , 4);
//	Sint16* vecYventanado = this->escalador->aplicarZoomYVector(vecY , 4);
	SDL_Texture* energiatext = RenderText(energia, "TPTaller/imagenes/Hilarious.ttf", vectorcolores[0], 15); // despues preguntar el nombre de cada uno
//	Sint16* vecX = 100;
//	Sint16* vecY = 100;
//	vecX[0]=x;
//	vecX[1]=x;
//	vecX[2]=2*x;
//	vecX[3]=2*x;
//	vecY[0]=y;
//	vecY[1]=y;
//	vecY[2]=2*y;
//	vecY[3]=2*y;
//	int retorno = filledPolygonRGBA(this->renderizador,vecX, vecY, 4, 255, 0, 0, RECT_OPACIDAD);

	SDL_Texture* vida_roja = loadTexture("TPTaller/imagenes/roja.png", this->renderizador);
	SDL_Texture* vida_verde = loadTexture("TPTaller/imagenes/verde.png", this->renderizador);
	renderTexture2(vida_roja, this->renderizador, x-w/2,y-h*2, w*2,h*2);
	renderTexture2(vida_verde, this->renderizador, x-w/2, y-h*2, w*2 * paquete.energia/100 ,h*2);
	renderTexture2(energiatext, this->renderizador,  x+w/3,y-h*1.2, w/2,h/2 );
	if(vida_roja) SDL_DestroyTexture(vida_roja);
	if(vida_verde) SDL_DestroyTexture(vida_verde);

	renderTexture2(gusanito, this->renderizador,x,y ,w , h  );
	if(paquete.arma_seleccionada == 1 && paquete.direccion ==1){
		SDL_Texture* bazooka = loadTexture("TPTaller/imagenes/bazooka2.png", this->renderizador);
		//printf(" El angulo del arma es %d \n", paquete.angulo_arma);
		renderTexture3(bazooka, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		renderTexture5(mira, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		//renderTexture2(mira, this->renderizador, x + cos(paquete.angulo_arma * PI /180) * escalaZoom + w, y + sin(paquete.angulo_arma * PI /180)*escalaZoom - h,w+5,h+5);
		if(bazooka) SDL_DestroyTexture(bazooka);
		//if(mira) SDL_DestroyTexture(mira);
	}
	if(paquete.arma_seleccionada == 1 && paquete.direccion ==1){
			SDL_Texture* bazooka = loadTexture("TPTaller/imagenes/bazooka2.png", this->renderizador);
			//printf(" El angulo del arma es %d \n", paquete.angulo_arma);
			renderTexture3(bazooka, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
			renderTexture5(mira, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
			//renderTexture2(mira, this->renderizador, x + cos(paquete.angulo_arma * PI /180) * escalaZoom + w, y + sin(paquete.angulo_arma * PI /180)*escalaZoom - h,w+5,h+5);
			if(bazooka) SDL_DestroyTexture(bazooka);
			//if(mira) SDL_DestroyTexture(mira);
	}
	if(paquete.arma_seleccionada == 1 && paquete.direccion == -1){
		SDL_Texture* bazooka = loadTexture("TPTaller/imagenes/bazookaizq.png", this->renderizador);
		renderTexture3(bazooka, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, w,h);
		renderTexture6(mira, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);

		if(bazooka) SDL_DestroyTexture(bazooka);
	}
	if(paquete.arma_seleccionada == 2 && paquete.direccion ==1){
			SDL_Texture* granada = loadTexture("TPTaller/imagenes/granadader.png", this->renderizador);
			//printf(" El angulo del arma es %d \n", paquete.angulo_arma);
			renderTexture3(granada, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
			renderTexture5(mira, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
			//renderTexture2(mira, this->renderizador, x + cos(paquete.angulo_arma * PI /180) * escalaZoom + w, y + sin(paquete.angulo_arma * PI /180)*escalaZoom - h,w+5,h+5);
			if(granada) SDL_DestroyTexture(granada);
			//if(mira) SDL_DestroyTexture(mira);
	}
	if(paquete.arma_seleccionada == 2 && paquete.direccion == -1){
		SDL_Texture* granada = loadTexture("TPTaller/imagenes/granadaizq.png", this->renderizador);
		renderTexture3(granada, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, w,h);
		renderTexture6(mira, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);

		if(granada) SDL_DestroyTexture(granada);
	}

	if(paquete.arma_seleccionada == 4 && paquete.direccion ==1){
			SDL_Texture* granadaholy = loadTexture("TPTaller/imagenes/granadaholyder.png", this->renderizador);
			//printf(" El angulo del arma es %d \n", paquete.angulo_arma);
			renderTexture3(granadaholy, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
			renderTexture5(mira, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
			//renderTexture2(mira, this->renderizador, x + cos(paquete.angulo_arma * PI /180) * escalaZoom + w, y + sin(paquete.angulo_arma * PI /180)*escalaZoom - h,w+5,h+5);
			if(granadaholy) SDL_DestroyTexture(granadaholy);
			//if(mira) SDL_DestroyTexture(mira);
	}
	if(paquete.arma_seleccionada == 4 && paquete.direccion == -1){
		SDL_Texture* granadaholy = loadTexture("TPTaller/imagenes/granadaholyizq.png", this->renderizador);
		renderTexture3(granadaholy, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, w,h);
		renderTexture6(mira, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);

		if(granadaholy) SDL_DestroyTexture(granadaholy);
	}

	SDL_DestroyTexture(energiatext);
	delete posicionVentanada;
}

void Dibujador::dibujarProyectil(int tipo_proyectil, b2Vec2 posicion_proyectil, b2Vec2 direccion_proyectil, b2Vec2 tamanio, int contador_segundos){
	if(tipo_proyectil){
		printf(" RECIBE EL TIPO DE PROYECTIL %d \n", tipo_proyectil);
		b2Vec2 posicion = posicion_proyectil;
		b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
		int anchoPX = escalador->aplicarZoomX( tamanio.x);
		int altoPX = escalador->aplicarZoomY( tamanio.y);
		int x = posicionVentanada->x - anchoPX/2;
		int y = posicionVentanada->y - altoPX/2;
		int w = anchoPX;
		int h = altoPX;
		int angulo = 0; //  = calcularAngulo(direccion_proyectil);
		SDL_Texture* misil;
		if(tipo_proyectil==1){
				//SDL_Texture* misil;
				if(direccion_proyectil.x>0){
					misil= loadTexture("TPTaller/imagenes/bazooka_misil.png", this->renderizador);
				}
				else{
					misil=loadTexture("TPTaller/imagenes/bazooka_misilizq.png", this->renderizador);
				}
				renderTextureCenter(misil, this->renderizador, x, y, w+5,h+5, angulo);
				if(misil) SDL_DestroyTexture(misil);
		}

		if(tipo_proyectil==2){
				//SDL_Texture* misil;
				if(direccion_proyectil.x>0){
					misil= loadTexture("TPTaller/imagenes/granadader.png", this->renderizador);
				}
				else{
					misil=loadTexture("TPTaller/imagenes/granadaizq.png", this->renderizador);
				}
				renderTextureCenter(misil, this->renderizador, x, y, w+5,h+5, angulo);
				if(misil) SDL_DestroyTexture(misil);
		}

		if(tipo_proyectil==3){

				misil = loadTexture("TPTaller/imagenes/dinamita_chispa.png", this->renderizador);
				renderTextureCenter(misil, this->renderizador, x, y, w-1,h-1, angulo);
				SDL_Texture* segundos;
				switch (contador_segundos){
					case 3:	segundos = loadTexture("TPTaller/imagenes/segundo_tres.png", this->renderizador); break;
					case 2: segundos = loadTexture("TPTaller/imagenes/segundo_tres.png", this->renderizador); break;
					case 1: segundos = loadTexture("TPTaller/imagenes/segundo_dos.png", this->renderizador); break;
					case 0: segundos = loadTexture("TPTaller/imagenes/segundo_uno.png", this->renderizador); break;
					default: segundos = loadTexture("TPTaller/imagenes/segundo_cero.png", this->renderizador); break;
				}
				renderTextureCenter(segundos, this->renderizador, x+16, y-16, w-1,h-1, angulo+45);
				if(misil) SDL_DestroyTexture(misil);
		}

		if(tipo_proyectil==4){
				//SDL_Texture* misil;

				if(direccion_proyectil.x>0){
					misil= loadTexture("TPTaller/imagenes/granadaholyder.png", this->renderizador);
				}
				else{
					misil=loadTexture("TPTaller/imagenes/granadaholyizq.png", this->renderizador);
				}
				renderTextureCenter(misil, this->renderizador, x, y, w+5,h+5, angulo);
				if(misil) SDL_DestroyTexture(misil);
		}

	}
}
void Dibujador::mostrarMenuArmas(int x, int y){
	//SDL_Texture* menu = loadTexture("TPTaller/imagenes/armas2.png", this->renderizador);
	renderTexture2(bazooka, this->renderizador, x - 100, y, 100,100);
	renderTexture2(granada, this->renderizador, x  , y, 100,100);
	renderTexture2(dinamita, this->renderizador, x - 100, y + 100, 100,100);
	renderTexture2(holy, this->renderizador, x , y +  100, 100,100);
	renderTexture2(kamikaze, this->renderizador, x - 100 , y + 200, 100,100);
	renderTexture2(patada, this->renderizador, x , y +200, 100,100);
}

void Dibujador::dibujarPaquete(structPaquete* paquete, char* nombre_cliente, int cliente_id, float aux){
	int figuras = paquete->cantidad_figuras;
	int personajes = paquete->cantidad_personajes;
	for (int i = 0 ; i < figuras ; i++ ){
		structFigura* vector = paquete->vector_figuras;
//		this->dibujarPaqueteFigura(vector[i]);
	}
	SDL_Color color = {0,0,0};
	SDL_Texture* nombre = RenderText(nombre_cliente, "TPTaller/imagenes/Hilarious.ttf", color, 20); // despues preguntar el nombre de cada uno
	renderTexture2(nombre, this->renderizador, 0 , 0 , 100, 30 );
	SDL_DestroyTexture(nombre);

	if (paquete->mensaje_mostrar[0] != NULL){
		char mensaje[50];
		SDL_Texture* cartel = RenderText(paquete->mensaje_mostrar, "TPTaller/imagenes/Hilarious.ttf", color, 20); // despues preguntar el nombre de cada uno
		renderTexture2(cartel, this->renderizador, 0 , this->escalado_x/2 , 200, 30 );
		SDL_DestroyTexture(cartel);

	}
	structPersonaje* vector1 = paquete->vector_personajes;
	for (int j = 0 ; j < personajes ; j ++){
		if (cliente_id == vector1[j].id_jugador){
			//if(paquete->show_proyectil) vector1[j].arma_seleccionada = 0; todo
			this->dibujarPaquetePersonaje(vector1[j], nombre_cliente, true, cliente_id, aux ); // es propio
		}
		else{
			this->dibujarPaquetePersonaje(vector1[j], nombre_cliente, false, cliente_id, aux); // no es propio
		}
	}

	if(paquete->show_proyectil){
		this->dibujarProyectil(paquete->tipo_proyectil, paquete->posicion_proyectil, paquete->direccion_proyectil, paquete->tamanio_proyectil, paquete->contador_segundos);
	}
}

bool Dibujador::init(){
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		loguear();
		logFile << "    Error   " << "\t  SDL No pudo inicializar! SDL Error: " <<  SDL_GetError()<< endl;
		success = false;
	} else{
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			loguear();
			logFile << "    Warning " << "\t  Linear texture filtering no habilitado! " <<  endl;
			success = false;
		}
		this->window = SDL_CreateWindow("WORMS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, escalador->getVentanaX() , escalador->getVentanaY(), SDL_WINDOW_SHOWN);
		if(this->window == NULL){
			loguear();
			logFile << "    Error   " << "\t  La ventana no pudo ser creada! SDL Error: " <<  SDL_GetError()<< endl;
			success = false;
		}
		else{
			this->renderizador = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(this->renderizador == NULL)	{
				loguear();
				logFile << "    Error   " << "\t  Renderer no pudo ser creado! SDL Error: " <<  SDL_GetError()<< endl;
				success = false;
			} else{
				SDL_SetRenderDrawColor(this->renderizador, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = (IMG_INIT_PNG |IMG_INIT_JPG); //
				int iniciadas = IMG_Init(imgFlags);
				if((iniciadas & imgFlags) != imgFlags){
					loguear();
					logFile << "    Error   " << "\t  SDL_image no puedo ser inicializado! SDL_image Error: " <<  SDL_GetError()<< endl;
					success = false;
				}

			 //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
				{
					loguear();
					logFile << "SDL_mixer no pudo ser inicializado! SDL_mixer Error: %s" << Mix_GetError() << endl;
					success = false;
				}
				if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3){
					  loguear();
					  logFile << "Error   \t Mix_Init: %s " << Mix_GetError() << endl;
				      success = false;
				}
				int frec, chan;
				Uint16 formato;
				if(Mix_QuerySpec(&frec, &formato, &chan) == 0){
					loguear();
					logFile << "Error" << endl;
				}
				printf("Frecuencia iniciada: %d, Formato: %d, Channel: %d",frec,formato, chan);
			}
		}
	}
	this->texturederecha = loadTexture("TPTaller/imagenes/gusanitoderecha.png", this->renderizador);
	this->textureizquierda = loadTexture("TPTaller/imagenes/gusanitoizquierda.png", this->renderizador);
	this->texturederechaNEGRO = loadTexture("TPTaller/imagenes/gusanitonegroder.png", this->renderizador);
	this->textureizquierdaNEGRO = loadTexture("TPTaller/imagenes/gusanitonegroizq.png" , this->renderizador);
	this->flechitaroja = loadTexture("TPTaller/imagenes/flechitaroja.png", this->renderizador);
	this->textureexplosion = loadTexture("TPTaller/imagenes/explosion.png", this->renderizador);
	return success;
}

void Dibujador::close(){
	SDL_DestroyRenderer(renderizador);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	SDL_Quit();
}


void Dibujador::dibujarMensaje(){
	char mensaje[90];
	sprintf (mensaje, "Se cerro el server, la ventana se cerrara en: %d segundos", this->contador_cerrarse);
	printf( " MENSAJE ES %s \n", mensaje);
	this->contador_cerrarse--;
	SDL_Color color = {0,0,0};
	SDL_Texture* mensaje_final = RenderText(mensaje, "TPTaller/imagenes/Hilarious.ttf", color, 60);
	renderTexture2(mensaje_final, this->renderizador, (this->escalado_x / 2)*2.5  , (this->escalado_y /2)*6 , 500, 50 );
	SDL_DestroyTexture(mensaje_final);
	SDL_Delay(1000);
}

int Dibujador::getContadorCerrarse(){
	return this->contador_cerrarse;
}

void Dibujador::mostrarCartelTurno(int nro_jugador, char* nombre){
	char mensaje[90];
	sprintf (mensaje, "Es el turno de : %s ", nombre);
	SDL_Color color = {0,0,0};
	SDL_Texture* mensaje_final = RenderText(mensaje, "TPTaller/imagenes/Hilarious.ttf", color, 60);
	renderTexture2(mensaje_final, this->renderizador, (this->escalado_x / 2)*2.5  , 0 , 500, 50 );
	SDL_DestroyTexture(mensaje_final);

}
void Dibujador::mostrarCartelEsperar(){
	char mensaje[90];
	sprintf (mensaje, "Esperando a que se conecten otros jugadores");
	SDL_Color color = {0,0,0};
	SDL_Texture* mensaje_final = RenderText(mensaje, "TPTaller/imagenes/Hilarious.ttf", color, 60);
	renderTexture2(mensaje_final, this->renderizador, (this->escalado_x / 2)*2.5  , 0 , 500, 50 );
	SDL_DestroyTexture(mensaje_final);

}

void Dibujador::mostrarCartel(char* mensaje, int posx, int posy, int x, int y){
	SDL_Color color = {0,0,0};
	if (posx == -1){
		posx = (this->escalado_x /2)*2.5;
	}
	SDL_Texture* mensaje_final = RenderText(mensaje, "TPTaller/imagenes/Hilarious.ttf", color, 60);
	renderTexture2(mensaje_final, this->renderizador, posx  , posy , x, y );
	SDL_DestroyTexture(mensaje_final);

}

void Dibujador::mostrarReloj(int reloj){
	reloj=reloj/1000;
	int aux=60;
	aux-=reloj;
	SDL_Color color = {0,0,0};
	char mensaje[3];
	sprintf(mensaje,"%d", aux);
	SDL_Texture* mensaje_final = RenderText(mensaje, "TPTaller/imagenes/Hilarious.ttf", color, 60);
	renderTexture2(mensaje_final, this->renderizador, (this->escalador->getVentanaX())-40 , 0 ,40 , 40 );
	SDL_DestroyTexture(mensaje_final);

}

void Dibujador::setMusica(Musica* musica){
	this->music = musica;
}


void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}





void Dibujador::borrarExplosion(b2Vec2 posicion, float32 radio){

	SDL_Surface* surf = this->surfaceTierra;
	int alto = surf->h;
	int ancho = surf->w;
	Uint32* pixeles = (Uint32*) surf->pixels;

	int pos[2];
	pos[0] = posicion.x * (ancho / escalador->getEscalaX());
	pos[1] = posicion.y * (alto / escalador->getEscalaY());

	int rad[2];
	rad[0] = radio * (ancho / escalador->getEscalaX());
	rad[1] = radio * (alto / escalador->getEscalaY());

	int pixel[2];

	for (int i= 0-rad[0]; i<rad[0]; i++){
		for (int j=0-rad[1]; j<rad[1]; j++){
			pixel[0] = pos[0] + i;
			pixel[1] = pos[1] + j;

			float32 x = (( ((float32) i*i) / ((float32) rad[0]*rad[0])));
			float32 y = (( ((float32) j*j) / ((float32) rad[1]*rad[1])));
			//printf(" LOS PUNTOS SON %d, %d y %d,%d y dan x=%f y=%f x+y = %f \n", i,j,rad[0],rad[1],x,y,x+y);
			if ( (x+y) <= 1) {
				putpixel(surfaceTierra,pixel[0],pixel[1],0);
			}
		}
	}
	SDL_UpdateTexture(this->textureTierra,NULL,pixeles,this->surfaceTierra->pitch);

}

void Dibujador::dibujarExplosion(){
	printf(" dibuja la explosion \n");
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion_explosion);
//	int anchoPX = escalador->aplicarZoomX( tam.x);
//	int altoPX = escalador->aplicarZoomY(tam.y);
	int x = posicionVentanada->x - 100/2;
	int y = posicionVentanada->y - 100/2;
//	renderTexture2(textureexplosion, this->renderizador, x, y, this->radio_explosion *25, this->radio_explosion * 15 );

	renderTexture2(textureexplosion, this->renderizador, x, y, 100, 100 );
	//renderTexture(textureexplosion, this->renderizador, 500, 500, 50, 50 );
	delete posicionVentanada;

}

void Dibujador::setPosicionExplosion(b2Vec2 pos, int radio){
	b2Vec2 posicion_nueva = pos;
	this->posicion_explosion=posicion_nueva;
	this->contador_explosion=50;
	this->radio_explosion = radio;
}

bool Dibujador::dibujar_explosion(){
//	if(this->contador_explosion==300){
//		this->contador_explosion=0;
//		return false;
//	}
	if(this->contador_explosion > 0){
		this->contador_explosion--;
		return true;

	}
	else{

		return false;
	}
	//return this->explosion;
	//contador_explosicion++;
}



