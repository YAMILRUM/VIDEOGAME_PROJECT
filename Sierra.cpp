#include "Sierra.h"
#include <cmath>
#include <algorithm>
namespace { constexpr float PI_S = 3.14159265358979323846f; }
Sierra::Sierra(Vector2 a, Vector2 b, float r, float p, float fase)
    : inicio(a), fin(b), centro(a), radio(r), periodo(std::max(p, 0.1f)),
      faseInicial(fase), tiempo(0), angulo(0) { Reiniciar(); }
void Sierra::Reiniciar() {
    tiempo = 0; angulo = 0; Actualizar(0);
}
void Sierra::Actualizar(float dt) {
    dt = std::clamp(dt, 0.0f, 0.033f);
    tiempo = std::fmod(tiempo + dt, periodo);
    float t = 0.5f - 0.5f * std::cos(2 * PI_S * (tiempo / periodo + faseInicial));
    centro = {inicio.x + (fin.x-inicio.x)*t, inicio.y + (fin.y-inicio.y)*t};
    angulo = std::fmod(angulo + dt * 360.0f, 360.0f);
}
bool Sierra::Colisiona(Rectangle jugador) const {
    return CheckCollisionCircleRec(centro, radio, jugador);
}
void Sierra::Dibujar() const {
    DrawLineEx(inicio, fin, 8, Color{47,64,87,255});
    DrawLineEx(inicio, fin, 2, GOLD);
    DrawCircleV(inicio, 5, GRAY); DrawCircleV(fin, 5, GRAY);
    DrawCircleV(centro, radio*0.78f, Color{180,194,207,255});
    for (int i=0; i<12; ++i) {
        float a = angulo*PI_S/180 + i*2*PI_S/12;
        auto punto = [&](float offset, float r) { return Vector2{centro.x+std::cos(a+offset)*r, centro.y+std::sin(a+offset)*r}; };
        DrawTriangle(punto(0,radio), punto(-0.24f,radio*0.72f), punto(0.24f,radio*0.72f), LIGHTGRAY);
        DrawCircleV(punto(0,radio*0.48f), 2, DARKGRAY);
    }
    DrawCircleV(centro, radio*0.28f, DARKGRAY);
    DrawCircleV(centro, radio*0.14f, GOLD);
}
