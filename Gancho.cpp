
#include "Gancho.h"
#include "raymath.h"

Gancho::Gancho() {
    activo = false;
    puntoAnclaje = {0, 0};
    alcanceMaximo = 450.0f;
}

void Gancho::Lanzar(Vector2 origen, Vector2 destino) {

    float distancia = Vector2Distance(origen, destino);

    if (distancia <= alcanceMaximo) {
        puntoAnclaje = destino;
        activo = true;
    }
}

void Gancho::Soltar() {
    activo = false;
}

void Gancho::Dibujar(Vector2 origen) const {

    if (!activo) {
        return;
    }

    DrawLineEx(
        origen,
        puntoAnclaje,
        3.0f,
        DARKGRAY
    );

    DrawCircleV(
        puntoAnclaje,
        6.0f,
        ORANGE
    );
}

bool Gancho::EstaActivo() const {
    return activo;
}

Vector2 Gancho::ObtenerAnclaje() const {
    return puntoAnclaje;
}
