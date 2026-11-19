
#include "Obstaculo.h"

Obstaculo::Obstaculo(
    float x,
    float y,
    float ancho,
    float alto
) {
    area = {x, y, ancho, alto};
}

void Obstaculo::Dibujar() const {

    const int cantidad = 5;

    float anchoPincho = area.width / cantidad;

    // Dibujar la base de los pinchos
    DrawRectangle(
        (int)area.x,
        (int)(area.y + area.height - 5),
        (int)area.width,
        5,
        MAROON
    );

    // Dibujar los cinco triangulos
    for (int i = 0; i < cantidad; i++) {

        float x = area.x + i * anchoPincho;

        // Punta superior
        Vector2 punta = {
            x + anchoPincho / 2,
            area.y
        };

        // Esquina inferior izquierda
        Vector2 izquierda = {
            x,
            area.y + area.height
        };

        // Esquina inferior derecha
        Vector2 derecha = {
            x + anchoPincho,
            area.y + area.height
        };

        // Dibujar triangulo rojo
        DrawTriangle(
            punta,
            izquierda,
            derecha,
            RED
        );

        // Contorno negro
        DrawLineV(punta, izquierda, BLACK);
        DrawLineV(izquierda, derecha, BLACK);
        DrawLineV(derecha, punta, BLACK);
    }
}

bool Obstaculo::Colisiona(Rectangle jugador) const {
    return CheckCollisionRecs(area, jugador);
}

