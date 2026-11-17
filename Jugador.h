
#ifndef JUGADOR_H
#define JUGADOR_H

#include "raylib.h"
#include <vector>

class Jugador {
private:
    Vector2 posicion;
    Vector2 velocidad;

    float ancho;
    float alto;

    float velocidadMovimiento;
    float fuerzaSalto;
    float gravedad;

    // Nuevos atributos
    float aceleracion;
    float friccion;
    float velocidadSprint;

    bool enSuelo;

    Color color;

    int teclaIzquierda;
    int teclaDerecha;
    int teclaSalto;
    int teclaSprint;

public:
    Jugador(
        float x,
        float y,
        Color colorJugador,
        int izquierda,
        int derecha,
        int salto,
        int sprint
    );
    Rectangle ObtenerRectangulo() const;

    void Actualizar(
        float deltaTime,
        const std::vector<Rectangle>& plataformas
    );

    void Dibujar();
};

#endif
