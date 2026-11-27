#include "Juego.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>

Juego::Juego() {
    InitWindow(1280, 720, "HUSS HUSS - Carrera");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // ESC sera pausa, no cierre automatico
    Jugador::CargarSprites();
    CargarMapa(TipoMapa::Industrial);
}

void Juego::CargarMapa(TipoMapa tipo) {
    nivel.Cargar(tipo);
    obstaculos.clear();
    sierras.clear();
    trampasJungla.clear();
    trampasPlaya.clear();
    puntosAnclaje.clear();
    if (tipo == TipoMapa::Playa) {
        trampasPlaya.emplace_back(TipoTrampaPlaya::Cangrejo, Vector2{650,600}, 55, 65);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Cangrejo, Vector2{920,600}, 55, 85);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Cangrejo, Vector2{1660,600}, 45, 105);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Coco, Vector2{824,376.16f}, 4.8f, 1100, 0.2f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Coco, Vector2{1444,347.18f}, 4.5f, 1100, 1.1f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Coco, Vector2{2064,318.2f}, 4.2f, 1100, 2.0f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Tiburon, Vector2{2295,620}, 5.8f, 170, 0.4f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Tiburon, Vector2{3260,620}, 5.2f, 175, 2.0f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Canon, Vector2{3710,548}, 5.4f, -240, 0.3f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Canon, Vector2{4210,548}, 4.8f, -270, 2.0f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Tentaculo, Vector2{5400,620}, 6.5f, 145, 0.3f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Tentaculo, Vector2{5650,610}, 6.0f, 110, 2.0f);
        trampasPlaya.emplace_back(TipoTrampaPlaya::Tentaculo, Vector2{6080,610}, 5.8f, 125, 3.0f);
        puntosAnclaje = {{550,300},{1250,300},{1900,300},{2300,300},{2800,280},{3270,300},{3800,280},{4350,300},{4900,280},{5400,300},{5950,280}};
        Reiniciar();
        return;
    }
    if (tipo == TipoMapa::Jungla) {
        puntosAnclaje = {{550,300},{1250,280},{1950,290},{2650,280},{3350,280},{4130,280},{4750,280},{5500,280},{6000,280}};
        trampasJungla.emplace_back(TipoTrampaJungla::Planta, Vector2{850,600});
        trampasJungla.emplace_back(TipoTrampaJungla::Planta, Vector2{1450,600}, 1.2f);
        trampasJungla.emplace_back(TipoTrampaJungla::Dardo, Vector2{1350,548}, 0, 330);
        trampasJungla.emplace_back(TipoTrampaJungla::Pendulo, Vector2{2850,350}, 0, 195);
        trampasJungla.emplace_back(TipoTrampaJungla::Pendulo, Vector2{3450,350}, 1.2f, 195);
        trampasJungla.emplace_back(TipoTrampaJungla::Bloque, Vector2{3780,350}, 0, 225);
        trampasJungla.emplace_back(TipoTrampaJungla::Tronco, Vector2{5450,600}, 0, 850);
        trampasJungla.emplace_back(TipoTrampaJungla::Planta, Vector2{5800,600}, 0.6f);
        trampasJungla.emplace_back(TipoTrampaJungla::Dardo, Vector2{6050,548}, 2.0f, 210);
        obstaculos.emplace_back(6100,580,45,20);
        Reiniciar();
        return;
    }
    puntosAnclaje = {{500,250}, {1050,270}, {1450,250}, {1900,260}, {2170,260}, {2600,250}, {3100,270}, {3610,260}, {4100,250}, {4870,260}, {5400,250}, {5840,260}};
    obstaculos.emplace_back(720,580,45,20);
    obstaculos.emplace_back(1350,580,45,20);
    obstaculos.emplace_back(1830,575,50,25);
    obstaculos.emplace_back(2370,575,55,25);
    obstaculos.emplace_back(2800,575,55,25);
    obstaculos.emplace_back(3360,570,55,30);
    obstaculos.emplace_back(4010,570,50,30);
    obstaculos.emplace_back(4625,570,60,30);
    obstaculos.emplace_back(5330,570,60,30);
    obstaculos.emplace_back(6100,570,65,30);
    // Pinchos elevados: 55 px libres a cada lado.
    obstaculos.emplace_back(4125,366,50,24);
    obstaculos.emplace_back(5205,466,50,24);
    sierras.emplace_back(Vector2{1660,345}, Vector2{1790,345}, 21, 4.50f);
    sierras.emplace_back(Vector2{2730,245}, Vector2{2730,340}, 20, 4.75f, 0.25f);
    sierras.emplace_back(Vector2{2460,270}, Vector2{2550,270}, 22, 3.12f);
    sierras.emplace_back(Vector2{3180,330}, Vector2{3180,455}, 22, 3.50f, 0.5f);
    sierras.emplace_back(Vector2{3880,365}, Vector2{3980,365}, 23, 3.00f);
    sierras.emplace_back(Vector2{5640,330}, Vector2{5640,455}, 23, 3.25f, 0.25f);
    Reiniciar();
}

void Juego::Ejecutar() {
    camara.offset = {640,360};
    camara.target = {640,360};
    camara.rotation = 0;
    camara.zoom = 1.0f;

    enum class Pantalla { Menu, Mapas, Controles, Jugando, Pausa };
    Pantalla pantalla = Pantalla::Menu;
    bool salir = false;
    float cuentaRegresiva = 3.75f;
    // Cuenta regresiva reiniciada al comenzar cada ronda.

    auto boton = [](Rectangle r, const char* titulo) {
        Vector2 raton = GetMousePosition();
        bool encima = CheckCollisionPointRec(raton, r);
        DrawRectangleRounded(r, 0.20f, 8, encima ? Color{44, 185, 214, 255}
                                                      : Color{28, 48, 79, 255});
        DrawRectangleRoundedLinesEx(r, 0.20f, 8, 2.0f, GOLD);
        int tam = 25;
        DrawText(titulo, (int)(r.x + (r.width - MeasureText(titulo,tam))/2),
                 (int)(r.y + (r.height - tam)/2), tam, WHITE);
        return encima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    };

    while (!WindowShouldClose() && !salir) {
        float deltaTime = GetFrameTime();
        bool seReinicio = false;
        if (pantalla == Pantalla::Jugando && IsKeyPressed(KEY_ESCAPE)) {
            pantalla = Pantalla::Pausa;
        } else if (pantalla == Pantalla::Pausa && IsKeyPressed(KEY_ESCAPE)) {
            pantalla = Pantalla::Jugando;
        }

        if (pantalla == Pantalla::Jugando && IsKeyPressed(KEY_R)) {
            Reiniciar();
            cuentaRegresiva = 3.75f;
            seReinicio = true;
        } else if (pantalla == Pantalla::Jugando &&
                   (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))) {
            if (carrera.EstaTerminada() && !carrera.PartidaTerminada()) {
                ReiniciarJugadores();
                carrera.NuevaRonda();
                cuentaRegresiva = 3.75f;
                seReinicio = true;
            }
        }

        // Solo avanzar la cuenta regresiva mientras se esta jugando.
        // ESC durante la cuenta regresiva la congela al pausar.
        if (pantalla == Pantalla::Jugando && cuentaRegresiva > 0.0f &&
            !seReinicio && !carrera.EstaTerminada()) {
            cuentaRegresiva = std::max(0.0f, cuentaRegresiva - std::min(deltaTime, 0.1f));
        }

        if (pantalla == Pantalla::Jugando && cuentaRegresiva <= 0.0f &&
            !seReinicio && !carrera.EstaTerminada()) {
            for (Sierra& sierra : sierras) sierra.Actualizar(deltaTime);
            for (auto& trampa : trampasJungla) trampa.Actualizar(deltaTime);
            for (auto& trampa : trampasPlaya) trampa.Actualizar(deltaTime);
            // El gancho se actualiza ANTES de la fisica del jugador.
            auto buscarAnclaje = [&](Vector2 origen, Vector2& destino) {
                float minima = 450.0f;
                bool encontrado = false;
                for (const Vector2& punto : puntosAnclaje) {
                    float distancia = Vector2Distance(origen, punto);
                    if (distancia < minima) {
                        minima = distancia;
                        destino = punto;
                        encontrado = true;
                    }
                }
                return encontrado;
            };

            auto actualizarGancho = [&](Gancho& gancho, Jugador& jugador, int tecla) {
                Rectangle r = jugador.ObtenerRectangulo();
                Vector2 centro = {r.x + r.width/2, r.y + r.height/2};
                if (IsKeyDown(tecla)) {
                    if (!gancho.EstaActivo()) {
                        Vector2 destino{};
                        if (buscarAnclaje(centro, destino)) gancho.Lanzar(centro, destino);
                    }
                } else {
                    gancho.Soltar();
                }
                jugador.ConfigurarGancho(gancho.EstaActivo(),
                                        gancho.EstaActivo() ? gancho.ObtenerAnclaje() : Vector2{0,0});
            };

            actualizarGancho(gancho1, jugador1, KEY_E);
            actualizarGancho(gancho2, jugador2, KEY_L);

            // Mientras el gancho esta activo: Shift recoge, abajo extiende.
            // Los controles se aplican solo si hay una cuerda conectada.
            jugador1.AjustarLongitudCuerda(deltaTime,
                gancho1.EstaActivo() && IsKeyDown(KEY_LEFT_SHIFT),
                gancho1.EstaActivo() && IsKeyDown(KEY_S));
            jugador2.AjustarLongitudCuerda(deltaTime,
                gancho2.EstaActivo() && (IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_K)),
                gancho2.EstaActivo() && IsKeyDown(KEY_DOWN));

            nivel.ActualizarBalsas(deltaTime);
            jugador1.Actualizar(deltaTime, nivel.ObtenerPlataformas());
            jugador2.Actualizar(deltaTime, nivel.ObtenerPlataformas());

            carrera.Actualizar(deltaTime, jugador1.ObtenerRectangulo(),
                               jugador2.ObtenerRectangulo());
            // Eliminar al jugador cuando sale por debajo de la pantalla.
            if (!carrera.EstaTerminada()) {
                if (jugador1.ObtenerRectangulo().y >= 720.0f) carrera.EliminarJugador(1);
                else if (jugador2.ObtenerRectangulo().y >= 720.0f) carrera.EliminarJugador(2);
            }
            if (!carrera.EstaTerminada()) {
                Rectangle r1 = jugador1.ObtenerRectangulo();
                Rectangle r2 = jugador2.ObtenerRectangulo();
                for (const Obstaculo& obstaculo : obstaculos) {
                    if (obstaculo.Colisiona(r1)) { carrera.EliminarJugador(1); break; }
                    if (obstaculo.Colisiona(r2)) { carrera.EliminarJugador(2); break; }
                }
            }
            if (!carrera.EstaTerminada()) {
                for (const Sierra& sierra : sierras) {
                    if (sierra.Colisiona(jugador1.ObtenerRectangulo())) { carrera.EliminarJugador(1); break; }
                    if (sierra.Colisiona(jugador2.ObtenerRectangulo())) { carrera.EliminarJugador(2); break; }
                }
            }
        }

        if (pantalla == Pantalla::Jugando && cuentaRegresiva <= 0.0f && !seReinicio && !carrera.EstaTerminada()) {
            for (const auto& trampa : trampasJungla) {
                if (trampa.Colisiona(jugador1.ObtenerRectangulo())) { carrera.EliminarJugador(1); break; }
                if (trampa.Colisiona(jugador2.ObtenerRectangulo())) { carrera.EliminarJugador(2); break; }
            }
        }
        if (pantalla == Pantalla::Jugando && cuentaRegresiva <= 0.0f && !seReinicio && !carrera.EstaTerminada()) {
            for (const auto& trampa : trampasPlaya) {
                if (trampa.Colisiona(jugador1.ObtenerRectangulo())) { carrera.EliminarJugador(1); break; }
                if (trampa.Colisiona(jugador2.ObtenerRectangulo())) { carrera.EliminarJugador(2); break; }
            }
        }
        Rectangle rect1 = jugador1.ObtenerRectangulo();
        Rectangle rect2 = jugador2.ObtenerRectangulo();
        float centro1 = rect1.x + rect1.width/2;
        float centro2 = rect2.x + rect2.width/2;
        float objetivoX = Clamp(std::max(centro1,centro2),640.0f,5760.0f);
        camara.target = {objetivoX,360};
        float bordeIzquierdo = camara.target.x - camara.offset.x/camara.zoom;
        if (pantalla == Pantalla::Jugando && cuentaRegresiva <= 0.0f &&
            !seReinicio && !carrera.EstaTerminada()) {
            if (rect1.x + rect1.width < bordeIzquierdo) carrera.EliminarJugador(1);
            else if (rect2.x + rect2.width < bordeIzquierdo) carrera.EliminarJugador(2);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camara);
        nivel.Dibujar(camara.target.x);
        for (const Vector2& punto : puntosAnclaje) {
            if (nivel.EsJungla()) {
                DrawLineEx({punto.x-12,0},{punto.x,punto.y},5,Color{55,100,46,255});
                DrawLineEx({punto.x-9,0},{punto.x+2,punto.y},2,Color{128,172,68,255});
            }
            float pulso = 1.5f + 1.0f * sinf((float)GetTime() * 3.0f + punto.x * 0.01f);
            DrawCircleV(punto, 16.0f + pulso, Fade(ORANGE, 0.16f));
            DrawCircleV(punto, 12.0f, Color{48, 61, 83, 255});
            DrawCircleV(punto, 8.0f, Color{255, 175, 40, 255});
            DrawCircleV(punto, 4.0f, Color{255, 245, 175, 255});
            DrawCircleLines((int)punto.x, (int)punto.y, 12.0f, BLACK);
            DrawLineEx({punto.x - 6.0f, punto.y + 12.0f},
                       {punto.x + 6.0f, punto.y + 12.0f}, 2.0f, GOLD);
        }
        for (const Obstaculo& obstaculo : obstaculos) obstaculo.Dibujar();
        for (const Sierra& sierra : sierras) sierra.Dibujar();
        for (const auto& trampa : trampasJungla) trampa.Dibujar();
        for (const auto& trampa : trampasPlaya) trampa.Dibujar();
        Rectangle r1 = jugador1.ObtenerRectangulo();
        Rectangle r2 = jugador2.ObtenerRectangulo();
        gancho1.Dibujar({r1.x+r1.width/2,r1.y+r1.height/2});
        gancho2.Dibujar({r2.x+r2.width/2,r2.y+r2.height/2});
        jugador1.Dibujar();
        jugador2.Dibujar();
        carrera.DibujarMeta();
        EndMode2D();
        DrawText("HUSS HUSS",500,20,30,BLACK);
        if (pantalla == Pantalla::Jugando || pantalla == Pantalla::Pausa) {
            carrera.DibujarInterfaz();
        }

        // Texto de salida de la cuenta regresiva, sin cambiar el mundo fisico.
        if (pantalla == Pantalla::Jugando && cuentaRegresiva > 0.0f) {
            const char* mensaje = cuentaRegresiva > 2.75f ? "3" :
                                  cuentaRegresiva > 1.75f ? "2" :
                                  cuentaRegresiva > 0.75f ? "1" : "YA!";
            int tam = (mensaje[0] == 'Y') ? 100 : 132;
            DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.30f));
            DrawText(mensaje, (1280 - MeasureText(mensaje, tam))/2 + 4,
                     255 + 4, tam, BLACK);
            DrawText(mensaje, (1280 - MeasureText(mensaje, tam))/2,
                     255, tam, GOLD);
            DrawText("PREPARATE PARA CORRER", 455, 435, 25, WHITE);
        }

        if (pantalla != Pantalla::Jugando) {
            DrawRectangle(0, 0, 1280, 720, Fade(Color{6, 12, 28, 255}, 0.91f));
            DrawText("HUSS HUSS", 430, 96, 62, GOLD);
            DrawText("CARRERAS DE AVENTURA", 455, 170, 24, SKYBLUE);

            if (pantalla == Pantalla::Menu) {
                if (boton({460, 270, 360, 64}, "JUGAR")) {
                    pantalla = Pantalla::Mapas;
                }
                if (boton({460, 360, 360, 64}, "CONTROLES")) pantalla = Pantalla::Controles;
                if (boton({460, 450, 360, 64}, "SALIR")) salir = true;
            } else if (pantalla == Pantalla::Mapas) {
                DrawText("SELECCIONA EL MAPA", 465, 225, 28, WHITE);
                if (boton({85, 310, 350, 90}, "PUEBLA 2077")) {
                    CargarMapa(TipoMapa::Industrial);
                    cuentaRegresiva = 3.75f;
                    pantalla = Pantalla::Jugando;
                }
                if (boton({465, 310, 350, 90}, "PUEBLA 1400")) {
                    CargarMapa(TipoMapa::Jungla);
                    cuentaRegresiva = 3.75f;
                    pantalla = Pantalla::Jugando;
                }
                if (boton({845, 310, 350, 90}, "CANCUN 2026")) {
                    CargarMapa(TipoMapa::Playa);
                    cuentaRegresiva = 3.75f;
                    pantalla = Pantalla::Jugando;
                }
                if (boton({460, 490, 360, 64}, "VOLVER")) pantalla = Pantalla::Menu;
            } else if (pantalla == Pantalla::Controles) {
                DrawText("VERDE: A/D mover | W saltar | SHIFT sprint | S slide | E gancho", 210, 265, 20, WHITE);
                DrawText("ROJO: FLECHAS mover/saltar/slide | SHIFT DER sprint | L gancho", 185, 305, 20, WHITE);
                DrawText("GANCHO: SHIFT recoge (rojo: K tambien) | ABAJO/S extiende", 210, 350, 20, GOLD);
                DrawText("ESC pausa | ENTER siguiente ronda | R reiniciar", 315, 390, 20, WHITE);
                if (boton({460, 485, 360, 64}, "VOLVER")) pantalla = Pantalla::Menu;
            } else if (pantalla == Pantalla::Pausa) {
                DrawText("PAUSA", 550, 240, 42, WHITE);
                if (boton({460, 335, 360, 64}, "CONTINUAR")) pantalla = Pantalla::Jugando;
                if (boton({460, 430, 360, 64}, "MENU PRINCIPAL")) {
                    Reiniciar();
                    pantalla = Pantalla::Menu;
                }
            }
        }
        EndDrawing();
    }
    Jugador::LiberarSprites();
    CloseWindow();
}

void Juego::ReiniciarJugadores() {
    nivel.ReiniciarBalsas();
    for (Sierra& sierra : sierras) sierra.Reiniciar();
    for (auto& trampa : trampasJungla) trampa.Reiniciar();
    for (auto& trampa : trampasPlaya) trampa.Reiniciar();
    gancho1.Soltar();
    gancho2.Soltar();
    jugador1 = Jugador(100,300,GREEN,KEY_A,KEY_D,KEY_W,KEY_LEFT_SHIFT,KEY_S);
    jugador2 = Jugador(200,300,RED,KEY_LEFT,KEY_RIGHT,KEY_UP,KEY_RIGHT_SHIFT,KEY_DOWN);
}

void Juego::Reiniciar() {
    ReiniciarJugadores();
    carrera.Reiniciar();
}