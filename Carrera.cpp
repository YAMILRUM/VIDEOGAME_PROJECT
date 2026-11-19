#include "Carrera.h"
#include <cstdio>

Carrera::Carrera() {
    tiempo = 0;
    ganador = 0;
    terminada = false;

    puntosJugador1 = 0;
    puntosJugador2 = 0;
    puntosParaGanar = 3;

    meta = {3100, 500, 30, 100};
}

void Carrera::RegistrarVictoria(int jugador) {
    if (terminada) return;

    ganador = jugador;
    terminada = true;

    if (ganador == 1) {
        puntosJugador1++;
    }
    else if (ganador == 2) {
        puntosJugador2++;
    }
}

void Carrera::Actualizar(
    float deltaTime,
    Rectangle jugador1,
    Rectangle jugador2
) {
    if (terminada) return;

    tiempo += deltaTime;

    if (CheckCollisionRecs(jugador1, meta)) {
        RegistrarVictoria(1);
    }
    else if (CheckCollisionRecs(jugador2, meta)) {
        RegistrarVictoria(2);
    }
}

void Carrera::EliminarJugador(int numeroJugador) {
    if (terminada) return;

    if (numeroJugador == 1) {
        RegistrarVictoria(2);
    }
    else if (numeroJugador == 2) {
        RegistrarVictoria(1);
    }
}

void Carrera::DibujarMeta() const {

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
}

void Carrera::DibujarInterfaz() const {

    char texto[100];

    // Cronometro
    std::snprintf(
        texto, sizeof(texto),
        "Tiempo: %.2f s", tiempo
    );
    DrawText(texto, 30, 30, 24, BLACK);

    // Marcador
    std::snprintf(
        texto, sizeof(texto),
        "VERDE: %d   |   ROJO: %d",
        puntosJugador1,
        puntosJugador2
    );

    DrawText(texto, 450, 80, 25, BLACK);

    if (!terminada) return;

    DrawRectangle(
        290, 225, 700, 230,
        Fade(BLACK, 0.85f)
    );

    if (PartidaTerminada()) {

        const char* mensaje =
            ganador == 1
            ? "VERDE GANA LA PARTIDA"
            : "ROJO GANA LA PARTIDA";

        DrawText(mensaje, 360, 280, 34, GOLD);

        DrawText(
            "Presiona R para reiniciar",
            425, 360, 23, WHITE
        );
    }
    else {

        const char* mensaje =
            ganador == 1
            ? "RONDA PARA EL VERDE"
            : "RONDA PARA EL ROJO";

        DrawText(mensaje, 385, 280, 34, WHITE);

        DrawText(
            "Presiona ENTER para continuar",
            395, 360, 22, WHITE
        );
    }
}

void Carrera::NuevaRonda() {
    if (!terminada || PartidaTerminada()) {
        return;
    }

    tiempo = 0;
    ganador = 0;
    terminada = false;
}

void Carrera::Reiniciar() {
    tiempo = 0;
    ganador = 0;
    terminada = false;

    puntosJugador1 = 0;
    puntosJugador2 = 0;
}

bool Carrera::EstaTerminada() const {
    return terminada;
}

bool Carrera::PartidaTerminada() const {
    return puntosJugador1 >= puntosParaGanar ||
           puntosJugador2 >= puntosParaGanar;
}
