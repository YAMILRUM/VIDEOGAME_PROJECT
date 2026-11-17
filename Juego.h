
#ifndef JUEGO_H
#define JUEGO_H

#include "Jugador.h"
#include "Nivel.h"
#include "Carrera.h"

class Juego {
private:
    Jugador jugador1{
        400, 300,
        BLUE,
        KEY_A,
        KEY_D,
        KEY_W,
        KEY_LEFT_SHIFT
    };

    Jugador jugador2{
        900, 300,
        RED,
        KEY_LEFT, 
        KEY_RIGHT,
        KEY_UP, 
        KEY_RIGHT_SHIFT
    };

    Nivel nivel;
    Carrera carrera;

public:
    Juego();
    void Ejecutar();
    void Reiniciar();
};

#endif
