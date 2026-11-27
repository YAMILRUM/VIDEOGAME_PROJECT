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
    tiempoBalsas = 0;
    plataformas.clear();
    if (tipo == TipoMapa::Playa) {
        plataformas = {{0,600,2200,120},{2390,600,780,120},{3350,600,900,120},{4450,600,850,120},{5500,600,900,120},
            {450,500,180,20},{700,405,170,20},{1100,500,180,20},{1450,500,180,60},{1740,490,180,20},
            {2220,515,150,20},{2600,490,180,20},{2850,395,170,20},{3190,515,140,20},
            {3500,500,180,20},{3750,400,180,20},{3990,500,180,60},{4270,515,160,20},
            {4650,500,180,20},{4900,405,180,20},{5320,515,160,20},{5700,500,180,20},{5940,405,180,20},{4260,575,180,20},{5310,575,180,20}};
        return;
    }
    if (tipo == TipoMapa::Jungla) {
        plataformas = {{0,600,1800,120},{2040,600,1960,120},{4260,600,2140,120},
            {430,500,180,20},{620,405,160,20},{1050,500,180,20},
            {1850,480,140,20},{2400,500,180,20},{2550,405,150,20},
            {3650,500,150,20},{4060,480,140,20},{4750,500,180,20},
            {5100,405,180,20},{5550,500,150,20}};
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

void Nivel::ReiniciarBalsas() {
    tiempoBalsas = 0;
    ActualizarBalsas(0);
}
void Nivel::ActualizarBalsas(float dt) {
    if (tipo != TipoMapa::Playa) return;
    tiempoBalsas += fminf(fmaxf(dt,0),.033f);
    for (auto& p : plataformas) {
        if (p.x == 4260 || p.x == 5310)
            p.y = 575 + 12*sinf(tiempoBalsas*1.5f+(p.x==5310?2.f:0.f));
    }
}

void Nivel::Dibujar(float camaraX) {
    if (tipo == TipoMapa::Playa) {
        const float t=(float)GetTime(), d=camaraX-640.f;
        DrawRectangleGradientV(-2000,-200,11000,800,ColorRGB(100,125,207),ColorRGB(255,173,120));
        float sun=920+d*.94f;
        for(int r=150;r>=90;r-=15) DrawCircleV({sun,290},r,ColorRGB(255,220,150,18));
        DrawCircleV({sun,290},78,ColorRGB(255,231,161));
        for(int i=-2;i<18;i++) {
            float x=i*530+d*.85f;
            DrawEllipse((int)x,145+i%3*35,110,15,ColorRGB(255,200,182,130));
            DrawEllipse((int)x+65,140+i%3*35,70,20,ColorRGB(255,211,190,120));
        }
        DrawRectangleGradientV(-2000,390,11000,510,ColorRGB(39,124,167),ColorRGB(36,208,192));
        for(int i=-2;i<18;i++) {
            float x=i*510+d*.72f;
            DrawEllipse((int)x,390,160,22,ColorRGB(80,139,142));
            DrawEllipse((int)x+50,382,70,26,ColorRGB(91,151,145));
        }
        // Hoteles con balcones, en la franja turistica del fondo.
        for(int i=0;i<12;i++) {
            float x=40+i*210+d*.48f, h=110+(i*73)%125, y=420-h;
            DrawRectangleGradientH((int)x,(int)y,155,(int)h,ColorRGB(246,208,181),ColorRGB(180,143,156));
            DrawRectangleRec({x+12,y-12,131,12},ColorRGB(246,227,197));
            for(float yy=y+18;yy<405;yy+=24) {
                for(int j=0;j<5;j++) DrawRectangleRec({x+9+j*29.f,yy,19,13},ColorRGB(83,146,166));
                DrawRectangleRec({x-3,yy+14,161,3},ColorRGB(255,232,202));
            }
        }
        // Olas y reflejos, sin colision.
        for(int k=0;k<7;k++) for(int i=-2;i<80;i++) {
            float x=i*125+d*.25f+22*sinf(t*.8f+k), y=430+k*32+5*sinf(i*.8f+t+k);
            if(x<camaraX-760||x>camaraX+760) continue;
            DrawLineEx({x,y},{x+42+k*4.f,y},2,ColorRGB(209,255,239,85));
        }
        for(int i=0;i<8;i++) {
            float x=fmodf(t*17+i*980,8200)-500+d*.3f,y=110+i%3*34;
            DrawLineEx({x-12,y-5},{x,y},2,ColorRGB(75,78,108));
            DrawLineEx({x,y},{x+12,y-5},2,ColorRGB(75,78,108));
        }
        // Barco turistico, silueta decorativa detras de la cubierta jugable.
        DrawRectangleRec({3350,545,900,55},ColorRGB(108,62,47));
        DrawTriangle({3350,545},{3450,545},{3450,600},ColorRGB(108,62,47));
        for(int i=0;i<3;i++) {
            float x=3500+i*270.f;
            DrawLineEx({x,540},{x,235},9,ColorRGB(103,66,53));
            DrawTriangle({x+8,250},{x+8,420},{x+125,420},ColorRGB(255,228,185));
        }
        // Palmeras curvas y hojas animadas, siempre decorativas.
        for(int i=0;i<19;i++) {
            float x=180+i*310.f;
            if(x>2100&&x<4450) continue;
            Vector2 base={x,600}, top={x+24,600-(180-(i%3)*18.f)*1.61f};
            Vector2 curva={x+8,600-(600-top.y)*.45f};
            DrawLineEx(base,curva,15,ColorRGB(139,91,61));
            DrawLineEx(curva,top,12,ColorRGB(164,112,69));
            for(int j=0;j<7;j++) {
                float a=3.14159f+j*3.14159f/6, sway=5*sinf(t*1.5f+i+j);
                Vector2 tip={top.x+85*cosf(a),top.y+48*sinf(a)+sway};
                Vector2 mid={(top.x+tip.x)/2,(top.y+tip.y)/2-15};
                DrawTriangle(top,mid,tip,ColorRGB(35,123,85));
                DrawTriangle(top,tip,{mid.x,mid.y+22},ColorRGB(66,163,94));
            }
            DrawCircleV({top.x-5,top.y+8},7,ColorRGB(109,73,43));
        }
        for(const Rectangle& p:plataformas) {
            if(p.x==4260 || p.x==5310) {
                for(int j=0;j<6;j++) {
                    float x=p.x+j*30.f;
                    DrawRectangleRec({x,p.y,30,20},ColorRGB(126,76,42));
                    DrawRectangleRec({x+2,p.y,26,5},ColorRGB(221,162,94));
                    DrawEllipse((int)x+15,(int)p.y+14,12,5,ColorRGB(173,111,61));
                }
                for(float x : {p.x+18,p.x+155}) DrawRectangleRec({x,p.y,7,20},ColorRGB(235,210,151));
                continue;
            }
            bool madera=(p.x>=2200&&p.x<4450)||p.height==60;
            Color base=madera?ColorRGB(151,96,59):ColorRGB(224,180,112);
            DrawRectangleRec(p,base);
            DrawRectangleRec({p.x,p.y,p.width,5},madera?ColorRGB(245,194,129):ColorRGB(255,230,174));
            if(madera) {
                for(float x=p.x+24;x<p.x+p.width-3;x+=38) {
                    DrawLineEx({x,p.y+5},{x,p.y+p.height},2,ColorRGB(106,67,46));
                    DrawCircleV({x-5,p.y+9},2,ColorRGB(90,64,52));
                }
            } else {
                for(float x=p.x+12;x<p.x+p.width-8;x+=31) DrawEllipse((int)x,(int)(p.y+12),4,2,ColorRGB(190,144,91));
                if(p.height>60) DrawRectangleRec({p.x,p.y+35,p.width,p.height-35},ColorRGB(195,147,91));
            }
        }
        return;
    }
    if (tipo == TipoMapa::Jungla) {
        const float t = (float)GetTime();
        const float d = camaraX - 640.0f;
        DrawRectangleGradientV(-2000,-200,11000,1100,ColorRGB(19,66,77),ColorRGB(224,222,155));
        float sol=430+d*.91f;
        for(int r=180;r>=80;r-=20) DrawCircleV({sol,105},r,ColorRGB(255,222,135,12));
        DrawCircleV({sol,105},65,ColorRGB(255,236,177));
        // Cordilleras y bosque distante con paralaje.
        for(int k=0;k<3;k++) {
            Color c=k==0?ColorRGB(112,158,145):k==1?ColorRGB(69,127,111):ColorRGB(39,94,80);
            float par=d*(.82f-k*.19f);
            for(int i=-5;i<35;i++) {
                float x=i*310+par, h=190+(i*i*37+k*83)%210;
                DrawTriangle({x-290,610},{x+290,610},{x,610-h},c);
                DrawEllipse((int)x,(int)(610-h*.65f),140,(int)(h*.42f),c);
            }
        }
        // Basamentos mesoamericanos: talud, tablero y escalinata ceremonial.
        for(int k=0;k<3;k++) {
            float x=850+k*2300+d*.28f;
            Color piedra=ColorRGB(137,151,117), sombra=ColorRGB(68,96,78);
            Color rojo=ColorRGB(151,96,67), turquesa=ColorRGB(76,140,125);
            for(int j=0;j<6;j++) {
                float w=570-j*78, y=565-j*48, l=x-w/2, r=x+w/2;
                DrawTriangle({l,y+48},{r,y+48},{r-19,y+10},sombra);
                DrawTriangle({l,y+48},{r-19,y+10},{l+19,y+10},sombra);
                DrawRectangleRec({l+19,y,w-38,18},piedra);
                DrawRectangleRec({l+15,y-3,w-30,5},ColorRGB(177,183,139));
                DrawRectangleRec({l+24,y+5,w-48,8},rojo);
                for(float q=l+30;q<r-35;q+=34) {
                    DrawRectangleLinesEx({q,y+5,23,8},1,turquesa);
                    DrawLineEx({q,y+23},{q-5,y+44},1,ColorRGB(91,116,91));
                }
                DrawRectangleRec({x-48,y-3,96,51},ColorRGB(111,132,102));
                for(int z=0;z<4;z++) {
                    DrawRectangleRec({x-36,y+z*12.f,72,10},piedra);
                    DrawRectangleRec({x-36,y+z*12.f+8,72,3},sombra);
                }
                DrawRectangleRec({x-48,y-3,10,51},ColorRGB(165,175,130));
                DrawRectangleRec({x+38,y-3,10,51},ColorRGB(165,175,130));
                for(int z=0;z<4;z++) DrawEllipse((int)(l+30+z*17),(int)(y+16),14,4,ColorRGB(62,117,71));
            }
            DrawRectangleRec({x-80,245,160,80},sombra);
            DrawRectangleRec({x-73,251,146,74},piedra);
            DrawRectangleRec({x-25,270,50,55},ColorRGB(24,64,55));
            for(float q : {x-62,x+42}) {
                DrawRectangleRec({q,265,20,60},rojo);
                DrawRectangleLinesEx({q+4,271,12,16},2,turquesa);
                DrawRectangleLinesEx({q+4,292,12,16},2,turquesa);
            }
            DrawRectangleRec({x-92,235,184,15},piedra);
            DrawRectangleRec({x-82,225,164,10},rojo);
            DrawRectangleRec({x-65,217,130,8},piedra);
            for(int j=0;j<7;j++) DrawLineEx({x-80+j*25.f,249},{x-85+j*25.f,280+(j*j%4)*12.f},3,ColorRGB(52,111,65));
        }
        // Arboles gigantes: troncos, ramas, doseles y lianas.
        for(int i=-3;i<26;i++) {
            float x=i*330+100+d*.12f, h=310+(i*i*29)%150, top=620-h;
            Color bark=ColorRGB(48,76,51), leaf=ColorRGB(28,91,62);
            DrawLineEx({x,640},{x+24,top},44,bark);
            DrawLineEx({x+8,620},{x+31,top},9,ColorRGB(82,112,67));
            DrawLineEx({x+9,top+100},{x-95,top+12},19,bark);
            DrawLineEx({x+18,top+70},{x+110,top-10},16,bark);
            DrawTriangle({x-65,640},{x+8,560},{x+15,640},bark);
            DrawTriangle({x+12,640},{x+15,560},{x+75,640},bark);
            for(int j=0;j<7;j++) {
                float a=x+(j-3)*39, b=top-22+(j*j*17)%55;
                DrawEllipse((int)a,(int)b,82,52,leaf);
                DrawEllipse((int)a,(int)b-19,65,32,ColorRGB(49,120,70));
                DrawEllipse((int)a-12,(int)b-30,36,13,ColorRGB(80,145,81));
            }
            for(int j=0;j<3;j++) {
                float a=x-100+j*94.f, len=65+(i*i+j*37)%100;
                for(int s=0;s<8;s++) {
                    float y=top+s*len/8, yy=top+(s+1)*len/8;
                    DrawLineEx({a+7*sinf(s*.5f+t*.6f),y},{a+7*sinf((s+1)*.5f+t*.6f),yy},3,ColorRGB(43,99,59));
                    if(s%2==0) DrawEllipse((int)a+((s%4)?8:-8),(int)y,10,4,ColorRGB(91,147,75));
                }
            }
        }
        // Cascadas ancladas a acantilados, con poza, espuma y bruma.
        for(float x : {1800.0f,4000.0f}) {
            float w=x<2000?240:260, cx=x+w*.5f;
            DrawRectangleGradientV((int)x-95,150,(int)w+190,570,ColorRGB(99,126,104),ColorRGB(39,75,70));
            for(int j=0;j<7;j++) {
                float a=x-100+j*(w+190)/7;
                float y=125+(j*j*19)%70;
                DrawRectangleGradientH((int)a,(int)y,65,(int)(720-y),ColorRGB(63,92,80),ColorRGB(108,132,108));
                DrawEllipse((int)a+25,(int)y,48,24,ColorRGB(53,111,64));
                for(int s=0;s<5;s++) DrawLineEx({a+5,y+65+s*95.f},{a+51,y+75+s*95.f},2,ColorRGB(53,82,73));
            }
            DrawRectangleRounded({cx-72,145,144,38},.6f,8,ColorRGB(31,104,108));
            DrawRectangleGradientH((int)cx-56,165,112,555,ColorRGB(56,157,165),ColorRGB(184,232,216));
            for(int j=0;j<15;j++) {
                float a=cx-50+j*7.f, y=170+fmodf(t*(125+j*7)+j*43,530);
                DrawLineEx({a,y},{a,y+32+(j%3)*13.f},2,ColorRGB(220,250,239,155));
            }
            DrawRectangleGradientV((int)x,675,(int)w,225,ColorRGB(43,150,155),ColorRGB(12,65,76));
            for(int j=0;j<13;j++) {
                float a=x+12+j*(w-24)/13, y=681+8*sinf(t*2+j);
                DrawEllipse((int)a,(int)y,19,5,ColorRGB(203,244,229,150));
                float mist=fmodf(t*30+j*13,85);
                DrawCircleV({cx+(j-6)*13.f,680-mist},13+mist*.15f,ColorRGB(205,240,224,(unsigned char)(65*(1-mist/85))));
            }
        }
        // Luz filtrada y restos de columnas, exclusivamente decorativos.
        for(int i=0;i<9;i++) {
            float x=180+i*760+d*.18f;
            DrawTriangle({x,0},{x+190,580},{x+65,580},ColorRGB(255,232,154,12));
            DrawTriangle({x+20,0},{x+125,580},{x+90,580},ColorRGB(255,240,178,10));
        }
        for(float x : {340.0f,1120.0f,2490.0f,3300.0f,4660.0f,5700.0f}) {
            DrawRectangleRec({x-24,578,96,22},ColorRGB(70,89,65));
            for(int j=0;j<4;j++) {
                float y=530-j*48.f;
                DrawRectangleGradientH((int)x,(int)y,48,47,ColorRGB(72,91,66),ColorRGB(120,137,92));
                DrawRectangleRec({x,y,48,3},ColorRGB(147,158,105));
                DrawLineEx({x+12,y+8},{x+24,y+23},2,ColorRGB(64,86,61));
                DrawLineEx({x+24,y+23},{x+36,y+8},2,ColorRGB(64,86,61));
            }
            DrawRectangleRec({x-8,330,64,10},ColorRGB(110,133,84));
            for(int j=0;j<6;j++) DrawEllipse((int)x+5+j*8,337+j%3*5,12,5,ColorRGB(50,112,62));
        }
        // Helechos de fondo, sin colision.
        for(int i=0;i<85;i++) {
            float x=i*79+25;
            if((x>1770&&x<2070)||(x>3970&&x<4290)) continue;
            for(int j=-2;j<=2;j++) {
                float a=x+j*15.f, y=565+std::abs(j)*8.f;
                DrawLineEx({x,600},{a,y},3,ColorRGB(34,100,59));
                DrawEllipse((int)a,(int)y,12,6,ColorRGB(64,137,69));
            }
        }
        // Superficies jugables: exactamente las mismas cajas de colision.
        for(const Rectangle& p:plataformas) {
            DrawRectangleRec(p,ColorRGB(61,62,45));
            DrawRectangleGradientV((int)p.x+2,(int)p.y+5,(int)p.width-4,(int)p.height-7,ColorRGB(150,137,98),ColorRGB(83,78,57));
            for(float y=p.y+9;y<p.y+p.height-3;y+=24) for(float x=p.x+3;x<p.x+p.width-3;x+=38) {
                float w=fminf(35,p.x+p.width-x-2), h=fminf(20,p.y+p.height-y-2);
                if(w>0&&h>0) { DrawRectangleRec({x,y,w,h},ColorRGB(139,129,92)); DrawLineEx({x,y},{x+w,y},1,ColorRGB(181,164,113)); }
            }
            DrawRectangleRec({p.x,p.y,p.width,5},ColorRGB(174,218,99));
            for(float x=p.x+4;x<p.x+p.width-5;x+=13) DrawRectangleRec({x,p.y+5,5,4},ColorRGB(67,135,55));
            DrawRectangleLinesEx(p,1,ColorRGB(48,66,43));
        }
        // Flores y pequenas raices dentro del borde del suelo, sin ocultar trampas.
        for(const Rectangle& p:plataformas) if(p.y==600) {
            for(float x=p.x+22;x<p.x+p.width-20;x+=83) {
                DrawLineEx({x,607},{x+7,620},2,ColorRGB(58,91,45));
                DrawEllipse((int)x+7,616,6,3,ColorRGB(91,148,58));
                if(((int)x/83)%3==0) DrawCircleV({x,608},2,ColorRGB(239,184,81));
            }
        }
        // Portal de llegada.
        for(int j=0;j<2;j++) {
            float x=6210+j*140;
            DrawRectangleGradientH((int)x,430,32,170,ColorRGB(100,95,64),ColorRGB(181,164,105));
            for(int s=0;s<6;s++) DrawLineEx({x,440+s*26.f},{x+32,440+s*26.f},2,ColorRGB(83,90,59));
        }
        DrawRectangle(6200,410,192,32,ColorRGB(196,165,81));
        DrawRectangle(6220,400,152,12,ColorRGB(238,206,110));
        DrawCircle(6296,425,10,ColorRGB(55,139,103));
        // Luciernagas ambientales, discretas y sin colision.
        for(int i=0;i<65;i++) {
            float x=i*103+30+12*sinf(t*.7f+i), y=230+(i*i*31)%300+10*sinf(t+i);
            DrawCircleV({x,y},5,ColorRGB(239,231,127,24));
            DrawCircleV({x,y},1.5f,ColorRGB(250,240,157,170));
        }
        return;
    }
    // PUEBLA 2077: decoracion sin colisiones.
    const float t = (float)GetTime(), d = camaraX-640.f;
    const Color cyan=ColorRGB(0,229,255), pink=ColorRGB(255,22,143), violet=ColorRGB(139,92,246);
    DrawRectangleGradientV(-2000,-200,11000,1100,ColorRGB(8,10,35),ColorRGB(78,29,95));
    float moon=950+d*.94f;
    for(int r=105;r>40;r-=10) DrawCircleV({moon,110},r,ColorRGB(151,94,255,9));
    DrawCircleV({moon,110},43,ColorRGB(219,205,255));
    for(int i=0;i<110;i++) DrawCircleV({float(i*79%8200)-500+d*.85f,float(20+i*i*17%240)},1+(i%3==0),ColorRGB(180,210,255,100));
    // Silueta volcanica estilizada.
    float v=370+d*.82f;
    DrawTriangle({v-440,500},{v+460,500},{v,205},ColorRGB(38,31,72));
    DrawTriangle({v-100,272},{v+105,272},{v,205},ColorRGB(130,120,171));
    // Tres estratos de arquitectura: siluetas, cristal y torres escalonadas.
    for(int layer=0;layer<3;layer++) {
        float par=d*(layer==0?.76f:layer==1?.57f:.28f);
        for(int i=-5;i<65;i++) {
            int seed=(i+10)*137+layer*83;
            float x=i*145+par, w=85+seed%48, h=100+seed%235, y=580-h;
            if(x+w<camaraX-750 || x>camaraX+750) continue;
            Color body=layer==0?ColorRGB(42,34,77):layer==1?ColorRGB(28,38,76):ColorRGB(17,25,49);
            Color edge=layer==0?violet:(seed%3==0?pink:cyan);
            DrawRectangleGradientH((int)x,(int)y,(int)w,(int)h,body,ColorRGB(46,55,91));
            DrawRectangleRec({x+w*.78f,y,w*.22f,h},Fade(ColorRGB(8,15,34),.55f));
            if(seed%3==0) {
                DrawRectangleRec({x+w*.16f,y-30,w*.68f,30},body);
                DrawRectangleRec({x+w*.32f,y-48,w*.36f,18},body);
            } else if(seed%3==1) {
                DrawTriangle({x,y},{x+w,y},{x+w*.72f,y-35},body);
            } else {
                DrawRectangleRec({x+8,y-16,w-16,16},body);
            }
            DrawRectangleRec({x+3,y,w-6,2},Fade(edge,.55f));
            for(float yy=y+15;yy<565;yy+=20) {
                DrawRectangleRec({x+5,yy,w-10,1},Fade(edge,.14f));
                for(float xx=x+10;xx+10<x+w*.77f;xx+=16) {
                    int k=(int)((xx-x)*3+(yy-y)*7)+seed;
                    if(k%7<4) DrawRectangleRec({xx,yy+3,9,7},Fade(k%5==0?pink:cyan,layer==2?.42f:.22f));
                }
            }
            if(layer==2) {
                DrawRectangleRec({x+3,y+5,3,h-5},Fade(edge,.15f));
                DrawRectangleRec({x+4,y+5,1,h-5},Fade(edge,.65f));
                for(float yy=y+65;yy<540;yy+=90) {
                    DrawRectangleRec({x-3,yy,w+6,6},ColorRGB(22,30,52));
                    DrawRectangleRec({x,yy,w,1},Fade(edge,.6f));
                }
                if(seed%4==0) {
                    DrawLineEx({x+w*.5f,y-15},{x+w*.5f,y-65},2,body);
                    DrawCircleV({x+w*.5f,y-65},3,Fade(pink,.55f+.35f*sinf(t*3+i)));
                }
            }
        }
    }
    // Torre BBVA: reinterpretacion futurista, no replica arquitectonica.
    float bx=1200+d*.48f;
    DrawRectangleGradientH((int)bx,115,174,465,ColorRGB(17,31,70),ColorRGB(48,79,126));
    DrawRectangleRec({bx+18,95,138,24},ColorRGB(29,52,94));
    for(int i=0;i<19;i++) {
        float y=145+i*22.f;
        DrawRectangleRec({bx+10,y,154,3},Fade(cyan,.48f));
        for(int j=0;j<5;j++) DrawRectangleRec({bx+15+j*30.f,y+5,20,11},ColorRGB(44,93,145,180));
    }
    for(int j=0;j<4;j++) DrawLineEx({bx+8+j*52.f,120},{bx+8+j*52.f,580},3,Fade(cyan,.65f));
    DrawRectangleRec({bx+10,122,154,39},ColorRGB(9,25,65));
    DrawText("BBVA",(int)bx+39,128,28,ColorRGB(160,239,255));
    // Estrella de Puebla.
    float wx=3000+d*.40f; Vector2 center={wx,360};
    DrawLineEx({wx-95,580},center,12,ColorRGB(43,49,82));
    DrawLineEx({wx+95,580},center,12,ColorRGB(43,49,82));
    DrawCircleLines((int)wx,360,130,cyan); DrawCircleLines((int)wx,360,125,violet);
    for(int i=0;i<16;i++) {float a=i*6.2831853f/16+t*.08f; Vector2 p={wx+130*cosf(a),360+130*sinf(a)}; DrawLineEx(center,p,1,Fade(violet,.55f)); DrawRectangleRec({p.x-8,p.y-5,16,10},ColorRGB(39,47,85)); DrawRectangleRec({p.x-6,p.y-3,12,3},pink);}
    DrawCircleV(center,12,cyan);
    // Trafico aereo y anuncios, en segundo plano.
    for(int i=0;i<9;i++) {float x=fmodf(t*(42+i*4)+i*890,8500)-600+d*.25f,y=185+i%4*55; DrawRectangleRec({x-42,y+4,42,2},Fade(cyan,.24f)); DrawRectangleRec({x,y,42,8},ColorRGB(66,83,130)); DrawRectangleRec({x+10,y-4,22,5},ColorRGB(95,130,175)); DrawRectangleRec({x+3,y+7,36,2},cyan);}
    for(int i=1;i<5;i+=2) {float x=260+i*1450; DrawRectangleRec({x,290,190,50},ColorRGB(13,24,53,225)); DrawRectangleLinesEx({x,290,190,50},2,Fade(pink,.75f)); DrawText("ANGELOPOLIS",(int)x+12,306,20,cyan);}
    // Superficies: geometria original intacta; paneles y luces dentro de p.
    for(const Rectangle& p:plataformas) {
        DibujarPlataforma(p,p.y==600,p.height==60);
        DrawRectangleRec({p.x,p.y,p.width,3},cyan);
        DrawRectangleRec({p.x,p.y+3,p.width,3},ColorRGB(44,104,138));
        if(p.y==600) {
            DrawRectangleRec({p.x,p.y+15,p.width,8},ColorRGB(49,65,89));
            DrawRectangleRec({p.x,p.y+23,p.width,2},Fade(violet,.8f));
            for(float x=p.x+8;x+76<=p.x+p.width;x+=84) {
                DrawRectangleRec({x,p.y+32,76,70},ColorRGB(14,23,42));
                DrawRectangleLinesEx({x,p.y+32,76,70},1,ColorRGB(68,83,110));
                DrawRectangleRec({x+5,p.y+38,66,17},ColorRGB(38,52,76));
                for(int k=0;k<5;k++) DrawRectangleRec({x+8+k*12.f,p.y+42,8,8},ColorRGB(17,30,49));
                DrawLineEx({x+8,p.y+76},{x+30,p.y+76},2,Fade(cyan,.7f));
                DrawLineEx({x+30,p.y+76},{x+40,p.y+65},2,Fade(cyan,.7f));
                DrawLineEx({x+40,p.y+65},{x+66,p.y+65},2,Fade(cyan,.7f));
                DrawCircleV({x+66,p.y+65},2,cyan);
                DrawRectangleRec({x+8,p.y+91,60,3},Fade(pink,.55f+.2f*sinf(t*2+x*.02f)));
            }
        } else if(p.height==60) {
            for(float x=p.x+12;x+24<=p.x+p.width;x+=32) {
                DrawRectangleRec({x,p.y+24,24,12},ColorRGB(14,24,44));
                DrawRectangleRec({x+3,p.y+27,18,3},Fade(violet,.8f));
            }
        } else {
            for(float x=p.x+8;x+22<=p.x+p.width;x+=30) {
                DrawRectangleRec({x,p.y+8,22,8},ColorRGB(12,23,43));
                DrawRectangleRec({x+3,p.y+11,16,2},Fade(violet,.8f));
            }
        }
    }

}

const std::vector<Rectangle>& Nivel::ObtenerPlataformas() const {
    return plataformas;
}
