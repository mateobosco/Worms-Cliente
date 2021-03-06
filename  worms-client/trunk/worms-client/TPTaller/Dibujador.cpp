#include "Dibujador.h"
#include "circulo.h"
#include "poligono.h"
#include "rectangulo.h"
#include "Personaje.h"
#include "Paquete.h"
#include "DibujadorExplosion.h"

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
	this->contador_explosion = 0;
	this->surfaceTierra=NULL;


}

Dibujador::~Dibujador(){
	if(this->escalador) delete this->escalador;
	delete this->dibujadorExp;
	if(this->textureAgua) SDL_DestroyTexture(this->textureAgua);
	if(this->textureCielo) SDL_DestroyTexture(this->textureCielo);
	if (this->textureTierra) SDL_DestroyTexture(textureTierra);
	if (this->texturederecha) SDL_DestroyTexture(texturederecha);
	if (this->textureizquierda) SDL_DestroyTexture(textureizquierda);
	if (this->texturederechaNEGRO) SDL_DestroyTexture(texturederechaNEGRO);
	if (this->textureizquierdaNEGRO) SDL_DestroyTexture(textureizquierdaNEGRO);
	if (this->vidaRoja) SDL_DestroyTexture(this->vidaRoja);
	if (this->vidaVerde) SDL_DestroyTexture(this->vidaVerde);
	if (this->surfaceTierra) SDL_FreeSurface(this->surfaceTierra);

//	if (this->surfaceTierra) SDL_FreeSurface(this->surfaceTierra);
//	if (this->dibujadorExp) delete this->dibujadorExp;
	this->close();
}


int Dibujador::loadFromRenderedText(SDL_Texture* texture, std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	//free();
	//SDL_Texture* mTexture;
	//Render text surface
	TTF_Font* gFont = TTF_OpenFont( "TPTaller/imagenes/Hilarious.ttf", 28 );
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	int mWeight = 0;
	int mHeight = 0;
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface( this->renderizador, textSurface );
		if( texture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWeight = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	TTF_CloseFont(gFont);
	return mWeight;
	//Return success
	//return texture != NULL;
}


void Dibujador::mostrarImagenPrincipal(){
	int transparencia=0;
	SDL_Texture * texturaFondo2 = this->loadTexture("TPTaller/imagenes/nuevainicial.png", this->renderizador);
	while(transparencia<=255){
		SDL_RenderClear(  this->renderizador );
		SDL_SetTextureAlphaMod(texturaFondo2, transparencia);
		this->renderTexture(texturaFondo2, this->renderizador, 0,0,800,600);
		transparencia+=2;
		SDL_RenderPresent(  this->renderizador );

	}
}
void Dibujador::mostrarImagenPrincipal2(){
	int transparencia=255;
	SDL_Texture * texturaFondo2 = this->loadTexture("TPTaller/imagenes/nuevainicial.png", this->renderizador);
	while(transparencia>=0){
		SDL_RenderClear(  this->renderizador );
		SDL_SetTextureAlphaMod(texturaFondo2, transparencia);
		this->renderTexture(texturaFondo2, this->renderizador, 0,0,800,600);
		transparencia-=5;
		SDL_RenderPresent(  this->renderizador );

	}
}


const char* Dibujador::mostrarPantallaPrincial(){
	SDL_Texture* fondo = this->loadTexture("TPTaller/imagenes/nuevainicial.png", this->renderizador);
	SDL_SetTextureAlphaMod(fondo, 150);
	bool quit = false;
	SDL_Texture* Texture=NULL;
	int ancho_text = 0;
	//Event handler
	SDL_Event e;
	int contador=20;
	//Set text color as black
	SDL_Color textColor = { 0, 0, 0, 0xFF };
	//The current input text.
	std::string inputText = "";
	char mensaje_nombre[40];
	char mensaje_ip[40];
	strcpy(mensaje_nombre, "Ingrese su nombre : ");
	strcpy(mensaje_ip, "Ingrese la ip del servidor : ");
	//this->mostrarCartel(mensaje_nombre,200,200,100,50 );
	//this->mostrarCartel(mensaje_ip,200,400,100,50 );
	SDL_Texture* textura_nombre=this->RenderText(mensaje_nombre, "TPTaller/imagenes/Hilarious.ttf", textColor, 40);
	SDL_Texture* textura_ip=this->RenderText(mensaje_ip, "TPTaller/imagenes/Hilarious.ttf", textColor, 40);
	//Enable text input
	SDL_StartTextInput();
	//While application is running
	while( !quit )
	{
		//The rerender text flag
		bool renderText = false;
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
//			if( e.type == SDLK_x )
//			{
//				quit = true;
//			}
			//Special key input
			if( e.type == SDL_KEYDOWN )
			{
				//Handle backspace
				if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
				{
					//lop off character
					inputText.pop_back();
					renderText = true;
				}
				if( e.key.keysym.sym == SDLK_RETURN )
							{
								quit = true;
							}
				//Handle copy
				else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
				{
					inputText = SDL_SetClipboardText( inputText.c_str() );
				}
				//Handle paste
				else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
				{
					inputText = SDL_GetClipboardText();
					renderText = true;
				}
			}
			//Special text input event
			else if( e.type == SDL_TEXTINPUT )
			{
				//Not copy or pasting
				if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
				{
					//Append character
					//printf(" AGREGA UN CARACTER \n");
					inputText += e.text.text;
					contador++;
					renderText = true;
				}
			}
		}

		//Rerender text if needed
		if( renderText )
		{
			//Text is not empty
			if( inputText != "" )
			{
				//Render new text

				ancho_text = this->loadFromRenderedText(Texture, inputText.c_str(), textColor );
			}
			//Text is empty
			else
			{
				//Render space texture
				ancho_text = this->loadFromRenderedText(Texture, " ", textColor );
			}
		}

		//Clear screen
		//SDL_SetRenderDrawColor(  this->renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(  this->renderizador );
		//char final[90];
		const char *final = inputText.c_str();
		SDL_Texture* text_final=this->RenderText(final, "TPTaller/imagenes/Hilarious.ttf", textColor, 40);
		//Render text textures
		this->renderTexture(fondo, this->renderizador, 0,0,800,600);
		this->renderTexture(textura_nombre, this->renderizador, 200,500,150,50);
		//this->renderTexture(textura_ip, this->renderizador, 200,300,150,50);
		this->renderTexture(text_final, this->renderizador, 350,500, ancho_text,50);
		if(text_final) SDL_DestroyTexture(text_final);
		//gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
		//gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );

		//Update screen
		SDL_RenderPresent(  this->renderizador );
	}
	printf(" EL INPUT TEXT QUEDO: %s \n", inputText.c_str());
	//Disable text input
	SDL_StopTextInput();
	SDL_DestroyTexture(fondo);
	return inputText.c_str();
}


void Dibujador::mostrarLoading(){
	SDL_Texture* loading = loadTexture("TPTaller/imagenes/loading", this->renderizador);
	this->renderTexture(loading, this->renderizador, 0 ,0 ,800,600);// TODO hardcodeado.
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
	if(posicionVentanada) delete posicionVentanada;
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
	if(vecX) delete[] vecX;
	if(vecY) delete[] vecY;
	if(vecYventanado) delete[] vecYventanado;
	if(vecXventanado) delete[] vecXventanado;
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
	if(vecX) delete[] vecX;
	if(vecY) delete[] vecY;
	if(vecYventanado) delete[] vecYventanado;
	if(vecXventanado) delete[] vecXventanado;
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
    if(surf) SDL_FreeSurface(surf);
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
	if(posicionVentanada) delete posicionVentanada;
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
		logFile <<"    Error    " <<"\t RenderCopy falló renderTexture " << endl;
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
		logFile <<"    Error    " <<"\t RenderCopy falló renderTexture2 "<< SDL_GetError() << endl;
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
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, (angulo + 15), &point,SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló renderTexture3" << endl;
	}
}

void Dibujador::renderTextureCenter(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, double angulo){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, angulo, NULL, SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló renderTextureCenter " << endl;
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
		logFile <<"    Error    " <<"\t RenderCopy falló renderTexture5 " << endl;
	}
}

void Dibujador::renderTexture6(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo, int punto1, int punto2){
	SDL_Rect dst;
	float seno_aux = (sin(angulo * PI/180));
	if(seno_aux < 0)
		seno_aux = -seno_aux;
	dst.x = x - cos(angulo * PI/180) * this->escalaZoom + (20 * seno_aux);
	dst.y = y + sin(angulo * PI/180) * this->escalaZoom;
	dst.w = w;
	dst.h = h;
	SDL_Point point;
	point.x=punto1;
	point.y=punto2;
	if(SDL_RenderCopyEx(ren, tex, NULL, &dst, angulo, &point,SDL_FLIP_NONE)!=0){
		loguear();
		logFile <<"    Error    " <<"\t RenderCopy falló  renderTexture6" << endl;
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
		logFile <<"    Error    " <<"\t RenderCopy falló renderTexture4" << endl;
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
        logFile << "No se pudo cargar la imagen en loadTexture: " << path.c_str() << "! SDL_image Error: " <<  IMG_GetError() << endl;
    } else{
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if(newTexture == NULL){
        	loguear();
        	logFile << "No se pudo crear textura de en loadTexture: " << path.c_str() << "! SDL Error: " <<  SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

SDL_Texture* Dibujador::loadTextureTierra(const std::string &path, SDL_Renderer *ren){
    SDL_Texture* newTexture = NULL;
//    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Surface* loadedSurface = IMG_Load("/home/mateo/git/Worms-Cliente/ worms-client/trunk/worms-client/TPTaller/imagenes/tierranuevaa.png");
    this->surfaceTierra = loadedSurface;

    if(loadedSurface == NULL){
        loguear();
        logFile << "No se pudo cargar la imagen  en loadTextureTierra: " << path.c_str() << "! SDL_image Error: " <<  SDL_GetError() << endl;
    } else{
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if(newTexture == NULL){
        	loguear();
        	logFile << "No se pudo crear textura de en loadTextureTierra: " << path.c_str() << "! SDL Error: " <<  SDL_GetError() << endl;
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
	this->vidaRoja = loadTexture("TPTaller/imagenes/roja.png", this->renderizador);
	this->vidaVerde = loadTexture("TPTaller/imagenes/verde.png", this->renderizador);
	this->texturamuerto = loadTexture("TPTaller/imagenes/muerto.png" , this->renderizador);
	this->texturederecha = loadTexture("TPTaller/imagenes/gusanitoderecha.png", this->renderizador);
	this->textureizquierda = loadTexture("TPTaller/imagenes/gusanitoizquierda.png", this->renderizador);
	this->texturederechaNEGRO = loadTexture("TPTaller/imagenes/gusanitonegroder.png", this->renderizador);
	this->textureizquierdaNEGRO = loadTexture("TPTaller/imagenes/gusanitonegroizq.png" , this->renderizador);

}

void Dibujador::dibujarFondo(){
	this->renderTexture(textureCielo, renderizador,0  , 0 , escalador->getPixelX(), escalador->getPixelY() );
	this->renderTexture(textureTierra, renderizador, 0 , 0 , escalador->getPixelX() , escalador->getPixelY() );
	this->dibujadorExp->actualizarExplosion();
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
		if(posicionVentanada) delete posicionVentanada;

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

		if(vecX) delete[] vecX;
		if(vecY) delete[] vecY;
		if(vecYventanado) delete[] vecYventanado;
		if(vecXventanado) delete[] vecXventanado;
	}
	return retorno;
}

void Dibujador::dibujarPaquetePersonaje(structPersonaje paquete, char* nombre_jugador, bool duenio, int cliente_id, float aux, int potencia, int contador, int jugador_turno){

	int dir = paquete.direccion;
	b2Vec2 tam = paquete.tamano;

	bool cargo = false;
	SDL_Texture* gusanito;
	if (dir ==1){
		if(paquete.arma_seleccionada == 3){
			gusanito = loadTexture("TPTaller/imagenes/gusano_tnt_der.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 2){
			gusanito = loadTexture("TPTaller/imagenes/gusanoGranadaDER.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 4){
			gusanito = loadTexture("TPTaller/imagenes/gusanoHolyDER.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 5){
			gusanito = loadTexture("TPTaller/imagenes/suicidader.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 6){
			gusanito = loadTexture("TPTaller/imagenes/patadaDER.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.energia<=0) gusanito = this->texturamuerto;
		else if (paquete.conectado == 0) gusanito = this->texturederechaNEGRO;
		else gusanito = this->texturederecha;

	}
	if (dir == -1){
		if(paquete.arma_seleccionada == 3){
			gusanito = loadTexture("TPTaller/imagenes/gusano_tnt_izq.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 2){
			gusanito = loadTexture("TPTaller/imagenes/gusanoGranadaIZQ.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 4){
			gusanito = loadTexture("TPTaller/imagenes/gusanoHolyIZQ.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 5){
			gusanito = loadTexture("TPTaller/imagenes/suicidaizq.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.arma_seleccionada == 6){
			gusanito = loadTexture("TPTaller/imagenes/patadaIZQ.png", this->renderizador);
			cargo = true;
		}
		else if(paquete.energia<=0) gusanito = this->texturamuerto;
		else if (paquete.conectado == 0) gusanito = this->textureizquierdaNEGRO;
		else gusanito = this->textureizquierda;
	}



	b2Vec2 posicion = paquete.posicion;
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
	int anchoPX = escalador->aplicarZoomX( tam.x);
	int altoPX = escalador->aplicarZoomY(tam.y);
	int x = posicionVentanada->x - anchoPX/2;
	int y = posicionVentanada->y - altoPX/2;
	int w = anchoPX;
	int h = altoPX;

	renderTexture2(gusanito, this->renderizador,x,y ,w , h  );
	if (cargo) SDL_DestroyTexture(gusanito);

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
	if(paquete.seleccionado[jugador_turno] == 1 && duenio){
		this->
		renderTexture2(flechitaroja, this->renderizador, x - anchoPX/((float32)escalador->getZoom()/100), ((y)*aux/100)+(y-140), 80, 80);
//		if (contador < 55) escalador->ubicarPosicion(paquete.posicion);
	}

	char energia[10];
	sprintf(energia,"%d", paquete.energia);

	SDL_Texture* energiatext = RenderText(energia, "TPTaller/imagenes/Hilarious.ttf", vectorcolores[0], 15); // despues preguntar el nombre de cada uno


	renderTexture2(vidaRoja, this->renderizador, x-w/2,y-h*2, w*2,h*2);
	renderTexture2(vidaVerde, this->renderizador, x-w/2, y-h*2, w*2 * paquete.energia/100 ,h*2);
	renderTexture2(energiatext, this->renderizador,  x+w/3,y-h*1.2, w/2,h/2 );


	if(paquete.arma_seleccionada == 1 && paquete.direccion ==1){
		SDL_Texture* bazooka = loadTexture("TPTaller/imagenes/bazooka2.png", this->renderizador);
		renderTexture3(bazooka, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		renderTexture5(mira, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		if(potencia >0){
			this->dibujarPotencia(potencia);
		}
		if(bazooka) SDL_DestroyTexture(bazooka);
		//if(mira) SDL_DestroyTexture(mira);
	}
	if(paquete.arma_seleccionada == 1 && paquete.direccion == -1){
		SDL_Texture* bazooka = loadTexture("TPTaller/imagenes/bazookaizq.png", this->renderizador);
		int angulo = -90 - (-45 + paquete.angulo_arma);
		renderTexture3(bazooka, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, angulo, w,h);
		renderTexture6(mira, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		this->dibujarPotencia(potencia);

		if(bazooka) SDL_DestroyTexture(bazooka);
	}
	if(paquete.arma_seleccionada == 2 && paquete.direccion ==1){
//		SDL_Texture* granada = loadTexture("TPTaller/imagenes/granadader.png", this->renderizador);
//		renderTexture3(granada, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		renderTexture5(mira, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		this->dibujarPotencia(potencia);
//		if(granada) SDL_DestroyTexture(granada);

	}
	if(paquete.arma_seleccionada == 2 && paquete.direccion == -1){
//		SDL_Texture* granada = loadTexture("TPTaller/imagenes/granadaizq.png", this->renderizador);
//		renderTexture3(granada, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, w,h);
		renderTexture6(mira, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		this->dibujarPotencia(potencia);
//		if(granada) SDL_DestroyTexture(granada);
	}

	if(paquete.arma_seleccionada == 4 && paquete.direccion ==1){
//		SDL_Texture* granadaholy = loadTexture("TPTaller/imagenes/granadaholyder.png", this->renderizador);
//		renderTexture3(granadaholy, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		renderTexture5(mira, this->renderizador,x + anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		this->dibujarPotencia(potencia);
//		if(granadaholy) SDL_DestroyTexture(granadaholy);
	}
	if(paquete.arma_seleccionada == 4 && paquete.direccion == -1){
//		SDL_Texture* granadaholy = loadTexture("TPTaller/imagenes/granadaholyizq.png", this->renderizador);
//		renderTexture3(granadaholy, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/100),y-5,w+5,h+5, paquete.angulo_arma, w,h);
		renderTexture6(mira, this->renderizador,x - anchoPX/((float32)escalador->getZoom()/50),y-5,w+5,h+5, paquete.angulo_arma, 0,h);
		this->dibujarPotencia(potencia);
//		if(granadaholy) SDL_DestroyTexture(granadaholy);
	}
	if(paquete.salto) this->music->playSonido(UP);
	if(paquete.movio) this->music->playSonido(WALK);
	if(paquete.perdioVida) this->music->playSonido(VIDA);

	if(energiatext) SDL_DestroyTexture(energiatext);
	if(posicionVentanada) delete posicionVentanada;
}

void Dibujador::dibujarProyectil(int tipo_proyectil, b2Vec2 posicion_proyectil, b2Vec2 direccion_proyectil, b2Vec2 tamanio, int contador_segundos, double angulo, bool* sonido_timer){

	if(tipo_proyectil){
		b2Vec2 posicion = posicion_proyectil;
		b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion);
		int anchoPX = escalador->aplicarZoomX( tamanio.x);
		int altoPX = escalador->aplicarZoomY( tamanio.y);
		int x = posicionVentanada->x - anchoPX/2;
		int y = posicionVentanada->y - altoPX/2;
		escalador->seguidorPosicion(x,y);
		int w = anchoPX;
		int h = altoPX;
		SDL_Texture* misil;
		SDL_Texture* segundos;

		if((tipo_proyectil == 2) || (tipo_proyectil == 3) || (tipo_proyectil == 4)){
			switch (contador_segundos){
				case 5:	{
					segundos = loadTexture("TPTaller/imagenes/segundo_cinco.png", this->renderizador);
					if(!sonido_timer[5]){
						this->music->playSonido(TICK);
						printf("Contador segundos 5: %d \n", contador_segundos);
						sonido_timer[5] = true;
					}
					break;
				}

				case 4:	{
					segundos = loadTexture("TPTaller/imagenes/segundo_cuatro.png", this->renderizador);
					if(!sonido_timer[4]){
						this->music->playSonido(TICK);
						printf("Contador segundos 4: %d \n", contador_segundos);
						sonido_timer[4] = true;
					}
					break;
				}
				case 3: {
					segundos = loadTexture("TPTaller/imagenes/segundo_tres.png", this->renderizador);
					if(!sonido_timer[3]){
						this->music->playSonido(TICK);
						printf("Contador segundos 3: %d \n", contador_segundos);
						sonido_timer[3] = true;
					}
					break;
				}
				case 2: {
					segundos = loadTexture("TPTaller/imagenes/segundo_dos.png", this->renderizador);
					if(!sonido_timer[2]){
						this->music->playSonido(TICK);
						printf("Contador segundos 2: %d \n", contador_segundos);
						sonido_timer[2] = true;
					}
					break;
				}
				case 1: {
					segundos = loadTexture("TPTaller/imagenes/segundo_uno.png", this->renderizador);
					if(!sonido_timer[1]){
						this->music->playSonido(TICK);
						printf("Contador segundos 1: %d \n", contador_segundos);
						sonido_timer[1] = true;
					}
					break;
				}
				case 0: {
					segundos = loadTexture("TPTaller/imagenes/segundo_cero.png", this->renderizador);
					if(!sonido_timer[0]){
						this->music->playSonido(TICK);
						printf("Contador segundos 0: %d \n", contador_segundos);
						sonido_timer[0] = true;
					}
					break;
				}
				default: {
					segundos = loadTexture("TPTaller/imagenes/segundo_cero.png", this->renderizador);
					this->music->playSonido(TICK);
					printf("Contador default %d \n", contador_segundos);
					break;
				}
			}
		}
		if(tipo_proyectil==1){
			float32 angulo_aux = atan2( direccion_proyectil.y, direccion_proyectil.x );
			angulo = angulo_aux * 180/PI;
			if(direccion_proyectil.x>0){
				misil= loadTexture("TPTaller/imagenes/bazooka_misil.png", this->renderizador);
				renderTextureCenter(misil, this->renderizador, x, y, w+5,h+5, angulo-45);
			}
			else{
				misil=loadTexture("TPTaller/imagenes/bazooka_misilizq.png", this->renderizador);
				renderTextureCenter(misil, this->renderizador, x, y, w+5,h+5, angulo-135);
			}
			if(misil) SDL_DestroyTexture(misil);
		}

		if(tipo_proyectil==2){
			misil = loadTexture("TPTaller/imagenes/granadader.png", this->renderizador);
			renderTextureCenter(misil, this->renderizador, x, y, w-1,h-1, angulo-45);
			renderTextureCenter(segundos, this->renderizador, x+16, y-16, w-1,h-1, 0);
			if(misil) SDL_DestroyTexture(misil);
			if(segundos) SDL_DestroyTexture(segundos);
		}

		if(tipo_proyectil==3){
			misil = loadTexture("TPTaller/imagenes/dinamita_chispa.png", this->renderizador);
			renderTextureCenter(misil, this->renderizador, x, y, w-1,h-1, angulo-45);
			renderTextureCenter(segundos, this->renderizador, x+16, y-16, w-1,h-1, 0);
			if(misil) SDL_DestroyTexture(misil);
			if(segundos) SDL_DestroyTexture(segundos);
		}

		if(tipo_proyectil==4){
			misil = loadTexture("TPTaller/imagenes/granadaholyder.png", this->renderizador);
			renderTextureCenter(misil, this->renderizador, x, y, w-1,h-1, angulo-45);
			renderTextureCenter(segundos, this->renderizador, x+16, y-16, w-1,h-1, 0);
			if(misil) SDL_DestroyTexture(misil);
			if(segundos) SDL_DestroyTexture(segundos);
		}
		if(posicionVentanada) delete posicionVentanada;
	}
}
void Dibujador::mostrarMenuArmas(int x, int y, int numero_granadas, int numero_dinamitas, int numero_holys){
	//SDL_Texture* menu = loadTexture("TPTaller/imagenes/armas2.png", this->renderizador);
	renderTexture2(bazooka, this->renderizador, x - 100, y, 100,100);
	SDL_Color color = {255,255,255};
	char cant_granada[10];
	char cant_dinamitas[10];
	char cant_holys[10];


	//strcpy(cant_granada, "x");
	sprintf (cant_granada, "x%d ", numero_granadas);
	sprintf (cant_dinamitas, "x%d ", numero_dinamitas);
	sprintf (cant_holys, "x%d ", numero_holys);

	renderTexture2(granada, this->renderizador, x  , y, 100,100);
	SDL_Texture* cant_grand = RenderText(cant_granada, "TPTaller/imagenes/Hilarious.ttf", color, 20); // despues preguntar el nombre de cada uno
	renderTexture2(cant_grand, this->renderizador, x , y+10 , 50, 20 );
	SDL_DestroyTexture(cant_grand);
	renderTexture2(dinamita, this->renderizador, x - 100, y + 100, 100,100);
	SDL_Texture* cant_din = RenderText(cant_dinamitas, "TPTaller/imagenes/Hilarious.ttf", color, 20); // despues preguntar el nombre de cada uno
	renderTexture2(cant_din, this->renderizador, x - 100 , y + 110 , 50, 20 );
	SDL_DestroyTexture(cant_din);
	renderTexture2(holy, this->renderizador, x , y +  100, 100,100);
	SDL_Texture* cant_holy = RenderText(cant_holys, "TPTaller/imagenes/Hilarious.ttf", color, 20); // despues preguntar el nombre de cada uno
	renderTexture2(cant_holy, this->renderizador, x , y+110 , 50, 20 );
	SDL_DestroyTexture(cant_holy);
	renderTexture2(kamikaze, this->renderizador, x - 100 , y + 200, 100,100);
	renderTexture2(patada, this->renderizador, x , y +200, 100,100);
}

void Dibujador::dibujarPaquete(structPaquete* paquete, char* nombre_cliente, int cliente_id, float aux, bool* sonido_timer){
//	int figuras = paquete->cantidad_figuras;
	int personajes = paquete->cantidad_personajes;
//	for (int i = 0 ; i < figuras ; i++ ){
//		structFigura* vector = paquete->vector_figuras;
//		this->dibujarPaqueteFigura(vector[i]);
//	}

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
	int potencia = paquete->potencia;
	for (int j = 0 ; j < personajes ; j ++){
		if (cliente_id == vector1[j].id_jugador){
			if(paquete->show_proyectil) vector1[j].arma_seleccionada = 0;
			this->dibujarPaquetePersonaje(vector1[j], nombre_cliente, true, cliente_id, aux, potencia, paquete->reloj, paquete->turno_jugador ); // es propio
		}
		else{
			this->dibujarPaquetePersonaje(vector1[j], nombre_cliente, false, cliente_id, aux, potencia, paquete->reloj, paquete->turno_jugador); // no es propio
		}
	}
	//if(paquete->salto) this->music->playSonido(UP);
	//if(paquete->movio) this->music->playSonido(WALK);
	if(paquete->show_proyectil){
		this->dibujarProyectil(paquete->tipo_proyectil, paquete->posicion_proyectil, paquete->direccion_proyectil, paquete->tamanio_proyectil, paquete->contador_segundos, paquete->angulo, sonido_timer);
	}
	else escalador->pararSeguidor();
}


void Dibujador::dibujarPotencia(int potencia){

	rectangleRGBA(this->renderizador, 20 , escalador->getVentanaY()-50, 121, escalador->getVentanaY()-20,0,0,0,255);
	if (potencia <= 0 || potencia > 100 ) return;
	boxRGBA(this->renderizador, 20 , escalador->getVentanaY()-50, 20 + potencia, escalador->getVentanaY()-20,255,0,0,255);

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
					logFile << "  SDL_mixer no pudo ser inicializado! SDL_mixer Error: " << Mix_GetError() << endl;
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
			}
		}
	}

	this->flechitaroja = loadTexture("TPTaller/imagenes/flechitaroja.png", this->renderizador);
//	this->textureexplosion = loadTexture("TPTaller/imagenes/explosion.png", this->renderizador);
	this->dibujadorExp = new DibujadorExplosion(this->renderizador,this->escalador);



	return success;
}

void Dibujador::close(){
	SDL_DestroyRenderer(renderizador);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

void Dibujador::dibujarMensaje(){
	char mensaje[90];
	sprintf (mensaje, "Se cerro el server, la ventana se cerrara en: %d segundos", this->contador_cerrarse);
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
	renderTexture2(mensaje_final, this->renderizador, 100  , 0 , 500, 50 );
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
			if (pixel[0] > ancho) continue;
			if (pixel[1] > alto) continue;
			float32 x = (( ((float32) i*i) / ((float32) rad[0]*rad[0])));
			float32 y = (( ((float32) j*j) / ((float32) rad[1]*rad[1])));
			if ( (x+y) <= 1) {
				putpixel(surfaceTierra,pixel[0],pixel[1],0);
			}
		}
	}
	SDL_UpdateTexture(this->textureTierra,NULL,pixeles,this->surfaceTierra->pitch);

}

void Dibujador::dibujarExplosion(int tipo){

	this->dibujadorExp->start(tipo,posicion_explosion, (float32) this->radio_explosion);

}

void Dibujador::dibujarExplosionHoly(){
	b2Vec2* posicionVentanada = escalador->aplicarZoomPosicion(posicion_explosion);

	int x = posicionVentanada->x - 100/2;
	int y = posicionVentanada->y - 100/2;

//	renderTexture2(textureexplosion, this->renderizador, x, y, escalador->getVentanaX()/4, escalador->getVentanaY()/3 );
//	renderTexture2(textureexplosion, this->renderizador, x-60, y-50, 240, 200 );

	if(posicionVentanada) delete posicionVentanada;

}

void Dibujador::setPosicionExplosion(b2Vec2 pos, int radio){
	b2Vec2 posicion_nueva = pos;
	this->posicion_explosion=posicion_nueva;
//	this->contador_explosion=SDL_GetTicks();
	this->contador_explosion = 1;
	this->radio_explosion = radio;
}
// todo, cuando se LAGEA queda horrible que explote hasta que se termine el contador, xq tarda un monton
// asi que lo hice intentando que sea en TIEMPO REAL. no sé si está bien hecho.
// EDIT: Al final lo arregle en main de cliente, para que se ejecute una sola vez. [Nahue.]
bool Dibujador::dibujar_explosion(){
//	if (contador_explosion == 0) return false;
//	int resultado = SDL_GetTicks() - contador_explosion;
//	if (resultado <= 3000) return true;
//	else return false;
	if (contador_explosion == 1){
		contador_explosion = 0;
		return true;
	}
	return false;

}

void Dibujador::dibujarViento(float32 viento){
	SDL_Color color = {0,0,0};
	char mensaje[33];//se permiten hasta 5 dígitos para la velocidad.
	if(viento > 0){
		sprintf(mensaje,"Viento oeste a %3.2f km por hora", viento);
	}
	if(viento < 0){
		sprintf(mensaje,"Viento este a %3.2f km por hora", -viento);
	}
	if(viento == 0){
		sprintf(mensaje,"Viento de %3.2f km por hora", viento);
	}
	SDL_Texture* mensaje_final = RenderText(mensaje, "TPTaller/imagenes/Hilarious.ttf", color, 60);
	renderTexture2(mensaje_final, this->renderizador, (this->escalador->getVentanaX()) - 500 , 40 , 300 , 40 );
	SDL_DestroyTexture(mensaje_final);
}

void Dibujador::resetearEscenario(std::string pathTierra){
////	SDL_FreeSurface(this->surfaceTierra);
	printf(" ENTRA EN RESETEAR ESCENARIO (DIBUJADOR) \n");
//	SDL_DestroyTexture(this->textureTierra);
//	this->textureTierra = this->dibujar_tierra(escalador,pathTierra);
	if (this->surfaceTierra) SDL_FreeSurface(this->surfaceTierra);
	this->surfaceTierra = IMG_Load(pathTierra.c_str());
	if (this->textureTierra) SDL_DestroyTexture(this->textureTierra);
	this->textureTierra = SDL_CreateTextureFromSurface(this->renderizador, this->surfaceTierra);
}

void Dibujador::aplicarExplosiones(structInicial* paqueteInicial){
	int cantidad = paqueteInicial->cantidadExplosiones;
	printf("cantidad explosiones:%d \n", cantidad);
	for (int i = 0; i< cantidad; i++){
		structExplosion explosion = paqueteInicial->explosiones[i];
		this->borrarExplosion(explosion.posicion,explosion.radio);
		printf("aplica explosion inical\n");
	}
}



void Dibujador::setGranadasAgotadas(){
	SDL_SetTextureAlphaMod(this->granada, 150);
}

void Dibujador::setHolysAgotadas(){
	SDL_SetTextureAlphaMod(this->holy, 150);


}

void Dibujador::setDinamitasAgotadas(){
	SDL_SetTextureAlphaMod(this->dinamita, 150);

}
