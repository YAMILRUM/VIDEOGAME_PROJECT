#pragma once
#include "raylib.h"
#include "Jugador.h"
#include "Nivel.h"
#include <vector>

enum class Estado { MENU, JUGANDO, FIN_RONDA, FIN_JUEGO };

class Juego {
public:
    Juego();
    void Ejecutar();

private:
    void Actualizar();
    void Dibujar();
    void ActualizarCamara();
    void RevisarEliminaciones();

    Estado estado;
    std::vector<Jugador> jugadores;
    Nivel nivel;
    Camera2D camara;
};