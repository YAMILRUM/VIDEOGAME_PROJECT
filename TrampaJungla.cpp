#include "TrampaJungla.h"
#include <cmath>
#include <algorithm>
namespace { constexpr float P=3.14159265f; }
TrampaJungla::TrampaJungla(TipoTrampaJungla t, Vector2 o, float f, float r)
: tipo(t), origen(o), centro(o), fase(f), recorrido(r) { Reiniciar(); }
void TrampaJungla::Reiniciar(){ tiempo=0; Actualizar(0); }
void TrampaJungla::Actualizar(float dt){
    tiempo += std::clamp(dt,0.0f,0.033f);
    if(tipo==TipoTrampaJungla::Planta){
        float t=std::fmod(tiempo+fase,3.6f); activa=t>=2.2f && t<3.1f;
        centro={origen.x,origen.y-35};
    } else if(tipo==TipoTrampaJungla::Pendulo){
        float a=0.72f*std::sin(2*P*(tiempo+fase)/4.8f);
        centro={origen.x+recorrido*std::sin(a),origen.y+recorrido*std::cos(a)}; activa=true;
    } else if(tipo==TipoTrampaJungla::Dardo){
        float t=std::fmod(tiempo+fase,4.5f);
        float d=(t-1.2f)*260.0f;
        activa=t>=1.2f && d<recorrido; centro={origen.x-d,origen.y};
    } else if(tipo==TipoTrampaJungla::Bloque){
        float t=std::fmod(tiempo+fase,5.5f);
        float f=t<2.0f?0.0f:t<2.65f?(t-2.0f)/0.65f:t<3.2f?1.0f:t<4.5f?1.0f-(t-3.2f)/1.3f:0.0f;
        centro={origen.x,origen.y+recorrido*f}; activa=t>=2.0f && t<4.5f;
    } else {
        float t=std::fmod(tiempo+fase,5.0f); float d=t*180;
        activa=d<recorrido; centro={origen.x-d,origen.y-24};
    }
}
bool TrampaJungla::Colisiona(Rectangle j) const {
    if(!activa) return false;
    if(tipo==TipoTrampaJungla::Dardo) return CheckCollisionRecs({centro.x-14,centro.y-4,28,8},j);
    if(tipo==TipoTrampaJungla::Bloque) return CheckCollisionRecs({centro.x-24,centro.y-24,48,48},j);
    return CheckCollisionCircleRec(centro,tipo==TipoTrampaJungla::Planta?25.0f:tipo==TipoTrampaJungla::Pendulo?28.0f:24.0f,j);
}
void TrampaJungla::Dibujar() const {
    if(tipo==TipoTrampaJungla::Planta){
        float x=centro.x,y=centro.y;
        float t=std::fmod(tiempo+fase,3.6f);
        bool aviso=t>=1.5f && t<2.2f;
        Color borde={34,48,32,255}, hoja={48,133,62,255}, luz={115,192,75,255};
        Vector2 anterior=origen;
        for(int j=1;j<=10;j++) {
            float f=j/10.0f;
            Vector2 p={x+7*std::sin(f*P),origen.y+(y-origen.y)*f};
            DrawLineEx(anterior,p,12,borde); DrawLineEx(anterior,p,7,hoja); anterior=p;
        }
        for(int lado : {-1,1}) {
            float q=origen.x+lado*17.f, h=origen.y-8;
            DrawEllipse((int)q,(int)h,19,9,borde);
            DrawEllipse((int)q,(int)h-2,17,6,hoja);
            DrawLineEx({origen.x,h},{q+lado*14,h-3},2,luz);
            for(int j=1;j<=3;j++) DrawLineEx({origen.x+lado*j*8.f,h-1},{origen.x+lado*(j*8.f+4),h-6},1,luz);
        }
        Color piel=activa?Color{198,45,69,255}:aviso?Color{242,133,37,255}:Color{147,46,100,255};
        float apertura=activa?12.f:aviso?3.f+2.f*std::sin(t*20):1.5f;
        DrawCircleV(centro,25,borde);
        DrawEllipse((int)x,(int)y,23,24,piel);
        DrawEllipse((int)x-7,(int)y-14,10,5,Fade(RAYWHITE,.25f));
        for(int j=0;j<5;j++) DrawCircleV({x-16+j*8.f,y-15+float(j%2)*4},2.5f,Color{245,186,123,255});
        DrawEllipse((int)x,(int)y+2,22,apertura+2,borde);
        DrawEllipse((int)x,(int)y+2,19,apertura,Color{55,15,35,255});
        if(activa) {
            DrawEllipse((int)x,(int)y+9,10,4,Color{232,88,118,255});
            for(int j=-2;j<=2;j++) {
                float q=x+j*7.f;
                DrawTriangle({q-3,y-8},{q,y-1},{q+3,y-8},Color{255,239,191,255});
                DrawTriangle({q-3,y+12},{q+3,y+12},{q,y+5},Color{255,239,191,255});
            }
        }
        if(aviso) DrawCircleLines((int)x,(int)y,26,Fade(ORANGE,.6f+.3f*std::sin(t*20)));
    } else if(tipo==TipoTrampaJungla::Pendulo){
        DrawLineEx(origen,centro,5,DARKGRAY); DrawCircleV(origen,8,GRAY);
        DrawCircleV(centro,20,DARKGRAY);
        for(int i=0;i<10;++i){float a=i*2*P/10; auto p=[&](float b,float r){return Vector2{centro.x+r*std::cos(b),centro.y+r*std::sin(b)};}; DrawTriangle(p(a,28),p(a-0.23f,18),p(a+0.23f,18),LIGHTGRAY);}
        DrawCircleV(centro,9,GRAY);
    } else if(tipo==TipoTrampaJungla::Dardo){
        float t=std::fmod(tiempo+fase,4.5f), x=origen.x,y=origen.y;
        Color stone={111,123,92,255}, shadow={48,65,49,255}, gold={218,173,79,255};
        DrawRectangleRec({x+12,y-28,32,56},shadow);
        DrawRectangleRec({x+8,y-25,32,50},stone);
        DrawRectangleRec({x+4,y-29,40,6},gold);
        DrawRectangleRec({x+4,y+23,40,6},shadow);
        DrawRectangleRounded({x,y-18,35,36},.25f,6,stone);
        DrawRectangleRec({x,y-5,22,10},shadow);
        DrawRectangleRec({x,y-5,22,2},gold);
        DrawTriangle({x+2,y-6},{x+8,y-1},{x+8,y-6},BEIGE);
        DrawLineEx({x+7,y-16},{x+23,y-19},5,shadow);
        Color eye=t<1.2f?ORANGE:Color{55,85,63,255};
        if(t<1.2f) DrawCircleV({x+14,y-12},9,Fade(ORANGE,.22f));
        DrawCircleV({x+14,y-12},4,eye);
        DrawLineEx({x+28,y-20},{x+23,y-9},2,gold);
        DrawLineEx({x+23,y-9},{x+30,y-3},2,gold);
        DrawEllipse((int)x+30,(int)y-26,13,4,DARKGREEN);
        if(activa){
            DrawLineEx({centro.x-9,centro.y},{centro.x+14,centro.y},3,gold);
            DrawTriangle({centro.x-14,centro.y},{centro.x-5,centro.y+4},{centro.x-5,centro.y-4},RAYWHITE);
            DrawLineEx({centro.x+8,centro.y-3},{centro.x+14,centro.y},2,RED);
            DrawLineEx({centro.x+8,centro.y+3},{centro.x+14,centro.y},2,RED);
        }
    } else if(tipo==TipoTrampaJungla::Bloque){
        float t=std::fmod(tiempo+fase,5.5f);
        bool aviso=t>=1.0f&&t<2.0f;
        Color edge={45,57,43,255}, stone={125,132,100,255}, gold={211,171,79,255};
        DrawRectangleRec({origen.x-31,origen.y-35,62,12},edge);
        DrawRectangleRec({origen.x-27,origen.y-35,54,4},gold);
        for(float y=origen.y;y<centro.y-24;y+=12) DrawRectangleLinesEx({origen.x-4,y,8,10},2,edge);
        Rectangle r={centro.x-24,centro.y-24,48,48};
        DrawRectangleRec(r,edge);
        DrawRectangleGradientV((int)r.x+3,(int)r.y+3,42,42,stone,Color{78,88,65,255});
        DrawRectangleLinesEx({r.x+5,r.y+5,38,38},2,gold);
        DrawRectangleRec({r.x+5,r.y+3,38,4},DARKGREEN);
        DrawLineEx({r.x+8,r.y+13},{r.x+19,r.y+18},4,edge);
        DrawLineEx({r.x+40,r.y+13},{r.x+29,r.y+18},4,edge);
        Color eye=aviso?ORANGE:activa?RED:edge;
        DrawRectangleRec({r.x+10,r.y+19,9,4},eye);
        DrawRectangleRec({r.x+29,r.y+19,9,4},eye);
        DrawRectangleRec({r.x+21,r.y+19,6,13},stone);
        DrawRectangleRec({r.x+12,r.y+34,24,6},edge);
        DrawLineEx({r.x+35,r.y+4},{r.x+31,r.y+11},1,edge);
        if(aviso) DrawRectangleLinesEx(r,2,Fade(ORANGE,.5f+.4f*std::sin(t*18)));
        if(t>=2.65f&&t<3.2f) for(int j=0;j<6;j++){
            float f=(t-2.65f)/.55f;
            DrawCircleV({centro.x+(j-2.5f)*(8+f*9),centro.y+24-f*12},4+f*5,Fade(BEIGE,(1-f)*.35f));
        }
    } else {
        float x=origen.x,y=origen.y;
        Color wood={112,69,36,255}, dark={55,43,29,255}, metal={164,141,81,255};
        DrawRectangleRec({x-22,y-100,78,100},dark);
        DrawRectangleRec({x-17,y-94,68,94},wood);
        for(int j=0;j<4;j++) DrawLineEx({x-15+j*17.f,y-93},{x-15+j*17.f,y},2,dark);
        DrawRectangleRounded({x-25,y-53,65,53},.35f,8,dark);
        DrawRectangleRec({x-25,y-8,81,8},metal);
        DrawRectangleRec({x-27,y-104,88,12},Color{105,119,78,255});
        DrawRectangleRec({x-27,y-104,88,4},DARKGREEN);
        float a=tiempo*2;
        Vector2 gear={x+22,y-72};
        for(int j=0;j<8;j++) {float b=a+j*P/4; DrawLineEx({gear.x+12*std::cos(b),gear.y+12*std::sin(b)},{gear.x+18*std::cos(b),gear.y+18*std::sin(b)},6,metal);}
        DrawCircleV(gear,14,metal); DrawCircleV(gear,8,dark); DrawCircleV(gear,3,metal);
        DrawLineEx({x+48,y-96},{x+42,y-57},4,DARKGREEN);
        for(int j=0;j<4;j++) DrawEllipse((int)x+44+(j%2?5:-5),(int)y-92+j*9,7,3,GREEN);
        if(!activa) return;
        DrawCircleV(centro,24,dark); DrawCircleV(centro,21,wood);
        DrawCircleV(centro,17,Color{192,137,77,255});
        DrawCircleLines((int)centro.x,(int)centro.y,13,BROWN);
        DrawCircleLines((int)centro.x,(int)centro.y,7,BROWN);
        float b=-(origen.x-centro.x)/24;
        for(int j=0;j<3;j++){float c=b+j*2*P/3; DrawLineEx({centro.x+17*std::cos(c),centro.y+17*std::sin(c)},{centro.x+23*std::cos(c),centro.y+23*std::sin(c)},3,metal);}
        DrawLineEx(centro,{centro.x+14*std::cos(b),centro.y+14*std::sin(b)},2,BROWN);
    }
}
