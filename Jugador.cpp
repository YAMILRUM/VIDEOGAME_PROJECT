
#include "Jugador.h"
#include <algorithm>
#include <cmath>

// Constructor
Jugador::Jugador(
    float x,
    float y,
    Color colorJugador,
    int izquierda,
    int derecha,
    int salto,
    int sprint,
    int deslizar
) {
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
    deslizando = false;

    velocidadDeslizamiento = 650.0f;
    friccionDeslizamiento = 900.0f;

    color = colorJugador;

    teclaIzquierda = izquierda;
    teclaDerecha = derecha;
    teclaSalto = salto;
    teclaSprint = sprint;
    teclaDeslizar = deslizar;
}


// Actualizar jugador
void Jugador::Actualizar(
    float deltaTime,
    const std::vector<Rectangle>& plataformas
) {
    // Evitar saltos grandes de tiempo
    deltaTime = std::min(deltaTime, 0.033f);

    // 1. Deslizamiento
    bool quiereDeslizar =
        IsKeyDown(teclaDeslizar) && enSuelo;

    if (quiereDeslizar && !deslizando) {

        // Reducir altura sin atravesar el suelo
        posicion.y += 40;
        alto = 30;
        deslizando = true;

        // Impulso inicial
        if (velocidad.x > 50.0f) {
            velocidad.x = velocidadDeslizamiento;
        }
        else if (velocidad.x < -50.0f) {
            velocidad.x = -velocidadDeslizamiento;
        }
    }
    else if (!quiereDeslizar && deslizando) {

        // Comprobar si puede levantarse
        Rectangle cuerpoNormal = {
            posicion.x,
            posicion.y - 40,
            ancho,
            70
        };

        bool puedeLevantarse = true;

        for (const Rectangle& plataforma : plataformas) {
            if (CheckCollisionRecs(cuerpoNormal, plataforma)) {
                puedeLevantarse = false;
                break;
            }
        }

        if (puedeLevantarse) {
            posicion.y -= 40;
            alto = 70;
            deslizando = false;
        }
    }

    // 2. Direccion horizontal
    int direccion = 0;

    if (IsKeyDown(teclaDerecha)) {
        direccion++;
    }

    if (IsKeyDown(teclaIzquierda)) {
        direccion--;
    }

    // 3. Movimiento horizontal
    if (deslizando) {

        // Permitir movimiento mientras esta agachado
        if (direccion != 0) {

            // Aceleracion reducida durante el slide
            float aceleracionSlide = 1000.0f;

            velocidad.x +=
                direccion * aceleracionSlide * deltaTime;

            // Mantener el impulso inicial de 650
            velocidad.x = std::clamp(
                velocidad.x,
                -velocidadDeslizamiento,
                velocidadDeslizamiento
            );

        }
        else {

            // Aplicar friccion si no presiona direccion
            float reduccion =
                friccionDeslizamiento * deltaTime;

            if (velocidad.x > 0.0f) {
                velocidad.x = std::max(
                    0.0f,
                    velocidad.x - reduccion
                );
            }
            else if (velocidad.x < 0.0f) {
                velocidad.x = std::min(
                    0.0f,
                    velocidad.x + reduccion
                );
            }
        }

    }
    else {

        // Movimiento normal
        bool sprint = IsKeyDown(teclaSprint);

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

        }
        else {

            // Friccion normal
            float reduccion = friccion * deltaTime;

            if (velocidad.x > 0.0f) {
                velocidad.x = std::max(
                    0.0f,
                    velocidad.x - reduccion
                );
            }
            else if (velocidad.x < 0.0f) {
                velocidad.x = std::min(
                    0.0f,
                    velocidad.x + reduccion
                );
            }
        }
    }


    // 4. Salto
    if (IsKeyPressed(teclaSalto) &&
        enSuelo &&
        !deslizando) {

        velocidad.y = fuerzaSalto;
        enSuelo = false;
    }

    // Salto variable
    if (IsKeyReleased(teclaSalto) &&
        velocidad.y < 0) {

        velocidad.y *= 0.5f;
    }

    // 5. Gravedad
    velocidad.y += gravedad * deltaTime;

    // 6. Movimiento horizontal
    posicion.x += velocidad.x * deltaTime;

    Rectangle jugadorRect = {
        posicion.x,
        posicion.y,
        ancho,
        alto
    };

    // Colisiones horizontales
    for (const Rectangle& plataforma : plataformas) {

        if (CheckCollisionRecs(jugadorRect, plataforma)) {

            if (velocidad.x > 0) {
                posicion.x = plataforma.x - ancho;
            }
            else if (velocidad.x < 0) {
                posicion.x =
                    plataforma.x + plataforma.width;
            }

            velocidad.x = 0;
            jugadorRect.x = posicion.x;
        }
    }

    // 7. Limites horizontales
    if (posicion.x < 0) {
        posicion.x = 0;
        velocidad.x = 0;
    }

    if (posicion.x + ancho > 3200) {
        posicion.x = 3200 - ancho;
        velocidad.x = 0;
    }

    // 8. Movimiento vertical
    posicion.y += velocidad.y * deltaTime;
    enSuelo = false;

    jugadorRect = {
        posicion.x,
        posicion.y,
        ancho,
        alto
    };

    // 9. Colisiones verticales
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

    // 10. Limite superior
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
        color
    );
}


// Obtener rectangulo de colision
Rectangle Jugador::ObtenerRectangulo() const {

    return {
        posicion.x,
        posicion.y,
        ancho,
        alto
    };
}

