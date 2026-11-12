#pragma once
#include "raylib.h"

class Nivel;

struct Controles {
    int izquierda, derecha, salto, deslizar;
};

class Jugador {
public:
    Jugador(Vector2 pos, Color color, Controles controles);
    void Actualizar(const Nivel& nivel);
    void Dibujar() const;
    Rectangle GetCaja() const;
    bool EstaVivo() const;
    void Eliminar();
    int  GetPuntos() const;
    void SumarPunto();

private:
    Rectangle caja;
    Vector2 velocidad;
    bool enSuelo, vivo;
    int puntos;
    Color color;
    Controles controles;
};