
#include "Carrera.h"
#include <cstdio>

Carrera::Carrera() {
    tiempo = 0;
    ganador = 0;
    terminada = false;

    meta = {1200, 500, 30, 100};
}

void Carrera::Actualizar(
    float deltaTime,
    Rectangle jugador1,
    Rectangle jugador2
) {
    if (terminada) {
        return;
    }

    tiempo += deltaTime;

    if (CheckCollisionRecs(jugador1, meta)) {
        ganador = 1;
        terminada = true;
    }
    else if (CheckCollisionRecs(jugador2, meta)) {
        ganador = 2;
        terminada = true;
    }
}

void Carrera::Dibujar() const {

    // Dibujar meta
    DrawRectangleRec(meta, LIGHTGRAY);

    for (int fila = 0; fila < 5; fila++) {
        for (int columna = 0; columna < 2; columna++) {
            Color color =
                (fila + columna) % 2 == 0
                ? BLACK : WHITE;

            DrawRectangle(
                (int)meta.x + columna * 15,
                (int)meta.y + fila * 20,
                15, 20, color
            );
        }
    }

    // Cronometro
    char texto[80];
    std::snprintf(
        texto, sizeof(texto),
        "Tiempo: %.2f s", tiempo
    );

    DrawText(texto, 30, 30, 25, BLACK);

    // Mostrar ganador
    if (terminada) {
        DrawRectangle(
            300, 240, 680, 180,
            Fade(BLACK, 0.75f)
        );

        const char* mensaje =
            ganador == 1
            ? "GANA EL JUGADOR AZUL"
            : "GANA EL JUGADOR ROJO";

        DrawText(mensaje, 365, 290, 35, WHITE);
        DrawText(
            "Presiona R para reiniciar",
            435, 355, 22, WHITE
        );
    }
}

void Carrera::Reiniciar() {
    tiempo = 0;
    ganador = 0;
    terminada = false;
}

bool Carrera::EstaTerminada() const {
    return terminada;
}
