#ifndef JUGADOR_H
#define JUGADOR_H

#include "raylib.h"
#include <vector>

class Jugador {
private:
    Vector2 posicion;
    Vector2 velocidad;
    float ancho, alto;
    float velocidadMovimiento, fuerzaSalto, gravedad;
    float aceleracion, friccion, velocidadSprint;
    bool enSuelo;
    Color color;
    int teclaIzquierda, teclaDerecha, teclaSalto, teclaSprint;
    int teclaDeslizar;
    bool deslizando;
    float velocidadDeslizamiento, friccionDeslizamiento;

    // Estado de la cuerda: la longitud se fija al engancharse.
    bool ganchoActivo;
    Vector2 anclajeGancho;
    float longitudCuerda;

    // Animacion visual: no altera fisica ni colisiones.
    float tiempoAnimacion;
    float tiempoEfecto;
    int orientacionVisual;
    int estadoVisualAnterior;


public:
    Jugador(float x, float y, Color colorJugador,
            int izquierda, int derecha, int salto, int sprint, int deslizar);
    Rectangle ObtenerRectangulo() const;
    void AplicarTraccion(Vector2 puntoAnclaje, float deltaTime);
    void ConfigurarGancho(bool activo, Vector2 puntoAnclaje);
    void AjustarLongitudCuerda(float deltaTime, bool recoger, bool extender);
    void Actualizar(float deltaTime, const std::vector<Rectangle>& plataformas);
    void Dibujar();
    static void CargarSprites();
    static void LiberarSprites();
};
#endif
