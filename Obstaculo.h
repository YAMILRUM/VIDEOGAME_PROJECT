
#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"

class Obstaculo {
private:
    Rectangle area;

public:
    Obstaculo(float x, float y,
              float ancho, float alto);

    void Dibujar() const;
    bool Colisiona(Rectangle jugador) const;
};

#endif
