
#ifndef CARRERA_H
#define CARRERA_H

#include "raylib.h"

class Carrera {
private:
    float tiempo;
    int ganador;
    bool terminada;

    Rectangle meta;

public:
    Carrera();

    void Actualizar(
        float deltaTime,
        Rectangle jugador1,
        Rectangle jugador2
    );

    void Dibujar() const;
    void Reiniciar();

    bool EstaTerminada() const;
};

#endif
