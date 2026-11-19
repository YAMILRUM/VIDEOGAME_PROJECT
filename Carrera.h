
#ifndef CARRERA_H
#define CARRERA_H

#include "raylib.h"

class Carrera {
private:
    float tiempo;
    int ganador;
    bool terminada;

    int puntosJugador1;
    int puntosJugador2;
    int puntosParaGanar;

    Rectangle meta;

    void RegistrarVictoria(int jugador);

public:
    Carrera();

    void Actualizar(
        float deltaTime,
        Rectangle jugador1,
        Rectangle jugador2
    );

    void DibujarMeta() const;
    void DibujarInterfaz() const;

    void EliminarJugador(int numeroJugador);

    void Reiniciar();
    void NuevaRonda();

    bool EstaTerminada() const;
    bool PartidaTerminada() const;
};

#endif
