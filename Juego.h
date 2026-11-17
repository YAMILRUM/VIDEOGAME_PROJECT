#ifndef JUEGO_H
#define JUEGO_H

#include "Jugador.h"
#include "Nivel.h"

class Juego {
private:
    Jugador jugador{300, 300};
    Nivel nivel;

public:
    Juego();
    void Ejecutar();
};

#endif
