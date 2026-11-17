
#ifndef NIVEL_H
#define NIVEL_H

#include "raylib.h"
#include <vector>

class Nivel {
private:
    std::vector<Rectangle> plataformas;

public:
    Nivel();

    void Dibujar();

    const std::vector<Rectangle>& ObtenerPlataformas() const;
};

#endif
