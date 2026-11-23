#include "Carrera.h"
#include <cstdio>

Carrera::Carrera() {
    tiempo = 0; ganador = 0; terminada = false;
    puntosJugador1 = 0; puntosJugador2 = 0; puntosParaGanar = 3;
    meta = {3100, 500, 30, 100};
}

void Carrera::RegistrarVictoria(int jugador) {
    if (terminada) return;
    ganador = jugador;
    terminada = true;
    if (ganador == 1) puntosJugador1++;
    else if (ganador == 2) puntosJugador2++;
}

void Carrera::Actualizar(float deltaTime, Rectangle jugador1, Rectangle jugador2) {
    if (terminada) return;
    tiempo += deltaTime;
    if (CheckCollisionRecs(jugador1, meta)) RegistrarVictoria(1);
    else if (CheckCollisionRecs(jugador2, meta)) RegistrarVictoria(2);
}

void Carrera::EliminarJugador(int numeroJugador) {
    if (terminada) return;
    if (numeroJugador == 1) RegistrarVictoria(2);
    else if (numeroJugador == 2) RegistrarVictoria(1);
}

void Carrera::DibujarMeta() const {
    // Arco futurista decorativo; el rectangulo meta conserva su colision.
    const Color metal = {43, 59, 78, 255};
    DrawRectangle((int)meta.x - 13, (int)meta.y - 28, 12, (int)meta.height + 28, metal);
    DrawRectangle((int)meta.x + (int)meta.width + 1, (int)meta.y - 28, 12, (int)meta.height + 28, metal);
    DrawRectangle((int)meta.x - 13, (int)meta.y - 28, (int)meta.width + 26, 12, GOLD);
    for (int fila = 0; fila < 5; fila++) {
        for (int columna = 0; columna < 2; columna++) {
            Color c = (fila + columna) % 2 == 0 ? BLACK : WHITE;
            DrawRectangle((int)meta.x + columna * 15, (int)meta.y + fila * 20, 15, 20, c);
        }
    }
    DrawCircle((int)meta.x - 7, (int)meta.y - 22, 4, SKYBLUE);
    DrawCircle((int)meta.x + (int)meta.width + 7, (int)meta.y - 22, 4, SKYBLUE);
}

void Carrera::DibujarInterfaz() const {
    const Color fondo = {12, 22, 43, 230};
    const Color verde = {85, 236, 142, 255};
    const Color rojo = {255, 112, 122, 255};
    char texto[128];

    DrawRectangleRounded({18, 15, 268, 69}, 0.22f, 8, fondo);
    DrawRectangleRoundedLinesEx({18, 15, 268, 69}, 0.22f, 8, 2, GOLD);
    DrawText("TIEMPO", 34, 23, 16, SKYBLUE);
    std::snprintf(texto, sizeof(texto), "%06.2f s", tiempo);
    DrawText(texto, 34, 43, 26, WHITE);

    DrawRectangleRounded({385, 65, 510, 70}, 0.22f, 8, fondo);
    DrawRectangleRoundedLinesEx({385, 65, 510, 70}, 0.22f, 8, 2, GOLD);
    DrawText("VERDE", 413, 81, 23, verde);
    DrawText("ROJO", 779, 81, 23, rojo);
    std::snprintf(texto, sizeof(texto), "%d  -  %d", puntosJugador1, puntosJugador2);
    DrawText(texto, 594, 78, 31, WHITE);
    DrawText("PRIMERO A 3", 580, 114, 13, LIGHTGRAY);

    if (!terminada) return;

    DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.58f));
    Rectangle panel = {240, 190, 800, 335};
    DrawRectangleRounded(panel, 0.13f, 12, fondo);
    DrawRectangleRoundedLinesEx(panel, 0.13f, 12, 3, GOLD);

    const bool partida = PartidaTerminada();
    const char* titulo = partida ? "CAMPEON DE HUSS HUSS" : "RONDA TERMINADA";
    int tituloTam = partida ? 39 : 45;
    DrawText(titulo, (1280 - MeasureText(titulo, tituloTam))/2, 223, tituloTam, GOLD);

    const char* vencedor = ganador == 1 ? "GANA EL VERDE" : "GANA EL ROJO";
    Color colorGanador = ganador == 1 ? verde : rojo;
    DrawText(vencedor, (1280 - MeasureText(vencedor, 37))/2, 311, 37, colorGanador);

    std::snprintf(texto, sizeof(texto), "MARCADOR FINAL: %d - %d", puntosJugador1, puntosJugador2);
    DrawText(texto, (1280 - MeasureText(texto, 25))/2, 379, 25, WHITE);
    const char* accion = partida ? "PRESIONA R PARA REINICIAR" : "PRESIONA ENTER PARA LA SIGUIENTE RONDA";
    DrawText(accion, (1280 - MeasureText(accion, 22))/2, 453, 22, LIGHTGRAY);
}

void Carrera::NuevaRonda() {
    if (!terminada || PartidaTerminada()) return;
    tiempo = 0; ganador = 0; terminada = false;
}

void Carrera::Reiniciar() {
    tiempo = 0; ganador = 0; terminada = false;
    puntosJugador1 = 0; puntosJugador2 = 0;
}

bool Carrera::EstaTerminada() const { return terminada; }
bool Carrera::PartidaTerminada() const {
    return puntosJugador1 >= puntosParaGanar || puntosJugador2 >= puntosParaGanar;
}
