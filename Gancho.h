
#ifndef GANCHO_H
#define GANCHO_H

#include "raylib.h"

class Gancho {
private:
    bool activo;
    Vector2 puntoAnclaje;
    float alcanceMaximo;

public:
    Gancho();

    void Lanzar(Vector2 origen, Vector2 destino);
    void Soltar();
    void Dibujar(Vector2 origen) const;

    bool EstaActivo() const;
    Vector2 ObtenerAnclaje() const;
};

#endif
