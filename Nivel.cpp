#include "Nivel.h"

void Nivel::Cargar() {}

void Nivel::Dibujar() const {
    DrawRectangle(0, 340, 2000, 110, DARKGREEN);
}

bool Nivel::ColisionaCon(Rectangle caja) const { return false; }

float Nivel::GetAncho() const { return 2000; }