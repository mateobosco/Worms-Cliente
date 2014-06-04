#include "Escalador.h"

#define BORDE1 100
#define BORDE2 50
#define VEL1 3
#define VEL2 7
#define VELZOOM 5


Escalador::Escalador(int ventanaX, int ventanaY, float32 escalaX, float32 escalaY, int ancho_esc, int alto_esc) {
	this->pixelesX = ancho_esc;
	this->pixelesY = alto_esc;
	this->centroX = pixelesX/2;
	this->centroY = pixelesY/2;
	this->ventanaX = ventanaX;
	this->ventanaY = ventanaY;
	this->escalaX = escalaX;
	this->escalaY = escalaY;
	this->offsetX = (pixelesX - ventanaX)/2;
	this->offsetY = (pixelesY - ventanaY)/2;
	this->zoom = 100;
}

Escalador::~Escalador() {

}

int Escalador::getPixelX(){
	return this->pixelesX;
}
int Escalador::getPixelY(){
	return this->pixelesY;
}
int Escalador::getVentanaX(){
	return this->ventanaX;
}
int Escalador::getVentanaY(){
	return this->ventanaY;
}
int Escalador::getOffsetX(){
	return this->offsetX;
}
int Escalador::getOffsetY(){
	return this->offsetY;
}

float32 Escalador::getEscalaX(){
	return this->escalaX;
}
float32 Escalador::getEscalaY(){
	return this->escalaY;
}

b2Vec2 Escalador::escalarPosicion(b2Vec2 posicionPixel){

	float32 escaladoX = ((posicionPixel.x+offsetX ) *((float)100/zoom) ) * ((float)escalaX/pixelesX);
	float32 escaladoY = ((posicionPixel.y+offsetY ) *((float)100/zoom) ) * ((float)escalaY/pixelesY);
	b2Vec2 escalado = b2Vec2(escaladoX,escaladoY);
	return escalado;
}

b2Vec2* Escalador::pixelarPosicion(b2Vec2 posicionEscalar){
	int pixelX = posicionEscalar.x / (this->escalaX/this->pixelesX);
	int pixelY = posicionEscalar.y / (this->escalaY/this->pixelesY);
	b2Vec2* pixelado = new b2Vec2(pixelX,pixelY);
	return pixelado;
}

//NO SE USA TODO
b2Vec2* Escalador::ventanarPosicion(b2Vec2 posicionEscalar){
	int ventanadoX = ventanarEnX(posicionEscalar.x);
	int ventanadoY = ventanarEnY(posicionEscalar.y);
	b2Vec2* pixelado = new b2Vec2(ventanadoX,ventanadoY);
	return pixelado;
}

//No se usa todo
Sint16* Escalador::pixelarVectorX(float32* vec, int cantidad){
	int i;
	Sint16* pixelado = new Sint16[cantidad];
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		pixelado[i] = numero * (this->pixelesX / this->escalaX);
	}
	return pixelado;
}

//NO SE USA TODO
Sint16* Escalador::pixelarVectorY(float32* vec, int cantidad){
	int i;
	Sint16* pixelado = new Sint16[cantidad];
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		pixelado[i] =  numero * (this->pixelesY / this->escalaY);
	}
	return pixelado;
}


int Escalador::ventanarEnX(float32 x){
	int valor = (int) (x *(pixelesX/escalaX));
	valor = valor - offsetX;
	//valor = valor+zoom;
	return valor;
}

int Escalador::ventanarEnY(float32 y){
	int valor = (int) (y *(pixelesY/escalaY));
	valor = valor - offsetY;
	//valor = valor+zoom;
	return valor;
}

//NO SE USA TODO
Sint16* Escalador::ventanarVectorX(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad];
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = ventanarEnX(numero);
	}
	return ventanado;
}

//NO SE USA TODO
Sint16* Escalador::ventanarVectorY(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad];
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = ventanarEnY(numero);
	}
	return ventanado;
}

Sint16* Escalador::aplicarZoomXVector(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad];
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = aplicarZoomXaEscala(numero);
	}
	return ventanado;
}

Sint16* Escalador::aplicarZoomYVector(float32* vec, int cantidad){
	int i;
	Sint16* ventanado = new Sint16[cantidad];
	for (i = 0 ; i < cantidad ; i++){
		float32 numero = vec[i];
		ventanado[i] = aplicarZoomYaEscala(numero);
	}
	return ventanado;
}


int Escalador::aplicarZoomX(float32 valor){
	int n = (int) (valor *(pixelesX/escalaX))*((float32)zoom/100);
	return n;
}

int Escalador::aplicarZoomY(float32 valor){
	int n = (int) (valor *(pixelesY/escalaY))*((float32)zoom/100);
	return n;
}

int Escalador::aplicarZoomXaEscala(float32 valor){
	int n = (int) ((valor *(pixelesX/escalaX)))*((float32)zoom/100) ;
	n = n - offsetX;
	return n;
}

int Escalador::aplicarZoomYaEscala(float32 valor){
	int n = (int) ((valor *(pixelesY/escalaY)))*((float32)zoom/100) ;
	n = n - offsetY;
	return n;
}

int Escalador::aplicarZoomXaPix(int valor){
	int n = (int) (valor)*((float32)zoom/100) ;
	n = n - offsetX;
	return n;
}

int Escalador::aplicarZoomYaPix(int valor){
	int n = (int) (valor)*((float32)zoom/100);
	n = n - offsetY;
	return n;
}

//NO SE USA TODO
b2Vec2* Escalador::aplicarZoomPixel(b2Vec2 posicionEscalar){
	int ventanadoX = aplicarZoomXaPix(posicionEscalar.x);
	int ventanadoY = aplicarZoomYaPix(posicionEscalar.y);
	b2Vec2* pixelado = new b2Vec2(ventanadoX,ventanadoY);
	return pixelado;
}

b2Vec2* Escalador::aplicarZoomPosicion(b2Vec2 posicionEscalar){
	int ventanadoX = aplicarZoomXaEscala(posicionEscalar.x);
	int ventanadoY = aplicarZoomYaEscala(posicionEscalar.y);
	b2Vec2* pixelado = new b2Vec2(ventanadoX,ventanadoY);
	return pixelado;
}

void Escalador::moverDerecha(int n){
//	if (((((offsetX + ventanaX-centroX)*(float)100/zoom))+centroX) >= pixelesX-5) return;
	if ((offsetX + ventanaX)/((float32) zoom/100) > pixelesX -5) return;
	offsetX += n;
}

void Escalador::moverIzquierda(int n){
//	if (((((offsetX+centroX)*(float32)zoom/100))-centroX) <= 5) return;
	if (offsetX < 5) return;
	offsetX -=n;
}

void Escalador::moverArriba(int n){
//	if (((((offsetY+centroY)*(float32)zoom/100))-centroY) <= 5) return;
	if (offsetY<5) return;
	offsetY -=n;
}

void Escalador::moverAbajo(int n){
//	if (((((offsetY + ventanaY-centroY)*(float32)100/zoom))+centroY) >= pixelesY-5) return;
	if ((offsetY + ventanaY)/((float32) zoom/100) > pixelesY -5) return;

	offsetY +=n;
}

int Escalador::zoomAlejar(){
	int zom = zoom - VELZOOM;
	if (zom <= 10) return -5;
	int offX = (offsetX -centroX ) / ((float32) (100 - VELZOOM) /100) + centroX * ((float32) (100 - VELZOOM) /100);
	int offY = (offsetY -centroY ) / ((float32) (100 - VELZOOM) /100) + centroY * ((float32) (100 - VELZOOM) /100);


//
//	if (offX < 0) return -1;
//	if (offY < 0) return -1;
//	if ((offX + ventanaX)/((float32) zom/100) > pixelesX -5) return -1;
//	if ((offY + ventanaY)/((float32) zom/100) > pixelesY -5) return -1;

	while (offX < 0){
		offX ++;
	}
	while (offY < 0){
		offY ++;
	}
	while ((offX + ventanaX)/((float32) zom/100) > pixelesX ){
		offX --;
		if (offX < 1) return -1;
	}
	while ((offY + ventanaY)/((float32) zom/100) > pixelesY ){
		if (offY < 1) return -1;
		offY --;
	}




	offsetX = offX;
	offsetY = offY;


	zoom -= VELZOOM;
	return 0;
}

int Escalador::zoomAcercar(){
	if (zoom >= 200) return -5;
	int zom = zoom + VELZOOM;
	int offX = (offsetX -centroX ) / ((float32) (100 + VELZOOM) /100) + centroX * ((float32) (100 + VELZOOM) /100);
	int offY = (offsetY -centroY ) / ((float32) (100 + VELZOOM) /100) + centroY * ((float32) (100 + VELZOOM) /100);



//	if (offX < 0) return -1;
//	if (offY < 0) return -1;
//	if ((offX + ventanaX)/((float32) zom/100) > pixelesX ) return -1;
//	if ((offY + ventanaY)/((float32) zom/100) > pixelesY ) return -1;

	while (offX < 0 && offX < 20){
		offX ++;
	}
	while (offY < 0 && offY < 20){
		offY ++;
	}
	while ((offX + ventanaX)/((float32) zom/100) > pixelesX-20 ){
		offX --;
		if (offX < 1) return -1;
	}
	while ((offY + ventanaY)/((float32) zom/100) > pixelesY-20 ){
		if (offY < 1) return -1;
		offY --;
	}



	offsetX = offX;
	offsetY = offY;

	this->zoom += VELZOOM;
	return 0;
}


int Escalador::getZoom(){
	return zoom;
}

void Escalador::hacerZoom(int* posicion_mouse_scroll){


	if (posicion_mouse_scroll[2] == 1){
//		centroX = abs(ventanaX - posicion_mouse_scroll[0]);
//		centroY = abs(ventanaY - posicion_mouse_scroll[1]);
		centroX = posicion_mouse_scroll[0];
		centroY = posicion_mouse_scroll[1];

		int resultado = this->zoomAcercar();





	}
	if (posicion_mouse_scroll[2] == -1){
//		centroX = abs(ventanaX - posicion_mouse_scroll[0]);
//		centroY = abs(ventanaY - posicion_mouse_scroll[1]);
		centroX = posicion_mouse_scroll[0];
		centroY = posicion_mouse_scroll[1];


		int resultado = this->zoomAlejar();
		if (resultado == 0) return;

	}



}
void Escalador::moverVentana(int* posicion_mouse){
	if (posicion_mouse[0] > (ventanaX - BORDE1) ){
		this->moverDerecha(VEL1);
	}
	if (posicion_mouse[1] > (ventanaY - BORDE1) ){
		this->moverAbajo(VEL1);
	}
	if (posicion_mouse[0] <  BORDE1 && posicion_mouse[0]>=0){
		this->moverIzquierda(VEL1);
	}
	if (posicion_mouse[1] <  BORDE1 && posicion_mouse[1]>=0 ){
		this->moverArriba(VEL1);
	}
	if (posicion_mouse[0] > (ventanaX - BORDE2) ){
		this->moverDerecha(VEL2);
	}
	if (posicion_mouse[1] > (ventanaY - BORDE2) ){
		this->moverAbajo(VEL2);
	}
	if (posicion_mouse[0] <  BORDE2 && posicion_mouse[0]>=0){
		this->moverIzquierda(VEL2);
	}
	if (posicion_mouse[1] <  BORDE2 && posicion_mouse[1]>=0 ){
		this->moverArriba(VEL2);
	}
	printf("los offset son %d %d y el zoom %d  y los CENTROS SON %d %d \n", offsetX, offsetY, zoom, centroX,centroY);

}
