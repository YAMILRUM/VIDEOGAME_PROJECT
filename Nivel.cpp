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

Nivel::Nivel() { Cargar(TipoMapa::Industrial); }

void Nivel::Cargar(TipoMapa nuevo) {
    tipo = nuevo;
    plataformas.clear();
    if (tipo == TipoMapa::Jungla) {
        plataformas = {{0,600,1800,120},{2040,600,1960,120},{4260,600,2140,120},
            {430,500,180,20},{700,405,180,20},{1150,500,180,20},
            {1850,480,140,20},{2400,500,180,20},{2670,405,180,20},
            {3500,500,180,20},{4060,480,140,20},{4750,500,180,20},
            {5100,405,180,20},{5700,500,180,20}};
        return;
    }
    // Tramos de suelo
    plataformas.push_back({0, 600, 2050, 120});
    plataformas.push_back({2290, 600, 1190, 120});
    plataformas.push_back({3740, 600, 980, 120});
    plataformas.push_back({5020, 600, 680, 120});
    plataformas.push_back({5980, 600, 420, 120});
    // Plataformas elevadas
    plataformas.push_back({420, 500, 160, 20});
    plataformas.push_back({520, 390, 160, 20});
    plataformas.push_back({780, 420, 150, 20});
    plataformas.push_back({1200, 420, 150, 20});
    plataformas.push_back({1380, 390, 160, 20});
    plataformas.push_back({1650, 490, 160, 20});
    plataformas.push_back({1900, 490, 150, 20});
    plataformas.push_back({2130, 460, 100, 20});
    plataformas.push_back({2400, 390, 160, 20});
    plataformas.push_back({2620, 390, 160, 20});
    plataformas.push_back({2900, 490, 160, 20});
    plataformas.push_back({3300, 390, 160, 20});
    plataformas.push_back({3540, 460, 140, 20});
    plataformas.push_back({3850, 490, 150, 20});
    plataformas.push_back({4070, 390, 160, 20});
    plataformas.push_back({4470, 490, 150, 20});
    plataformas.push_back({4800, 460, 140, 20});
    plataformas.push_back({5150, 490, 160, 20});
    plataformas.push_back({5400, 390, 160, 20});
    plataformas.push_back({5780, 460, 120, 20});
    // Tuneles
    plataformas.push_back({950, 500, 180, 60});
    plataformas.push_back({1510, 500, 120, 60});
    plataformas.push_back({3070, 500, 180, 60});
    plataformas.push_back({4260, 500, 180, 60});
    plataformas.push_back({5520, 500, 150, 60});
}

void Nivel::Dibujar(float camaraX) {
    if (tipo == TipoMapa::Jungla) {
        const float t = (float)GetTime();
        const float desplazamiento = (camaraX - 640.0f);
        // Capas decorativas: nunca se agregan a plataformas.
        DrawRectangleGradientV(-2000,-200,11000,1100,ColorRGB(32,83,79),ColorRGB(203,220,151));
        float sol = 420 + desplazamiento * 0.88f;
        DrawCircleV({sol,110},110,ColorRGB(246,222,143,35));
        DrawCircleV({sol,110},65,ColorRGB(255,235,170,190));
        for(int capa=0;capa<3;++capa){
            float par = desplazamiento * (0.78f-capa*0.22f);
            Color c = capa==0?ColorRGB(104,155,126):capa==1?ColorRGB(66,119,91):ColorRGB(37,85,65);
            for(int i=-4;i<31;++i){
                float x=i*290+par;
                float h=180+(i*i*37+capa*71)%160;
                DrawEllipse((int)x, (int)(490-h*.5f),205,(int)h,c);
                DrawRectangle((int)x-15,(int)(490-h*.5f),30,(int)(h*.5f+125),c);
            }
        }
        // Piramides escalonadas, con escalinata y santuario.
        for(int k=0;k<3;++k){
            float x=1100+k*2200+desplazamiento*.18f;
            float escala=k==2?1.18f:.85f;
            for(int j=0;j<7;++j){
                float w=(620-j*72)*escala, y=555-j*43*escala;
                DrawRectangleRec({x-w/2,y,w,46*escala},ColorRGB(104,119,92));
                DrawRectangleRec({x-w/2,y,w,5*escala},ColorRGB(160,168,121));
                for(float a=x-w/2+24;a<x+w/2-10;a+=48) DrawLineEx({a,y+6},{a,y+40*escala},1,ColorRGB(78,99,79));
                DrawRectangleRec({x-42*escala,y,84*escala,46*escala},ColorRGB(139,145,108));
                for(int s=0;s<4;++s) DrawLineEx({x-42*escala,y+s*11*escala},{x+42*escala,y+s*11*escala},2,ColorRGB(91,110,84));
            }
            float y=555-6*43*escala;
            DrawRectangleRec({x-68*escala,y-75*escala,136*escala,75*escala},ColorRGB(137,145,104));
            DrawRectangleRec({x-20*escala,y-50*escala,40*escala,50*escala},ColorRGB(42,70,58));
            DrawRectangleRec({x-80*escala,y-80*escala,160*escala,12*escala},ColorRGB(171,175,119));
        }
        // Arboles cercanos con raices, ramas y copas superpuestas.
        for(int i=-2;i<23;++i){
            float x=i*325+90+desplazamiento*.06f;
            float h=230+(i*i*29)%125;
            DrawLineEx({x,610},{x+12,610-h},32,ColorRGB(51,77,49));
            DrawLineEx({x+5,590},{x+17,610-h},9,ColorRGB(90,108,61));
            DrawLineEx({x,555},{x-60,610-h*.65f},15,ColorRGB(51,77,49));
            for(int j=0;j<5;++j){
                float a=x+(j-2)*42, b=610-h-22+(j*j*17)%35;
                DrawEllipse((int)a,(int)b,75,48,ColorRGB(29,91,54));
                DrawEllipse((int)a,(int)b-15,63,32,ColorRGB(49,119,62));
            }
        }
        // Cascadas y rios solo en los huecos reales del suelo.
        for(float x : {1800.0f,4000.0f}){
            float w=x<2000?240:260;
            DrawRectangleGradientH((int)x+35,245,95,475,ColorRGB(64,145,147,130),ColorRGB(183,231,211,190));
            for(int j=0;j<12;++j){
                float yy=245+fmodf(t*150+j*43,455);
                DrawLineEx({x+42+j*7,yy},{x+42+j*7,yy+20},2,ColorRGB(213,250,230,150));
            }
            DrawRectangleGradientV((int)x,675,(int)w,225,ColorRGB(47,144,146),ColorRGB(15,65,78));
            for(int j=0;j<8;++j){
                float a=x+12+j*27, yy=683+8*sinf(t*2+j);
                DrawLineEx({a,yy},{a+17,yy},2,ColorRGB(172,226,209));
            }
        }
        // Piedra jugable: borde superior brillante y detalles dentro del rectangulo.
        for(const Rectangle& p:plataformas){
            DrawRectangleRec(p,ColorRGB(81,76,58));
            DrawRectangleRec({p.x+2,p.y+5,p.width-4,p.height-7},ColorRGB(139,132,96));
            for(float y=p.y+9;y<p.y+p.height-3;y+=24){
                for(float x=p.x+3;x<p.x+p.width-3;x+=38){
                    float w=fminf(35,p.x+p.width-x-2), h=fminf(20,p.y+p.height-y-2);
                    if(w>0&&h>0) DrawRectangleRec({x,y,w,h},ColorRGB(153,145,106));
                }
            }
            DrawRectangleRec({p.x,p.y,p.width,5},ColorRGB(172,211,91));
            for(float x=p.x+4;x<p.x+p.width-5;x+=13){
                DrawRectangleRec({x,p.y+5,5,4},ColorRGB(83,139,55));
            }
            DrawRectangleLinesEx(p,1,ColorRGB(56,70,45));
        }
        // Portal decorativo detras de la meta existente.
        DrawRectangle(6210,430,32,170,ColorRGB(129,113,69));
        DrawRectangle(6350,430,32,170,ColorRGB(129,113,69));
        DrawRectangle(6200,410,192,32,ColorRGB(196,165,81));
        DrawRectangle(6220,400,152,12,ColorRGB(238,206,110));
        return;
    }
    // El juego ya esta dentro de BeginMode2D(camara).
    // Fondo opaco cubre el blanco original sin tocar Juego.cpp.
    DrawRectangle(-2000, -200, 11000, 1100, ColorRGB(13, 24, 49));

    // Luna y halo urbano.
    DrawCircle(420, 110, 58, Fade(ColorRGB(69, 106, 159), 0.26f));
    DrawCircle(420, 110, 34, ColorRGB(180, 208, 228));
    DrawCircle(410, 103, 5, Fade(ColorRGB(90, 125, 160), 0.32f));

    // Siluetas de la ciudad: dos capas de profundidad.
    // Se usa una coordenada visual estable en vez de depender de texturas.
    for (int i = -3; i < 60; ++i) {
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
    for (int i = -2; i < 46; ++i) {
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

    // Clasificar por geometria, no por indice.
    for (size_t i = 0; i < plataformas.size(); ++i) {
        const Rectangle& p = plataformas[i];
        DibujarPlataforma(p, p.y == 600, p.height == 60);
    }
}

const std::vector<Rectangle>& Nivel::ObtenerPlataformas() const {
    return plataformas;
}
