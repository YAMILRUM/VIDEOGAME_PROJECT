
#include "Juego.h"
#include "raylib.h"

Juego::Juego() {
    InitWindow(1280, 720, "SpeedRunners");
    SetTargetFPS(60);
}

void Juego::Ejecutar() {

    while (!WindowShouldClose()) {

        // Actualizar jugador con las plataformas
        jugador.Actualizar(
            GetFrameTime(),
            nivel.ObtenerPlataformas()
        );

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("SpeedRunners", 500, 50, 40, BLACK);

        // Dibujar escenario
        nivel.Dibujar();

        // Dibujar personaje
        jugador.Dibujar();

        EndDrawing();
    }

    CloseWindow();
}

