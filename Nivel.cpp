#include "Nivel.h"

Nivel::Nivel() {

    // Suelo principal
    plataformas.push_back({0, 600, 1280, 120});

    // Plataformas elevadas
    plataformas.push_back({150, 480, 200, 20});
    plataformas.push_back({450, 380, 200, 20});
    plataformas.push_back({750, 450, 200, 20});
    plataformas.push_back({1000, 330, 180, 20});
}

void Nivel::Dibujar() {

    for (const Rectangle& plataforma : plataformas) {
        DrawRectangleRec(plataforma, GRAY);
    }
}

const std::vector<Rectangle>& Nivel::ObtenerPlataformas() const {
    return plataformas;
}
