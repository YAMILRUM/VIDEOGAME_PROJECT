
#include "Jugador.h"
#include <algorithm>
#include <cmath>

// Constructor
Jugador::Jugador(float x, float y) {
    posicion = {x, y};
    velocidad = {0, 0};

    ancho = 50;
    alto = 70;

    velocidadMovimiento = 300;
    velocidadSprint = 500;

    aceleracion = 1800;
    friccion = 2200;

    fuerzaSalto = -550;
    gravedad = 1200;

    enSuelo = false;
}

// Actualizar jugador
void Jugador::Actualizar(
    float deltaTime,
    const std::vector<Rectangle>& plataformas
) {
    // Evitar saltos grandes de tiempo
    deltaTime = std::min(deltaTime, 0.033f);

    // 1. Movimiento horizontal
    int direccion = 0;

    if (IsKeyDown(KEY_D)) direccion++;
    if (IsKeyDown(KEY_A)) direccion--;

    bool sprint = IsKeyDown(KEY_LEFT_SHIFT);

    float velocidadMaxima =
        sprint ? velocidadSprint : velocidadMovimiento;

    if (direccion != 0) {

        velocidad.x +=
            direccion * aceleracion * deltaTime;

        velocidad.x = std::clamp(
            velocidad.x,
            -velocidadMaxima,
            velocidadMaxima
        );

    } else {

        // Aplicar friccion
        float reduccion = friccion * deltaTime;

        if (velocidad.x > 0) {
            velocidad.x =
                std::max(0.0f, velocidad.x - reduccion);
        }
        else if (velocidad.x < 0) {
            velocidad.x =
                std::min(0.0f, velocidad.x + reduccion);
        }
    }

    // 2. Salto
    if (IsKeyPressed(KEY_SPACE) && enSuelo) {
        velocidad.y = fuerzaSalto;
        enSuelo = false;
    }

    // Salto variable
    if (IsKeyReleased(KEY_SPACE) && velocidad.y < 0) {
        velocidad.y *= 0.5f;
    }

    // 3. Gravedad
    velocidad.y += gravedad * deltaTime;

    // 4. Movimiento horizontal
    posicion.x += velocidad.x * deltaTime;

    Rectangle jugadorRect = {
        posicion.x, posicion.y, ancho, alto
    };

    for (const Rectangle& plataforma : plataformas) {
        if (CheckCollisionRecs(jugadorRect, plataforma)) {

            if (velocidad.x > 0) {
                posicion.x = plataforma.x - ancho;
            }
            else if (velocidad.x < 0) {
                posicion.x = plataforma.x + plataforma.width;
            }

            velocidad.x = 0;
            jugadorRect.x = posicion.x;
        }
    }

    // 5. Limites horizontales
    if (posicion.x < 0) {
        posicion.x = 0;
        velocidad.x = 0;
    }

    if (posicion.x + ancho > 1280) {
        posicion.x = 1280 - ancho;
        velocidad.x = 0;
    }

    // 6. Movimiento vertical
    posicion.y += velocidad.y * deltaTime;
    enSuelo = false;

    jugadorRect = {
        posicion.x, posicion.y, ancho, alto
    };

    // 7. Colisiones verticales
    for (const Rectangle& plataforma : plataformas) {

        if (CheckCollisionRecs(jugadorRect, plataforma)) {

            if (velocidad.y > 0) {
                posicion.y = plataforma.y - alto;
                velocidad.y = 0;
                enSuelo = true;
            }
            else if (velocidad.y < 0) {
                posicion.y =
                    plataforma.y + plataforma.height;
                velocidad.y = 0;
            }

            jugadorRect.y = posicion.y;
        }
    }

    // 8. Limite superior
    if (posicion.y < 0) {
        posicion.y = 0;
        velocidad.y = 0;
    }
}

// Dibujar jugador
void Jugador::Dibujar() {
    DrawRectangle(
        (int)posicion.x,
        (int)posicion.y,
        (int)ancho,
        (int)alto,
        BLUE
    );
}
