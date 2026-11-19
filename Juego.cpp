
#include "Juego.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <limits>

// Constructor
Juego::Juego() {

    InitWindow(1280, 720, "HUSS HUSS - Carrera");
    SetTargetFPS(60);

    // Puntos de anclaje distribuidos por el nivel
    puntosAnclaje = {
        {500, 250},
        {1050, 270},
        {1600, 200},
        {2200, 250},
        {2750, 220}
    };

    // Trampas repartidas por el mapa
    obstaculos.emplace_back(650, 570, 90, 30);
    obstaculos.emplace_back(1250, 570, 90, 30);
    obstaculos.emplace_back(1900, 570, 90, 30);
    obstaculos.emplace_back(2600, 570, 90, 30);
}


// Ejecutar juego
void Juego::Ejecutar() {

    camara.offset = {640, 360};
    camara.target = {640, 360};
    camara.rotation = 0;
    camara.zoom = 1.0f;

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();
        bool seReinicio = false;

        // 1. Reinicio completo
        if (IsKeyPressed(KEY_R)) {

            TraceLog(LOG_INFO, "TECLA R DETECTADA");

            Reiniciar();
            seReinicio = true;
        }

        // 2. Siguiente ronda
        else if (IsKeyPressed(KEY_ENTER) ||
                 IsKeyPressed(KEY_KP_ENTER)) {

            TraceLog(LOG_INFO, "TECLA ENTER DETECTADA");

            if (carrera.EstaTerminada() &&
                !carrera.PartidaTerminada()) {

                ReiniciarJugadores();
                carrera.NuevaRonda();

                seReinicio = true;

                TraceLog(
                    LOG_INFO,
                    "NUEVA RONDA INICIADA"
                );
            }
        }

        // 3. Actualizar jugadores y ganchos
        if (!seReinicio && !carrera.EstaTerminada()) {

            jugador1.Actualizar(
                deltaTime,
                nivel.ObtenerPlataformas()
            );

            jugador2.Actualizar(
                deltaTime,
                nivel.ObtenerPlataformas()
            );

            // Obtener centros de los jugadores
            Rectangle cuerpo1 =
                jugador1.ObtenerRectangulo();

            Rectangle cuerpo2 =
                jugador2.ObtenerRectangulo();

            Vector2 origen1 = {
                cuerpo1.x + cuerpo1.width / 2,
                cuerpo1.y + cuerpo1.height / 2
            };

            Vector2 origen2 = {
                cuerpo2.x + cuerpo2.width / 2,
                cuerpo2.y + cuerpo2.height / 2
            };

            // Buscar punto de anclaje mas cercano
            auto buscarAnclaje = [&](Vector2 origen) {

                Vector2 mejorPunto = origen;
                float distanciaMinima = 450.0f;

                for (const Vector2& punto : puntosAnclaje) {

                    float distancia =
                        Vector2Distance(origen, punto);

                    if (distancia < distanciaMinima) {

                        distanciaMinima = distancia;
                        mejorPunto = punto;
                    }
                }

                return mejorPunto;
            };

            // Gancho del jugador verde
            if (IsKeyDown(KEY_E)) {

                if (!gancho1.EstaActivo()) {

                    Vector2 destino =
                        buscarAnclaje(origen1);

                    // Solo lanzar si hay un punto
                    // de anclaje dentro del alcance
                    if (Vector2Distance(origen1, destino)
                        > 0.01f) {

                        gancho1.Lanzar(
                            origen1,
                            destino
                        );
                    }
                }
            }
            else {
                gancho1.Soltar();
            }

            // Gancho del jugador rojo
            if (IsKeyDown(KEY_L)) {

                if (!gancho2.EstaActivo()) {

                    Vector2 destino =
                        buscarAnclaje(origen2);

                    if (Vector2Distance(origen2, destino)
                        > 0.01f) {

                        gancho2.Lanzar(
                            origen2,
                            destino
                        );
                    }
                }
            }
            else {
                gancho2.Soltar();
            }

            // Actualizar carrera
            carrera.Actualizar(
                deltaTime,
                jugador1.ObtenerRectangulo(),
                jugador2.ObtenerRectangulo()
            );

            // 4. Comprobar colisiones con obstaculos
            if (!carrera.EstaTerminada()) {

                Rectangle r1 =
                    jugador1.ObtenerRectangulo();

                Rectangle r2 =
                    jugador2.ObtenerRectangulo();

                for (const Obstaculo& obstaculo : obstaculos) {

                    bool golpea1 =
                        obstaculo.Colisiona(r1);

                    bool golpea2 =
                        obstaculo.Colisiona(r2);

                    // Mantener prioridad existente
                    if (golpea1) {

                        carrera.EliminarJugador(1);
                        break;
                    }

                    if (golpea2) {

                        carrera.EliminarJugador(2);
                        break;
                    }
                }
            }
        }

        // 5. Obtener posiciones
        Rectangle rect1 =
            jugador1.ObtenerRectangulo();

        Rectangle rect2 =
            jugador2.ObtenerRectangulo();

        float centro1 =
            rect1.x + rect1.width / 2;

        float centro2 =
            rect2.x + rect2.width / 2;

        // 6. Camara sigue al jugador adelantado
        float jugadorAdelante =
            std::max(centro1, centro2);

        float objetivoX = Clamp(
            jugadorAdelante,
            640.0f,
            2560.0f
        );

        camara.target = {objetivoX, 360};

        // Borde izquierdo de la camara
        float bordeIzquierdo =
            camara.target.x -
            camara.offset.x / camara.zoom;

        // 7. Eliminacion por quedarse atras
        if (!seReinicio && !carrera.EstaTerminada()) {

            if (rect1.x + rect1.width < bordeIzquierdo) {

                carrera.EliminarJugador(1);
            }
            else if (
                rect2.x + rect2.width < bordeIzquierdo
            ) {

                carrera.EliminarJugador(2);
            }
        }

        // 8. Dibujar juego
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camara);

        // Escenario
        nivel.Dibujar();

        // Puntos de anclaje
        for (const Vector2& punto : puntosAnclaje) {

            DrawCircleV(
                punto,
                9,
                ORANGE
            );

            DrawCircleLines(
                (int)punto.x,
                (int)punto.y,
                9,
                BLACK
            );
        }

        // Obstaculos
        for (const Obstaculo& obstaculo : obstaculos) {

            obstaculo.Dibujar();
        }

        // Centros actuales de los jugadores
        Rectangle r1 =
            jugador1.ObtenerRectangulo();

        Rectangle r2 =
            jugador2.ObtenerRectangulo();

        Vector2 centroGancho1 = {
            r1.x + r1.width / 2,
            r1.y + r1.height / 2
        };

        Vector2 centroGancho2 = {
            r2.x + r2.width / 2,
            r2.y + r2.height / 2
        };

        // Dibujar cuerdas
        gancho1.Dibujar(centroGancho1);
        gancho2.Dibujar(centroGancho2);

        // Jugadores
        jugador1.Dibujar();
        jugador2.Dibujar();

        // Meta
        carrera.DibujarMeta();

        EndMode2D();

        // Interfaz
        DrawText(
            "HUSS HUSS",
            500, 20, 30, BLACK
        );

        carrera.DibujarInterfaz();

        EndDrawing();
    }

    CloseWindow();
}


// Reiniciar jugadores
void Juego::ReiniciarJugadores() {

    // Soltar ambos ganchos
    gancho1.Soltar();
    gancho2.Soltar();

    jugador1 = Jugador(
        100, 300,
        GREEN,
        KEY_A, KEY_D,
        KEY_W, KEY_LEFT_SHIFT,
        KEY_S
    );

    jugador2 = Jugador(
        200, 300,
        RED,
        KEY_LEFT, KEY_RIGHT,
        KEY_UP, KEY_RIGHT_SHIFT,
        KEY_DOWN
    );
}


// Reinicio completo
void Juego::Reiniciar() {

    ReiniciarJugadores();
    carrera.Reiniciar();
}
