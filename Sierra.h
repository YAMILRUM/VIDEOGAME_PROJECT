#ifndef SIERRA_H
#define SIERRA_H
#include "raylib.h"
class Sierra {
private:
    Vector2 inicio, fin, centro;
    float radio, periodo, faseInicial, tiempo, angulo;
public:
    Sierra(Vector2 inicio, Vector2 fin, float radio, float periodo, float fase = 0.0f);
    void Actualizar(float dt);
    void Reiniciar();
    void Dibujar() const;
    bool Colisiona(Rectangle jugador) const;
};
#endif
