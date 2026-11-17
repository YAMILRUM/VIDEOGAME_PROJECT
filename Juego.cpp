
#include "Juego.h"
#include "raylib.h"

Juego::Juego() {
    InitWindow(1280, 720, "HUSS HUSS - Carrera");
    SetTargetFPS(60);
}

void Juego::Ejecutar() {

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        if (!carrera.EstaTerminada()) {

            jugador1.Actualizar(
                deltaTime,
                nivel.ObtenerPlataformas()
            );

            jugador2.Actualizar(
                deltaTime,
                nivel.ObtenerPlataformas()
            );

            carrera.Actualizar(
                deltaTime,
                jugador1.ObtenerRectangulo(),
                jugador2.ObtenerRectangulo()
            );
        }

        if (IsKeyPressed(KEY_R)) {
            Reiniciar();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("HUSS HUSS", 500, 50, 40, BLACK);

        nivel.Dibujar();

        jugador1.Dibujar();
        jugador2.Dibujar();

        carrera.Dibujar();

        EndDrawing();
    }

    CloseWindow();
}


void Juego::Reiniciar() {

    jugador1 = Jugador(
        200, 300,
        BLUE,
        KEY_A,
        KEY_D,
        KEY_W,
        KEY_LEFT_SHIFT
    );

    jugador2 = Jugador(
        900, 300,
        RED,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_RIGHT_SHIFT
    );

    carrera.Reiniciar();
}

