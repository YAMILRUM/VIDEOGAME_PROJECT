#include "Jugador.h"
#include <algorithm>
#include <cmath>

Jugador::Jugador(float x, float y, Color colorJugador,
                 int izquierda, int derecha, int salto, int sprint, int deslizar) {
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
    ganchoActivo = false;
    anclajeGancho = {0, 0};
    longitudCuerda = 0.0f;
    tiempoAnimacion = 0.0f;
    tiempoEfecto = 0.0f;
    orientacionVisual = 1;
    estadoVisualAnterior = -1;
}

Rectangle Jugador::ObtenerRectangulo() const {
    return {posicion.x, posicion.y, ancho, alto};
}

// Atlas de 6 columnas x 2 filas: verde arriba, rojo abajo.
// Cargar despues de InitWindow y liberar antes de CloseWindow.
namespace {
Texture2D atlasPersonajes = {};
bool atlasListo = false;
}

void Jugador::CargarSprites() {
    if (atlasListo) return;
    if (FileExists("assets/personajes.png")) {
        atlasPersonajes = LoadTexture("assets/personajes.png");
        atlasListo = atlasPersonajes.id != 0;
        if (atlasListo) SetTextureFilter(atlasPersonajes, TEXTURE_FILTER_BILINEAR);
    }
}

void Jugador::LiberarSprites() {
    if (atlasListo) UnloadTexture(atlasPersonajes);
    atlasPersonajes = {};
    atlasListo = false;
}

void Jugador::Dibujar() {
    if (!atlasListo) {
        DrawRectangleRec(ObtenerRectangulo(), color);
        return;
    }

    const bool verde = color.g > color.r;
    const float rapidez = std::fabs(velocidad.x);
    const bool corriendo = enSuelo && !deslizando && rapidez > 40.0f;
    const int estado = deslizando ? 4 : ganchoActivo ? 5 :
                       !enSuelo ? 3 : corriendo ? 1 : 0;

    // Atlas actual: quieto, carrera A/B, salto, slide, gancho.
    // La velocidad regula la cadencia de carrera.
    int fotograma = estado;
    if (estado == 1) {
        const float cuadrosPorSegundo = std::clamp(rapidez / 55.0f, 7.0f, 15.0f);
        fotograma = 1 + (static_cast<int>(tiempoAnimacion * cuadrosPorSegundo) % 2);
    }

    const float celdaW = atlasPersonajes.width / 6.0f;
    const float celdaH = atlasPersonajes.height / 2.0f;
    Rectangle origen = {
        fotograma * celdaW,
        verde ? 0.0f : celdaH,
        celdaW * static_cast<float>(orientacionVisual),
        celdaH
    };

    const float pies = posicion.y + alto;
    const float dibujoW = deslizando ? 105.0f : 83.0f;
    const float dibujoH = deslizando ? 48.0f : 91.0f;
    const float fase = tiempoAnimacion * 10.0f;
    const float rebote = corriendo ? std::fabs(std::sin(fase)) * 2.5f : 0.0f;
    const float respiracion = estado == 0 ? std::sin(tiempoAnimacion * 2.5f) * 1.0f : 0.0f;
    const float inclinar = corriendo ? (orientacionVisual * 4.0f) : 0.0f;

    // Polvo puramente visual; no se guarda ni modifica la posicion del jugador.
    if (enSuelo && (corriendo || deslizando)) {
        const float frecuencia = deslizando ? 0.11f : 0.17f;
        const float fasePolvo = std::fmod(tiempoEfecto, frecuencia) / frecuencia;
        const float atras = -orientacionVisual * (18.0f + 24.0f * fasePolvo);
        const float opacidad = 0.45f * (1.0f - fasePolvo);
        DrawCircleV({posicion.x + ancho * 0.5f + atras, pies - 4.0f},
                    4.0f + 6.0f * fasePolvo,
                    Fade(LIGHTGRAY, opacidad));
    }

    Rectangle destino = {
        posicion.x + ancho * 0.5f,
        pies - rebote + respiracion,
        dibujoW,
        dibujoH
    };
    // Origen inferior central: mantiene los pies sobre la superficie.
    DrawTexturePro(atlasPersonajes, origen, destino,
                   {dibujoW * 0.5f, dibujoH},
                   inclinar, WHITE);
}

void Jugador::ConfigurarGancho(bool activo, Vector2 puntoAnclaje) {
    if (!activo) {
        ganchoActivo = false;
        return;
    }

    if (!ganchoActivo) {
        Vector2 centro = {posicion.x + ancho * 0.5f,
                          posicion.y + alto * 0.5f};
        float dx = centro.x - puntoAnclaje.x;
        float dy = centro.y - puntoAnclaje.y;
        longitudCuerda = std::sqrt(dx * dx + dy * dy);
        longitudCuerda = std::max(60.0f, longitudCuerda);
    }
    anclajeGancho = puntoAnclaje;
    ganchoActivo = true;
}

// Permite recoger o extender la cuerda sin cambiar el punto de anclaje.
void Jugador::AjustarLongitudCuerda(float deltaTime, bool recoger, bool extender) {
    if (!ganchoActivo || recoger == extender) return;

    const float velocidadAjuste = 180.0f;
    const float longitudMinima = 65.0f;
    const float longitudMaxima = 450.0f;
    float cambio = (extender ? 1.0f : -1.0f) *
                   velocidadAjuste * std::clamp(deltaTime, 0.0f, 0.033f);
    longitudCuerda = std::clamp(longitudCuerda + cambio,
                               longitudMinima, longitudMaxima);
}

// Se conserva el metodo para compatibilidad con el codigo anterior.
// La nueva simulacion del gancho ocurre dentro de Actualizar().
void Jugador::AplicarTraccion(Vector2 puntoAnclaje, float deltaTime) {
    (void)deltaTime;
    ConfigurarGancho(true, puntoAnclaje);
}

void Jugador::Actualizar(float deltaTime,
                         const std::vector<Rectangle>& plataformas) {
    deltaTime = std::clamp(deltaTime, 0.0f, 0.033f);

    // Deslizamiento y comprobacion de espacio para levantarse.
    bool quiereDeslizar = IsKeyDown(teclaDeslizar) && enSuelo;
    if (quiereDeslizar && !deslizando) {
        posicion.y += 40;
        alto = 30;
        deslizando = true;
        if (velocidad.x > 50.0f) velocidad.x = velocidadDeslizamiento;
        else if (velocidad.x < -50.0f) velocidad.x = -velocidadDeslizamiento;
    } else if (!quiereDeslizar && deslizando) {
        Rectangle cuerpoNormal = {posicion.x, posicion.y - 40, ancho, 70};
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

    int direccion = 0;
    if (IsKeyDown(teclaDerecha)) ++direccion;
    if (IsKeyDown(teclaIzquierda)) --direccion;

    if (deslizando) {
        if (direccion != 0) {
            velocidad.x += direccion * 1000.0f * deltaTime;
            velocidad.x = std::clamp(velocidad.x,
                                     -velocidadDeslizamiento,
                                      velocidadDeslizamiento);
        } else {
            float reduccion = friccionDeslizamiento * deltaTime;
            if (velocidad.x > 0) velocidad.x = std::max(0.0f, velocidad.x - reduccion);
            else velocidad.x = std::min(0.0f, velocidad.x + reduccion);
        }
    } else if (ganchoActivo) {
        // Control en el aire sin frenar bruscamente el pendulo.
        velocidad.x += direccion * 700.0f * deltaTime;
        velocidad.x = std::clamp(velocidad.x, -850.0f, 850.0f);
    } else {
        bool sprint = IsKeyDown(teclaSprint);
        float maxima = sprint ? velocidadSprint : velocidadMovimiento;
        if (direccion != 0) {
            velocidad.x += direccion * aceleracion * deltaTime;
            velocidad.x = std::clamp(velocidad.x, -maxima, maxima);
        } else {
            float reduccion = friccion * deltaTime;
            if (velocidad.x > 0) velocidad.x = std::max(0.0f, velocidad.x - reduccion);
            else velocidad.x = std::min(0.0f, velocidad.x + reduccion);
        }
    }

    if (IsKeyPressed(teclaSalto) && enSuelo && !deslizando) {
        velocidad.y = fuerzaSalto;
        enSuelo = false;
    }
    if (IsKeyReleased(teclaSalto) && velocidad.y < 0) velocidad.y *= 0.5f;
    velocidad.y += gravedad * deltaTime;
    velocidad.y = std::clamp(velocidad.y, -900.0f, 900.0f);

    // Integracion con colisiones por ejes, como en la version estable.
    posicion.x += velocidad.x * deltaTime;
    Rectangle cuerpo = ObtenerRectangulo();
    for (const Rectangle& plataforma : plataformas) {
        if (CheckCollisionRecs(cuerpo, plataforma)) {
            if (velocidad.x > 0) posicion.x = plataforma.x - ancho;
            else if (velocidad.x < 0) posicion.x = plataforma.x + plataforma.width;
            velocidad.x = 0;
            cuerpo.x = posicion.x;
        }
    }
    if (posicion.x < 0) { posicion.x = 0; velocidad.x = 0; }
    if (posicion.x + ancho > 3200) { posicion.x = 3200 - ancho; velocidad.x = 0; }

    posicion.y += velocidad.y * deltaTime;
    enSuelo = false;
    cuerpo = ObtenerRectangulo();
    for (const Rectangle& plataforma : plataformas) {
        if (CheckCollisionRecs(cuerpo, plataforma)) {
            if (velocidad.y > 0) {
                posicion.y = plataforma.y - alto;
                enSuelo = true;
            } else if (velocidad.y < 0) {
                posicion.y = plataforma.y + plataforma.height;
            }
            velocidad.y = 0;
            cuerpo.y = posicion.y;
        }
    }
    if (posicion.y < 0) { posicion.y = 0; velocidad.y = 0; }

    // Restriccion de cuerda: impide alejarse mas de la longitud fijada.
    if (ganchoActivo) {
        Vector2 centro = {posicion.x + ancho * 0.5f,
                          posicion.y + alto * 0.5f};
        float dx = centro.x - anclajeGancho.x;
        float dy = centro.y - anclajeGancho.y;
        float distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia > longitudCuerda && distancia > 0.001f) {
            float nx = dx / distancia;
            float ny = dy / distancia;
            float exceso = distancia - longitudCuerda;
            Vector2 anterior = posicion;
            posicion.x -= nx * exceso;
            posicion.y -= ny * exceso;

            // Cancelar velocidad que estira la cuerda; preservar la tangencial.
            float radial = velocidad.x * nx + velocidad.y * ny;
            if (radial > 0.0f) {
                velocidad.x -= radial * nx;
                velocidad.y -= radial * ny;
            }

            // No permitir que la correccion de la cuerda atraviese plataformas.
            // Si la posicion corregida colisiona, retrocedemos ese ajuste.
            Rectangle corregido = ObtenerRectangulo();
            bool bloqueado = (posicion.x < 0 || posicion.x + ancho > 3200 || posicion.y < 0);
            for (const Rectangle& plataforma : plataformas) {
                if (CheckCollisionRecs(corregido, plataforma)) {
                    bloqueado = true;
                    break;
                }
            }
            if (bloqueado) {
                posicion = anterior;
                // La cuerda queda temporalmente floja ante un obstaculo.
            }
        }
    }

    // Actualizacion de animacion sin intervenir en la fisica.
    if (velocidad.x > 20.0f) orientacionVisual = 1;
    else if (velocidad.x < -20.0f) orientacionVisual = -1;
    const bool corriendo = enSuelo && !deslizando && std::fabs(velocidad.x) > 40.0f;
    const int estadoActual = deslizando ? 4 : ganchoActivo ? 5 :
                             !enSuelo ? 3 : corriendo ? 1 : 0;
    if (estadoActual != estadoVisualAnterior) {
        tiempoAnimacion = 0.0f;
        estadoVisualAnterior = estadoActual;
    } else {
        tiempoAnimacion += deltaTime;
    }
    tiempoEfecto += deltaTime;
}




