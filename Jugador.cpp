#include "Jugador.h"
#include "Nivel.h"

Jugador::Jugador(Vector2 pos, Color color, Controles controles)
    : caja{pos.x, pos.y, 40, 40}, velocidad{0, 0}, enSuelo(false),
      vivo(true), puntos(0), color(color), controles(controles) {}

void Jugador::Actualizar(const Nivel& nivel) {}
void Jugador::Dibujar() const { DrawRectangleRec(caja, color); }
Rectangle Jugador::GetCaja() const { return caja; }
bool Jugador::EstaVivo() const { return vivo; }
void Jugador::Eliminar() { vivo = false; }
int Jugador::GetPuntos() const { return puntos; }
void Jugador::SumarPunto() { puntos++; }