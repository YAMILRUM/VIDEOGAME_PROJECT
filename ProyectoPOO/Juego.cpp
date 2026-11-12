#include "Juego.h"

Juego::Juego() : estado(Estado::JUGANDO), camara{} {
    InitWindow(800, 450, "SpeedRunners");
    SetTargetFPS(60);
    nivel.Cargar();

    jugadores.push_back(Jugador({100, 300}, RED,  {KEY_A, KEY_D, KEY_W, KEY_S}));
    jugadores.push_back(Jugador({100, 300}, BLUE, {KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN}));

    camara.offset = {400, 225};
    camara.target = {400, 225};
    camara.rotation = 0.0f;
    camara.zoom = 1.0f;
}

void Juego::Ejecutar() {
    while (!WindowShouldClose()) {
        Actualizar();
        Dibujar();
    }
    CloseWindow();
}

void Juego::Actualizar() {
    for (auto& j : jugadores) j.Actualizar(nivel);
    ActualizarCamara();
    RevisarEliminaciones();
}

void Juego::Dibujar() {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    BeginMode2D(camara);
    nivel.Dibujar();
    for (const auto& j : jugadores) j.Dibujar();
    EndMode2D();
    EndDrawing();
}

void Juego::ActualizarCamara() {}
void Juego::RevisarEliminaciones() {}