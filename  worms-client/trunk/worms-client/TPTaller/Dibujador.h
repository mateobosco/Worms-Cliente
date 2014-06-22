#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_pixels.h>
#include <stdexcept>
//#include <SDL2/SDL_mixer.h>

#include "Agua.h"

#include "Musica.h"

#define PI 3.14159265

extern ofstream logFile;

class Dibujable;
class Figura;
class Circulo;
class Poligono;
class Rectangulo;
class Personaje;
struct structFigura;
struct structPersonaje;
struct structPaquete;
struct structInicial;

class Dibujador {

public:
	Dibujador();
	Dibujador(SDL_Renderer* renderer, Escalador* esc);
	~Dibujador();
	int limpiarFondo();
	void dibujarFiguras(Figura** figuras, int cantidad);
	void dibujarPersonajes(Personaje** personajes, int cantidad);

	int dibujarCirculo(Circulo* circulo);
	int dibujarPoligono(Poligono* poligono);
	int dibujarRectangulo(Rectangulo* rectangulo);
	int dibujarPersonaje(Personaje* personaje);
	SDL_Texture* dibujarPersonaje2(Personaje* personaje);
	SDL_Renderer* getRenderizador();
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
	void renderTexture2(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
	SDL_Texture* dibujar_cielo(Escalador* escalador, std::string path, int hasta);
	SDL_Texture* dibujarAgua(Escalador* escalador, Agua* agua);
	SDL_Texture* dibujar_tierra(Escalador* escalador, std::string path);
	void iniciarFondo(Agua* agua, std::string pathCielo, std::string pathTierra);
	void dibujarFondo();
	int dibujarPaqueteFigura(structFigura paquete);
	void dibujarPaquetePersonaje(structPersonaje paquete, char* nombre_jugador, bool duenio, int cliente_id, float aux, int potencia);
	SDL_Texture* RenderText(std::string message, std::string fontFile,  SDL_Color color, int fontSize);
	bool init();
	void close();
	void actualizar();
	void dibujarPaquete(structPaquete* paquete, char* nombre_cliente, int id_cliente, float aux);
	void dibujarMensaje();
	void dibujar_agua(Agua* agua);
	int getContadorCerrarse();
	void hacerZoom(int* pos);
	void alejarZoom(int* pos);
	void mostrarMenuArmas(int x, int y);
	void mostrarCartelTurno(int nro_jugador, char* nombre);
	void mostrarCartelEsperar();
	void mostrarCartel(char* mensaje, int posx, int posy, int x, int y);
	void mostrarReloj(int reloj);
	void renderTexture3(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo, int punto1, int punto2);
	void renderTexture5(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo, int punto1, int punto2);
	void renderTexture4(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo);
	void borrarExplosion(b2Vec2 pos, float32 radio);
	void renderTexture6(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, int angulo, int punto1, int punto2);
	const char* mostrarPantallaPrincial();
	int loadFromRenderedText(SDL_Texture* texture, std::string textureText, SDL_Color textColor );
	void mostrarLoading();
	void mostrarImagenPrincipal();



	void dibujarProyectil(int tipo_proyectil, b2Vec2 posicion_proyectil, b2Vec2 direccion_proyectil, b2Vec2 tamanio, int contador_segundos, double angulo);
	void renderTextureCenter(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, double angulo);
	void setMusica(Musica* musica);
	//void borrarExplosion(b2Vec2 posicion, float32 radio);
	SDL_Texture* loadTextureTierra(const std::string &path, SDL_Renderer *ren);
	void dibujarExplosion();
	void dibujarExplosionHoly();
	bool explosion;
	void setPosicionExplosion(b2Vec2 posicion, int radio);
	bool dibujar_explosion();
	void dibujarPotencia(int potencia);
	void dibujarViento(float32 viento);
	void resetearEscenario(std::string pathTierra);
	void aplicarExplosiones(structInicial* paqueteInicial);
	int tipo_explosion;

protected:
	Escalador* escalador;
	SDL_Renderer *renderizador;
	float32 escalado_x, escalado_y;
	Musica* music;

private:
	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	//SDL_Texture* loadTextureTierra(const std::string &path, SDL_Renderer *ren);
	SDL_Texture* textureCielo;
	SDL_Texture* textureAgua;
	SDL_Texture* textureTierra;
	SDL_Surface* surfaceTierra;
	SDL_Texture* flechitaroja;
	SDL_Texture* bazooka;
	SDL_Texture* granada;
	SDL_Texture* dinamita;
	SDL_Texture* holy;
	SDL_Texture* kamikaze;
	SDL_Texture* patada;
	SDL_Texture* mira;
	SDL_Texture* textureexplosion;
	//SDL_Surface* surfaceTierra;
	SDL_Window* window;
	SDL_Texture* texturederecha;
	SDL_Texture* textureizquierda;
	SDL_Texture* texturederechaNEGRO;
	SDL_Texture* textureizquierdaNEGRO;
	SDL_Texture* texturamuerto;
	SDL_Texture* texturahundido;
	SDL_Texture* texturaFondo;
	float32 oscilaragua;
	int contador_cerrarse;
	int escalaZoom;
	int radio_explosion;
	b2Vec2 posicion_explosion;
	int contador_explosion;


};

#endif /* DIBUJADOR_H_ */
