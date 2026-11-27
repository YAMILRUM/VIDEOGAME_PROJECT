#include "TrampaPlaya.h"
#include <cmath>
#include <algorithm>
TrampaPlaya::TrampaPlaya(TipoTrampaPlaya t, Vector2 o, float a, float v, float f):tipo(t),origen(o),alcance(a),velocidad(v),fase(f){Reiniciar();}
void TrampaPlaya::Reiniciar(){tiempo=0; Actualizar(0);}
void TrampaPlaya::Actualizar(float dt){
    tiempo+=std::max(0.f,std::min(dt,.05f));
    if(tipo==TipoTrampaPlaya::Cangrejo){
        float p=std::fmod(tiempo*velocidad+fase,4*alcance);
        x=origen.x-alcance+(p<=2*alcance?p:4*alcance-p); y=origen.y;
    }else if(tipo==TipoTrampaPlaya::Tentaculo){
        float p=std::fmod(tiempo+fase,alcance), d=p-1.2f;
        aviso=p<1.2f;
        cayendo=d>=0 && d<2.2f;
        x=origen.x;
        y=origen.y-velocidad*std::sin(3.14159265f*std::max(0.f,std::min(1.f,d/2.2f)));
    }else if(tipo==TipoTrampaPlaya::Canon){
        float p=std::fmod(tiempo+fase,alcance);
        float d=p-1.2f;
        aviso=p<1.2f;
        cayendo=d>=0 && d<1.05f;
        float dir=velocidad<0?-1.f:1.f;
        x=origen.x+dir*43+velocidad*std::max(0.f,d);
        y=origen.y;
    }else if(tipo==TipoTrampaPlaya::Tiburon){
        float p=std::fmod(tiempo+fase,alcance);
        float d=p-(alcance-1.35f);
        aviso=d>=-1.1f && d<0;
        cayendo=d>=0 && d<1.35f;
        float u=std::max(0.f,std::min(1.f,d/1.35f));
        x=origen.x; y=origen.y-velocidad*std::sin(u*3.14159265f);
    }else{
        float p=std::fmod(tiempo+fase,alcance);
        aviso=p>=alcance-2.3f && p<alcance-1.3f;
        float d=p-(alcance-1.3f);
        x=origen.x; y=origen.y+std::max(0.f, .5f*velocidad*d*d)*(d>=0?1.f:0.f);
        cayendo=d>=0 && y<590;
    }
}
Vector2 TrampaPlaya::PuntoTentaculo(float u) const {
    return {x+32*u*std::sin(tiempo*3+u*2),origen.y+(y-origen.y)*u};
}
bool TrampaPlaya::Colisiona(Rectangle r) const{
    if(tipo==TipoTrampaPlaya::Tentaculo){
        if(!cayendo || origen.y-y<20) return false;
        for(int i=0;i<=20;i++) {
            float u=i/20.f;
            if(CheckCollisionCircleRec(PuntoTentaculo(u),16-10*u,r)) return true;
        }
        return false;
    }
    if(tipo==TipoTrampaPlaya::Cangrejo) return CheckCollisionRecs(r,{x-24,y-29,48,29});
    if(tipo==TipoTrampaPlaya::Tiburon) return cayendo && CheckCollisionRecs(r,{x-22,y-30,44,55});
    if(tipo==TipoTrampaPlaya::Canon) return cayendo && CheckCollisionCircleRec({x,y},11,r);
    return cayendo && CheckCollisionCircleRec({x,y},10,r);
}
void TrampaPlaya::Dibujar() const{
    if(tipo==TipoTrampaPlaya::Cangrejo){
        Color oscuro={148,48,48,255}, coral={238,91,70,255}, claro={255,154,102,255};
        DrawEllipse((int)x,(int)y-1,34,4,{100,65,45,90});
        for(int s=-1;s<=1;s+=2){
            for(int i=0;i<3;i++){
                float a=std::sin(tiempo*12+i*1.7f)*5;
                Vector2 raiz={x+s*(12+i*5.f),y-13}, rodilla={x+s*(25+i*5.f),y-10+a};
                DrawLineEx(raiz,rodilla,5,oscuro); DrawLineEx(rodilla,{rodilla.x+s*5,y-2},4,coral);
            }
            Vector2 pinza={x+s*30,y-27+std::sin(tiempo*5)*3};
            DrawLineEx({x+s*18,y-18},pinza,7,oscuro);
            DrawCircleV(pinza,10,coral);
            DrawTriangle({pinza.x-9,pinza.y},{pinza.x+9,pinza.y},{pinza.x,pinza.y-14},coral);
            DrawLineEx({pinza.x,pinza.y-11},{pinza.x,pinza.y-3},2,oscuro);
            DrawLineEx({x+s*9,y-25},{x+s*12,y-36},4,oscuro);
            DrawCircleV({x+s*12,y-36},5,RAYWHITE); DrawCircleV({x+s*12+1,y-36},2,BLACK);
        }
        DrawEllipse((int)x,(int)y-17,25,15,oscuro); DrawEllipse((int)x,(int)y-20,23,12,coral);
        DrawEllipse((int)x-6,(int)y-25,11,3,claro);
    }else if(tipo==TipoTrampaPlaya::Tentaculo){
        if(aviso){
            DrawEllipse((int)x,(int)origen.y-12,32,5,ORANGE);
            DrawText("!",(int)x-5,(int)origen.y-65,24,ORANGE);
            for(int i=0;i<7;i++) DrawCircleLines((int)x+(i-3)*8,(int)origen.y-15-(int)std::fmod(tiempo*45+i*7,30.f),3+i%3,RAYWHITE);
        }
        if(cayendo && origen.y-y>=20){
            // Capas continuas: contorno, piel, vientre y brillo.
            for(int capa=0;capa<4;capa++){
                for(int i=0;i<=80;i++){
                    float u=i/80.f, r=16-10*u;
                    Vector2 p=PuntoTentaculo(u);
                    if(capa==0) DrawCircleV(p,r,{54,23,78,255});
                    if(capa==1) DrawCircleV(p,r*.88f,{116,48,155,255});
                    if(capa==2) DrawCircleV({p.x+r*.26f,p.y},r*.61f,{194,91,159,255});
                    if(capa==3) DrawCircleV({p.x-r*.42f,p.y},r*.23f,{190,117,218,255});
                }
            }
            // Ventosas dobles, con borde carnoso y cavidad sombreada.
            for(int i=1;i<=9;i++){
                float u=i/10.f, r=16-10*u;
                Vector2 p=PuntoTentaculo(u);
                float a=r*.31f;
                for(int j=0;j<2;j++){
                    Vector2 c={p.x+r*(j==0?.12f:.58f),p.y+(j==0?0.f:3.f)};
                    DrawCircleV(c,a,{104,39,105,255});
                    DrawCircleV(c,a*.83f,{248,169,198,255});
                    DrawCircleV({c.x,c.y+1},a*.48f,{137,53,119,255});
                    DrawCircleV({c.x-a*.25f,c.y-a*.35f},a*.19f,{255,221,225,255});
                }
            }
            // Espuma en la superficie, sin modificar el area de dano.
            for(int i=0;i<9;i++){
                float k=std::fmod(tiempo*1.7f+i*.13f,1.f);
                float lado=i%2==0?-1.f:1.f;
                DrawCircleV({x+lado*(12+30*k),origen.y-4-12*std::sin(k*3.14159f)},2.5f*(1-k)+.5f,{224,250,250,(unsigned char)(190*(1-k))});
            }
        }
    }else if(tipo==TipoTrampaPlaya::Canon){
        float p=std::fmod(tiempo+fase,alcance), d=p-1.2f;
        float dir=velocidad<0?-1.f:1.f;
        float retroceso=(d>=0 && d<.25f)?12*(1-d/.25f):0;
        float cx=origen.x-dir*retroceso, cy=origen.y;
        Color hierro={48,55,67,255}, oro={204,153,72,255};
        Color borde={24,29,37,255}, luz={111,125,138,255};
        auto punto=[&](float u,float v)->Vector2{return {cx+dir*u,cy+v};};
        DrawEllipse((int)origen.x,(int)cy+51,36,5,{35,24,22,100});
        DrawRectangleRec({origen.x-27,cy+13,54,28},{75,43,29,255});
        DrawRectangleRec({origen.x-24,cy+15,48,23},{155,94,48,255});
        for(int i=0;i<3;i++){
            float yy=cy+19+i*7.f;
            DrawLineEx({origen.x-23,yy},{origen.x+23,yy},2,{104,59,35,255});
        }
        DrawRectangleRec({origen.x-31,cy+36,62,10},{66,42,31,255});
        for(int k=-1;k<=1;k+=2){
            float wx=origen.x+k*20.f, wy=cy+40;
            DrawCircleV({wx,wy},12,borde);
            DrawCircleV({wx,wy},9,{151,91,47,255});
            for(int j=0;j<6;j++){
                float ang=j*1.0472f;
                DrawLineEx({wx,wy},{wx+8*std::cos(ang),wy+8*std::sin(ang)},2,{79,48,31,255});
            }
            DrawCircleV({wx,wy},4,oro);
            DrawCircleV({wx,wy},2,borde);
        }
        DrawCircleV(punto(-23,3),10,borde);
        DrawCircleV(punto(-25,2),6,hierro);
        DrawLineEx(punto(-18,3),punto(35,0),29,borde);
        DrawLineEx(punto(-17,2),punto(34,0),24,hierro);
        DrawLineEx(punto(-15,-6),punto(31,-8),5,luz);
        DrawLineEx(punto(-15,9),punto(31,7),4,{32,38,48,255});
        for(float u: {-13.f,9.f,29.f}){
            DrawLineEx(punto(u,-13),punto(u,13),6,borde);
            DrawLineEx(punto(u,-12),punto(u,12),3,oro);
            DrawCircleV(punto(u,-9),1.6f,{255,217,139,255});
        }
        DrawCircleV(punto(0,7),7,borde);
        DrawCircleV(punto(0,7),4,oro);
        DrawEllipse((int)(cx+dir*35),(int)cy,9,16,borde);
        DrawEllipse((int)(cx+dir*36),(int)cy,7,14,oro);
        DrawEllipse((int)(cx+dir*37),(int)cy,5,10,{13,18,25,255});
        DrawEllipse((int)(cx+dir*38),(int)cy+3,3,5,{38,44,52,255});
        if(aviso){
            DrawCircleLines((int)(cx+dir*35),(int)cy,18+3*std::sin(tiempo*18),ORANGE);
            DrawText("!",(int)origen.x-5,(int)cy-49,24,ORANGE);
        }
        if(d>=0 && d<.45f){
            for(int k=0;k<5;k++) DrawCircleV({origen.x+dir*(43+d*75+k*5),cy-k*5.f-d*24},6+d*14,{190,185,175,(unsigned char)(170*(1-d/.45f))});
            if(d<.12f){
                float f=1-d/.12f;
                DrawCircleV({origen.x+dir*43,cy},19*f,ORANGE);
                DrawCircleV({origen.x+dir*45,cy},11*f,YELLOW);
                DrawCircleV({origen.x+dir*46,cy},5*f,RAYWHITE);
                for(int j=0;j<5;j++){
                    float a=(j-2)*.35f;
                    DrawLineEx({origen.x+dir*43,cy},{origen.x+dir*(43+28*f*std::cos(a)),cy+28*f*std::sin(a)},2,ORANGE);
                }
            }
        }
        if(cayendo){
            DrawCircleV({x,y},11,hierro);
            DrawCircleV({x-3,y-4},4,{132,145,155,255});
        }
    }else if(tipo==TipoTrampaPlaya::Tiburon){
        Color azul={53,105,135,255}, sombra={31,67,93,255}, vientre={213,235,231,255};
        if(aviso){
            float f=std::sin(tiempo*12);
            DrawTriangle({x-22,origen.y},{x+22,origen.y},{x+4,origen.y-32},azul);
            DrawEllipse((int)x,(int)origen.y,35+5*f,5,ORANGE);
            DrawText("!",(int)x-5,(int)origen.y-60,22,ORANGE);
        }
        if(cayendo){
            DrawTriangle({x-9,y+25},{x+9,y+25},{x-27,y+55},sombra);
            DrawTriangle({x+9,y+25},{x-9,y+25},{x+27,y+55},azul);
            DrawEllipse((int)x,(int)y+5,25,39,sombra);
            DrawEllipse((int)x,(int)y,22,35,azul);
            DrawEllipse((int)x,(int)y+12,15,22,vientre);
            DrawTriangle({x-18,y+3},{x-42,y+26},{x-15,y+22},azul);
            DrawTriangle({x+18,y+3},{x+15,y+22},{x+42,y+26},azul);
            DrawEllipse((int)x,(int)y-13,17,12,{66,32,43,255});
            for(int i=0;i<5;i++){
                float a=x-12+i*6.f;
                DrawTriangle({a-3,y-19},{a+3,y-19},{a,y-12},RAYWHITE);
            }
            DrawCircleV({x-17,y-25},4,RAYWHITE); DrawCircleV({x+17,y-25},4,RAYWHITE);
            DrawCircleV({x-17,y-25},2,BLACK); DrawCircleV({x+17,y-25},2,BLACK);
            float p=std::fmod(tiempo+fase,alcance)-(alcance-1.35f);
            float splash=1.f-std::min(1.f,std::min(p,1.35f-p)*5.f);
            for(int i=0;i<8;i++) DrawCircleV({x+(i-3.5f)*9,origen.y-std::abs(std::sin(i*2.f))*30*splash},2+3*splash,{220,255,250,190});
        }
    }else{
        float p=std::fmod(tiempo+fase,alcance);
        if(aviso){
            DrawCircleLines((int)x,(int)origen.y,17,ORANGE);
            DrawEllipse((int)x,598,19+4*std::sin(tiempo*18),3,ORANGE);
            DrawText("!",(int)x-4,(int)origen.y-35,22,ORANGE);
            DrawLineEx({x-18,origen.y-12},{x-26,origen.y-18},3,ORANGE);
            DrawLineEx({x+18,origen.y-12},{x+26,origen.y-18},3,ORANGE);
        }
        if(p<alcance-1.3f || cayendo){
            Vector2 c={x,cayendo?y:origen.y};
            DrawCircleV(c,11,{89,51,30,255}); DrawCircleV({c.x-2,c.y-2},9,{151,94,49,255});
            float a=cayendo?tiempo*12:0;
            for(int i=0;i<3;i++){float b=a+i*2.0944f; DrawCircleV({c.x+4*std::cos(b),c.y+4*std::sin(b)},1.8f,{65,39,25,255});}
        }else if(y<680){
            float k=(y-590)/90; for(int i=0;i<5;i++) DrawCircleV({x+(i-2)*8.f*(1+k),596-10*std::sin(k*3.14159f)},3,{226,192,133,(unsigned char)(150*(1-k))});
        }
    }
}
