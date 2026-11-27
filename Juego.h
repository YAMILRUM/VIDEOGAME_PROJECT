#ifndef JUEGO_H
#define JUEGO_H
#include "Jugador.h"
#include "Nivel.h"
#include "Carrera.h"
#include "raylib.h"
#include "Obstaculo.h"
#include "Gancho.h"
#include "Sierra.h"
#include "TrampaJungla.h"
#include "TrampaPlaya.h"
#include <vector>
class Juego {
private:
    Jugador jugador1{100,300,GREEN,KEY_A,KEY_D,KEY_W,KEY_LEFT_SHIFT,KEY_S};
    Jugador jugador2{200,300,RED,KEY_J,KEY_K,KEY_I,KEY_RIGHT_SHIFT,KEY_M};
    Nivel nivel;
    void CargarMapa(TipoMapa tipo);
    Carrera carrera;
    Camera2D camara{};
    std::vector<Obstaculo> obstaculos;
    std::vector<Sierra> sierras;
    std::vector<TrampaJungla> trampasJungla;
    std::vector<TrampaPlaya> trampasPlaya;
    Gancho gancho1;
    Gancho gancho2;
    std::vector<Vector2> puntosAnclaje;
public:
    Juego();
    void Ejecutar();
    void Reiniciar();
    void ReiniciarJugadores();
};
#endif
