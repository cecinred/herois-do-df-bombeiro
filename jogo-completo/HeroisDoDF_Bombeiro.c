/* =====================================================================
   HEROIS DO DF: MISSAO BOMBEIRO  -  Fases 1 a 10  (TELA CHEIA)
   ---------------------------------------------------------------------
   Jogo educativo (6-12 anos) sobre o Corpo de Bombeiros Militar do DF.
   Compilar (Linux):
     gcc HeroisDoDF_Bombeiro.c -o herois -lraylib -lm -lpthread -ldl -lrt -lX11
   Compilar (Windows / MinGW):
     gcc HeroisDoDF_Bombeiro.c -o herois.exe -lraylib -lopengl32 -lgdi32 -lwinmm
   ===================================================================== */

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define W 600
#define H 800
#define MP   160
#define MF   24
#define ME   500
#define MJT  24

typedef enum { MENU, JOGO, GAMEOVER, FIMJOGO, MUSEU } Estado;

enum {
    IC_MANGUEIRA, IC_EXTINTOR, IC_MACHADO, IC_CAPACETE,
    IC_RADIO, IC_KIT, IC_CONE, IC_ESCADA,
    IC_VIDEOGAME, IC_SKATE, IC_PIPOCA, IC_BOLA, IC_DOCE
};

typedef struct { float x,y,vx,vy,a,r; Color c; bool on; } Part;
typedef struct { float x,y,vx,vy,t; Color c; bool on; } Foguete;
typedef struct { float x,y,vx,vy,a; Color c; bool on; } Estil;
typedef struct { float x,y,vx,vy,a; bool on; } Jato;
typedef struct { float x,y; float fogo; } Foco;
typedef struct { const char*pergunta; const char*op[3]; int correta; } Quiz;

static Part    pts[MP];
static Foguete fogs[MF];
static Estil   est[ME];
static Jato    jts[MJT];

static void DBold(const char*t,int x,int y,int sz,Color c){
    DrawText(t,x+1,y+2,sz,(Color){0,0,0,150});
    DrawText(t,x,  y,  sz,c);
}

/* =====================================================================
   HELPERS VISUAIS GERAIS
   ===================================================================== */
static void DrawCloud(int x,int y,float s,Color c){
    DrawCircle(x,        y,        (int)(18*s),c);
    DrawCircle((int)(x+18*s),(int)(y+4*s),(int)(22*s),c);
    DrawCircle((int)(x+40*s),y,        (int)(16*s),c);
    DrawCircle((int)(x+20*s),(int)(y-10*s),(int)(16*s),c);
    DrawRectangle(x-2,y,(int)(44*s),(int)(12*s),c);
}
static void DrawBush(int x,int y,float s){
    DrawCircle(x,y,(int)(12*s),(Color){40,120,50,255});
    DrawCircle((int)(x+10*s),(int)(y+2*s),(int)(10*s),(Color){46,135,56,255});
    DrawCircle((int)(x-9*s),(int)(y+2*s),(int)(9*s),(Color){36,110,46,255});
    DrawCircle((int)(x+2*s),(int)(y-6*s),(int)(9*s),(Color){52,150,62,255});
}
static void DrawFlower(int x,int y,Color petal){
    DrawRectangle(x-1,y,2,8,(Color){40,120,50,255});
    DrawCircle(x-4,y-2,3,petal); DrawCircle(x+4,y-2,3,petal);
    DrawCircle(x,  y-5,3,petal); DrawCircle(x,  y+1,3,petal);
    DrawCircle(x,  y-2,2,(Color){255,220,60,255});
}
static void DrawFlowerBed(int x,int y,int w){
    DrawRectangle(x,y,w,10,(Color){70,150,70,255});
    Color cols[4]={(Color){230,70,90,255},(Color){240,200,40,255},
                   (Color){200,90,210,255},(Color){240,120,60,255}};
    for(int i=0;i*22<w;i++) DrawFlower(x+10+i*22,y+2,cols[i%4]);
}
static void DrawTree(int x,int y,float s){
    DrawEllipse(x,(int)(y+30*s),(int)(20*s),(int)(6*s),(Color){0,0,0,50});
    DrawRectangle((int)(x-5*s),y,(int)(10*s),(int)(30*s),(Color){95,60,30,255});
    DrawCircle(x,(int)(y-6*s),(int)(26*s),(Color){34,110,44,255});
    DrawCircle((int)(x-16*s),(int)(y+2*s),(int)(18*s),(Color){42,128,52,255});
    DrawCircle((int)(x+16*s),(int)(y+2*s),(int)(18*s),(Color){42,128,52,255});
    DrawCircle(x,(int)(y-20*s),(int)(18*s),(Color){54,150,64,255});
    DrawCircle((int)(x-6*s),(int)(y-12*s),(int)(5*s),(Color){90,190,90,140});
}
static void DrawLamppost(int x,int groundY){
    DrawRectangle(x-2,groundY-90,4,90,(Color){60,60,70,255});
    DrawRectangle(x-12,groundY-92,24,6,(Color){60,60,70,255});
    DrawCircle(x-12,groundY-86,5,(Color){255,235,150,230});
    DrawCircle(x+12,groundY-86,5,(Color){255,235,150,230});
    DrawRectangle(x-3,groundY-2,6,6,(Color){40,40,50,255});
}
static void DrawHydrant(int x,int groundY){
    DrawRectangle(x-7,groundY-22,14,22,(Color){200,40,30,255});
    DrawCircle(x,groundY-24,8,(Color){200,40,30,255});
    DrawCircle(x,groundY-24,3,(Color){255,210,0,255});
    DrawRectangle(x-12,groundY-16,4,6,(Color){170,30,20,255});
    DrawRectangle(x+8, groundY-16,4,6,(Color){170,30,20,255});
    DrawRectangle(x-9,groundY-2,18,4,(Color){150,30,20,255});
}
static void DrawSidewalk(int x,int y,int w,int h){
    DrawRectangle(x,y,w,h,(Color){185,182,172,255});
    for(int i=0;i<w;i+=28) DrawLine(x+i,y,x+i,y+h,(Color){150,148,140,255});
    DrawRectangle(x,y,w,3,(Color){205,202,192,255});
}
static void DrawParkedCar(int x,int y,Color c){
    DrawEllipse(x+28,y+26,30,6,(Color){0,0,0,60});
    DrawRectangle(x,y,56,22,c);
    DrawRectangle(x+8,y-12,38,14,c);
    DrawRectangle(x+12,y-9,12,9,(Color){150,190,220,220});
    DrawRectangle(x+28,y-9,12,9,(Color){150,190,220,220});
    DrawCircle(x+14,y+22,7,(Color){25,25,25,255}); DrawCircle(x+42,y+22,7,(Color){25,25,25,255});
    DrawCircle(x+14,y+22,3,(Color){120,120,120,255}); DrawCircle(x+42,y+22,3,(Color){120,120,120,255});
}
static void DrawWall(int x,int y,int w){
    DrawRectangle(x,y,w,28,(Color){214,204,184,255});
    DrawRectangle(x,y,w,5,(Color){186,176,156,255});
    for(int i=0;i<w;i+=16) DrawRectangle(x+i,y+8,3,20,(Color){196,186,166,255});
}
static void DrawSmokeColumn(float x,float baseY,float intensity,float seed){
    if(intensity<=0) return;
    int n=(int)(7*intensity)+4;
    for(int i=0;i<n;i++){
        float t=fmodf((float)GetTime()*0.4f+seed+i*0.16f,1.f);
        float yy=baseY - t*160*intensity;
        float xx=x + sinf(t*6+seed)*18*t;
        float r=(6+t*24)*intensity;
        unsigned char a=(unsigned char)((1-t)*120);
        DrawCircle((int)xx,(int)yy,(int)r,(Color){95,95,100,a});
    }
}
static void DrawBurnMark(int x,int y,int w,int h){
    DrawRectangle(x,y,w,h,(Color){35,30,28,180});
    for(int i=0;i<6;i++)
        DrawCircle(x+GetRandomValue(0,w),y+GetRandomValue(0,h),GetRandomValue(3,7),(Color){40,38,36,160});
}

/* =====================================================================
   PONTOS TURISTICOS DE BRASILIA
   ===================================================================== */
static void DrawCatedral(int cx,int gy,float s){
    int top=gy-(int)(150*s), waist=gy-(int)(70*s);
    DrawTriangle((Vector2){cx-56*s,(float)gy},(Vector2){cx+56*s,(float)gy},
                 (Vector2){(float)cx,(float)waist},(Color){120,180,220,170});
    for(int k=0;k<16;k++){
        float f=(k/15.f)*2-1;
        float bx=cx+f*58*s, wx=cx+f*20*s, tx=cx+f*66*s;
        DrawLineEx((Vector2){bx,(float)gy},(Vector2){wx,(float)waist},2.5f,(Color){236,236,242,255});
        DrawLineEx((Vector2){wx,(float)waist},(Vector2){tx,(float)top},2.5f,(Color){236,236,242,255});
    }
    DrawEllipse(cx,gy,(int)(60*s),(int)(8*s),(Color){182,182,192,255});
}
static void DrawCongresso(int cx,int gy,float s){
    DrawRectangle((int)(cx-92*s),(int)(gy-8*s),(int)(184*s),(int)(12*s),(Color){226,226,232,255});
    DrawCircleSector((Vector2){cx+46*s,gy-8*s},30*s,180,360,28,(Color){236,236,242,255});
    DrawEllipse((int)(cx-46*s),(int)(gy-12*s),(int)(32*s),(int)(11*s),(Color){236,236,242,255});
    DrawEllipse((int)(cx-46*s),(int)(gy-15*s),(int)(27*s),(int)(8*s),(Color){170,185,205,255});
    DrawRectangle((int)(cx-11*s),(int)(gy-92*s),(int)(9*s),(int)(84*s),(Color){236,236,242,255});
    DrawRectangle((int)(cx+3*s), (int)(gy-92*s),(int)(9*s),(int)(84*s),(Color){236,236,242,255});
    for(int y=0;y<10;y++){
        DrawRectangle((int)(cx-10*s),(int)(gy-86*s+y*8*s),(int)(7*s),(int)(4*s),(Color){150,180,210,200});
        DrawRectangle((int)(cx+4*s), (int)(gy-86*s+y*8*s),(int)(7*s),(int)(4*s),(Color){150,180,210,200});
    }
}
static void DrawEsplanada(int cx,int gy,float s){
    int n=8; int bw=(int)(22*s); int gap=(int)(8*s);
    int total=n*(bw+gap); int x0=cx-total/2;
    for(int i=0;i<n;i++){
        int bx=x0+i*(bw+gap), bh=(int)((60+(i%2)*6)*s);
        DrawRectangle(bx,gy-bh,bw,bh,(Color){150,165,195,255});
        DrawRectangle(bx,gy-bh,bw,(int)(4*s),(Color){120,135,165,255});
        for(int wy=8;wy<bh-4;wy+=9)
            for(int wx=3;wx<bw-4;wx+=8)
                DrawRectangle(bx+wx,gy-bh+wy,(int)(5*s),(int)(5*s),(Color){95,150,200,200});
    }
}
static void DrawEstadio(int cx,int cy,float s){
    DrawEllipse(cx,cy,(int)(120*s),(int)(42*s),(Color){214,218,228,255});
    DrawEllipse(cx,cy,(int)(96*s), (int)(30*s),(Color){120,160,120,255});
    for(int i=0;i<26;i++){
        float a=i*(2*PI/26);
        int px=(int)(cx+cosf(a)*112*s), py=(int)(cy+sinf(a)*38*s);
        DrawRectangle(px-2,py-2,4,(int)(20*s),(Color){242,242,247,255});
    }
    DrawEllipseLines(cx,cy,(int)(120*s),(int)(42*s),(Color){180,185,195,255});
}
static void DrawSkylineBrasilia(int baseY,float s){
    DrawEsplanada(W/2,baseY,s*0.8f);
    DrawCongresso(W/2,baseY,s);
    DrawCatedral(140,baseY,s*0.7f);
    DrawCatedral(W-140,baseY,s*0.7f);
}

/* =====================================================================
   ILUSTRACOES DE FERRAMENTAS / OBJETOS
   ===================================================================== */
static void DrawIcon(int id,int cx,int cy,float s,bool sel){
    if(sel) DrawCircle(cx,cy,(int)(34*s),(Color){255,255,200,60});
    switch(id){
      case IC_MANGUEIRA:
        for(int r=0;r<4;r++) DrawCircleLines((int)(cx-6*s),cy,(8+r*5)*s,(r%2)?(Color){235,205,40,255}:(Color){205,60,40,255});
        DrawCircle((int)(cx-6*s),cy,(int)(4*s),(Color){80,80,90,255});
        DrawRectangle((int)(cx+10*s),(int)(cy-3*s),(int)(16*s),(int)(6*s),(Color){180,180,190,255});
        DrawTriangle((Vector2){cx+26*s,cy-5*s},(Vector2){cx+26*s,cy+5*s},(Vector2){cx+34*s,(float)cy},(Color){150,150,160,255});
        break;
      case IC_EXTINTOR:
        DrawRectangleRounded((Rectangle){cx-10*s,cy-16*s,20*s,34*s},0.4f,6,(Color){200,30,30,255});
        DrawRectangle((int)(cx-11*s),(int)(cy-22*s),(int)(22*s),(int)(8*s),(Color){40,40,45,255});
        DrawRectangle((int)(cx-2*s),(int)(cy-30*s),(int)(4*s),(int)(10*s),(Color){40,40,45,255});
        DrawLine((int)(cx+9*s),(int)(cy-20*s),(int)(cx+20*s),(int)(cy-6*s),(Color){30,30,30,255});
        DrawCircle((int)(cx+20*s),(int)(cy-6*s),(int)(3*s),(Color){30,30,30,255});
        DrawRectangle((int)(cx-8*s),(int)(cy-2*s),(int)(16*s),(int)(10*s),(Color){240,240,240,180});
        break;
      case IC_MACHADO:
        DrawRectangleRounded((Rectangle){cx-2*s,cy-22*s,5*s,44*s},0.5f,4,(Color){150,90,40,255}); /* Cabo */
        DrawRectangle((int)(cx),(int)(cy-16*s),(int)(18*s),(int)(12*s),(Color){205,205,215,255}); /* Lâmina horiz */
        DrawRectangle((int)(cx+14*s),(int)(cy-16*s),(int)(4*s),(int)(12*s),(Color){220,220,230,255}); /* Fio da lâmina */
        DrawRectangle((int)(cx-3*s),(int)(cy-18*s),(int)(6*s),(int)(16*s),(Color){90,90,100,255}); /* Encaixe */
        break;
      case IC_CAPACETE:
        DrawRectangle((int)(cx-18*s),(int)(cy+6*s),(int)(36*s),(int)(6*s),(Color){255,170,0,255});
        DrawCircleSector((Vector2){(float)cx,cy+6*s},16*s,180,360,20,(Color){210,40,20,255});
        DrawRectangle((int)(cx-4*s),(int)(cy-14*s),(int)(8*s),(int)(10*s),(Color){170,20,10,255});
        DrawRectangle((int)(cx-11*s),(int)(cy-1*s),(int)(22*s),(int)(4*s),(Color){255,230,120,255});
        break;
      case IC_RADIO:
        DrawRectangleRounded((Rectangle){cx-9*s,cy-14*s,18*s,32*s},0.3f,6,(Color){40,45,55,255});
        DrawRectangle((int)(cx-2*s),(int)(cy-26*s),(int)(3*s),(int)(12*s),(Color){40,45,55,255});
        DrawCircle((int)(cx-1*s),(int)(cy-26*s),(int)(3*s),(Color){40,45,55,255});
        DrawRectangle((int)(cx-6*s),(int)(cy-10*s),(int)(12*s),(int)(8*s),(Color){90,200,120,255});
        for(int i=0;i<3;i++) DrawRectangle((int)(cx-6*s),(int)(cy+2*s+i*4*s),(int)(12*s),(int)(2*s),(Color){90,90,100,255});
        break;
      case IC_KIT:
        DrawRectangleRounded((Rectangle){cx-18*s,cy-12*s,36*s,26*s},0.2f,6,(Color){246,246,246,255});
        DrawRectangle((int)(cx-18*s),(int)(cy-12*s),(int)(36*s),(int)(7*s),(Color){222,222,222,255});
        DrawRectangle((int)(cx-3*s),(int)(cy-7*s),(int)(6*s),(int)(16*s),(Color){220,30,30,255});
        DrawRectangle((int)(cx-9*s),(int)(cy-1*s),(int)(18*s),(int)(5*s),(Color){220,30,30,255});
        break;
      case IC_CONE:
        DrawTriangle((Vector2){(float)cx,cy-20*s},(Vector2){cx-14*s,cy+14*s},(Vector2){cx+14*s,cy+14*s},(Color){240,120,20,255});
        DrawRectangle((int)(cx-18*s),(int)(cy+14*s),(int)(36*s),(int)(5*s),(Color){200,90,10,255});
        DrawRectangle((int)(cx-8*s),(int)(cy-2*s),(int)(16*s),(int)(5*s),(Color){255,255,255,255});
        break;
      case IC_ESCADA:
        DrawRectangle((int)(cx-12*s),(int)(cy-22*s),(int)(4*s),(int)(44*s),(Color){222,182,42,255});
        DrawRectangle((int)(cx+8*s), (int)(cy-22*s),(int)(4*s),(int)(44*s),(Color){222,182,42,255});
        for(int i=0;i<5;i++) DrawRectangle((int)(cx-12*s),(int)(cy-18*s+i*9*s),(int)(24*s),(int)(3*s),(Color){200,160,30,255});
        break;
      case IC_VIDEOGAME:
        DrawRectangleRounded((Rectangle){cx-20*s,cy-8*s,40*s,18*s},0.6f,8,(Color){70,80,100,255});
        DrawCircle((int)(cx-20*s),(int)(cy+2*s),(int)(8*s),(Color){70,80,100,255});
        DrawCircle((int)(cx+20*s),(int)(cy+2*s),(int)(8*s),(Color){70,80,100,255});
        DrawCircle((int)(cx+12*s),(int)(cy-2*s),(int)(3*s),(Color){230,80,80,255});
        DrawCircle((int)(cx+18*s),(int)(cy-2*s),(int)(3*s),(Color){90,200,120,255});
        DrawRectangle((int)(cx-16*s),(int)(cy-3*s),(int)(3*s),(int)(8*s),(Color){40,40,50,255}); /* d-pad vert */
        DrawRectangle((int)(cx-19*s),(int)(cy),       (int)(9*s),(int)(2*s),(Color){40,40,50,255}); /* d-pad horiz */
        break;
      case IC_SKATE:
        DrawRectangleRounded((Rectangle){cx-22*s,cy-3*s,44*s,7*s},0.8f,6,(Color){150,80,40,255});
        DrawCircle((int)(cx-12*s),(int)(cy+9*s),(int)(4*s),(Color){240,200,40,255});
        DrawCircle((int)(cx+12*s),(int)(cy+9*s),(int)(4*s),(Color){240,200,40,255});
        break;
      case IC_PIPOCA:
        for(int i=0;i<6;i++) DrawCircle((int)(cx-10*s+(i%3)*10*s),(int)(cy-14*s-(i/3)*6*s),(int)(5*s),(Color){250,240,200,255});
        DrawRectangleRounded((Rectangle){cx-14*s,cy-6*s,28*s,22*s},0.1f,4,(Color){220,40,40,255});
        DrawRectangle((int)(cx-9*s),(int)(cy-6*s),(int)(3*s),(int)(22*s),(Color){255,255,255,200});
        DrawRectangle((int)(cx+4*s),(int)(cy-6*s),(int)(3*s),(int)(22*s),(Color){255,255,255,200});
        break;
      case IC_BOLA:
        DrawCircle(cx,cy,(int)(16*s),(Color){246,246,246,255});
        DrawCircleLines(cx,cy,16*s,(Color){40,40,40,255});
        DrawPoly((Vector2){(float)cx,(float)cy},5,6*s,0,(Color){30,30,30,255});
        break;
      case IC_DOCE:
        DrawCircle(cx,(int)(cy-6*s),(int)(12*s),(Color){240,90,140,255});
        DrawCircle(cx,(int)(cy-6*s),(int)(7*s),(Color){250,180,210,255});
        DrawRectangle((int)(cx-1*s),(int)(cy+4*s),(int)(2*s),(int)(16*s),(Color){240,240,240,255});
        break;
    }
}
static void DrawToolCard(Rectangle b,int icon,const char*label,bool chosen,bool hov){
    Color bg = chosen?(Color){36,92,46,255}:(hov?(Color){54,64,92,255}:(Color){40,48,72,255});
    DrawRectangleRounded(b,0.16f,8,bg);
    DrawRectangleRoundedLines(b,0.16f,8,chosen?(Color){120,240,130,255}:(Color){255,200,0,255});
    DrawIcon(icon,(int)(b.x+b.width/2),(int)(b.y+b.height/2-10),b.width/110.f,chosen);
    DBold(label,(int)(b.x+b.width/2-MeasureText(label,14)/2),(int)(b.y+b.height-22),14,WHITE);
}

/* =====================================================================
   BOMBEIRO GENERICO (figurante) + CENARIO DE QUARTEL
   ===================================================================== */
static void DrawBombeiroTrabalhando(int x,int y,int fr){
    int arm=(fr%2)?-4:4;
    DrawRectangle(x+4,y+44,7,6,(Color){25,25,25,255}); DrawRectangle(x+14,y+44,7,6,(Color){25,25,25,255});
    DrawRectangle(x+5,y+28,7,16,(Color){30,30,40,255}); DrawRectangle(x+14,y+28,7,16,(Color){30,30,40,255});
    DrawRectangle(x+4,y+12,18,18,(Color){200,60,40,255}); DrawRectangle(x+4,y+16,18,3,(Color){255,230,120,255});
    DrawRectangle(x+1,y+13,4,12+arm,(Color){200,60,40,255}); DrawRectangle(x+21,y+13,4,12-arm,(Color){200,60,40,255});
    DrawRectangle(x+7,y,14,11,(Color){255,205,150,255}); DrawRectangle(x+6,y-3,16,4,(Color){255,170,0,255});
}
static void DrawQuartelBG(void){
    ClearBackground((Color){150,195,235,255});
    DrawCloud(70,60,1.0f,(Color){255,255,255,210}); DrawCloud(360,42,1.2f,(Color){255,255,255,200});
    for(int i=0;i<7;i++) DrawCircle(20+i*95,150,30,(Color){70,140,70,120});
    DrawRectangle(20,120,560,250,(Color){202,207,217,255});
    DrawRectangleLines(20,120,560,250,(Color){120,125,135,255});
    DrawRectangle(20,100,560,30,(Color){190,40,30,255}); DrawRectangle(20,98,560,6,(Color){255,200,0,255});
    DBold("CORPO DE BOMBEIROS MILITAR - DF", W/2-MeasureText("CORPO DE BOMBEIROS MILITAR - DF",18)/2,104,18,WHITE);
    DrawCircle(60,150,18,(Color){255,200,0,255}); DrawCircle(60,150,13,(Color){190,40,30,255});
    DBold("CBM",45,143,14,WHITE);
    for(int g=0;g<3;g++){
        int gx=70+g*165;
        DrawRectangle(gx,180,150,180,(Color){72,82,97,255});
        for(int l=0;l<7;l++) DrawRectangle(gx,184+l*25,150,3,(Color){52,57,67,255});
        DrawRectangle(gx+10,180,130,8,(Color){255,200,0,255});
    }
    DrawRectangle(85,250,120,110,(Color){200,30,30,255}); DrawRectangle(90,262,110,16,(Color){255,255,255,255});
    DrawRectangle(105,230,80,18,(Color){180,220,255,200}); DrawRectangle(120,212,70,10,(Color){255,255,0,255});
    DrawCircle(115,360,16,(Color){25,25,25,255}); DrawCircle(175,360,16,(Color){25,25,25,255});
    for(int i=0;i<3;i++){
        int lx=400+i*50;
        DrawRectangle(lx,210,46,140,(Color){90,30,20,255});
        DrawRectangleLines(lx,210,46,140,(Color){50,15,10,255});
        DrawCircle(lx+38,278,3,(Color){255,210,40,255});
        DrawRectangle(lx+6,220,34,10,(Color){255,200,0,200});
    }
    DrawRectangle(560,150,40,210,(Color){70,75,90,255});
    for(int b=0;b<6;b++) DrawRectangle(566,160+b*32,28,5,(Color){110,115,130,255});
    /* Bandeira e mastro de metal removidos */
    DrawRectangle(0,360,W,H-360,(Color){92,97,107,255});
    DrawRectangle(0,360,W,8,(Color){72,77,87,255});
    DrawBombeiroTrabalhando(300,300,(int)(GetTime()*3)); DrawBombeiroTrabalhando(360,305,(int)(GetTime()*3)+1);
}
static void DrawEPIicon(int bit,int x,int y){
    if(bit==1){ DrawRectangle(x-11,y-2,9,12,(Color){30,30,30,255}); DrawRectangle(x+2, y-2,9,12,(Color){30,30,30,255}); }
    else if(bit==2){ DrawRectangle(x-6,y-12,12,24,(Color){200,200,210,255}); }
    else if(bit==4){ DrawCircle(x,y,11,(Color){70,70,80,255}); DrawCircle(x,y,6,(Color){160,200,230,255}); }
    else if(bit==8){ DrawRectangle(x-12,y+4,24,4,(Color){255,170,0,255}); DrawCircleSector((Vector2){(float)x,(float)y+4},11,180,360,16,(Color){210,40,20,255}); }
    else { DrawRectangle(x-9,y-6,8,14,(Color){200,60,0,255}); DrawRectangle(x+1,y-6,8,14,(Color){200,60,0,255}); }
}

/* =====================================================================
   PARTICULAS / FOGUETES
   ===================================================================== */
static void ResetParts(void){ for(int i=0;i<MP;i++) pts[i].on=false; }
static void ResetRockets(void){ for(int i=0;i<MF;i++) fogs[i].on=false; for(int i=0;i<ME;i++) est[i].on=false; }
static void ResetJatos(void){ for(int i=0;i<MJT;i++) jts[i].on=false; }

static void EmitPart(float x,float y){
    for(int i=0;i<MP;i++) if(!pts[i].on){
        pts[i]=(Part){ x+(float)GetRandomValue(-40,40), y, GetRandomValue(-15,15)/10.f, -(float)GetRandomValue(3,9), 1.f, (float)GetRandomValue(6,20), GetRandomValue(0,1)?(Color){255,(unsigned char)GetRandomValue(80,180),0,255}:(Color){120,120,120,255}, true };
        break;
    }
}
static void UpdParts(void){
    for(int i=0;i<MP;i++){ if(!pts[i].on) continue;
        pts[i].x+=pts[i].vx; pts[i].y+=pts[i].vy; pts[i].a-=0.014f; if(pts[i].a<=0) pts[i].on=false;
    }
}
static void DrawParts(void){
    for(int i=0;i<MP;i++){ if(!pts[i].on) continue; Color c=pts[i].c; c.a=(unsigned char)(pts[i].a*200); DrawCircle((int)pts[i].x,(int)pts[i].y,(int)pts[i].r,c); }
}
static void Explode(int k){
    Color cor=fogs[k].c;
    for(int j=0;j<40;j++) for(int i=0;i<ME;i++) if(!est[i].on){
        float ang=GetRandomValue(0,628)/100.f, v=GetRandomValue(15,80)/10.f;
        est[i]=(Estil){fogs[k].x,fogs[k].y,cosf(ang)*v,sinf(ang)*v,1.f,cor,true}; break;
    }
    fogs[k].on=false;
}
static void LaunchRocket(void){
    Color cores[]={RED,YELLOW,BLUE,GREEN,MAGENTA,ORANGE,SKYBLUE,WHITE};
    for(int i=0;i<MF;i++) if(!fogs[i].on){
        fogs[i]=(Foguete){(float)GetRandomValue(80,W-80),(float)H, GetRandomValue(-20,20)/10.f, -(float)GetRandomValue(12,18),0, cores[GetRandomValue(0,7)],true}; break;
    }
}
static void UpdFoguetes(void){
    for(int i=0;i<MF;i++){ if(!fogs[i].on) continue;
        fogs[i].x+=fogs[i].vx; fogs[i].y+=fogs[i].vy; fogs[i].vy+=0.18f; fogs[i].t+=GetFrameTime();
        if(fogs[i].vy>=0||fogs[i].t>2.5f) Explode(i);
    }
    for(int i=0;i<ME;i++){ if(!est[i].on) continue;
        est[i].x+=est[i].vx; est[i].y+=est[i].vy; est[i].vy+=0.07f; est[i].vx*=0.98f; est[i].a-=0.016f; if(est[i].a<=0) est[i].on=false;
    }
}
static void DrawFoguetes(void){
    for(int i=0;i<MF;i++){ if(!fogs[i].on) continue; DrawCircle((int)fogs[i].x,(int)fogs[i].y,4,fogs[i].c); }
    for(int i=0;i<ME;i++){ if(!est[i].on) continue; Color c=est[i].c; c.a=(unsigned char)(est[i].a*255); DrawCircle((int)est[i].x,(int)est[i].y,2,c); }
}

/* =====================================================================
   PERSONAGENS
   ===================================================================== */
#define EQ_BOTAS    1
#define EQ_CILINDRO 2
#define EQ_MASCARA  4
#define EQ_CAPACETE 8
#define EQ_LUVAS    16
#define EQ_TUDO     (EQ_BOTAS|EQ_CILINDRO|EQ_MASCARA|EQ_CAPACETE|EQ_LUVAS)

static void DrawClara(int x,int y,bool dir,int fr,int gear){
    int po=(fr%4<2)?3:-3;
    if(gear&EQ_CILINDRO) DrawRectangle(x+9,y+10,12,20,(Color){200,200,210,255});
    DrawRectangle(x+6,y+28,8,16+po,(Color){30,40,160,255}); DrawRectangle(x+16,y+28,8,16-po,(Color){30,40,160,255});
    if(gear&EQ_BOTAS){ DrawRectangle(x+4,y+42+po,11,8,(Color){25,25,25,255}); DrawRectangle(x+15,y+42-po,11,8,(Color){25,25,25,255}); }
    else { DrawRectangle(x+5,y+44+po,9,5,(Color){60,40,20,255}); DrawRectangle(x+16,y+44-po,9,5,(Color){60,40,20,255}); }
    DrawRectangle(x+5,y+12,20,18,(Color){60,110,210,255});
    DrawRectangle(x+5,y+17,20,3,(Color){255,220,0,255});
    DrawRectangle(x+1,y+13+po,5,12,(Color){60,110,210,255}); DrawRectangle(x+24,y+13-po,5,12,(Color){60,110,210,255});
    if(gear&EQ_LUVAS){ DrawRectangle(x+1,y+23+po,6,6,(Color){200,60,0,255}); DrawRectangle(x+24,y+23-po,6,6,(Color){200,60,0,255}); }
    else { DrawRectangle(x+1,y+24+po,5,5,(Color){255,205,150,255}); DrawRectangle(x+24,y+24-po,5,5,(Color){255,205,150,255}); }
    /* Correção do sprite da Clara: Cabelo ajustado (sem pixel para fora da cabeça) e Segundo olho adicionado */
    DrawRectangle(x+8,y,14,12,(Color){255,205,150,255});
    if(gear&EQ_MASCARA){ DrawRectangle(x+7,y+4,16,8,(Color){70,70,80,255}); DrawRectangle(x+10,y+5,10,5,(Color){160,200,230,255}); }
    else { 
        DrawRectangle(dir?x+15:x+8,y+3,3,3,(Color){20,20,20,255}); 
        DrawRectangle(dir?x+9:x+14,y+3,3,3,(Color){20,20,20,255}); /* Novo Olho */
    }
    if(gear&EQ_CAPACETE){ DrawRectangle(x+6,y-2,18,4,(Color){255,170,0,255}); DrawRectangle(x+8,y-9,14,8,(Color){220,40,0,255}); }
    else { DrawRectangle(x+7,y-2,16,3,(Color){90,55,20,255}); }
}
static void DrawCapitao(int x,int y){
    DrawRectangle(x+5,y+44,9,7,(Color){25,25,25,255}); DrawRectangle(x+16,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+6,y+28,8,18,(Color){20,20,30,255}); DrawRectangle(x+16,y+28,8,18,(Color){20,20,30,255});
    DrawRectangle(x+4,y+12,22,18,(Color){200,40,30,255}); DrawRectangle(x+4,y+12,22,4,(Color){255,210,0,255});
    DrawRectangle(x+13,y+16,4,14,(Color){255,210,0,255}); DrawRectangle(x+1,y+13,5,14,(Color){200,40,30,255});
    DrawRectangle(x+24,y+13,5,14,(Color){200,40,30,255}); DrawRectangle(x+7,y,16,12,(Color){255,210,160,255});
    DrawRectangle(x+10,y+4,3,3,(Color){20,20,20,255}); DrawRectangle(x+17,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+6,y-3,18,5,(Color){240,240,240,255}); DrawRectangle(x+8,y-8,14,6,(Color){240,240,240,255});
}
static void DrawAna(int x,int y){
    DrawRectangle(x+5,y+44,9,7,(Color){25,25,25,255}); DrawRectangle(x+16,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+6,y+28,8,18,(Color){40,40,50,255}); DrawRectangle(x+16,y+28,8,18,(Color){40,40,50,255});
    DrawRectangle(x+5,y+12,20,18,(Color){230,120,20,255}); DrawRectangle(x+5,y+17,20,3,(Color){255,255,255,255});
    DrawRectangle(x+1,y+13,5,14,(Color){230,120,20,255}); DrawRectangle(x+24,y+13,5,14,(Color){230,120,20,255});
    DrawRectangle(x+7,y,16,12,(Color){240,195,150,255});
    DrawRectangle(x+11,y+4,3,3,(Color){20,20,20,255}); DrawRectangle(x+17,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+6,y-2,18,4,(Color){255,170,0,255}); DrawRectangle(x+8,y-8,14,7,(Color){230,120,20,255});
}
static void DrawSilva(int x,int y){
    DrawRectangle(x+5,y+44,9,7,(Color){25,25,25,255}); DrawRectangle(x+16,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+6,y+28,8,18,(Color){30,30,40,255}); DrawRectangle(x+16,y+28,8,18,(Color){30,30,40,255});
    DrawRectangle(x+5,y+12,20,18,(Color){30,30,200,255}); DrawRectangle(x+5,y+17,20,3,(Color){255,220,0,255});
    DrawRectangle(x+1,y+13,5,14,(Color){30,30,200,255}); DrawRectangle(x+24,y+13,5,14,(Color){30,30,200,255});
    DrawRectangle(x+7,y,16,12,(Color){235,185,140,255});
    DrawRectangle(x+9,y+4,3,3,(Color){20,20,20,255}); DrawRectangle(x+16,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+6,y-2,18,4,(Color){50,50,60,255}); DrawRectangle(x+8,y-7,14,5,(Color){50,50,60,255});
}
static void DrawDog(int x,int y,float aba,Color base_color){
    int wag=(int)(sinf(GetTime()*8)*4);
    DrawRectangle(x,y,26,14,base_color);
    DrawRectangle(x-8,y-8,14,14,base_color);
    DrawRectangle(x-10,y-10,6,8,(Color){120,80,40,255});
    DrawRectangle(x-7,y-4,3,3,(Color){20,20,20,255}); DrawRectangle(x-10,y,4,3,(Color){20,20,20,255});
    DrawRectangle(x+24,y+2,8+wag,4,base_color);
    DrawRectangle(x+2,y+14,5,8,(Color){150,100,55,255}); DrawRectangle(x+18,y+14,5,8,(Color){150,100,55,255});
    if(aba>0){ const char*au="AU!"; DBold(au,x-6,y-26,14,(Color){255,255,120,255}); }
}
static void DrawCat(int x,int y){
    DrawRectangle(x,y,16,10,(Color){200,200,200,255});
    DrawRectangle(x-6,y-6,10,10,(Color){200,200,200,255});
    DrawTriangle((Vector2){x-6,y-6},(Vector2){x-2,y-12},(Vector2){x,y-6},(Color){200,200,200,255});
    DrawTriangle((Vector2){x,y-6},(Vector2){x+2,y-12},(Vector2){x+4,y-6},(Color){200,200,200,255});
    DrawRectangle(x-4,y-3,2,2,BLACK); DrawRectangle(x,y-3,2,2,BLACK);
    DrawRectangle(x+14,y-4,2,8,(Color){200,200,200,255});
}

/* =====================================================================
   HUD / TELAS PADRAO
   ===================================================================== */
static const char* NOME_FASE[]={ "",
    "Conhecendo o Quartel","Chamada de Emergência","Preparação da Viatura",
    "Deslocamento pela Cidade","Incêndio Residencial","Primeiros Socorros",
    "Resgate Animal","Acidente de Trânsito","Temporada de Queimadas",
    "Grande Missão em Brasília" };
static const char* LOCAL_FASE[]={ "",
    "Quartel Central - Brasília","Bairro residencial","Pátio da viatura",
    "Esplanada dos Ministérios","Casa em chamas","Rua - vítima caída",
    "Parque da Cidade","Próximo ao estádio","Área rural do DF",
    "Brasília - vários locais" };

static void DrawHUD(int fase,int estrelas,int chances,float timer,const char*info){
    DrawRectangle(0,0,W,52,(Color){0,0,0,170});
    char ts[40]; sprintf(ts,"FASE %d  %02d:%02d",fase,(int)timer/60,(int)timer%60);
    DBold(ts,12,7,20,(Color){255,220,50,255});
    DBold(LOCAL_FASE[fase],12,30,15,(Color){180,200,230,255});
    DBold("ESTRELAS:",300,8,14,(Color){255,230,120,255});
    for(int i=0;i<5;i++){
        Color sc=(i<estrelas)?(Color){255,210,40,255}:(Color){70,70,40,255};
        int sx=388+i*22, sy=16;
        DrawTriangle((Vector2){sx,sy-7},(Vector2){sx-6,sy+6},(Vector2){sx+6,sy+6},sc);
        DrawTriangle((Vector2){sx,sy+9},(Vector2){sx-6,sy-4},(Vector2){sx+6,sy-4},sc);
    }
    DBold("VIDAS:",300,32,13,(Color){255,120,120,255});
    for(int i=0;i<3;i++){
        Color hc=(i<chances)?(Color){255,45,75,255}:(Color){60,20,20,255};
        int hx=372+i*22, hy=33;
        DrawCircle(hx+3,hy+4,4,hc); DrawCircle(hx+9,hy+4,4,hc);
        DrawTriangle((Vector2){hx-1,hy+6},(Vector2){hx+13,hy+6},(Vector2){hx+6,hy+15},hc);
    }
    if(info&&info[0]) DBold(info,W/2-MeasureText(info,15)/2,H-26,15,(Color){210,210,210,220});
}
static void DrawFaseConcluida(int fase,const char*l1,const char*l2){
    DrawRectangle(0,H/2-140,W,250,(Color){170,0,0,245});
    DrawRectangle(0,H/2-144,W,8,(Color){255,195,0,255}); DrawRectangle(0,H/2+102,W,8,(Color){255,195,0,255});
    char m1[64]; sprintf(m1,"FASE %d CONCLUÍDA!",fase);
    DBold(m1,W/2-MeasureText(m1,30)/2+2,H/2-122,30,(Color){0,0,0,160});
    DBold(m1,W/2-MeasureText(m1,30)/2,H/2-124,30,(Color){255,230,50,255});
    DBold("VOCÊ APRENDEU:",W/2-MeasureText("VOCÊ APRENDEU:",20)/2,H/2-44,20,WHITE);
    if(l1[0]) DBold(l1,W/2-MeasureText(l1,18)/2,H/2-14,18,(Color){200,230,255,255});
    if(l2[0]) DBold(l2,W/2-MeasureText(l2,18)/2,H/2+10,18,(Color){200,230,255,255});
    const char*cont="Clique ou ESPACO para continuar";
    DBold(cont,W/2-MeasureText(cont,16)/2,H/2+58,16,(Color){255,255,180,255});
}
static void DrawIntro(int fase,const char*l1,const char*l2,const char*l3){
    ClearBackground((Color){18,22,40,255});
    DrawSkylineBrasilia(620,1.0f);
    DrawRectangle(0,0,W,H,(Color){10,14,32,90});
    DrawRectangle(0,90,W,70,(Color){0,40,120,235});
    DrawRectangle(0,93,W,4,(Color){255,200,0,255}); DrawRectangle(0,156,W,4,(Color){255,200,0,255});
    char tit[48]; sprintf(tit,"FASE %d",fase);
    DBold(tit,W/2-MeasureText(tit,22)/2,104,22,(Color){255,230,50,255});
    DBold(NOME_FASE[fase],W/2-MeasureText(NOME_FASE[fase],19)/2,131,19,WHITE);
    DrawCapitao(70,470);
    DrawRectangleRounded((Rectangle){120,250,400,180},0.12f,8,(Color){245,245,250,255});
    DrawRectangleRoundedLines((Rectangle){120,250,400,180},0.12f,8,(Color){0,40,120,255});
    DrawTriangle((Vector2){120,470},(Vector2){145,420},(Vector2){175,420},(Color){245,245,250,255});
    DBold("Capitão Almeida:",138,264,20,(Color){180,30,20,255});
    if(l1[0]) DBold(l1,138,300,18,(Color){30,30,40,255});
    if(l2[0]) DBold(l2,138,328,18,(Color){30,30,40,255});
    if(l3[0]) DBold(l3,138,356,18,(Color){30,30,40,255});
    const char*go="Clique ou ESPACO para começar!";
    DBold(go,W/2-MeasureText(go,18)/2,560,18,(Color){255,230,80,255});
}

/* =====================================================================
   QUIZ GENERICO
   ===================================================================== */
static Rectangle QZ_BTN[3];
static int   qz_sel;
static float qz_flash;
static bool  qz_ok;
static void QuizInit(int yBase){
    qz_sel=-1; qz_flash=0; qz_ok=false;
    for(int i=0;i<3;i++) QZ_BTN[i]=(Rectangle){40.f, yBase+i*80.f,520,68};
}
static int QuizUpdate(Quiz*set,int passo,Vector2 m, int *chances, Estado *st){
    if(qz_flash>0){
        qz_flash-=GetFrameTime();
        if(qz_flash<=0 && qz_ok){ qz_ok=false; return 1; }
        return 0;
    }
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        for(int i=0;i<3;i++) if(CheckCollisionPointRec(m,QZ_BTN[i])){
            qz_sel=i; 
            qz_ok=(i==set[passo].correta); 
            qz_flash=qz_ok?0.8f:1.0f;
            if(!qz_ok) {
                (*chances)--; 
                if(*chances<=0) *st=GAMEOVER;
            }
        }
    }
    return 0;
}
static void QuizDraw(Quiz*set,int passo){
    DBold(set[passo].pergunta,W/2-MeasureText(set[passo].pergunta,19)/2, (int)QZ_BTN[0].y-26,19,WHITE);
    for(int i=0;i<3;i++){
        Color bc={0,70,150,255};
        if(qz_flash>0 && i==qz_sel) bc=qz_ok?(Color){30,150,40,255}:(Color){170,30,30,255};
        DrawRectangleRounded(QZ_BTN[i],0.2f,6,bc);
        DrawRectangleRoundedLines(QZ_BTN[i],0.2f,6,(Color){255,200,0,255});
        DBold(set[passo].op[i],(int)QZ_BTN[i].x+18,(int)QZ_BTN[i].y+25,19,WHITE);
    }
    if(qz_flash>0){
        const char*msg=qz_ok?"Isso mesmo!":"Tente a opção correta. (-1 VIDA)";
        Color mc=qz_ok?(Color){120,255,120,255}:(Color){255,150,150,255};
        DBold(msg,W/2-MeasureText(msg,20)/2,700,20,mc);
    }
}

/* =====================================================================
   MANGUEIRA / FOCOS GENERICOS
   ===================================================================== */
static void DrawFoco(float x,float y,float s){
    if(s<=0) return;
    float t=(float)GetTime(); float oy=sinf(t*5+x)*5; float fl=sinf(t*9+x)*4;
    DrawCircle((int)x,(int)(y+oy),(int)(22*s),(Color){210,40,0,170});
    for(int k=-1;k<=1;k++){
        float fx=x+k*9*s; float hh=(28+(k==0?10:0))*s + fl;
        DrawTriangle((Vector2){fx-7*s,y+6*s},(Vector2){fx+7*s,y+6*s}, (Vector2){fx+sinf(t*8+k)*4,(y-hh)},(Color){255,120,0,210});
    }
    DrawTriangle((Vector2){x-6*s,y+4*s},(Vector2){x+6*s,y+4*s}, (Vector2){x+sinf(t*10)*3,(y-22*s-fl)},(Color){255,210,50,225});
}
static void HoseFire(float camx,float ang){
    for(int i=0;i<MJT;i++) if(!jts[i].on){
        float rad=ang*(PI/180.f);
        jts[i]=(Jato){camx+55,(float)(H-95),cosf(rad)*18.f,sinf(rad)*18.f,1.f,true}; break;
    }
}
static void HoseMove(Foco*foco,int n,int*apag,float windvx){
    for(int i=0;i<MJT;i++){ if(!jts[i].on) continue;
        jts[i].vx+=windvx; jts[i].x+=jts[i].vx; jts[i].y+=jts[i].vy; jts[i].vy+=0.16f; jts[i].a-=0.010f;
        if(jts[i].a<=0||jts[i].y>H){ jts[i].on=false; continue; }
        for(int f=0;f<n;f++){ if(foco[f].fogo<=0) continue;
            if(fabsf(jts[i].x-foco[f].x)<32 && fabsf(jts[i].y-foco[f].y)<32){
                foco[f].fogo-=0.20f; if(foco[f].fogo<=0){ foco[f].fogo=0; (*apag)++; }
                jts[i].on=false; break;
            }
        }
    }
}
static void DrawJatos(void){
    for(int i=0;i<MJT;i++){ if(!jts[i].on) continue; Color wc={100,180,255,(unsigned char)(jts[i].a*255)}; DrawCircle((int)jts[i].x,(int)jts[i].y,5,wc); }
}
static void DrawViaturaMangueira(int cx,float ang){
    DrawRectangle(cx,H-105,110,50,(Color){200,30,30,255});
    DrawRectangle(cx+5,H-125,50,24,(Color){200,30,30,255});
    DrawRectangle(cx+8,H-122,28,16,(Color){180,220,255,200});
    DrawCircle(cx+18,H-55,16,(Color){30,30,30,255}); DrawCircle(cx+18,H-55,8,(Color){80,80,80,255});
    DrawCircle(cx+90,H-55,16,(Color){30,30,30,255}); DrawCircle(cx+90,H-55,8,(Color){80,80,80,255});
    DrawRectangle(cx+10,H-103,90,8,(Color){255,255,0,255});
    /* Sirene posicionada no teto da viatura */
    DrawRectangle(cx+20,H-133,18,8,(int)(GetTime()*8)%2?(Color){255,40,40,255}:(Color){40,40,255,255});
    float rad=ang*(PI/180.f); int mx=cx+55, my=H-95;
    DrawLineEx((Vector2){(float)mx,(float)my},(Vector2){mx+cosf(rad)*42,my+sinf(rad)*42},3.0f,(Color){40,100,40,255});
    DrawCircle(mx+(int)(cosf(rad)*42),my+(int)(sinf(rad)*42),5,(Color){120,180,255,255});
}
static void HoseControls(float*camx,float*ang){
    if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A))  *camx-=3;
    if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) *camx+=3;
    if(*camx<0) *camx=0; if(*camx>W-110) *camx=W-110;
    if(IsKeyDown(KEY_UP)||IsKeyDown(KEY_W))    *ang-=1.6f;
    if(IsKeyDown(KEY_DOWN)||IsKeyDown(KEY_S))  *ang+=1.6f;
    if(*ang>-15.f) *ang=-15.f; if(*ang<-165.f) *ang=-165.f;
    if(IsKeyPressed(KEY_SPACE)) HoseFire(*camx,*ang);
}

/* =====================================================================
   FASE 1 - CONHECENDO O QUARTEL
   ===================================================================== */
typedef struct { const char*nome; int bit; Rectangle btn; bool usado; } ItemEPI;
static ItemEPI epi[5];
static int f1_equip, f1_vestidos; static float f1_flash;
static void InitF1(void){
    const char* nomes[5]={"BOTAS","CILINDRO","MASCARA","CAPACETE","LUVAS"};
    int bits[5]={EQ_BOTAS,EQ_CILINDRO,EQ_MASCARA,EQ_CAPACETE,EQ_LUVAS};
    for(int i=0;i<5;i++){
        epi[i].nome=nomes[i]; epi[i].bit=bits[i]; epi[i].usado=false;
        epi[i].btn=(Rectangle){ 30.f+(i%3)*190.f, 600.f+(i/3)*70.f, 170, 56 };
    }
    f1_equip=0; f1_vestidos=0; f1_flash=0;
}
static void UpdF1(Vector2 m){
    if(f1_flash>0) f1_flash-=GetFrameTime();
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for(int i=0;i<5;i++){ if(epi[i].usado) continue;
            if(CheckCollisionPointRec(m,epi[i].btn)){ epi[i].usado=true; f1_equip|=epi[i].bit; f1_vestidos++; f1_flash=0.5f; }
        }
}
static void DrawF1(int estrelas,int chances,float timer){
    DrawQuartelBG();
    int cx=270, cy=395;
    DrawEllipse(cx+15,cy+126,46,10,(Color){0,0,0,70}); DrawRectangle(cx-16,cy+118,62,12,(Color){60,65,80,255});
    DrawClara(cx,cy,true,(int)(GetTime()*6),f1_equip);
    DrawRectangle(20,540,560,38,(Color){0,40,90,220}); DrawRectangleLines(20,540,560,38,(Color){255,200,0,150});
    DBold("Clique para vestir todo o equipamento de proteção (EPI)!", W/2-MeasureText("Clique para vestir todo o equipamento de proteção (EPI)!",16)/2,550,16,(Color){255,230,120,255});
    for(int i=0;i<5;i++){
        Rectangle b=epi[i].btn; Color bg=epi[i].usado?(Color){36,92,46,255}:(Color){0,60,140,255};
        DrawRectangleRounded(b,0.25f,6,bg); DrawRectangleRoundedLines(b,0.25f,6,epi[i].usado?(Color){120,240,130,255}:(Color){255,200,0,255});
        DrawEPIicon(epi[i].bit,(int)(b.x+26),(int)(b.y+28));
        DBold(epi[i].usado?"VESTIDO":epi[i].nome,(int)(b.x+50),(int)(b.y+18),18,WHITE);
    }
    if(f1_flash>0) DBold("+ Equipamento vestido!",W/2-MeasureText("+ Equipamento vestido!",18)/2,584,18,(Color){120,255,120,255});
    DrawHUD(1,estrelas,chances,timer,NULL);
    char p[32]; sprintf(p,"Vestido: %d/5",f1_vestidos); DBold(p,W/2-MeasureText(p,16)/2,512,16,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 2 - CHAMADA DE EMERGENCIA
   ===================================================================== */
static Quiz f2_q[]={
  {"Você vê fumaça saindo de uma casa. O que fazer?",{"Entrar para apagar","Manter distância e chamar adulto","Tirar foto e postar"},1},
  {"Para chamar os Bombeiros, qual número devo ligar?",{"190","192","193"},2},
  {"Qual o endereço da emergência?",{"Rua das Flores, 102","Av. Central, 50","Não sei"},0},
  {"O atendente pergunta: o que aconteceu?",{"Explico que tem fumaça e fogo na casa","Fico em silêncio","Conto uma piada"},0},
  {"Ele pergunta: há vítimas?",{"Minto que não","Informo se tem pessoas no local","Desligo o telefone"},1},
};
#define F2_TOTAL 5
static int f2_passo;
static void InitF2(void){ f2_passo=0; QuizInit(400); }
static void DrawF2(int estrelas,int chances,float timer){
    ClearBackground((Color){120,170,215,255});
    DrawCloud(380,40,1.0f,(Color){255,255,255,180});
    DrawRectangle(0,300,W,H-300,(Color){95,140,80,255}); DrawSidewalk(0,300,W,28);
    /* Casa */
    DrawRectangle(60,140,190,160,(Color){175,150,120,255});
    DrawTriangle((Vector2){48,140},(Vector2){262,140},(Vector2){155,80},(Color){130,50,40,255});
    DrawRectangle(100,180,44,44,(Color){80,100,140,255}); DrawRectangleLines(100,180,44,44,(Color){50,40,30,255});
    DrawRectangle(180,210,46,90,(Color){90,55,30,255}); /* Porta */
    
    /* Placa do número na casa (acima da porta) */
    DrawRectangle(183, 185, 40, 20, (Color){240,240,240,255});
    DrawRectangleLines(183, 185, 40, 20, BLACK);
    DBold("102", 191, 188, 14, BLACK);
    
    DrawFoco(150,150,0.7f); DrawSmokeColumn(150,150,0.7f,1.3f);
    DrawFlowerBed(330,300,120); 
    
    /* Placa de Rua Ajustada */
    DrawRectangle(420, 240, 4, 60, GRAY); 
    DrawRectangle(370, 245, 104, 24, BLUE);
    DBold("RUA DAS FLORES", 376, 252, 10, WHITE);
    
    DrawRectangle(500,250,12,40,(Color){60,160,90,255}); DrawRectangle(501,232,10,16,(Color){255,205,150,255});
    DrawRectangle(512,244,5,9,(Color){40,40,50,255}); DBold("Você",494,292,12,(Color){30,40,30,255});
    DrawRectangle(40,340,520,30,(Color){0,40,90,235});
    DBold("193 Bombeiros   |   192 SAMU   |   190 Polícia", 58,348,16,(Color){255,230,120,255});
    
    int qp2=(f2_passo<F2_TOTAL)?f2_passo:F2_TOTAL-1;
    QuizDraw(f2_q,qp2);
    DrawHUD(2,estrelas,chances,timer,"Escolha a ação correta");
    char p[32]; sprintf(p,"Pergunta %d/%d", (f2_passo<F2_TOTAL)?f2_passo+1:F2_TOTAL, F2_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,660,18,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 3 - PREPARACAO DA VIATURA
   ===================================================================== */
typedef struct { const char*nome; bool certo; bool carregado; int icon; Rectangle btn; } ItemVia;
static ItemVia f3[8];
static int f3_carregados, f3_total_certos; static float f3_flash; static bool f3_flash_ok;
static void InitF3(void){
    const char* nomes[8]={"MANGUEIRA","VIDEOGAME","EXTINTOR","KIT SOCORROS","SKATE","ESCADA","MACHADO","PIPOCA"};
    bool certos[8]={true,false,true,true,false,true,true,false};
    int  icons[8]={IC_MANGUEIRA,IC_VIDEOGAME,IC_EXTINTOR,IC_KIT,IC_SKATE,IC_ESCADA,IC_MACHADO,IC_PIPOCA};
    f3_total_certos=0;
    for(int i=0;i<8;i++){
        f3[i].nome=nomes[i]; f3[i].certo=certos[i]; f3[i].carregado=false; f3[i].icon=icons[i];
        f3[i].btn=(Rectangle){ 24.f+(i%4)*139.f, 470.f+(i/4)*138.f, 130, 124 };
        if(certos[i]) f3_total_certos++;
    }
    f3_carregados=0; f3_flash=0; f3_flash_ok=false;
}
static void UpdF3(Vector2 m, int *chances, Estado *st){
    if(f3_flash>0) f3_flash-=GetFrameTime();
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for(int i=0;i<8;i++){
            if(f3[i].carregado) continue;
            if(CheckCollisionPointRec(m,f3[i].btn)){
                if(f3[i].certo){ f3[i].carregado=true; f3_carregados++; f3_flash_ok=true; }
                else { f3_flash_ok=false; (*chances)--; if(*chances<=0) *st=GAMEOVER; }
                f3_flash=0.7f;
            }
        }
}
static void DrawF3(int estrelas,int chances,float timer){
    ClearBackground((Color){150,195,235,255});
    DrawRectangle(0,360,W,H-360,(Color){92,97,107,255}); DrawRectangle(0,360,W,8,(Color){72,77,87,255});
    int tx=150,ty=120;
    DrawRectangle(tx,ty,360,90,(Color){200,30,30,255}); DrawRectangle(tx,ty+30,360,16,(Color){255,255,255,255});
    DrawRectangle(tx+250,ty-30,110,40,(Color){200,30,30,255}); DrawRectangle(tx+258,ty-26,40,30,(Color){180,220,255,200});
    DrawCircle(tx+70,ty+100,22,(Color){30,30,30,255}); DrawCircle(tx+300,ty+100,22,(Color){30,30,30,255});
    DBold("CBMDF",tx+30,ty+8,30,WHITE);
    DrawRectangle(tx+150,ty-14,60,12,(Color){255,255,0,255});
    DrawSilva(60,300); DBold("Bombeiro Silva",40,360,13,(Color){40,40,60,255});
    DBold("Carregue só os itens da viatura:", W/2-MeasureText("Carregue só os itens da viatura:",17)/2,438,17,WHITE);
    for(int i=0;i<8;i++){
        bool hov=CheckCollisionPointRec(GetMousePosition(),f3[i].btn);
        DrawToolCard(f3[i].btn,f3[i].icon,f3[i].carregado?"CARREGADO":f3[i].nome,f3[i].carregado,hov&&!f3[i].carregado);
    }
    if(f3_flash>0){
        const char*msg=f3_flash_ok?"Item certo carregado!":"Esse item não é da viatura! (-1 VIDA)";
        Color mc=f3_flash_ok?(Color){120,255,120,255}:(Color){255,150,150,255};
        DBold(msg,W/2-MeasureText(msg,17)/2,H-44,17,mc);
    }
    DrawHUD(3,estrelas,chances,timer,NULL);
    char p[40]; sprintf(p,"Carregados: %d/%d",f3_carregados,f3_total_certos); DBold(p,W/2-MeasureText(p,16)/2,250,16,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 4 - DESLOCAMENTO PELA CIDADE
   ===================================================================== */
typedef struct { float x,y; int lane; bool on; Color c; } Carro;
static Carro f4_cars[8];
static float f4_truckx, f4_prog, f4_inv, f4_spawn; static bool f4_sirene;
static const float LANE_X[3]={170,300,430};
static void InitF4(void){
    for(int i=0;i<8;i++) f4_cars[i].on=false;
    f4_truckx=LANE_X[1]; f4_prog=0; f4_sirene=false; f4_inv=0; f4_spawn=0;
}
static void SpawnCarro(void){
    for(int i=0;i<8;i++) if(!f4_cars[i].on){
        int l=GetRandomValue(0,2);
        Color cs[]={(Color){200,200,210,255},(Color){80,120,200,255},(Color){60,160,90,255},(Color){220,180,40,255}};
        f4_cars[i]=(Carro){LANE_X[l],-60.f,l,true,cs[GetRandomValue(0,3)]}; break;
    }
}
static void UpdF4(float dt,int *chances,Estado *st){
    if(f4_inv>0) f4_inv-=dt;
    if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A))  f4_truckx-=4;
    if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) f4_truckx+=4;
    if(f4_truckx<150) f4_truckx=150; if(f4_truckx>450) f4_truckx=450;
    f4_sirene = IsKeyDown(KEY_SPACE);
    f4_prog += dt*6.0f; if(f4_prog>100) f4_prog=100;
    f4_spawn+=dt; if(f4_spawn>0.7f){ f4_spawn=0; SpawnCarro(); }
    float vel = f4_sirene?6.5f:4.5f;
    for(int i=0;i<8;i++){ if(!f4_cars[i].on) continue;
        f4_cars[i].y += vel*60*dt;
        if(f4_sirene && f4_cars[i].y<H-180 && f4_cars[i].y>120){
            if(f4_cars[i].x<W/2) f4_cars[i].x-=2; else f4_cars[i].x+=2;
            if(f4_cars[i].x < 155) f4_cars[i].x = 155;
            if(f4_cars[i].x > 445) f4_cars[i].x = 445;
        }
        if(f4_cars[i].y>H+60) f4_cars[i].on=false;
        if(f4_inv<=0){
            Rectangle rt={f4_truckx-26,(float)(H-150),52,90}; Rectangle rc={f4_cars[i].x-22,f4_cars[i].y-40,44,80};
            if(CheckCollisionRecs(rt,rc)){
                (*chances)--; f4_inv=1.2f; f4_cars[i].on=false;
                for(int p=0;p<10;p++) EmitPart(f4_truckx,H-150);
                if(*chances<=0) *st=GAMEOVER;
            }
        }
    }
    UpdParts();
}
static void DrawCaminhao(int cx,int by,bool sirene){
    DrawRectangle(cx-26,by-90,52,92,(Color){200,30,30,255}); DrawRectangle(cx-26,by-60,52,12,(Color){255,255,255,255});
    DrawRectangle(cx-22,by-86,44,22,(Color){180,220,255,220}); DrawRectangle(cx-16,by-100,32,8,(Color){255,255,0,255});
    DrawCircle(cx-16,by+4,12,(Color){25,25,25,255}); DrawCircle(cx+16,by+4,12,(Color){25,25,25,255});
    if(sirene){
        DrawCircle(cx-8,by-96,5,(int)(GetTime()*10)%2?(Color){255,40,40,255}:(Color){40,40,255,255});
        DrawCircle(cx+8,by-96,5,(int)(GetTime()*10)%2?(Color){40,40,255,255}:(Color){255,40,40,255});
    }
}
static void DrawF4(int estrelas,int chances,float timer){
    ClearBackground((Color){135,185,225,255});
    DrawEsplanada(W/2,150,0.7f); DrawCongresso(300,150,0.55f);
    DrawRectangle(140,0,320,H,(Color){55,55,60,255}); DrawRectangleLines(140,0,320,H,(Color){30,30,35,255});
    int off=(int)(GetTime()*300)%60;
    for(int l=0;l<2;l++){ int lx=235+l*130; for(int y=-60;y<H;y+=60) DrawRectangle(lx-3,y+off,6,30,(Color){230,210,80,255}); }
    DrawSidewalk(120,0,20,H); DrawSidewalk(460,0,20,H);
    for(int i=0;i<8;i++){ if(!f4_cars[i].on) continue;
        DrawRectangle((int)f4_cars[i].x-22,(int)f4_cars[i].y-40,44,80,f4_cars[i].c);
        DrawRectangle((int)f4_cars[i].x-18,(int)f4_cars[i].y-34,36,18,(Color){40,50,70,200});
        DrawRectangle((int)f4_cars[i].x-18,(int)f4_cars[i].y+18,36,16,(Color){40,50,70,200});
    }
    DrawParts();
    if(f4_inv<=0 || ((int)(GetTime()*12))%2) DrawCaminhao((int)f4_truckx,H-58,f4_sirene);
    DrawRectangle(40,70,520,18,(Color){0,0,0,160}); DrawRectangle(42,72,(int)(516*f4_prog/100),14,(Color){60,200,90,255});
    const char*marcos[]={"Quartel","Esplanada","Catedral","Local"};
    for(int i=0;i<4;i++) DBold(marcos[i],42+i*168,92,12,(Color){240,240,240,255});
    DrawHUD(4,estrelas,chances,timer,f4_sirene?"SIRENE LIGADA - corredor aberto!":"SETAS: desviar | ESPACO: sirene");
}

/* =====================================================================
   FASE 5 - INCENDIO RESIDENCIAL
   ===================================================================== */
#define F5_NF 6
static Foco f5_foco[F5_NF];
static int  f5_apag, f5_total; static float f5_cx, f5_ang, f5_espalha;
static void InitF5(void){
    float fx[F5_NF]={175,300,425,175,300,425}; float fy[F5_NF]={225,225,225,335,335,335};
    f5_total=F5_NF; f5_apag=0;
    for(int i=0;i<F5_NF;i++){ f5_foco[i].x=fx[i]; f5_foco[i].y=fy[i]; f5_foco[i].fogo=1.f; }
    f5_cx=W/2-55; f5_ang=-70.f; f5_espalha=0; ResetJatos();
}
static void UpdF5(float dt){
    HoseControls(&f5_cx,&f5_ang); HoseMove(f5_foco,F5_NF,&f5_apag,0.f);
    f5_espalha+=dt; if(f5_espalha>4.f){ f5_espalha=0; for(int f=0;f<F5_NF;f++) if(f5_foco[f].fogo>0 && f5_foco[f].fogo<1.f) f5_foco[f].fogo+=0.05f; }
}
static void DrawF5(int estrelas,int chances,float timer){
    ClearBackground((Color){40,34,46,255});
    DrawCloud(100,55,1.1f,(Color){70,65,72,200}); DrawCloud(420,42,1.3f,(Color){75,70,78,200});
    DrawRectangle(0,470,W,H-470,(Color){70,95,60,255}); DrawSidewalk(0,470,W,30);
    DrawRectangle(20,360,90,110,(Color){120,140,170,255}); DrawTriangle((Vector2){12,360},(Vector2){118,360},(Vector2){65,318},(Color){80,90,120,255});
    DrawRectangle(490,360,90,110,(Color){150,170,140,255}); DrawTriangle((Vector2){482,360},(Vector2){588,360},(Vector2){535,318},(Color){90,110,80,255});
    
    DrawRectangle(110,150,380,330,(Color){150,120,92,255}); DrawTriangle((Vector2){90,150},(Vector2){510,150},(Vector2){300,80},(Color){120,46,34,255});
    for(int r=0;r<2;r++) for(int c=0;c<3;c++){
        int wx=140+c*125, wy=190+r*110; DrawRectangle(wx,wy,70,70,(Color){95,75,58,255});
        DrawRectangleLines(wx,wy,70,70,(Color){55,40,30,255}); DrawLine(wx+35,wy,wx+35,wy+70,(Color){55,40,30,255}); DrawLine(wx,wy+35,wx+70,wy+35,(Color){55,40,30,255});
    }
    DrawRectangle(275,390,50,90,(Color){80,50,30,255});
    DrawWall(110,452,380); DrawFlowerBed(120,500,150); DrawTree(60,520,1.0f);
    DrawParkedCar(360,512,(Color){70,110,170,255}); DrawHydrant(520,500);
    
    DrawAna(160, 480);
    DrawDog(200, 520, 0, (Color){180,130,70,255});
    DrawCat(240, 520);

    for(int f=0;f<F5_NF;f++){ DrawFoco(f5_foco[f].x,f5_foco[f].y,f5_foco[f].fogo); if(f5_foco[f].fogo>0) DrawSmokeColumn(f5_foco[f].x,f5_foco[f].y-10,f5_foco[f].fogo*0.7f,(float)f); }
    DrawJatos(); DrawViaturaMangueira((int)f5_cx,f5_ang);
    DrawHUD(5,estrelas,chances,timer,"SETAS: mover/mirar | ESPACO: água");
    char p[40]; sprintf(p,"Focos apagados: %d/%d",f5_apag,f5_total); DBold(p,W/2-MeasureText(p,17)/2,H-48,17,(Color){120,200,255,255});
}

/* =====================================================================
   FASE 6 - PRIMEIROS SOCORROS
   ===================================================================== */
static Quiz f6_q[]={
  /* Alternativas embaralhadas para variar a posicao correta */
  {"Você vê uma pessoa caída no chão. Devo primeiro:",{"Chamar e ver se ela responde","Sacudir com força","Sair correndo"},0},
  {"Ela não acorda. Para quem devo ligar?",{"190","Para um conhecido","192"},2},
  {"Posso mover a vítima de lugar?",{"Sim, arrasto rápido","Só se houver perigo por perto","Sim, para ela sentar"},1},
  {"Enquanto a ajuda não chega, eu:",{"Dou água para ela","Vou embora","Fico por perto e a observo"},2},
};
#define F6_TOTAL 4
static int f6_passo;
static void InitF6(void){ f6_passo=0; QuizInit(400); }
static void DrawF6(int estrelas,int chances,float timer){
    ClearBackground((Color){150,200,235,255});
    DrawCloud(110,70,1.0f,(Color){255,255,255,210}); DrawCloud(440,52,1.2f,(Color){255,255,255,190});
    DrawCongresso(450, 150, 0.5f);
    DrawRectangle(60,120,150,150,(Color){120,135,160,255});
    for(int r=0;r<3;r++) for(int c=0;c<3;c++) DrawRectangle(78+c*42,134+r*44,28,30,(Color){180,210,235,220});
    
    DrawSidewalk(0,250,W,42); DrawLamppost(110,250); DrawTree(515,250,1.0f); DrawFlowerBed(250,250,150);
    DrawRectangle(150,256,90,16,(Color){70,120,200,255}); DrawRectangle(232,254,16,16,(Color){255,205,150,255});
    DrawAna(280,216); DBold("Bombeira Ana",270,278,12,(Color){40,50,80,255});
    DrawRectangle(40,320,520,32,(Color){0,40,90,255});
    DBold("Emergência médica? Ligue 192 (SAMU)", 52,328,16,(Color){255,230,120,255});
    int qp6=(f6_passo<F6_TOTAL)?f6_passo:F6_TOTAL-1;
    QuizDraw(f6_q,qp6);
    DrawHUD(6,estrelas,chances,timer,"Escolha a ação correta");
    char p[32]; sprintf(p,"Pergunta %d/%d",(f6_passo<F6_TOTAL)?f6_passo+1:F6_TOTAL,F6_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,670,18,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 7 - RESGATE ANIMAL
   ===================================================================== */
static float f7_hx, f7_hy, f7_dx, f7_dy; static int f7_st; static bool f7_got, f7_win; static float f7_aba;
static void InitF7(void){ f7_hx=300; f7_hy=130; f7_st=0; f7_dx=(float)GetRandomValue(150,450); f7_dy=560; f7_got=false; f7_win=false; f7_aba=0; }
static void UpdF7(float dt){
    f7_aba+=dt; if(f7_aba>1.4f) f7_aba=0;
    if(f7_st==0){
        if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A))  f7_hx-=3.5f;
        if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) f7_hx+=3.5f;
        if(f7_hx<80) f7_hx=80; if(f7_hx>520) f7_hx=520;
        if(IsKeyPressed(KEY_SPACE)) f7_st=1;
    } else if(f7_st==1){
        f7_hy+=4; if(f7_hy>=f7_dy){ f7_got=(fabsf(f7_hx-f7_dx)<36); f7_st=2; }
    } else {
        f7_hy-=4; if(f7_got){ f7_dx=f7_hx; f7_dy=f7_hy+44; }
        if(f7_hy<=130){ if(f7_got) f7_win=true; else f7_st=0; }
    }
}
static void DrawF7(int estrelas,int chances,float timer){
    ClearBackground((Color){130,195,240,255});
    DrawCloud(90,70,1.1f,(Color){255,255,255,220}); DrawCloud(360,48,1.3f,(Color){255,255,255,200});
    DrawCongresso(300,150,0.5f);
    DrawRectangle(0,600,W,200,(Color){80,165,75,255}); DrawRectangle(0,600,W,8,(Color){110,195,100,255});
    for(int i=0;i<10;i++) DrawBush(20+i*64,604,0.8f);
    for(int i=0;i<6;i++) DrawTree(40+i*100,540,1.1f);
    
    DrawRectangle(140,560,330,10,(Color){110,80,40,255}); DrawRectangle(150,556,310,4,(Color){140,105,55,255});
    DrawRectangle(60,118,480,8,(Color){90,90,100,255}); DrawRectangle(536,112,8,20,(Color){70,70,80,255});
    DrawRectangle(-20,100,90,40,(Color){200,30,30,255});
    DrawLine((int)f7_hx,126,(int)f7_hx,(int)f7_hy,(Color){60,60,60,255});
    DrawRectangleLines((int)f7_hx-16,(int)f7_hy,32,18,(Color){200,160,60,255}); DrawRectangle((int)f7_hx-16,(int)f7_hy+16,32,3,(Color){200,160,60,255});
    DrawRectangle((int)f7_hx-3,118,6,10,(Color){230,180,60,255});
    
    if(!f7_got) DrawDog((int)f7_dx-13,(int)f7_dy-14,f7_aba>0.7f?1:0, (Color){240,230,210,255});
    else        DrawDog((int)f7_hx-13,(int)f7_hy-2,0, (Color){240,230,210,255});
    DrawHUD(7,estrelas,chances,timer,"SETAS: mover | ESPACO: descer a cesta");
    const char*inf="Alinhe a cesta com o cachorro e desça com calma!"; DBold(inf,W/2-MeasureText(inf,15)/2,H-50,15,(Color){30,40,60,255});
}

/* =====================================================================
   FASE 8 - ACIDENTE DE TRANSITO
   ===================================================================== */
typedef struct { float x,y; bool posto; } Cone;
static Cone f8[6]; static int f8_count;
static void InitF8(void){
    float ang0=0;
    for(int i=0;i<6;i++){ float a=ang0 + i*(2*PI/6); f8[i].x=300+cosf(a)*150; f8[i].y=380+sinf(a)*120; f8[i].posto=false; }
    f8_count=0;
}
static void UpdF8(Vector2 m){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for(int i=0;i<6;i++) if(!f8[i].posto) if(fabsf(m.x-f8[i].x)<34 && fabsf(m.y-f8[i].y)<34){ f8[i].posto=true; f8_count++; break; }
}
static void DrawF8(int estrelas,int chances,float timer){
    ClearBackground((Color){150,200,235,255});
    DrawCloud(120,56,1.0f,(Color){255,255,255,200}); DrawCloud(470,40,1.1f,(Color){255,255,255,185});
    DrawEstadio(300,100,1.3f);
    DBold("Arena BRB Mané Garrincha",300-MeasureText("Arena BRB Mané Garrincha",13)/2,160,13,(Color){235,240,250,255});
    DrawRectangle(0,200,W,470,(Color){60,60,66,255}); DrawRectangle(0,200,W,6,(Color){90,90,96,255});
    for(int y=216;y<660;y+=60) DrawRectangle(W/2-4,y,8,30,(Color){230,210,80,255});
    DrawSidewalk(0,640,W,30); DrawLamppost(70,200); DrawLamppost(530,200);
    
    DrawRectangle(255,330,90,46,(Color){180,60,60,255}); DrawRectangle(265,318,60,16,(Color){120,140,180,220});
    DrawRectangle(250,360,100,8,(Color){120,40,40,255}); DrawCircle(275,380,10,(Color){25,25,25,255}); DrawCircle(325,380,10,(Color){25,25,25,255});
    DrawAna(160, 310);
    DrawCapitao(130, 330);
    DrawParkedCar(100, 220, DARKGREEN); DrawParkedCar(450, 500, BLUE);
    
    DrawSmokeColumn(300,300, 0.8f, 1.0f);
    for(int i=0;i<6;i++){
        if(f8[i].posto) DrawIcon(IC_CONE, (int)f8[i].x, (int)f8[i].y-4, 1.0f, false);
        else DrawCircleLines((int)f8[i].x,(int)f8[i].y,16,(Color){255,255,255,180});
    }
    DrawHUD(8,estrelas,chances,timer,"Clique nos pontos para isolar a área");
    char p[40]; sprintf(p,"Cones colocados: %d/6",f8_count); DBold(p,W/2-MeasureText(p,16)/2,H-50,16,(Color){40,40,60,255});
}

/* =====================================================================
   FASE 9 - TEMPORADA DE QUEIMADAS
   ===================================================================== */
#define F9_NF 9
static Foco f9_foco[F9_NF];
static int  f9_apag, f9_total; static float f9_cx, f9_ang, f9_espalha, f9_wind;
static void InitF9(void){
    f9_total=F9_NF; f9_apag=0;
    for(int i=0;i<F9_NF;i++){
        f9_foco[i].x=80+ (i%3)*180 + GetRandomValue(-20,20);
        f9_foco[i].y=240+ (i/3)*90  + GetRandomValue(-15,15);
        f9_foco[i].fogo=1.f;
    }
    f9_cx=W/2-55; f9_ang=-55.f; f9_espalha=0; f9_wind=GetRandomValue(-6,6)/100.f; ResetJatos();
}
static void UpdF9(float dt){
    HoseControls(&f9_cx,&f9_ang); HoseMove(f9_foco,F9_NF,&f9_apag,f9_wind);
    f9_espalha+=dt; if(f9_espalha>3.0f){ f9_espalha=0; for(int f=0;f<F9_NF;f++) if(f9_foco[f].fogo>0 && f9_foco[f].fogo<1.f) f9_foco[f].fogo+=0.07f; }
}
static void DrawF9(int estrelas,int chances,float timer){
    ClearBackground((Color){175,120,80,255});
    DrawRectangle(0,0,W,180,(Color){190,140,95,255});
    DrawCloud(120,60,1.4f,(Color){150,130,120,200}); DrawCloud(430,90,1.6f,(Color){140,120,110,190});
    DrawRectangle(0,180,W,H-180,(Color){120,100,55,255}); DrawRectangle(0,180,W,10,(Color){135,115,65,255});
    for(int i=0;i<9;i++){
        int tx=20+i*68; DrawRectangle(tx,150,9,44,(Color){80,55,25,255}); DrawCircle(tx+4,144,16,(Color){110,95,45,255}); DrawCircle(tx+12,150,11,(Color){95,80,38,255});
    }
    DrawTree(50, 220, 0.9f); DrawTree(550, 280, 0.9f);
    
    for(int i=0;i<5;i++){
        int wy=470+i*30, wlen=(f9_wind>0)?40:-40;
        DrawLine(W/2+(int)(sinf(GetTime()*3+i)*20),wy,W/2+wlen+(int)(sinf(GetTime()*3+i)*20),wy,(Color){230,220,200,90});
    }
    for(int f=0;f<F9_NF;f++){ DrawFoco(f9_foco[f].x,f9_foco[f].y,f9_foco[f].fogo); if(f9_foco[f].fogo>0) DrawSmokeColumn(f9_foco[f].x,f9_foco[f].y-8,f9_foco[f].fogo*0.6f,(float)f*1.7f); }
    DrawJatos(); DrawViaturaMangueira((int)f9_cx,f9_ang);
    DrawHUD(9,estrelas,chances,timer,"SETAS: mover/mirar | ESPACO: água");
    char p[48]; sprintf(p,"Focos apagados: %d/%d  (cuidado com o vento!)",f9_apag,f9_total); DBold(p,W/2-MeasureText(p,15)/2,H-50,15,(Color){255,235,200,255});
}

/* =====================================================================
   FASE 10 - GRANDE MISSAO EM BRASILIA
   ===================================================================== */
static Quiz f10_q[]={
  {"Para chamar os bombeiros, ligue:",{"190","192","193"},2},
  {"Posso entrar sozinho no incêndio?",{"Sim, sou herói","Não, nunca sozinho","Só se for rápido"},1},
};
#define F10_QTOTAL 2
static ItemVia f10_it[6];
static Foco f10_foco[4];
struct { float x,y; bool salva; } f10_vits[3];
static int f10_etapa, f10_qpasso, f10_load, f10_need, f10_apag, f10_vsalvas;
static float f10_cx, f10_ang, f10_flash; static bool f10_flash_ok;
static void InitF10(void){
    f10_etapa=0; f10_qpasso=0; QuizInit(400);
    const char* nomes[6]={"MANGUEIRA","BOLA","EXTINTOR","VIDEOGAME","DOCE","KIT SOCORROS"};
    bool certos[6]={true,false,true,false,false,true};
    int  icons[6]={IC_MANGUEIRA,IC_BOLA,IC_EXTINTOR,IC_VIDEOGAME,IC_DOCE,IC_KIT};
    f10_need=0;
    for(int i=0;i<6;i++){
        f10_it[i].nome=nomes[i]; f10_it[i].certo=certos[i]; f10_it[i].carregado=false; f10_it[i].icon=icons[i];
        f10_it[i].btn=(Rectangle){ 40.f+(i%3)*180.f, 380.f+(i/3)*150.f, 168, 138 };
        if(certos[i]) f10_need++;
    }
    f10_load=0; f10_flash=0; f10_flash_ok=false;
    float fx[4]={170,300,430,300}; float fy[4]={350,300,350,450};
    for(int i=0;i<4;i++){ f10_foco[i].x=fx[i]; f10_foco[i].y=fy[i]; f10_foco[i].fogo=1.f; }
    f10_vits[0].x=200; f10_vits[0].y=400; f10_vits[0].salva=false;
    f10_vits[1].x=400; f10_vits[1].y=400; f10_vits[1].salva=false;
    f10_vits[2].x=100; f10_vits[2].y=480; f10_vits[2].salva=false;
    f10_vsalvas=0; f10_apag=0; f10_cx=W/2-55; f10_ang=-60.f; ResetJatos();
}
static bool UpdF10(float dt,Vector2 m, int *chances, Estado *st){ 
    if(f10_flash>0) f10_flash-=GetFrameTime();
    if(f10_etapa==0){
        if(QuizUpdate(f10_q,f10_qpasso,m,chances,st)==1){ f10_qpasso++; if(f10_qpasso>=F10_QTOTAL) f10_etapa=1; }
    } else if(f10_etapa==1){
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            for(int i=0;i<6;i++){
                if(f10_it[i].carregado) continue;
                if(CheckCollisionPointRec(m,f10_it[i].btn)){
                    if(f10_it[i].certo){ f10_it[i].carregado=true; f10_load++; f10_flash_ok=true; }
                    else { f10_flash_ok=false; (*chances)--; if(*chances<=0) *st=GAMEOVER; }
                    f10_flash=0.7f;
                }
            }
        if(f10_load>=f10_need){ f10_etapa=2; ResetJatos(); }
    } else {
        HoseControls(&f10_cx,&f10_ang);
        HoseMove(f10_foco,4,&f10_apag,0.f);
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for(int i=0; i<3; i++) {
                if(!f10_vits[i].salva && fabsf(m.x-f10_vits[i].x)<30 && fabsf(m.y-f10_vits[i].y)<30) {
                    f10_vits[i].salva=true; f10_vsalvas++;
                }
            }
        }
        if(f10_apag>=4 && f10_vsalvas>=3) return true;
    }
    return false;
}
static void DrawF10BG(void){
    ClearBackground((Color){135,190,230,255});
    DrawCloud(100,54,1.1f,(Color){255,255,255,210}); DrawCloud(420,38,1.3f,(Color){255,255,255,190});
    DrawCatedral(120,235,0.7f);
    DrawEstadio(W/2,180,0.8f);
    DrawCongresso(480,235,0.85f);
    DrawRectangle(0,235,W,6,(Color){120,130,160,255});
    DBold("BRASÍLIA - Catedral, Estádio e Congresso", W/2-MeasureText("BRASÍLIA - Catedral, Estádio e Congresso",13)/2,214,13,(Color){40,50,80,255});
}
static void DrawF10(int estrelas,int chances,float timer){
    DrawF10BG();
    if(f10_etapa==0){
        DrawRectangle(40,340,520,30,(Color){0,40,90,255}); DBold("Faça a ligação de emergência correta!",52,348,16,(Color){255,230,120,255});
        int qp10=(f10_qpasso<F10_QTOTAL)?f10_qpasso:F10_QTOTAL-1; QuizDraw(f10_q,qp10);
        DrawHUD(10,estrelas,chances,timer,"Etapa 1/3: Ligação de emergência");
    } else if(f10_etapa==1){
        DBold("Escolha os 3 equipamentos certos:", W/2-MeasureText("Escolha os 3 equipamentos certos:",18)/2,348,18,(Color){30,40,70,255});
        for(int i=0;i<6;i++) DrawToolCard(f10_it[i].btn,f10_it[i].icon,f10_it[i].carregado?"OK":f10_it[i].nome,f10_it[i].carregado,false);
        if(f10_flash>0){
            const char*msg=f10_flash_ok?"Equipamento certo!":"Esse não serve! (-1 VIDA)";
            Color mc=f10_flash_ok?(Color){40,150,40,255}:(Color){180,40,40,255};
            DBold(msg,W/2-MeasureText(msg,17)/2,H-78,17,mc);
        }
        DrawHUD(10,estrelas,chances,timer,"Etapa 2/3: Equipamentos");
        char p[24]; sprintf(p,"Certos: %d/%d",f10_load,f10_need); DBold(p,W/2-MeasureText(p,16)/2,H-50,16,(Color){255,220,120,255});
    } else {
        for(int f=0;f<4;f++){ DrawFoco(f10_foco[f].x,f10_foco[f].y,f10_foco[f].fogo); if(f10_foco[f].fogo>0) DrawSmokeColumn(f10_foco[f].x,f10_foco[f].y-8,f10_foco[f].fogo*0.6f,(float)f); }
        for(int v=0;v<3;v++){
            if(!f10_vits[v].salva) {
                int px = f10_vits[v].x; int py = f10_vits[v].y;
                DrawCircle(px, py-10, 8, (Color){255,200,150,255}); /* Cabeça */
                DrawRectangle(px-6, py-2, 12, 16, (Color){200,80,60,255}); /* Tronco */
                DrawRectangle(px-6, py+14, 4, 10, (Color){30,30,40,255}); /* Perna Esq */
                DrawRectangle(px+2, py+14, 4, 10, (Color){30,30,40,255}); /* Perna Dir */
                DrawCircleLines(px, py+5, 25, (Color){255,255,255,(unsigned char)(150+sinf(GetTime()*10)*100)});
            }
        }
        DrawJatos(); DrawViaturaMangueira((int)f10_cx,f10_ang);
        DrawHUD(10,estrelas,chances,timer,"CLIQUE para Resgatar | ESPACO para Apagar");
        char p[50]; sprintf(p,"Focos: %d/4 | Vítimas salvas: %d/3",f10_apag, f10_vsalvas); DBold(p,W/2-MeasureText(p,16)/2,H-50,16,(Color){40,50,80,255});
    }
}

/* =====================================================================
   TEXTOS DO MENTOR E LICOES
   ===================================================================== */
static const char* INTRO[11][3]={
  {"","",""},
  {"Bem-vinda ao quartel, Cadete Clara!","Vamos aprender a vestir o","equipamento de proteção (EPI)."},
  {"Viu fumaça? Não se arrisque!","Mantenha distância, chame um","adulto e ligue para o 193."},
  {"Antes de sair, preparamos a viatura.","Carregue apenas os equipamentos","certos do caminhão."},
  {"O Bombeiro Silva vai dirigir.","Use a sirene para abrir o","corredor de emergência!"},
  {"Chegamos à ocorrência!","Nunca entre sozinho. Use a","mangueira e apague os focos."},
  {"Alguém caiu e não se levanta!","Verifique se ela responde, chame","ajuda (192) e não a movimente."},
  {"Um cachorro ficou preso no parque!","Use o guindaste com calma para","um resgate seguro do animal."},
  {"Acidente perto do estádio!","Não se aproxime das vítimas.","Isole a área com os cones."},
  {"Queimada na área rural!","O fogo se espalha rápido e há","vento. Combata todos os focos."},
  {"A GRANDE MISSÃO chegou, Clara!","Grande operação de salvamento!","Apague o fogo e resgate a população!"},
};
static const char* LICAO[11][2]={
  {"",""},
  {"O EPI protege o bombeiro: capacete,","máscara, cilindro, luvas e botas."},
  {"Em emergência: mantenha distância,","chame um adulto e ligue 193."},
  {"A viatura leva mangueiras, escadas,","extintores e kit de primeiros socorros."},
  {"A sirene abre o corredor de emergência.","Sempre dê passagem para a viatura!"},
  {"Nunca brinque com fogo e nunca","entre sozinho. Mantenha distância!"},
  {"Verifique a consciência, chame o 192","e não mova a vítima sem necessidade."},
  {"Resgate animais com calma e cuidado.","Respeite sempre os animais!"},
  {"Isole a área, não se aproxime e","acione a emergência para proteger todos."},
  {"Queimadas destroem a natureza.","Prevenir e combater protege o ambiente."},
  {"Você dominou todas as missões!","Parabéns, herói do Distrito Federal!"},
};

static void IniciarFase(int fase){
    switch(fase){
        case 1: InitF1(); break;  case 2: InitF2(); break;  case 3: InitF3(); break;
        case 4: InitF4(); break;  case 5: InitF5(); break;  case 6: InitF6(); break;
        case 7: InitF7(); break;  case 8: InitF8(); break;  case 9: InitF9(); break;
        case 10: InitF10(); break;
    }
}

/* =====================================================================
   MUSEU DOS BOMBEIROS
   ===================================================================== */
static const char* MUSEU_SEC[5]={"História do Corpo de Bombeiros","Frota de Veículos","Equipamentos","Curiosidades","Galeria de Medalhas"};
static const char* MUSEU_TXT[4][14]={
  { "O Corpo de Bombeiros Militar do DF (CBMDF)",
    "foi fundado por Dom Pedro II, mas em 1960",
    "mudou-se para a nova capital, Brasília.",
    "Eles acompanharam a construção da cidade",
    "desde o início, protegendo os pioneiros.",
    "",
    "Hoje, é uma corporação moderna que atua",
    "em combate a incêndios, resgate veicular,",
    "salvamento aquático e terrestre,",
    "protegendo toda a população do DF." },
  { "Os bombeiros usaram vários veículos",
    "diferentes ao longo do tempo. Clique em",
    "cada veículo abaixo para conhecer mais",
    "sobre a frota do CBMDF, da moto de apoio",
    "ao caminhão com escada mecânica.",
    "",
    "Uma curiosidade legal é a MR 01, uma moto",
    "usada pelos bombeiros para chegar rápido",
    "em emergências." },
  { "No museu temos diversos equipamentos:",
    "- Mangueira: essencial para o combate.",
    "- Extintor: princípio de incêndio.",
    "- Machado: abrir acessos em resgates.",
    "- Escada: alcançar andares altos.",
    "- Capacete/EPI: proteção máxima.",
    "- Rádio: comunicação rápida da equipe.",
    "- Kit Socorros: atendimento às vítimas." },
  { "- Prevenção: Ter um extintor e conhecer",
    "as saídas de emergência salva vidas.",
    "- Animais: O CBMDF realiza centenas de",
    "resgates de animais silvestres e",
    "domésticos por ano.",
    "- Viaturas Especiais: O caminhão Auto",
    "Escada Mecânica pode chegar a mais",
    "de 50 metros de altura!",
    "- História: A exposição Bravos Heróis",
    "celebra os 50 anos da corporação no DF." },
};

static void DrawMedalha(int x,int y,int num,bool ligada){
    Color c=ligada?(Color){255,200,40,255}:(Color){80,80,70,255};
    DrawCircle(x,y,16,c); DrawCircle(x,y,11,ligada?(Color){255,235,140,255}:(Color){110,110,100,255});
    char n[6]; sprintf(n,"%d",num); DBold(n,x-MeasureText(n,14)/2,y-7,14,(Color){120,70,0,255});
    DrawRectangle(x-7,y+14,5,12,(Color){0,120,60,255}); DrawRectangle(x+2,y+14,5,12,(Color){0,90,170,255});
}

static void DrawRodas(int x1,int x2,int y){
    DrawCircle(x1,y,13,(Color){25,25,25,255}); DrawCircle(x1,y,6,(Color){120,120,120,255});
    DrawCircle(x2,y,13,(Color){25,25,25,255}); DrawCircle(x2,y,6,(Color){120,120,120,255});
}

static void DrawCaminhaoModerno(int x,int y,float s){ (void)s;
    DrawRectangle(x,y,150,42,(Color){205,25,25,255}); DrawRectangle(x,y+30,150,8,(Color){255,255,255,255});
    DrawRectangle(x+110,y-22,40,30,(Color){215,30,30,255}); DrawRectangle(x+116,y-18,26,18,(Color){180,220,255,230});
    DrawRectangle(x+8,y+6,90,18,(Color){170,15,15,255}); for(int i=0;i<4;i++) DrawRectangleLines(x+8+i*22,y+6,22,18,(Color){120,10,10,255});
    DrawRectangle(x+20,y-12,60,12,(Color){150,150,160,255}); 
    
    /* Giroflex (sirene) ajustado para ficar acima da cabine */
    DrawRectangle(x+116,y-30,28,8,(Color){255,230,0,255});
    DrawCircle(x+122,y-26,3,(Color){255,40,40,255}); DrawCircle(x+136,y-26,3,(Color){40,40,255,255});
    
    DBold("CBMDF",x+40,y+12,14,(Color){255,255,255,255}); DrawRodas(x+28,x+118,y+42);
}

static void DrawCaminhaoAntigo(int x,int y){
    DrawRectangle(x,y,90,28,(Color){150,40,30,255}); DrawRectangle(x+90,y-2,34,30,(Color){120,30,20,255});
    DrawRectangle(x+96,y+2,20,14,(Color){200,220,235,200}); DrawRectangle(x+10,y-10,60,12,(Color){90,90,100,255});
    DrawRodas(x+24,x+96,y+30);
}
static void DrawCaminhaoEscada(int x,int y,float s){ (void)s;
    DrawRectangle(x,y,150,40,(Color){205,25,25,255}); DrawRectangle(x,y+28,150,8,(Color){255,255,255,255});
    DrawRectangle(x+112,y-20,38,28,(Color){215,30,30,255}); DrawRectangle(x+118,y-16,24,16,(Color){180,220,255,230});
    DrawRectangle(x+6,y-6,70,10,(Color){150,150,160,255});
    for(int i=0;i<10;i++){ int sx=x+20+i*13, sy=y-6-i*11; DrawLine(sx,sy,sx+13,sy-11,(Color){230,210,60,255}); DrawLine(sx+2,sy+4,sx+9,sy-4,(Color){230,210,60,255}); }
    DrawLine(x+20,y,x+150,y-110,(Color){200,180,40,255}); DBold("CBMDF",x+40,y+12,14,(Color){255,255,255,255}); DrawRodas(x+28,x+118,y+40);
}
static void DrawAmbulancia(int x,int y,float s){ (void)s;
    DrawRectangle(x,y,140,46,(Color){250,250,252,255}); DrawRectangle(x,y+34,140,12,(Color){230,60,40,255});
    DrawRectangle(x,y+20,140,6,(Color){255,200,0,255}); DrawRectangle(x-2,y+6,34,40,(Color){250,250,252,255});
    DrawRectangle(x+2,y+10,26,18,(Color){180,220,255,230}); DrawRectangle(x+86,y+10,30,10,(Color){220,40,40,255});
    DrawRectangle(x+96,y,10,30,(Color){220,40,40,255}); DrawRectangle(x+40,y-12,40,8,(Color){255,230,0,255});
    DrawCircle(x+50,y-8,3,(Color){255,40,40,255}); DrawCircle(x+70,y-8,3,(Color){40,40,255,255}); DrawRodas(x+30,x+110,y+46);
}
static void DrawVeiculoSalvamento(int x,int y,float s){ (void)s;
    DrawRectangle(x,y,150,48,(Color){235,150,20,255}); DrawRectangle(x,y+34,150,8,(Color){40,40,50,255});
    DrawRectangle(x+108,y-20,42,30,(Color){235,150,20,255}); DrawRectangle(x+114,y-16,28,18,(Color){180,220,255,230});
    DrawRectangle(x+8,y+6,94,22,(Color){205,125,10,255}); for(int i=0;i<5;i++) DrawRectangleLines(x+8+i*19,y+6,19,22,(Color){150,90,5,255});
    DrawRectangle(x+20,y-12,50,12,(Color){70,70,80,255}); DrawRectangle(x+4,y-18,30,8,(Color){255,230,0,255});
    DBold("RESGATE",x+30,y+14,13,(Color){40,30,10,255}); DrawRodas(x+28,x+118,y+48);
}
static void DrawMoto(int x,int y,float s){ (void)s;
    DrawCircle(x,y+20,18,(Color){25,25,25,255}); DrawCircle(x,y+20,8,(Color){120,120,120,255});
    DrawCircle(x+70,y+20,18,(Color){25,25,25,255}); DrawCircle(x+70,y+20,8,(Color){120,120,120,255});
    DrawRectangle(x+8,y+2,56,14,(Color){205,25,25,255}); DrawTriangle((Vector2){x+8,y+16},(Vector2){x+36,y+16},(Vector2){x+8,y+2},(Color){205,25,25,255});
    DrawRectangle(x+58,y-6,12,12,(Color){180,15,15,255}); DrawLine(x+64,y,x+78,y-10,(Color){40,40,40,255});
    DrawRectangle(x+18,y-6,22,8,(Color){40,40,50,255}); DrawRectangle(x+10,y-14,10,8,(Color){255,230,0,255});
    DBold("MR-01",x+18,y+4,11,(Color){255,255,255,255});
}
static const char* VEIC_NOME[6]={"Caminhão Moderno (ABT)","Caminhão Antigo","Caminhão Escada (AEM)","Ambulância de Resgate","Veículo de Salvamento","Motocicleta MR-01"};
static const char* VEIC_INFO[6][4]={
  {"Auto Bomba Tanque atual do CBMDF.","Leva água, mangueiras e bombas","potentes para combater incêndios","em casas e prédios de Brasília."},
  {"Viatura histórica dos primeiros","anos do CBMDF. Mostra como eram","os caminhões quando Brasília","estava sendo construída."},
  {"Auto Escada Mecânica. Sua escada","alcança andares altos para","resgatar pessoas e combater fogo","em edifícios elevados."},
  {"Usada em emergências médicas.","Leva a equipe de socorro e","equipamentos para atender vítimas","e levá-las ao hospital."},
  {"Veículo de resgate pesado. Carrega","ferramentas de salvamento para","acidentes, desabamentos e","situações que exigem força."},
  {"A famosa moto de apoio. Ágil e","rápida, chega antes no trânsito","de Brasília para iniciar o","atendimento de emergência."},
};
static void DrawVeiculo(int id,int x,int y,float s){
    switch(id){ case 0: DrawCaminhaoModerno(x,y,s); break; case 1: DrawCaminhaoAntigo(x,y); break; case 2: DrawCaminhaoEscada(x,y,s); break;
        case 3: DrawAmbulancia(x,y,s); break; case 4: DrawVeiculoSalvamento(x,y,s); break; case 5: DrawMoto(x,y,s); break; }
}
static Rectangle VeicCard(int i){ return (Rectangle){ 40.f+(i%2)*270.f, 300.f+(i/2)*120.f, 250, 104 }; }

static void DrawMuseuPagina(int pag,int estrelas,int museuVeic,Vector2 mouse){
    ClearBackground((Color){28,24,40,255});
    DrawRectangle(0,40,W,66,(Color){90,20,10,255}); DrawRectangle(0,43,W,4,(Color){255,200,0,255}); DrawRectangle(0,102,W,4,(Color){255,200,0,255});
    const char*tit="MUSEU DOS BOMBEIROS"; DBold(tit,W/2-MeasureText(tit,26)/2,62,26,(Color){255,230,60,255});

    if(pag==0){
        DBold("Escolha o que deseja conhecer:", W/2-MeasureText("Escolha o que deseja conhecer:",17)/2,140,17,(Color){210,215,235,255});
        for(int i=0;i<5;i++){
            Rectangle b={70,182+i*72,460,58}; bool hov=CheckCollisionPointRec(mouse,b);
            DrawRectangleRounded(b,0.2f,6,hov?(Color){0,90,160,255}:(Color){40,55,90,255});
            DrawRectangleRoundedLines(b,0.2f,6,(Color){255,200,0,255}); DBold(MUSEU_SEC[i],(int)b.x+22,(int)b.y+19,19,WHITE);
        }
        Rectangle bk={175,640,250,48}; bool h2=CheckCollisionPointRec(mouse,bk);
        DrawRectangleRounded(bk,0.3f,8,h2?(Color){120,30,30,255}:(Color){70,20,20,255}); DrawRectangleRoundedLines(bk,0.3f,8,(Color){255,140,60,255});
        DBold("VOLTAR AO MENU",(int)(bk.x+bk.width/2-MeasureText("VOLTAR AO MENU",19)/2),(int)(bk.y+14),19,WHITE);
    } else {
        int s=pag-1; DBold(MUSEU_SEC[s],W/2-MeasureText(MUSEU_SEC[s],22)/2,128,22,(Color){255,205,130,255});
        DrawRectangleLines(40,158,520,520,(Color){80,80,110,255});
        if(s==1){
            if(museuVeic<0){
                DBold("Clique em um veículo para ver os detalhes:", W/2-MeasureText("Clique em um veículo para ver os detalhes:",15)/2,210,15,(Color){210,215,235,255});
                for(int i=0;i<6;i++){
                    Rectangle c=VeicCard(i); bool hov=CheckCollisionPointRec(mouse,c);
                    DrawRectangleRounded(c,0.12f,6,hov?(Color){55,70,110,255}:(Color){38,48,78,255});
                    DrawRectangleRoundedLines(c,0.12f,6,(Color){255,200,0,255}); DrawVeiculo(i,(int)c.x+30,(int)c.y+45,1.0f);
                    DBold(VEIC_NOME[i],(int)c.x+10,(int)c.y+82,13,(Color){235,238,250,255});
                }
            } else {
                DBold(VEIC_NOME[museuVeic],W/2-MeasureText(VEIC_NOME[museuVeic],20)/2,196,20,(Color){255,225,150,255});
                DrawRectangleRounded((Rectangle){90,250,420,150},0.08f,6,(Color){40,48,72,255}); DrawVeiculo(museuVeic,200,360,1.3f);
                for(int i=0;i<4;i++) if(VEIC_INFO[museuVeic][i][0]) DBold(VEIC_INFO[museuVeic][i],70,450+i*36,18,(Color){222,228,242,255});
                Rectangle vb={175,610,250,44}; bool hv=CheckCollisionPointRec(mouse,vb);
                DrawRectangleRounded(vb,0.3f,8,hv?(Color){0,110,160,255}:(Color){0,70,120,255}); DrawRectangleRoundedLines(vb,0.3f,8,(Color){255,200,0,255});
                DBold("VER OUTROS VEÍCULOS",(int)(vb.x+vb.width/2-MeasureText("VER OUTROS VEÍCULOS",16)/2),(int)(vb.y+13),16,WHITE);
            }
        } else if(s<4){
            for(int i=0;i<14;i++) if(MUSEU_TXT[s][i] != NULL && MUSEU_TXT[s][i][0] != '\0') DBold(MUSEU_TXT[s][i],62,188+i*32,18,(Color){222,228,242,255});
            if(s==2){ 
                DrawIcon(IC_MANGUEIRA,120,580,1.0f,false); DrawIcon(IC_EXTINTOR,240,580,1.0f,false); DrawIcon(IC_MACHADO,360,580,1.0f,false); DrawIcon(IC_ESCADA,480,580,1.0f,false);
                DrawIcon(IC_CAPACETE,180,640,1.0f,false); DrawIcon(IC_RADIO,300,640,1.0f,false); DrawIcon(IC_KIT,420,640,1.0f,false);
            }
        } else {
            DBold("Você conquistou todas as 10 medalhas!", W/2-MeasureText("Você conquistou todas as 10 medalhas!",18)/2,180,18,(Color){255,230,140,255});
            for(int i=0;i<10;i++) DrawMedalha(90+(i%5)*105, 270+(i/5)*120, i+1, i<estrelas || estrelas>=10);
            DrawCircle(W/2,540,42,(Color){255,200,40,255}); DrawCircle(W/2,540,30,(Color){255,235,140,255});
            DBold("DF",W/2-MeasureText("DF",24)/2,526,24,(Color){150,90,0,255});
            DrawRectangle(W/2-16,572,12,34,(Color){0,120,60,255}); DrawRectangle(W/2+4,572,12,34,(Color){0,90,170,255});
            DBold("Bombeiro Honorário do DF", W/2-MeasureText("Bombeiro Honorário do DF",18)/2,624,18,(Color){255,205,130,255});
        }
        Rectangle bk={185,710,230,44}; bool h2=CheckCollisionPointRec(mouse,bk);
        DrawRectangleRounded(bk,0.3f,8,h2?(Color){0,120,60,255}:(Color){0,80,40,255}); DrawRectangleRoundedLines(bk,0.3f,8,(Color){120,255,150,255});
        DBold("VOLTAR",(int)(bk.x+bk.width/2-MeasureText("VOLTAR",19)/2),(int)(bk.y+12),19,WHITE);
    }
}

/* =====================================================================
   PROGRAMA PRINCIPAL
   ===================================================================== */
int main(void){
    SetConfigFlags(FLAG_MSAA_4X_HINT); InitWindow(W,H,"Heróis do DF: Missão Bombeiro");
    int mon=GetCurrentMonitor(), mw=GetMonitorWidth(mon), mh=GetMonitorHeight(mon);
    if(mw>0 && mh>0) SetWindowSize(mw,mh); ToggleFullscreen(); SetTargetFPS(60);

    RenderTexture2D target=LoadRenderTexture(W,H); SetTextureFilter(target.texture,TEXTURE_FILTER_POINT);
    Estado st=MENU; int fase=1, estrelas=0, chances=3, animFr=0; float timer=0, animT=0, celebraT=0, rocketT=0;
    bool faseIntro=true, faseConcluida=false, museuLiberado=false; int museuPag=0, museuVeic=-1;

    ResetParts(); ResetRockets(); ResetJatos();

    while(!WindowShouldClose()){
        if(IsKeyPressed(KEY_F11)) ToggleFullscreen();
        float dt=GetFrameTime(); if(dt>0.05f) dt=0.05f; animT+=dt; if(animT>0.09f){ animFr++; animT=0; }

        float scale=fminf((float)GetScreenWidth()/W,(float)GetScreenHeight()/H);
        Vector2 rawm=GetMousePosition();
        Vector2 mouse={ (rawm.x-(GetScreenWidth()-W*scale)*0.5f)/scale, (rawm.y-(GetScreenHeight()-H*scale)*0.5f)/scale };

        /* UPDATE */
        if(st==JOGO){
            if(faseIntro){ if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)||IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER)){ faseIntro=false; timer=0; } }
            else if(faseConcluida){
                celebraT+=dt; rocketT+=dt; if(rocketT>0.35f){ rocketT=0; LaunchRocket(); } UpdFoguetes();
                if(celebraT>0.6f && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)||IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))){
                    faseConcluida=false; celebraT=0; ResetRockets(); fase++;
                    if(fase>10){ st=FIMJOGO; museuLiberado=true; celebraT=0; } else { faseIntro=true; chances=3; IniciarFase(fase); }
                }
            } else {
                timer+=dt;
                switch(fase){
                    case 1: UpdF1(mouse); if(f1_vestidos>=5){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 2: if(QuizUpdate(f2_q,f2_passo,mouse,&chances,&st)==1) f2_passo++; if(f2_passo>=F2_TOTAL){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 3: UpdF3(mouse,&chances,&st); if(f3_carregados>=f3_total_certos){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 4: UpdF4(dt,&chances,&st); if(f4_prog>=100){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 5: UpdF5(dt); if(f5_apag>=f5_total){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 6: if(QuizUpdate(f6_q,f6_passo,mouse,&chances,&st)==1) f6_passo++; if(f6_passo>=F6_TOTAL){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 7: UpdF7(dt); if(f7_win){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 8: UpdF8(mouse); if(f8_count>=6){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 9: UpdF9(dt); if(f9_apag>=f9_total){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 10: if(UpdF10(dt,mouse,&chances,&st)){ faseConcluida=true; estrelas++; celebraT=0; } break;
                }
            }
        }

        /* DRAW */
        BeginTextureMode(target);
        if(st==MENU){
            ClearBackground((Color){15,18,40,255}); DrawSkylineBrasilia(150,0.7f);
            for(int i=0;i<8;i++){ float fx=40.f+i*75,fy=(float)(H-50)+sinf(GetTime()*2+i)*8; DrawCircle((int)fx,(int)fy,26,(Color){220,50,0,200}); DrawCircle((int)fx,(int)(fy-20),15,(Color){255,140,0,180}); }
            DrawRectangle(0,H-60,W,60,(Color){20,6,0,220}); DrawRectangle(0,70,W,86,(Color){0,40,120,235}); DrawRectangle(0,73,W,4,(Color){255,200,0,255}); DrawRectangle(0,152,W,4,(Color){255,200,0,255});
            const char*t1="HERÓIS DO DF"; DBold(t1,W/2-MeasureText(t1,40)/2+2,86,40,(Color){0,0,0,170}); DBold(t1,W/2-MeasureText(t1,40)/2,84,40,(Color){255,230,50,255});
            const char*t2="MISSÃO BOMBEIRO"; DBold(t2,W/2-MeasureText(t2,22)/2,128,22,(Color){255,170,90,255});
            DBold("193 Bombeiros   192 SAMU   190 Polícia", W/2-MeasureText("193 Bombeiros   192 SAMU   190 Polícia",16)/2,180,16,(Color){255,230,120,255});
            DrawCapitao(70,260); DBold("Cap. Almeida",48,316,12,WHITE); DrawClara(190,260,true,(int)(GetTime()*6),EQ_TUDO); DBold("Cadete Clara",168,316,12,WHITE);
            DrawAna(330,260); DBold("Bombeira Ana",308,316,12,WHITE); DrawSilva(450,260); DBold("Bombeiro Silva",430,316,12,WHITE);
            const char* col1[]={"1- Quartel","2- Emergência","3- Viatura","4- Deslocamento","5- Incêndio"}; const char* col2[]={"6- Prim. Socorros","7- Resgate Animal","8- Acidente","9- Queimadas","10- Grande Missão"};
            for(int i=0;i<5;i++){ DBold(col1[i],60,360+i*26,16,(Color){200,200,225,225}); DBold(col2[i],320,360+i*26,15,(Color){200,200,225,225}); }
            Rectangle bp={170,500,260,52}; bool hov=CheckCollisionPointRec(mouse,bp);
            DrawRectangleRounded(bp,0.3f,8,hov?(Color){0,150,40,255}:(Color){0,100,30,255}); DrawRectangleRoundedLines(bp,0.3f,8,(Color){255,200,0,255});
            const char*bt="COMECAR FASE 1"; DBold(bt,(int)(bp.x+bp.width/2-MeasureText(bt,20)/2),(int)(bp.y+16),20,WHITE);
            Rectangle bmu={170,562,260,48}; bool hmu=CheckCollisionPointRec(mouse,bmu);
            if(museuLiberado){
                DrawRectangleRounded(bmu,0.3f,8,hmu?(Color){150,40,20,255}:(Color){100,30,15,255}); DrawRectangleRoundedLines(bmu,0.3f,8,(Color){255,200,0,255});
                const char*bm="MUSEU DOS BOMBEIROS"; DBold(bm,(int)(bmu.x+bmu.width/2-MeasureText(bm,17)/2),(int)(bmu.y+15),17,WHITE);
            } else {
                DrawRectangleRounded(bmu,0.3f,8,(Color){45,45,55,255}); DrawRectangleRoundedLines(bmu,0.3f,8,(Color){90,90,110,255});
                int lx=(int)bmu.x+40, ly=(int)bmu.y+14; DrawRectangle(lx,ly+8,18,14,(Color){180,180,190,255}); DrawRectangleLines(lx,ly+8,18,14,(Color){90,90,100,255});
                DrawCircleLines(lx+9,ly+8,7,(Color){180,180,190,255}); DrawRectangle(lx+7,ly+13,4,5,(Color){90,90,100,255});
                const char*bm="MUSEU (BLOQUEADO)"; DBold(bm,(int)(bmu.x+bmu.width/2-MeasureText(bm,16)/2)+12,(int)(bmu.y+16),16,(Color){150,150,170,255});
            }
            DBold("Termine as 10 fases para virar Bombeiro Honorário!", W/2-MeasureText("Termine as 10 fases para virar Bombeiro Honorário!",14)/2,628,14,(Color){180,190,220,220});
            DBold("F11: janela/tela cheia   ESC: sair", W/2-MeasureText("F11: janela/tela cheia   ESC: sair",13)/2,650,13,(Color){140,150,180,200});
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ fase=1; estrelas=0; chances=3; faseIntro=true; faseConcluida=false; timer=0; IniciarFase(1); st=JOGO; }
            if(museuLiberado && hmu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ museuPag=0; museuVeic=-1; st=MUSEU; }
        } else if(st==JOGO){
            if(faseIntro){ DrawIntro(fase,INTRO[fase][0],INTRO[fase][1],INTRO[fase][2]); }
            else {
                switch(fase){ case 1: DrawF1(estrelas,chances,timer); break; case 2: DrawF2(estrelas,chances,timer); break; case 3: DrawF3(estrelas,chances,timer); break; case 4: DrawF4(estrelas,chances,timer); break; case 5: DrawF5(estrelas,chances,timer); break; case 6: DrawF6(estrelas,chances,timer); break; case 7: DrawF7(estrelas,chances,timer); break; case 8: DrawF8(estrelas,chances,timer); break; case 9: DrawF9(estrelas,chances,timer); break; case 10: DrawF10(estrelas,chances,timer); break; }
                if(faseConcluida){ DrawFoguetes(); DrawFaseConcluida(fase,LICAO[fase][0],LICAO[fase][1]); }
            }
        } else if(st==FIMJOGO){
            ClearBackground((Color){8,16,45,255}); celebraT+=dt; rocketT+=dt; if(rocketT>0.35f){ rocketT=0; LaunchRocket(); } UpdFoguetes(); DrawFoguetes();
            DrawCircle(W/2,180,46,(Color){255,200,40,255}); DrawCircle(W/2,180,34,(Color){255,230,120,255}); DBold("DF",W/2-MeasureText("DF",26)/2,166,26,(Color){150,90,0,255});
            DrawRectangle(W/2-18,210,12,40,(Color){0,120,60,255}); DrawRectangle(W/2+6,210,12,40,(Color){0,90,170,255});
            DrawRectangle(0,H/2-140,W,270,(Color){20,0,80,245}); DrawRectangle(0,H/2-144,W,8,(Color){255,195,0,255}); DrawRectangle(0,H/2+122,W,8,(Color){255,195,0,255});
            const char*vt="VOCÊ SE TORNOU UM"; DBold(vt,W/2-MeasureText(vt,20)/2,H/2-126,20,WHITE);
            const char*v1="BOMBEIRO HONORÁRIO DO DF!"; DBold(v1,W/2-MeasureText(v1,24)/2+2,H/2-98,24,(Color){0,0,0,160}); DBold(v1,W/2-MeasureText(v1,24)/2,H/2-100,24,(Color){255,230,50,255});
            const char*v2="Parabéns, Cadete Clara!"; DBold(v2,W/2-MeasureText(v2,19)/2,H/2-64,19,(Color){255,205,130,255});
            const char*v3="Você completou as 10 fases e aprendeu"; const char*v4="a agir corretamente em emergências.";
            DBold(v3,W/2-MeasureText(v3,15)/2,H/2-34,15,(Color){200,210,255,255}); DBold(v4,W/2-MeasureText(v4,15)/2,H/2-14,15,(Color){200,210,255,255});
            char ve[40]; sprintf(ve,"Estrelas de Segurança: %d/10",estrelas); DBold(ve,W/2-MeasureText(ve,18)/2,H/2+12,18,(Color){255,210,60,255});
            const char*v5="O Museu dos Bombeiros foi desbloqueado!"; DBold(v5,W/2-MeasureText(v5,15)/2,H/2+42,15,(Color){255,200,120,255});
            const char*v6="Lembre-se: em emergências, ligue 193!"; DBold(v6,W/2-MeasureText(v6,15)/2,H/2+64,15,(Color){255,200,120,255});
            Rectangle br={175,H/2+150,250,46}; bool hov=CheckCollisionPointRec(mouse,br);
            DrawRectangleRounded(br,0.3f,8,hov?(Color){150,40,20,255}:(Color){100,30,15,255}); DrawRectangleRoundedLines(br,0.3f,8,(Color){255,200,0,255}); DBold("VISITAR O MUSEU",(int)(br.x+br.width/2-MeasureText("VISITAR O MUSEU",19)/2),(int)(br.y+13),19,WHITE);
            Rectangle br2={175,H/2+206,250,46}; bool hov2=CheckCollisionPointRec(mouse,br2);
            DrawRectangleRounded(br2,0.3f,8,hov2?(Color){0,155,0,255}:(Color){0,95,0,255}); DrawRectangleRoundedLines(br2,0.3f,8,(Color){90,255,90,255}); DBold("VOLTAR AO MENU",(int)(br2.x+br2.width/2-MeasureText("VOLTAR AO MENU",19)/2),(int)(br2.y+13),19,WHITE);
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ museuPag=0; museuVeic=-1; st=MUSEU; ResetRockets(); }
            if(hov2&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ st=MENU; ResetRockets(); }
        } else if(st==MUSEU){
            DrawMuseuPagina(museuPag,estrelas,museuVeic,mouse);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(museuPag==0){
                    for(int i=0;i<5;i++){ Rectangle b={70,182+i*72,460,58}; if(CheckCollisionPointRec(mouse,b)){ museuPag=i+1; museuVeic=-1; break; } }
                    Rectangle bk={175,640,250,48}; if(CheckCollisionPointRec(mouse,bk)) st=MENU;
                } else if(museuPag==2 && museuVeic<0){
                    for(int i=0;i<6;i++){ Rectangle c=VeicCard(i); if(CheckCollisionPointRec(mouse,c)){ museuVeic=i; break; } }
                    Rectangle bk={185,710,230,44}; if(CheckCollisionPointRec(mouse,bk)) museuPag=0;
                } else if(museuPag==2 && museuVeic>=0){
                    Rectangle vb={175,610,250,44}; if(CheckCollisionPointRec(mouse,vb)) museuVeic=-1;
                    Rectangle bk={185,710,230,44}; if(CheckCollisionPointRec(mouse,bk)) museuPag=0;
                } else { Rectangle bk={185,710,230,44}; if(CheckCollisionPointRec(mouse,bk)) museuPag=0; }
            }
        } else { /* GAMEOVER */
            ClearBackground((Color){12,6,6,255});
            for(int i=0;i<8;i++){ float fx=40+i*72,fy=(float)(H-45)+sinf(GetTime()*1.5f+i)*7; DrawCircle((int)fx,(int)fy,28,(Color){200,35,0,185}); DrawCircle((int)fx,(int)(fy-20),16,(Color){255,110,0,165}); }
            DrawRectangle(0,180,W,100,(Color){140,0,0,230}); DrawRectangle(0,183,W,4,(Color){255,90,0,255}); DrawRectangle(0,276,W,4,(Color){255,90,0,255});
            char go[40]; sprintf(go,"OPS! TENTE DE NOVO - FASE %d",fase); DBold(go,W/2-MeasureText(go,24)/2,210,24,(Color){255,75,45,255});
            const char*sub="Você perdeu todas as vidas!"; DBold(sub,W/2-MeasureText(sub,17)/2,320,17,(Color){200,145,95,255});
            Rectangle bt2={175,400,250,52}; bool hov=CheckCollisionPointRec(mouse,bt2);
            DrawRectangleRounded(bt2,0.3f,8,hov?(Color){195,45,0,255}:(Color){120,25,0,255}); DrawRectangleRoundedLines(bt2,0.3f,8,(Color){255,140,45,255}); DBold("TENTAR NOVAMENTE",(int)(bt2.x+bt2.width/2-MeasureText("TENTAR NOVAMENTE",19)/2),(int)(bt2.y+16),19,WHITE);
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ chances=3; timer=0; faseIntro=true; faseConcluida=false; IniciarFase(fase); st=JOGO; ResetParts(); }
            Rectangle bm={175,470,250,48}; hov=CheckCollisionPointRec(mouse,bm);
            DrawRectangleRounded(bm,0.3f,8,hov?(Color){55,55,85,255}:(Color){28,28,55,255}); DrawRectangleRoundedLines(bm,0.3f,8,(Color){140,140,195,255}); DBold("MENU PRINCIPAL",(int)(bm.x+bm.width/2-MeasureText("MENU PRINCIPAL",19)/2),(int)(bm.y+14),19,LIGHTGRAY);
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ st=MENU; ResetRockets(); }
        }
        EndTextureMode();

        BeginDrawing(); ClearBackground(BLACK);
        Rectangle src={0,0,(float)W,-(float)H}; Rectangle dst={(GetScreenWidth()-W*scale)*0.5f,(GetScreenHeight()-H*scale)*0.5f,W*scale,H*scale};
        DrawTexturePro(target.texture,src,dst,(Vector2){0,0},0,WHITE);
        EndDrawing();
    }
    UnloadRenderTexture(target); CloseWindow(); return 0;
}