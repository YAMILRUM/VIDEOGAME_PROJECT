#ifndef NIVEL_H
#define NIVEL_H
#include "raylib.h"
#include <vector>
enum class TipoMapa { Industrial, Jungla };
class Nivel {
private:
    std::vector<Rectangle> plataformas;
    TipoMapa tipo = TipoMapa::Industrial;
public:
    Nivel();
    void Cargar(TipoMapa nuevo);
    void Dibujar(float camaraX = 640.0f);
    bool EsJungla() const { return tipo == TipoMapa::Jungla; }
    const std::vector<Rectangle>& ObtenerPlataformas() const;
};
#endif
