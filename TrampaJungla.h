#ifndef TRAMPA_JUNGLA_H
#define TRAMPA_JUNGLA_H
#include "raylib.h"
enum class TipoTrampaJungla { Planta, Pendulo, Tronco, Dardo, Bloque };
class TrampaJungla {
    TipoTrampaJungla tipo;
    Vector2 origen, centro;
    float tiempo = 0, fase, recorrido;
    bool activa = false;
public:
    TrampaJungla(TipoTrampaJungla tipo, Vector2 origen, float fase=0, float recorrido=0);
    void Actualizar(float dt);
    void Reiniciar();
    void Dibujar() const;
    bool Colisiona(Rectangle jugador) const;
};
#endif
