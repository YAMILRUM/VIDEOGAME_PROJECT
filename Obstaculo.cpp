#include "Obstaculo.h"
#include <cmath>

Obstaculo::Obstaculo(float x, float y, float ancho, float alto) {
    area = {x, y, ancho, alto};
}

void Obstaculo::Dibujar() const {
    const int cantidad = 5;
    const float anchoPincho = area.width / cantidad;
    const float baseY = area.y + area.height;

    // Placa industrial: se mantiene dentro del area de colision.
    DrawRectangleRec({area.x, baseY - 7.0f, area.width, 7.0f}, Color{40, 47, 65, 255});
    DrawRectangleRec({area.x, baseY - 7.0f, area.width, 2.0f}, Color{255, 197, 54, 255});

    for (int i = 0; i < cantidad; ++i) {
        float x = area.x + i * anchoPincho;
        Vector2 punta = {x + anchoPincho / 2.0f, area.y};
        Vector2 izquierda = {x, baseY - 6.0f};
        Vector2 derecha = {x + anchoPincho, baseY - 6.0f};

        // Orden de vertices probado en el proyecto para DrawTriangle.
        DrawTriangle(punta, izquierda, derecha, Color{235, 74, 77, 255});
        DrawTriangle({punta.x, punta.y + 5.0f},
                     {izquierda.x + 4.0f, izquierda.y - 2.0f},
                     {punta.x, izquierda.y - 2.0f},
                     Color{255, 151, 114, 255});
        DrawLineEx(punta, izquierda, 2.0f, Color{28, 33, 48, 255});
        DrawLineEx(izquierda, derecha, 2.0f, Color{28, 33, 48, 255});
        DrawLineEx(derecha, punta, 2.0f, Color{28, 33, 48, 255});
    }

    // Tornillos de la base.
    DrawCircleV({area.x + 5.0f, baseY - 3.0f}, 2.0f, LIGHTGRAY);
    DrawCircleV({area.x + area.width - 5.0f, baseY - 3.0f}, 2.0f, LIGHTGRAY);
}

bool Obstaculo::Colisiona(Rectangle jugador) const {
    return CheckCollisionRecs(area, jugador);
}

