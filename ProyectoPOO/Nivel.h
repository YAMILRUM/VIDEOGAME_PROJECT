#pragma once
#include "raylib.h"
#include <vector>

class Nivel {
public:
    void Cargar();
    void Dibujar() const;
    bool ColisionaCon(Rectangle caja) const;
    float GetAncho() const;

private:
    std::vector<std::vector<int>> bloques;
    int tamBloque = 40;
};