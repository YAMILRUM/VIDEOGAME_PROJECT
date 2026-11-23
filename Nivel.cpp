#include "Nivel.h"
#include "raylib.h"
#include <cmath>

namespace {
Color ColorRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
    return Color{r, g, b, a};
}

void DibujarPlataforma(Rectangle p, bool suelo, bool tunel) {
    const Color metal = ColorRGB(47, 64, 87);
    const Color metalOscuro = ColorRGB(27, 38, 58);
    const Color borde = ColorRGB(17, 29, 48);
    const Color luz = ColorRGB(58, 223, 237);
    const Color peligro = ColorRGB(249, 179, 58);

    // Todos los detalles son visuales; p no se modifica.
    DrawRectangleRec(p, metalOscuro);
    DrawRectangleLinesEx(p, 2.0f, borde);
    DrawRectangle(static_cast<int>(p.x), static_cast<int>(p.y),
                  static_cast<int>(p.width), suelo ? 14 : 7, metal);
    DrawRectangle(static_cast<int>(p.x), static_cast<int>(p.y),
                  static_cast<int>(p.width), 3, luz);

    if (suelo) {
        for (int x = static_cast<int>(p.x) + 24; x < p.x + p.width; x += 75) {
            DrawRectangle(x, static_cast<int>(p.y + 26), 40, 3, ColorRGB(75, 93, 117));
            DrawCircle(x + 8, static_cast<int>(p.y + 12), 2, ColorRGB(155, 171, 187));
        }
    } else {
        for (int x = static_cast<int>(p.x) + 14; x < p.x + p.width - 5; x += 36) {
            DrawCircle(x, static_cast<int>(p.y + 10), 2, ColorRGB(154, 171, 188));
        }
    }

    if (tunel) {
        // Panel superior industrial y franjas de advertencia.
        DrawRectangle(static_cast<int>(p.x + 5), static_cast<int>(p.y + 10),
                      static_cast<int>(p.width - 10), 32, metal);
        for (int x = static_cast<int>(p.x + 9); x < p.x + p.width - 12; x += 28) {
            DrawRectangle(x, static_cast<int>(p.y + 44), 14, 7, peligro);
        }
        DrawRectangle(static_cast<int>(p.x + 10), static_cast<int>(p.y + 18),
                      static_cast<int>(p.width - 20), 3, Fade(luz, 0.7f));
    }
}
} // namespace

Nivel::Nivel() {
    // Misma geometria original: NO cambiar las colisiones.
    plataformas.push_back({0, 600, 3200, 120});
    plataformas.push_back({150, 480, 200, 20});
    plataformas.push_back({450, 380, 200, 20});
    plataformas.push_back({750, 450, 200, 20});
    plataformas.push_back({1100, 400, 200, 20});
    plataformas.push_back({1450, 320, 200, 20});
    plataformas.push_back({1800, 450, 200, 20});
    plataformas.push_back({2150, 370, 200, 20});
    plataformas.push_back({2500, 460, 200, 20});
    plataformas.push_back({2850, 350, 200, 20});
    plataformas.push_back({950, 500, 220, 60});
    plataformas.push_back({2250, 500, 220, 60});
}

void Nivel::Dibujar() {
    // El juego ya esta dentro de BeginMode2D(camara).
    // Fondo opaco cubre el blanco original sin tocar Juego.cpp.
    DrawRectangle(-2000, -200, 8500, 1100, ColorRGB(13, 24, 49));

    // Luna y halo urbano.
    DrawCircle(420, 110, 58, Fade(ColorRGB(69, 106, 159), 0.26f));
    DrawCircle(420, 110, 34, ColorRGB(180, 208, 228));
    DrawCircle(410, 103, 5, Fade(ColorRGB(90, 125, 160), 0.32f));

    // Siluetas de la ciudad: dos capas de profundidad.
    // Se usa una coordenada visual estable en vez de depender de texturas.
    for (int i = -3; i < 50; ++i) {
        int x = i * 110;
        int alto = 130 + ((i * i * 19 + i * 11 + 10000) % 120);
        DrawRectangle(x, 550 - alto, 88, alto, ColorRGB(27, 45, 76));
        for (int f = 0; f < (alto - 15) / 25; ++f) {
            for (int c = 0; c < 3; ++c) {
                if ((i + f * 2 + c) % 4 != 0)
                    DrawRectangle(x + 12 + c * 24, 550 - alto + 16 + f * 25,
                                  9, 11, Fade(ColorRGB(72, 176, 217), 0.45f));
            }
        }
    }
    for (int i = -2; i < 45; ++i) {
        int x = i * 145 + 40;
        int alto = 95 + ((i * i * 31 + 30000) % 125);
        DrawRectangle(x, 590 - alto, 116, alto, ColorRGB(35, 56, 85));
        for (int f = 0; f < (alto - 18) / 29; ++f) {
            for (int c = 0; c < 4; ++c) {
                if ((i * 3 + f + c) % 3 != 0)
                    DrawRectangle(x + 13 + c * 25, 590 - alto + 16 + f * 29,
                                  10, 12, Fade(ColorRGB(245, 187, 96), 0.55f));
            }
        }
    }

    // Plataformas: las ultimas dos son los techos de los tuneles.
    for (size_t i = 0; i < plataformas.size(); ++i) {
        DibujarPlataforma(plataformas[i], i == 0, i >= 10);
    }
}

const std::vector<Rectangle>& Nivel::ObtenerPlataformas() const {
    return plataformas;
}