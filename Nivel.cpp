
#include "Nivel.h"
#include "raylib.h"

Nivel::Nivel() {

    // Suelo principal
    plataformas.push_back({0, 600, 3200, 120});

    // Primera zona
    plataformas.push_back({150, 480, 200, 20});
    plataformas.push_back({450, 380, 200, 20});
    plataformas.push_back({750, 450, 200, 20});

    // Segunda zona
    plataformas.push_back({1100, 400, 200, 20});
    plataformas.push_back({1450, 320, 200, 20});
    plataformas.push_back({1800, 450, 200, 20});

    // Tercera zona
    plataformas.push_back({2150, 370, 200, 20});
    plataformas.push_back({2500, 460, 200, 20});
    plataformas.push_back({2850, 350, 200, 20});

    // Tunel bajo 1
    plataformas.push_back({950, 500, 220, 60});

    // Tunel bajo 2
    plataformas.push_back({2250, 500, 220, 60});
}

void Nivel::Dibujar() {
    for (const Rectangle& plataforma : plataformas) {
        DrawRectangleRec(plataforma, PURPLE);
    }
}

const std::vector<Rectangle>& Nivel::ObtenerPlataformas() const {
    return plataformas;
}
