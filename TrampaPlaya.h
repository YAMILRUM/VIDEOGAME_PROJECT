#ifndef TRAMPA_PLAYA_H
#define TRAMPA_PLAYA_H
#include "raylib.h"
enum class TipoTrampaPlaya { Cangrejo, Coco, Tiburon, Canon, Tentaculo };
class TrampaPlaya {
    TipoTrampaPlaya tipo;
    Vector2 origen;
    float alcance, velocidad, fase, tiempo=0, x=0, y=0;
    bool cayendo=false, aviso=false;
    Vector2 PuntoTentaculo(float u) const;
public:
    TrampaPlaya(TipoTrampaPlaya tipo, Vector2 origen, float alcance, float velocidad, float fase=0);
    void Reiniciar();
    void Actualizar(float dt);
    void Dibujar() const;
    bool Colisiona(Rectangle jugador) const;
};
#endif
