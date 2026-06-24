/* =====================================================================
   HEROIS DO DF: MISSAO BOMBEIRO  -  Fases 1 a 10  (TELA CHEIA)
   ---------------------------------------------------------------------
   Jogo educativo (6-12 anos) sobre o Corpo de Bombeiros Militar do DF.
   Estilo visual (pixel-art de retangulos), HUD, telas de conclusao e
   sistema de particulas/foguetes herdados do prototipo do grupo.

   O jogo roda em uma resolucao virtual de 600x800 (vertical) e e
   escalado para a tela cheia mantendo a proporcao (barras laterais),
   com o mouse ajustado. F11 alterna janela/tela cheia. ESC sai.

   Personagens: Capitao Almeida (mentor), Cadete Clara (jogadora),
   Bombeira Ana (resgates), Bombeiro Silva (motorista).

   Compilar (Linux):
     gcc HeroisDoDF_Bombeiro.c -o herois -lraylib -lm -lpthread -ldl -lrt -lX11
   Compilar (Windows / MinGW):
     gcc HeroisDoDF_Bombeiro.c -o herois.exe -lraylib -lopengl32 -lgdi32 -lwinmm
   ===================================================================== */

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define W 600          /* largura virtual  */
#define H 800          /* altura  virtual  */

#define MP   160       /* particulas        */
#define MF   24        /* foguetes          */
#define ME   500       /* estilhacos        */
#define MJT  24        /* jatos de agua     */

/* =====================================================================
   ESTADOS
   ===================================================================== */
typedef enum { MENU, JOGO, GAMEOVER, FIMJOGO, MUSEU } Estado;
/* Fases sao um inteiro 1..10. */

/* =====================================================================
   ESTRUTURAS GENERICAS
   ===================================================================== */
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

/* Texto fino e legivel: uma sombra escura leve para dar contraste
   (sem engrossar a letra) e a fonte por cima, fina e nitida. */
static void DBold(const char*t,int x,int y,int sz,Color c){
    DrawText(t,x+1,y+2,sz,(Color){0,0,0,150});
    DrawText(t,x,  y,  sz,c);
}

/* =====================================================================
   PARTICULAS / FOGUETES (celebracao)
   ===================================================================== */
static void ResetParts(void){ for(int i=0;i<MP;i++) pts[i].on=false; }
static void ResetRockets(void){ for(int i=0;i<MF;i++) fogs[i].on=false;
                                for(int i=0;i<ME;i++) est[i].on=false; }
static void ResetJatos(void){ for(int i=0;i<MJT;i++) jts[i].on=false; }

static void EmitPart(float x,float y){
    for(int i=0;i<MP;i++) if(!pts[i].on){
        pts[i]=(Part){ x+(float)GetRandomValue(-40,40), y,
            GetRandomValue(-15,15)/10.f, -(float)GetRandomValue(3,9),
            1.f, (float)GetRandomValue(6,20),
            GetRandomValue(0,1)?(Color){255,(unsigned char)GetRandomValue(80,180),0,255}
                               :(Color){120,120,120,255}, true };
        break;
    }
}
static void UpdParts(void){
    for(int i=0;i<MP;i++){ if(!pts[i].on) continue;
        pts[i].x+=pts[i].vx; pts[i].y+=pts[i].vy; pts[i].a-=0.014f;
        if(pts[i].a<=0) pts[i].on=false;
    }
}
static void DrawParts(void){
    for(int i=0;i<MP;i++){ if(!pts[i].on) continue;
        Color c=pts[i].c; c.a=(unsigned char)(pts[i].a*200);
        DrawCircle((int)pts[i].x,(int)pts[i].y,(int)pts[i].r,c);
    }
}
static void Explode(int k){
    Color cor=fogs[k].c;
    for(int j=0;j<40;j++) for(int i=0;i<ME;i++) if(!est[i].on){
        float ang=GetRandomValue(0,628)/100.f, v=GetRandomValue(15,80)/10.f;
        est[i]=(Estil){fogs[k].x,fogs[k].y,cosf(ang)*v,sinf(ang)*v,1.f,cor,true};
        break;
    }
    fogs[k].on=false;
}
static void LaunchRocket(void){
    Color cores[]={RED,YELLOW,BLUE,GREEN,MAGENTA,ORANGE,SKYBLUE,WHITE};
    for(int i=0;i<MF;i++) if(!fogs[i].on){
        fogs[i]=(Foguete){(float)GetRandomValue(80,W-80),(float)H,
            GetRandomValue(-20,20)/10.f, -(float)GetRandomValue(12,18),0,
            cores[GetRandomValue(0,7)],true};
        break;
    }
}
static void UpdFoguetes(void){
    for(int i=0;i<MF;i++){ if(!fogs[i].on) continue;
        fogs[i].x+=fogs[i].vx; fogs[i].y+=fogs[i].vy; fogs[i].vy+=0.18f;
        fogs[i].t+=GetFrameTime();
        if(fogs[i].vy>=0||fogs[i].t>2.5f) Explode(i);
    }
    for(int i=0;i<ME;i++){ if(!est[i].on) continue;
        est[i].x+=est[i].vx; est[i].y+=est[i].vy; est[i].vy+=0.07f;
        est[i].vx*=0.98f; est[i].a-=0.016f;
        if(est[i].a<=0) est[i].on=false;
    }
}
static void DrawFoguetes(void){
    for(int i=0;i<MF;i++){ if(!fogs[i].on) continue; DrawCircle((int)fogs[i].x,(int)fogs[i].y,4,fogs[i].c); }
    for(int i=0;i<ME;i++){ if(!est[i].on) continue;
        Color c=est[i].c; c.a=(unsigned char)(est[i].a*255);
        DrawCircle((int)est[i].x,(int)est[i].y,2,c);
    }
}

/* =====================================================================
   PERSONAGENS (pixel-art de retangulos)
   ===================================================================== */
#define EQ_BOTAS    1
#define EQ_CILINDRO 2
#define EQ_MASCARA  4
#define EQ_CAPACETE 8
#define EQ_LUVAS    16
#define EQ_TUDO     (EQ_BOTAS|EQ_CILINDRO|EQ_MASCARA|EQ_CAPACETE|EQ_LUVAS)

static void DrawClara(int x,int y,bool dir,int fr,int gear){
    int po=(fr%4<2)?3:-3;
    if(gear&EQ_CILINDRO){
        DrawRectangle(x+9,y+10,12,20,(Color){200,200,210,255});
        DrawRectangle(x+9,y+10,12,4,(Color){90,90,100,255});
    }
    DrawRectangle(x+6,y+28,8,16+po,(Color){30,40,160,255});
    DrawRectangle(x+16,y+28,8,16-po,(Color){30,40,160,255});
    if(gear&EQ_BOTAS){
        DrawRectangle(x+4,y+42+po,11,8,(Color){25,25,25,255});
        DrawRectangle(x+15,y+42-po,11,8,(Color){25,25,25,255});
        DrawRectangle(x+4,y+48+po,11,2,(Color){200,200,0,255});
    } else {
        DrawRectangle(x+5,y+44+po,9,5,(Color){60,40,20,255});
        DrawRectangle(x+16,y+44-po,9,5,(Color){60,40,20,255});
    }
    DrawRectangle(x+5,y+12,20,18,(Color){60,110,210,255});
    DrawRectangle(x+5,y+17,20,3,(Color){255,220,0,255});
    DrawRectangle(x+1,y+13+po,5,12,(Color){60,110,210,255});
    DrawRectangle(x+24,y+13-po,5,12,(Color){60,110,210,255});
    if(gear&EQ_LUVAS){
        DrawRectangle(x+1,y+23+po,6,6,(Color){200,60,0,255});
        DrawRectangle(x+24,y+23-po,6,6,(Color){200,60,0,255});
    } else {
        DrawRectangle(x+1,y+24+po,5,5,(Color){255,205,150,255});
        DrawRectangle(x+24,y+24-po,5,5,(Color){255,205,150,255});
    }
    DrawRectangle(x+7,y,16,12,(Color){255,205,150,255});
    DrawRectangle(dir?x-2:x+23,y+2,4,9,(Color){90,55,20,255});
    if(gear&EQ_MASCARA){
        DrawRectangle(x+7,y+4,16,8,(Color){70,70,80,255});
        DrawRectangle(x+10,y+5,10,5,(Color){160,200,230,255});
    } else {
        DrawRectangle(dir?x+15:x+8,y+3,3,3,(Color){20,20,20,255});
    }
    if(gear&EQ_CAPACETE){
        DrawRectangle(x+6,y-2,18,4,(Color){255,170,0,255});
        DrawRectangle(x+8,y-9,14,8,(Color){220,40,0,255});
        DrawRectangle(x+10,y-12,10,4,(Color){180,20,0,255});
    } else {
        DrawRectangle(x+7,y-2,16,3,(Color){90,55,20,255});
    }
}
static void DrawCapitao(int x,int y){
    DrawRectangle(x+5,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+16,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+6,y+28,8,18,(Color){20,20,30,255});
    DrawRectangle(x+16,y+28,8,18,(Color){20,20,30,255});
    DrawRectangle(x+4,y+12,22,18,(Color){200,40,30,255});
    DrawRectangle(x+4,y+12,22,4,(Color){255,210,0,255});
    DrawRectangle(x+13,y+16,4,14,(Color){255,210,0,255});
    DrawRectangle(x+1,y+13,5,14,(Color){200,40,30,255});
    DrawRectangle(x+24,y+13,5,14,(Color){200,40,30,255});
    DrawRectangle(x+7,y,16,12,(Color){255,210,160,255});
    DrawRectangle(x+8,y+8,14,3,(Color){120,90,60,255});
    DrawRectangle(x+10,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+17,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+6,y-3,18,5,(Color){240,240,240,255});
    DrawRectangle(x+8,y-8,14,6,(Color){240,240,240,255});
    DrawRectangle(x+12,y-7,6,4,(Color){200,40,30,255});
}
static void DrawAna(int x,int y){
    DrawRectangle(x+5,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+16,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+6,y+28,8,18,(Color){40,40,50,255});
    DrawRectangle(x+16,y+28,8,18,(Color){40,40,50,255});
    DrawRectangle(x+5,y+12,20,18,(Color){230,120,20,255});
    DrawRectangle(x+5,y+17,20,3,(Color){255,255,255,255});
    DrawRectangle(x+1,y+13,5,14,(Color){230,120,20,255});
    DrawRectangle(x+24,y+13,5,14,(Color){230,120,20,255});
    DrawCircleLines(x+9,y+16,6,(Color){240,230,180,255});
    DrawCircleLines(x+9,y+16,3,(Color){240,230,180,255});
    DrawRectangle(x+7,y,16,12,(Color){240,195,150,255});
    DrawRectangle(x+5,y+1,4,11,(Color){40,30,20,255});
    DrawRectangle(x+21,y+1,4,11,(Color){40,30,20,255});
    DrawRectangle(x+11,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+17,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+6,y-2,18,4,(Color){255,170,0,255});
    DrawRectangle(x+8,y-8,14,7,(Color){230,120,20,255});
}
static void DrawSilva(int x,int y){
    DrawRectangle(x+5,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+16,y+44,9,7,(Color){25,25,25,255});
    DrawRectangle(x+6,y+28,8,18,(Color){30,30,40,255});
    DrawRectangle(x+16,y+28,8,18,(Color){30,30,40,255});
    DrawRectangle(x+5,y+12,20,18,(Color){30,30,200,255});
    DrawRectangle(x+5,y+17,20,3,(Color){255,220,0,255});
    DrawRectangle(x+1,y+13,5,14,(Color){30,30,200,255});
    DrawRectangle(x+24,y+13,5,14,(Color){30,30,200,255});
    DrawCircleLines(x+27,y+18,6,(Color){60,60,70,255});
    DrawRectangle(x+7,y,16,12,(Color){235,185,140,255});
    DrawRectangle(x+9,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+16,y+4,3,3,(Color){20,20,20,255});
    DrawRectangle(x+6,y-2,18,4,(Color){50,50,60,255});
    DrawRectangle(x+8,y-7,14,5,(Color){50,50,60,255});
    DrawRectangle(x+12,y-6,6,4,(Color){255,170,0,255});
}
/* cachorrinho da Fase 7 */
static void DrawDog(int x,int y,float aba){
    int wag=(int)(sinf(GetTime()*8)*4);
    DrawRectangle(x,y,26,14,(Color){180,130,70,255});      /* corpo */
    DrawRectangle(x-8,y-8,14,14,(Color){180,130,70,255});  /* cabeca */
    DrawRectangle(x-10,y-10,6,8,(Color){120,80,40,255});   /* orelha */
    DrawRectangle(x-7,y-4,3,3,(Color){20,20,20,255});      /* olho */
    DrawRectangle(x-10,y,4,3,(Color){20,20,20,255});       /* focinho */
    DrawRectangle(x+24,y+2,8+wag,4,(Color){180,130,70,255});/* rabo */
    DrawRectangle(x+2,y+14,5,8,(Color){150,100,55,255});
    DrawRectangle(x+18,y+14,5,8,(Color){150,100,55,255});
    if(aba>0){ const char*au="AU!"; DBold(au,x-6,y-26,14,(Color){255,255,120,255}); }
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
    if(info&&info[0])
        DBold(info,W/2-MeasureText(info,15)/2,H-26,15,(Color){210,210,210,220});
}
static void DrawFaseConcluida(int fase,const char*l1,const char*l2){
    DrawRectangle(0,H/2-140,W,250,(Color){170,0,0,245});
    DrawRectangle(0,H/2-144,W,8,(Color){255,195,0,255});
    DrawRectangle(0,H/2+102,W,8,(Color){255,195,0,255});
    char m1[64]; sprintf(m1,"FASE %d CONCLUÍDA!",fase);
    DBold(m1,W/2-MeasureText(m1,30)/2+2,H/2-122,30,(Color){0,0,0,160});
    DBold(m1,W/2-MeasureText(m1,30)/2,H/2-124,30,(Color){255,230,50,255});
    int sx=W/2, sy=H/2-78;
    DrawTriangle((Vector2){sx,sy-14},(Vector2){sx-12,sy+10},(Vector2){sx+12,sy+10},(Color){255,210,40,255});
    DrawTriangle((Vector2){sx,sy+16},(Vector2){sx-12,sy-8},(Vector2){sx+12,sy-8},(Color){255,210,40,255});
    DBold("VOCÊ APRENDEU:",W/2-MeasureText("VOCÊ APRENDEU:",20)/2,H/2-44,20,WHITE);
    if(l1[0]) DBold(l1,W/2-MeasureText(l1,18)/2,H/2-14,18,(Color){200,230,255,255});
    if(l2[0]) DBold(l2,W/2-MeasureText(l2,18)/2,H/2+10,18,(Color){200,230,255,255});
    const char*cont="Clique ou ESPACO para continuar";
    DBold(cont,W/2-MeasureText(cont,16)/2,H/2+58,16,(Color){255,255,180,255});
}
static void DrawIntro(int fase,const char*l1,const char*l2,const char*l3){
    ClearBackground((Color){18,22,40,255});
    DrawRectangle(0,90,W,70,(Color){0,40,120,235});
    DrawRectangle(0,93,W,4,(Color){255,200,0,255});
    DrawRectangle(0,156,W,4,(Color){255,200,0,255});
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
   QUIZ GENERICO (usado nas Fases 2, 6 e 10)
   ===================================================================== */
static Rectangle QZ_BTN[3];
static int   qz_sel;
static float qz_flash;
static bool  qz_ok;
static void QuizInit(void){
    qz_sel=-1; qz_flash=0; qz_ok=false;
    for(int i=0;i<3;i++) QZ_BTN[i]=(Rectangle){40.f,360.f+i*90.f,520,72};
}
/* retorna 1 quando o jogador acerta e o feedback termina (avancar) */
static int QuizUpdate(Quiz*set,int passo,Vector2 m){
    if(qz_flash>0){
        qz_flash-=GetFrameTime();
        if(qz_flash<=0 && qz_ok){ qz_ok=false; return 1; }
        return 0;
    }
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for(int i=0;i<3;i++) if(CheckCollisionPointRec(m,QZ_BTN[i])){
            qz_sel=i; qz_ok=(i==set[passo].correta); qz_flash=qz_ok?0.8f:1.0f;
        }
    return 0;
}
static void QuizDraw(Quiz*set,int passo){
    DBold(set[passo].pergunta,40,338,19,WHITE);
    for(int i=0;i<3;i++){
        Color bc={0,70,150,255};
        if(qz_flash>0 && i==qz_sel) bc=qz_ok?(Color){30,150,40,255}:(Color){170,30,30,255};
        DrawRectangleRounded(QZ_BTN[i],0.2f,6,bc);
        DrawRectangleRoundedLines(QZ_BTN[i],0.2f,6,(Color){255,200,0,255});
        DBold(set[passo].op[i],(int)QZ_BTN[i].x+18,(int)QZ_BTN[i].y+25,19,WHITE);
    }
    if(qz_flash>0){
        const char*msg=qz_ok?"Isso mesmo!":"Tente a opção correta.";
        Color mc=qz_ok?(Color){120,255,120,255}:(Color){255,150,150,255};
        DBold(msg,W/2-MeasureText(msg,20)/2,700,20,mc);
    }
}

/* =====================================================================
   MANGUEIRA / FOCOS GENERICOS (Fases 5, 9 e 10)
   ===================================================================== */
static void DrawFoco(float x,float y,float s){
    if(s<=0) return;
    float oy=sinf(GetTime()*5+x)*6;
    DrawCircle((int)x,(int)(y+oy),(int)(20*s),(Color){230,50,0,210});
    DrawCircle((int)x,(int)(y-12+oy),(int)(13*s),(Color){255,140,0,200});
    DrawCircle((int)x,(int)(y-22+oy),(int)(7*s),(Color){255,220,60,180});
}
static void HoseFire(float camx,float ang){
    for(int i=0;i<MJT;i++) if(!jts[i].on){
        float rad=ang*(PI/180.f);
        jts[i]=(Jato){camx+55,(float)(H-95),cosf(rad)*18.f,sinf(rad)*18.f,1.f,true};
        break;
    }
}
static void HoseMove(Foco*foco,int n,int*apag,float windvx){
    for(int i=0;i<MJT;i++){ if(!jts[i].on) continue;
        jts[i].vx+=windvx; jts[i].x+=jts[i].vx; jts[i].y+=jts[i].vy; jts[i].vy+=0.16f;
        jts[i].a-=0.010f; if(jts[i].a<=0||jts[i].y>H){ jts[i].on=false; continue; }
        for(int f=0;f<n;f++){ if(foco[f].fogo<=0) continue;
            if(fabsf(jts[i].x-foco[f].x)<32 && fabsf(jts[i].y-foco[f].y)<32){
                foco[f].fogo-=0.20f;
                if(foco[f].fogo<=0){ foco[f].fogo=0; (*apag)++; }
                jts[i].on=false; break;
            }
        }
    }
}
static void DrawJatos(void){
    for(int i=0;i<MJT;i++){ if(!jts[i].on) continue;
        Color wc={100,180,255,(unsigned char)(jts[i].a*255)};
        DrawCircle((int)jts[i].x,(int)jts[i].y,5,wc);
    }
}
static void DrawViaturaMangueira(int cx,float ang){
    DrawRectangle(cx,H-105,110,50,(Color){200,30,30,255});
    DrawRectangle(cx+5,H-125,50,24,(Color){200,30,30,255});
    DrawRectangle(cx+8,H-122,28,16,(Color){180,220,255,200});
    DrawCircle(cx+18,H-55,16,(Color){30,30,30,255}); DrawCircle(cx+18,H-55,8,(Color){80,80,80,255});
    DrawCircle(cx+90,H-55,16,(Color){30,30,30,255}); DrawCircle(cx+90,H-55,8,(Color){80,80,80,255});
    DrawRectangle(cx,H-55,110,5,(Color){160,160,160,255});
    float rad=ang*(PI/180.f);
    int mx=cx+55, my=H-95;
    DrawLine(mx,my,mx+(int)(cosf(rad)*42),my+(int)(sinf(rad)*42),(Color){80,200,80,255});
    DrawCircle(mx+(int)(cosf(rad)*42),my+(int)(sinf(rad)*42),5,(Color){120,180,255,255});
}
/* controla camera/angulo padrao das fases de mangueira */
static void HoseControls(float*camx,float*ang){
    if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A))  *camx-=3;
    if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) *camx+=3;
    if(*camx<0) *camx=0;
    if(*camx>W-110) *camx=W-110;
    if(IsKeyDown(KEY_UP)||IsKeyDown(KEY_W))    *ang-=1.6f;
    if(IsKeyDown(KEY_DOWN)||IsKeyDown(KEY_S))  *ang+=1.6f;
    if(*ang>-15.f)  *ang=-15.f;
    if(*ang<-165.f) *ang=-165.f;
    if(IsKeyPressed(KEY_SPACE)) HoseFire(*camx,*ang);
}

/* =====================================================================
   FASE 1 - CONHECENDO O QUARTEL (vestir o EPI)
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
        for(int i=0;i<5;i++){
            if(epi[i].usado) continue;
            if(CheckCollisionPointRec(m,epi[i].btn)){
                epi[i].usado=true; f1_equip|=epi[i].bit; f1_vestidos++; f1_flash=0.5f;
            }
        }
}
static void DrawF1(int estrelas,int chances,float timer){
    ClearBackground((Color){40,46,60,255});
    for(int c=0;c<6;c++){
        DrawRectangle(c*100,60,96,140,(Color){55,62,80,255});
        DrawRectangleLines(c*100,60,96,140,(Color){30,35,50,255});
    }
    DrawRectangle(40,120,150,330,(Color){90,30,20,255});
    DrawRectangleLines(40,120,150,330,(Color){40,15,10,255});
    DBold("CBMDF",70,135,26,(Color){255,210,40,255});
    int cx=380, cy=240;
    DrawRectangle(cx-30,cy+120,90,12,(Color){30,35,50,255});
    DrawClara(cx,cy,true,(int)(GetTime()*6),f1_equip);
    for(int i=0;i<5;i++){
        Color bc=epi[i].usado?(Color){40,90,40,255}:(Color){0,60,140,255};
        DrawRectangleRounded(epi[i].btn,0.25f,6,bc);
        DrawRectangleRoundedLines(epi[i].btn,0.25f,6,(Color){255,200,0,255});
        const char*tx=epi[i].usado?"VESTIDO":epi[i].nome;
        DBold(tx,(int)(epi[i].btn.x+epi[i].btn.width/2-MeasureText(tx,18)/2),(int)(epi[i].btn.y+18),18,WHITE);
    }
    if(f1_flash>0){
        const char*ok="+ Equipamento vestido!";
        DBold(ok,W/2-MeasureText(ok,18)/2,560,18,(Color){120,255,120,255});
    }
    DrawHUD(1,estrelas,chances,timer,"Clique para vestir o equipamento");
    char p[32]; sprintf(p,"Vestido: %d/5",f1_vestidos);
    DBold(p,W/2-MeasureText(p,16)/2,575,16,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 2 - CHAMADA DE EMERGENCIA (ligar 193)
   ===================================================================== */
static Quiz f2_q[]={
  {"Você vê fumaça saindo de uma casa. O que fazer?",
     {"Entrar para apagar","Manter distância e chamar um adulto","Tirar foto e postar"},1},
  {"Para chamar os Bombeiros, qual número devo ligar?",
     {"190","192","193"},2},
  {"O atendente pergunta o endereço. O que devo responder?",
     {"Não sei, desligo","Informo a rua e o número da casa","Mando ele descobrir"},1},
  {"Ele pergunta: o que aconteceu?",
     {"Explico que tem fumaça e fogo na casa","Fico em silêncio","Conto uma piada"},0},
  {"Ele pergunta: se há vítimas?",
     {"Minto que não","Informo se tem pessoas no local","Desligo o telefone"},1},
};
#define F2_TOTAL 5
static int f2_passo;
static void InitF2(void){ f2_passo=0; QuizInit(); }
static void DrawF2(int estrelas,int chances,float timer){
    ClearBackground((Color){30,30,55,255});
    DrawRectangle(60,120,180,150,(Color){150,120,90,255});
    DrawTriangle((Vector2){50,120},(Vector2){250,120},(Vector2){150,70},(Color){120,40,30,255});
    DrawRectangle(100,180,40,40,(Color){70,90,130,255});
    DrawRectangle(170,200,45,70,(Color){80,50,30,255});
    for(int i=0;i<5;i++){
        float fx=120+i*22;
        Color sm={120,120,120,(unsigned char)(150-i*20)};
        DrawCircle((int)fx,100-((int)(GetTime()*30)%40),12+i*2,sm);
    }
    DrawRectangle(420,210,12,22,(Color){60,160,90,255});
    DrawRectangle(421,196,10,12,(Color){255,205,150,255});
    DBold("Você",414,236,12,WHITE);
    DrawRectangle(40,290,520,46,(Color){0,40,90,255});
    DBold("193 Bombeiros | 192 SAMU | 190 Polícia",
             52,302,16,(Color){255,230,120,255});
    int qp2=(f2_passo<F2_TOTAL)?f2_passo:F2_TOTAL-1;
    QuizDraw(f2_q,qp2);
    DrawHUD(2,estrelas,chances,timer,"Escolha a ação correta");
    int pn2=(f2_passo<F2_TOTAL)?f2_passo+1:F2_TOTAL;
    char p[32]; sprintf(p,"Pergunta %d/%d",pn2,F2_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,632,18,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 3 - PREPARACAO DA VIATURA (carregar itens certos)
   ===================================================================== */
typedef struct { const char*nome; bool certo; bool carregado; Rectangle btn; } ItemVia;
static ItemVia f3[8];
static int f3_carregados, f3_total_certos; static float f3_flash; static bool f3_flash_ok;
static void InitF3(void){
    const char* nomes[8]={"MANGUEIRAS","VIDEOGAME","EXTINTORES","KIT 1o SOCORROS",
                          "SKATE","ESCADAS","EQUIP. RESGATE","PIPOCA"};
    bool certos[8]={true,false,true,true,false,true,true,false};
    f3_total_certos=0;
    for(int i=0;i<8;i++){
        f3[i].nome=nomes[i]; f3[i].certo=certos[i]; f3[i].carregado=false;
        f3[i].btn=(Rectangle){ 30.f+(i%2)*210.f, 420.f+(i/2)*70.f, 190, 56 };
        if(certos[i]) f3_total_certos++;
    }
    f3_carregados=0; f3_flash=0; f3_flash_ok=false;
}
static void UpdF3(Vector2 m){
    if(f3_flash>0) f3_flash-=GetFrameTime();
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for(int i=0;i<8;i++){
            if(f3[i].carregado) continue;
            if(CheckCollisionPointRec(m,f3[i].btn)){
                if(f3[i].certo){ f3[i].carregado=true; f3_carregados++; f3_flash_ok=true; }
                else f3_flash_ok=false;
                f3_flash=0.7f;
            }
        }
}
static void DrawF3(int estrelas,int chances,float timer){
    ClearBackground((Color){45,50,40,255});
    int tx=120,ty=110;
    DrawRectangle(tx,ty,360,90,(Color){200,30,30,255});
    DrawRectangle(tx,ty+30,360,16,(Color){255,255,255,255});
    DrawRectangle(tx+250,ty-30,110,40,(Color){200,30,30,255});
    DrawRectangle(tx+258,ty-26,40,30,(Color){180,220,255,200});
    DrawCircle(tx+70,ty+100,22,(Color){30,30,30,255}); DrawCircle(tx+70,ty+100,11,(Color){90,90,90,255});
    DrawCircle(tx+300,ty+100,22,(Color){30,30,30,255}); DrawCircle(tx+300,ty+100,11,(Color){90,90,90,255});
    DBold("CBMDF",tx+30,ty+8,30,WHITE);
    DrawRectangle(tx+150,ty-14,60,12,(Color){255,255,0,255});
    DrawCircle(tx+165,ty-8,5,(int)(GetTime()*4)%2?(Color){255,40,40,255}:(Color){40,40,255,255});
    DrawCircle(tx+195,ty-8,5,(int)(GetTime()*4)%2?(Color){40,40,255,255}:(Color){255,40,40,255});
    DrawSilva(40,300);
    DBold("Bombeiro Silva",20,360,13,(Color){200,220,255,255});
    for(int i=0;i<8;i++){
        Color bc=f3[i].carregado?(Color){40,90,40,255}:(Color){70,70,90,255};
        DrawRectangleRounded(f3[i].btn,0.2f,6,bc);
        DrawRectangleRoundedLines(f3[i].btn,0.2f,6,(Color){255,200,0,255});
        const char*tx2=f3[i].carregado?"CARREGADO":f3[i].nome;
        DBold(tx2,(int)(f3[i].btn.x+f3[i].btn.width/2-MeasureText(tx2,15)/2),(int)(f3[i].btn.y+20),15,WHITE);
    }
    if(f3_flash>0){
        const char*msg=f3_flash_ok?"Item certo carregado!":"Esse item não é da viatura!";
        Color mc=f3_flash_ok?(Color){120,255,120,255}:(Color){255,150,150,255};
        DBold(msg,W/2-MeasureText(msg,17)/2,H-50,17,mc);
    }
    DrawHUD(3,estrelas,chances,timer,"Carregue só os itens da viatura");
    char p[40]; sprintf(p,"Carregados: %d/%d",f3_carregados,f3_total_certos);
    DBold(p,W/2-MeasureText(p,16)/2,250,16,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 4 - DESLOCAMENTO PELA CIDADE (dirigir a viatura)
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
        Color cs[]={(Color){200,200,210,255},(Color){80,120,200,255},
                    (Color){60,160,90,255},(Color){220,180,40,255}};
        f4_cars[i]=(Carro){LANE_X[l],-60.f,l,true,cs[GetRandomValue(0,3)]};
        break;
    }
}
static void UpdF4(float dt,int *chances,Estado *st){
    if(f4_inv>0) f4_inv-=dt;
    if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A))  f4_truckx-=4;
    if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) f4_truckx+=4;
    if(f4_truckx<150) f4_truckx=150;
    if(f4_truckx>450) f4_truckx=450;
    f4_sirene = IsKeyDown(KEY_SPACE);
    f4_prog += dt*6.0f; if(f4_prog>100) f4_prog=100;
    f4_spawn+=dt;
    if(f4_spawn>0.7f){ f4_spawn=0; SpawnCarro(); }
    float vel = f4_sirene?6.5f:4.5f;
    for(int i=0;i<8;i++){ if(!f4_cars[i].on) continue;
        f4_cars[i].y += vel*60*dt;
        if(f4_sirene && f4_cars[i].y<H-180 && f4_cars[i].y>120){
            if(f4_cars[i].x<W/2) f4_cars[i].x-=2; else f4_cars[i].x+=2;
        }
        if(f4_cars[i].y>H+60) f4_cars[i].on=false;
        if(f4_inv<=0){
            Rectangle rt={f4_truckx-26,(float)(H-150),52,90};
            Rectangle rc={f4_cars[i].x-22,f4_cars[i].y-40,44,80};
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
    DrawRectangle(cx-26,by-90,52,92,(Color){200,30,30,255});
    DrawRectangle(cx-26,by-60,52,12,(Color){255,255,255,255});
    DrawRectangle(cx-22,by-86,44,22,(Color){180,220,255,220});
    DrawCircle(cx-16,by+4,12,(Color){25,25,25,255});
    DrawCircle(cx+16,by+4,12,(Color){25,25,25,255});
    DrawRectangle(cx-16,by-100,32,8,(Color){255,255,0,255});
    if(sirene){
        DrawCircle(cx-8,by-96,5,(int)(GetTime()*10)%2?(Color){255,40,40,255}:(Color){40,40,255,255});
        DrawCircle(cx+8,by-96,5,(int)(GetTime()*10)%2?(Color){40,40,255,255}:(Color){255,40,40,255});
        for(int r=0;r<3;r++){ int rr=20+r*14+((int)(GetTime()*40)%14);
            DrawCircleLines(cx,by-96,rr,(Color){255,255,120,(unsigned char)(120-r*35)}); }
    }
}
static void DrawF4(int estrelas,int chances,float timer){
    ClearBackground((Color){60,70,60,255});
    DrawRectangle(140,0,320,H,(Color){55,55,60,255});
    DrawRectangleLines(140,0,320,H,(Color){30,30,35,255});
    int off=(int)(GetTime()*300)%60;
    for(int l=0;l<2;l++){ int lx=235+l*130;
        for(int y=-60;y<H;y+=60) DrawRectangle(lx-3,y+off,6,30,(Color){230,210,80,255}); }
    for(int i=0;i<10;i++){
        DrawRectangle(20,i*80,110,70,(Color){70,80,100,255});
        DrawRectangle(470,i*80,110,70,(Color){70,80,100,255});
    }
    for(int i=0;i<8;i++){ if(!f4_cars[i].on) continue;
        DrawRectangle((int)f4_cars[i].x-22,(int)f4_cars[i].y-40,44,80,f4_cars[i].c);
        DrawRectangle((int)f4_cars[i].x-18,(int)f4_cars[i].y-34,36,18,(Color){40,50,70,200});
        DrawRectangle((int)f4_cars[i].x-18,(int)f4_cars[i].y+18,36,16,(Color){40,50,70,200});
    }
    DrawParts();
    if(f4_inv<=0 || ((int)(GetTime()*12))%2)
        DrawCaminhao((int)f4_truckx,H-58,f4_sirene);
    DrawRectangle(40,70,520,18,(Color){0,0,0,160});
    DrawRectangle(42,72,(int)(516*f4_prog/100),14,(Color){60,200,90,255});
    const char*marcos[]={"Quartel","Esplanada","Catedral","Local"};
    for(int i=0;i<4;i++) DBold(marcos[i],42+i*168,92,12,(Color){220,220,220,255});
    DrawHUD(4,estrelas,chances,timer,
            f4_sirene?"SIRENE LIGADA - corredor aberto!":"SETAS: desviar | ESPACO: sirene");
}

/* =====================================================================
   FASE 5 - INCENDIO RESIDENCIAL (apagar focos)
   ===================================================================== */
#define F5_NF 6
static Foco f5_foco[F5_NF];
static int  f5_apag, f5_total; static float f5_cx, f5_ang, f5_espalha;
static void InitF5(void){
    float fx[F5_NF]={175,300,425,175,300,425};
    float fy[F5_NF]={225,225,225,335,335,335};
    f5_total=F5_NF; f5_apag=0;
    for(int i=0;i<F5_NF;i++){ f5_foco[i].x=fx[i]; f5_foco[i].y=fy[i]; f5_foco[i].fogo=1.f; }
    f5_cx=W/2-55; f5_ang=-70.f; f5_espalha=0; ResetJatos();
}
static void UpdF5(float dt){
    HoseControls(&f5_cx,&f5_ang);
    HoseMove(f5_foco,F5_NF,&f5_apag,0.f);
    f5_espalha+=dt;
    if(f5_espalha>4.f){ f5_espalha=0;
        for(int f=0;f<F5_NF;f++) if(f5_foco[f].fogo>0 && f5_foco[f].fogo<1.f) f5_foco[f].fogo+=0.05f;
    }
}
static void DrawF5(int estrelas,int chances,float timer){
    ClearBackground((Color){20,12,18,255});
    DrawRectangle(110,150,380,330,(Color){140,110,85,255});                 /* corpo */
    DrawTriangle((Vector2){90,150},(Vector2){510,150},(Vector2){300,80},(Color){110,40,30,255}); /* telhado */
    for(int r=0;r<2;r++) for(int c=0;c<3;c++){                              /* janelas 2x3 */
        int wx=140+c*125, wy=190+r*110;
        DrawRectangle(wx,wy,70,70,(Color){95,75,58,255});
        DrawRectangleLines(wx,wy,70,70,(Color){55,40,30,255});
        DrawLine(wx+35,wy,wx+35,wy+70,(Color){55,40,30,255});
        DrawLine(wx,wy+35,wx+70,wy+35,(Color){55,40,30,255});
    }
    DrawRectangle(275,390,50,90,(Color){80,50,30,255});                     /* porta (separada) */
    DrawRectangleLines(275,390,50,90,(Color){50,30,18,255});
    DrawCircle(316,436,3,(Color){230,200,90,255});                          /* macaneta */
    for(int f=0;f<F5_NF;f++) DrawFoco(f5_foco[f].x,f5_foco[f].y,f5_foco[f].fogo);
    DrawJatos();
    DrawViaturaMangueira((int)f5_cx,f5_ang);
    DrawHUD(5,estrelas,chances,timer,"SETAS: mover/mirar | ESPACO: água");
    char p[40]; sprintf(p,"Focos apagados: %d/%d",f5_apag,f5_total);
    DBold(p,W/2-MeasureText(p,17)/2,H-48,17,(Color){120,200,255,255});
}

/* =====================================================================
   FASE 6 - PRIMEIROS SOCORROS (quiz educativo)
   ===================================================================== */
static Quiz f6_q[]={
  {"Você vê uma pessoa caída no chão. Devo primeiro:",
     {"Sacudir com força","Chamar e ver se ela responde","Sair correndo"},1},
  {"Ela não acorda. Para quem devo ligar?",
     {"190","192","Para um conhecido"},1},
  {"Posso mover a vítima de lugar?",
     {"Sim, arrasto rápido","Só se houver perigo por perto","Sim, para ela sentar"},1},
  {"Enquanto a ajuda não chega, eu:",
     {"Vou embora","Fico por perto e a observo","Dou água para ela"},1},
};
#define F6_TOTAL 4
static int f6_passo;
static void InitF6(void){ f6_passo=0; QuizInit(); }
static void DrawF6(int estrelas,int chances,float timer){
    ClearBackground((Color){35,45,55,255});
    DrawRectangle(0,250,W,40,(Color){70,75,80,255});           /* calcada */
    /* vitima caida */
    DrawRectangle(150,250,90,16,(Color){70,120,200,255});
    DrawRectangle(232,248,16,16,(Color){255,205,150,255});
    DrawRectangle(150,250,16,16,(Color){40,40,60,255});
    /* Bombeira Ana ajoelhada ao lado */
    DrawAna(280,210);
    DBold("Bombeira Ana",270,272,12,(Color){200,220,255,255});
    DrawRectangle(40,300,520,32,(Color){0,40,90,255});
    DBold("Emergência médica? Ligue 192 (SAMU)",
             52,308,16,(Color){255,230,120,255});
    int qp6=(f6_passo<F6_TOTAL)?f6_passo:F6_TOTAL-1;
    QuizDraw(f6_q,qp6);
    DrawHUD(6,estrelas,chances,timer,"Escolha a ação correta");
    int pn6=(f6_passo<F6_TOTAL)?f6_passo+1:F6_TOTAL;
    char p[32]; sprintf(p,"Pergunta %d/%d",pn6,F6_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,632,18,(Color){255,220,120,255});
}

/* =====================================================================
   FASE 7 - RESGATE ANIMAL (guindaste/cesta no Parque)
   ===================================================================== */
static float f7_hx, f7_hy, f7_dx, f7_dy; static int f7_st; static bool f7_got, f7_win; static float f7_aba;
static void InitF7(void){
    f7_hx=300; f7_hy=130; f7_st=0;
    f7_dx=(float)GetRandomValue(150,450); f7_dy=560; f7_got=false; f7_win=false; f7_aba=0;
}
static void UpdF7(float dt){
    f7_aba+=dt; if(f7_aba>1.4f) f7_aba=0;
    if(f7_st==0){
        if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A))  f7_hx-=3.5f;
        if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) f7_hx+=3.5f;
        if(f7_hx<80) f7_hx=80; if(f7_hx>520) f7_hx=520;
        if(IsKeyPressed(KEY_SPACE)) f7_st=1;
    } else if(f7_st==1){
        f7_hy+=4;
        if(f7_hy>=f7_dy){
            f7_got=(fabsf(f7_hx-f7_dx)<36);
            f7_st=2;
        }
    } else {
        f7_hy-=4;
        if(f7_got){ f7_dx=f7_hx; f7_dy=f7_hy+44; }
        if(f7_hy<=130){
            if(f7_got) f7_win=true;
            else { f7_st=0; }       /* errou: tenta de novo */
        }
    }
}
static void DrawF7(int estrelas,int chances,float timer){
    ClearBackground((Color){120,180,230,255});                 /* ceu */
    DrawRectangle(0,600,W,200,(Color){70,150,70,255});         /* gramado */
    for(int i=0;i<6;i++){                                      /* arvores do parque */
        int tx=40+i*100;
        DrawRectangle(tx,540,14,70,(Color){90,60,30,255});
        DrawCircle(tx+7,535,34,(Color){40,130,50,255});
    }
    /* estrutura/galho onde o cao esta preso */
    DrawRectangle(140,560,330,10,(Color){110,80,40,255});
    /* trilho do guindaste */
    DrawRectangle(60,118,480,8,(Color){90,90,100,255});
    /* cabo + cesta */
    DrawLine((int)f7_hx,126,(int)f7_hx,(int)f7_hy,(Color){60,60,60,255});
    DrawRectangleLines((int)f7_hx-16,(int)f7_hy,32,18,(Color){200,160,60,255});
    DrawRectangle((int)f7_hx-16,(int)f7_hy+16,32,3,(Color){200,160,60,255});
    /* cachorro */
    if(!f7_got) DrawDog((int)f7_dx-13,(int)f7_dy-14,f7_aba>0.7f?1:0);
    else        DrawDog((int)f7_hx-13,(int)f7_hy-2,0);
    DrawHUD(7,estrelas,chances,timer,"SETAS: mover | ESPACO: descer a cesta");
    const char*inf="Alinhe a cesta com o cachorro e desça com calma!";
    DBold(inf,W/2-MeasureText(inf,15)/2,H-50,15,(Color){40,40,60,255});
}

/* =====================================================================
   FASE 8 - ACIDENTE DE TRANSITO (isolar a area com cones)
   ===================================================================== */
typedef struct { float x,y; bool posto; } Cone;
static Cone f8[6];
static int f8_count;
static void InitF8(void){
    /* cones em um anel ao redor do acidente (centro ~ 300,360) */
    float ang0=0;
    for(int i=0;i<6;i++){
        float a=ang0 + i*(2*PI/6);
        f8[i].x=300+cosf(a)*150; f8[i].y=360+sinf(a)*120; f8[i].posto=false;
    }
    f8_count=0;
}
static void UpdF8(Vector2 m){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for(int i=0;i<6;i++) if(!f8[i].posto){
            if(fabsf(m.x-f8[i].x)<34 && fabsf(m.y-f8[i].y)<34){ f8[i].posto=true; f8_count++; break; }
        }
}
static void DrawCone(int x,int y){
    DrawTriangle((Vector2){x,y-22},(Vector2){x-12,y+8},(Vector2){x+12,y+8},(Color){240,120,20,255});
    DrawRectangle(x-9,y-6,18,4,(Color){255,255,255,255});
    DrawRectangle(x-16,y+8,32,5,(Color){200,90,10,255});
}
static void DrawF8(int estrelas,int chances,float timer){
    ClearBackground((Color){70,75,80,255});
    /* asfalto + faixas */
    DrawRectangle(0,150,W,520,(Color){55,55,60,255});
    for(int y=170;y<660;y+=60) DrawRectangle(W/2-4,y,8,30,(Color){230,210,80,255});
    /* estadio ao fundo */
    DrawCircleLines(300,120,90,(Color){180,180,200,255});
    DBold("Arena BRB Mané Garrincha",300-MeasureText("Arena BRB Mané Garrincha",13)/2,60,13,(Color){210,210,230,255});
    /* carro acidentado (tombado) no centro */
    DrawRectangle(255,330,90,46,(Color){180,60,60,255});
    DrawRectangle(265,318,60,16,(Color){120,140,180,220});
    DrawCircle(275,380,10,(Color){25,25,25,255});
    DrawCircle(325,380,10,(Color){25,25,25,255});
    for(int p=0;p<3;p++){ /* fumacinha */
        Color sm={120,120,120,120};
        DrawCircle(300+p*8,300-((int)(GetTime()*20)%30),10,sm);
    }
    /* marcas dos cones a colocar */
    for(int i=0;i<6;i++){
        if(f8[i].posto) DrawCone((int)f8[i].x,(int)f8[i].y);
        else DrawCircleLines((int)f8[i].x,(int)f8[i].y,16,(Color){255,255,255,180});
    }
    DrawHUD(8,estrelas,chances,timer,"Clique nos pontos para isolar a área");
    char p[40]; sprintf(p,"Cones colocados: %d/6",f8_count);
    DBold(p,W/2-MeasureText(p,16)/2,H-50,16,(Color){255,210,120,255});
}

/* =====================================================================
   FASE 9 - TEMPORADA DE QUEIMADAS (combate ao fogo na area rural)
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
    f9_cx=W/2-55; f9_ang=-55.f; f9_espalha=0;
    f9_wind=GetRandomValue(-6,6)/100.f;   /* vento empurra a agua */
    ResetJatos();
}
static void UpdF9(float dt){
    HoseControls(&f9_cx,&f9_ang);
    HoseMove(f9_foco,F9_NF,&f9_apag,f9_wind);
    f9_espalha+=dt;
    if(f9_espalha>3.0f){ f9_espalha=0;     /* o fogo cresce mais rapido no campo */
        for(int f=0;f<F9_NF;f++) if(f9_foco[f].fogo>0 && f9_foco[f].fogo<1.f) f9_foco[f].fogo+=0.07f;
    }
}
static void DrawF9(int estrelas,int chances,float timer){
    ClearBackground((Color){90,70,40,255});                    /* terra/ceu de fumaca */
    DrawRectangle(0,0,W,180,(Color){150,110,80,255});
    DrawRectangle(0,180,W,H-180,(Color){95,80,45,255});
    for(int i=0;i<7;i++){                                      /* moitas/arvores secas */
        int tx=30+i*85;
        DrawRectangle(tx,150,10,40,(Color){80,55,25,255});
        DrawCircle(tx+5,145,18,(Color){70,100,40,255});
    }
    for(int f=0;f<F9_NF;f++) DrawFoco(f9_foco[f].x,f9_foco[f].y,f9_foco[f].fogo);
    DrawJatos();
    DrawViaturaMangueira((int)f9_cx,f9_ang);
    DrawHUD(9,estrelas,chances,timer,"SETAS: mover/mirar | ESPACO: água");
    char p[48]; sprintf(p,"Focos apagados: %d/%d  (cuidado com o vento!)",f9_apag,f9_total);
    DBold(p,W/2-MeasureText(p,15)/2,H-50,15,(Color){255,220,160,255});
}

/* =====================================================================
   FASE 10 - GRANDE MISSAO EM BRASILIA (tudo junto)
   etapa 0: ligacao 193 (quiz) | 1: escolher equipamentos | 2: apagar fogo
   ===================================================================== */
static Quiz f10_q[]={
  {"Para chamar os bombeiros, ligue:",{"190","192","193"},2},
  {"Posso entrar sozinho no incêndio?",{"Sim, sou herói","Não, nunca sozinho","Só se for rápido"},1},
};
#define F10_QTOTAL 2
static ItemVia f10_it[6];
static Foco f10_foco[4];
static int f10_etapa, f10_qpasso, f10_load, f10_need, f10_apag;
static float f10_cx, f10_ang, f10_flash; static bool f10_flash_ok;
static void InitF10(void){
    f10_etapa=0; f10_qpasso=0; QuizInit();
    const char* nomes[6]={"MANGUEIRA","BOLA","EXTINTOR","VIDEOGAME","DOCE","CAPACETE"};
    bool certos[6]={true,false,true,false,false,true};
    f10_need=0;
    for(int i=0;i<6;i++){
        f10_it[i].nome=nomes[i]; f10_it[i].certo=certos[i]; f10_it[i].carregado=false;
        f10_it[i].btn=(Rectangle){ 60.f+(i%2)*240.f, 360.f+(i/2)*80.f, 220, 60 };
        if(certos[i]) f10_need++;
    }
    f10_load=0; f10_flash=0; f10_flash_ok=false;
    float fx[4]={170,300,430,300}; float fy[4]={300,260,300,400};
    for(int i=0;i<4;i++){ f10_foco[i].x=fx[i]; f10_foco[i].y=fy[i]; f10_foco[i].fogo=1.f; }
    f10_apag=0; f10_cx=W/2-55; f10_ang=-60.f; ResetJatos();
}
/* retorna true quando a fase inteira termina */
static bool UpdF10(float dt,Vector2 m){
    if(f10_flash>0) f10_flash-=GetFrameTime();
    if(f10_etapa==0){
        if(QuizUpdate(f10_q,f10_qpasso,m)==1){ f10_qpasso++; if(f10_qpasso>=F10_QTOTAL) f10_etapa=1; }
    } else if(f10_etapa==1){
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            for(int i=0;i<6;i++){
                if(f10_it[i].carregado) continue;
                if(CheckCollisionPointRec(m,f10_it[i].btn)){
                    if(f10_it[i].certo){ f10_it[i].carregado=true; f10_load++; f10_flash_ok=true; }
                    else f10_flash_ok=false;
                    f10_flash=0.7f;
                }
            }
        if(f10_load>=f10_need){ f10_etapa=2; ResetJatos(); }
    } else {
        HoseControls(&f10_cx,&f10_ang);
        HoseMove(f10_foco,4,&f10_apag,0.f);
        if(f10_apag>=4) return true;
    }
    return false;
}
static void DrawF10BG(void){
    ClearBackground((Color){25,30,55,255});
    /* Catedral (cones) + Esplanada + Estadio ao fundo, bem simplificados */
    for(int i=0;i<8;i++){
        int bx=20+i*72;
        DrawLine(bx,260,bx+18,150,(Color){180,190,220,255});
        DrawLine(bx+36,260,bx+18,150,(Color){180,190,220,255});
    }
    DrawCircle(80,150,16,(Color){200,210,235,255});            /* cupula catedral */
    DrawRectangle(0,260,W,6,(Color){120,130,160,255});
    DBold("BRASÍLIA - Esplanada, Catedral e Estádio",
             W/2-MeasureText("BRASÍLIA - Esplanada, Catedral e Estádio",13)/2,234,13,(Color){200,210,235,255});
}
static void DrawF10(int estrelas,int chances,float timer){
    DrawF10BG();
    if(f10_etapa==0){
        DrawRectangle(40,300,520,40,(Color){0,40,90,255});
        DBold("Faça a ligação de emergência correta!",52,310,16,(Color){255,230,120,255});
        int qp10=(f10_qpasso<F10_QTOTAL)?f10_qpasso:F10_QTOTAL-1;
        QuizDraw(f10_q,qp10);
        DrawHUD(10,estrelas,chances,timer,"Etapa 1/3: Ligação de emergência");
    } else if(f10_etapa==1){
        DBold("Escolha os 3 equipamentos certos:",
                 W/2-MeasureText("Escolha os 3 equipamentos certos:",18)/2,310,18,WHITE);
        for(int i=0;i<6;i++){
            Color bc=f10_it[i].carregado?(Color){40,90,40,255}:(Color){70,70,90,255};
            DrawRectangleRounded(f10_it[i].btn,0.2f,6,bc);
            DrawRectangleRoundedLines(f10_it[i].btn,0.2f,6,(Color){255,200,0,255});
            const char*tx=f10_it[i].carregado?"OK":f10_it[i].nome;
            DBold(tx,(int)(f10_it[i].btn.x+f10_it[i].btn.width/2-MeasureText(tx,17)/2),(int)(f10_it[i].btn.y+21),17,WHITE);
        }
        if(f10_flash>0){
            const char*msg=f10_flash_ok?"Equipamento certo!":"Esse não serve para a missão!";
            Color mc=f10_flash_ok?(Color){120,255,120,255}:(Color){255,150,150,255};
            DBold(msg,W/2-MeasureText(msg,17)/2,H-80,17,mc);
        }
        DrawHUD(10,estrelas,chances,timer,"Etapa 2/3: Equipamentos");
        char p[24]; sprintf(p,"Certos: %d/%d",f10_load,f10_need);
        DBold(p,W/2-MeasureText(p,16)/2,H-50,16,(Color){255,220,120,255});
    } else {
        for(int f=0;f<4;f++) DrawFoco(f10_foco[f].x,f10_foco[f].y,f10_foco[f].fogo);
        DrawJatos();
        DrawViaturaMangueira((int)f10_cx,f10_ang);
        DrawHUD(10,estrelas,chances,timer,"Etapa 3/3: SETAS mover/mirar | ESPACO água");
        char p[32]; sprintf(p,"Focos: %d/4",f10_apag);
        DBold(p,W/2-MeasureText(p,16)/2,H-50,16,(Color){120,200,255,255});
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
  {"A GRANDE MISSÃO chegou, Clara!","Use tudo: ligação, equipamentos","e combate ao incêndio!"},
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
   MUSEU DOS BOMBEIROS (liberado ao concluir as 10 fases)
   ===================================================================== */
static const char* MUSEU_SEC[5]={
    "História do Corpo de Bombeiros",
    "Veículos Antigos",
    "Equipamentos",
    "Curiosidades",
    "Galeria de Medalhas"
};
static const char* MUSEU_TXT[4][9]={
  { "O CBMDF é o grupo de bombeiros que cuida",
    "de Brasília. Eles ajudam quando há",
    "incêndios, acidentes e outras situações",
    "perigosas, sempre protegendo as pessoas",
    "e a cidade.",
    "",
    "Eles estão presentes desde o começo da",
    "história de Brasília, quando a capital",
    "ainda estava sendo construída." },
  { "Os bombeiros usaram vários veículos",
    "diferentes ao longo do tempo. No museu,",
    "dá para ver viaturas antigas que mostram",
    "como os carros dos bombeiros mudaram",
    "com os anos.",
    "",
    "Uma curiosidade legal é a MR 01, uma moto",
    "usada pelos bombeiros para chegar rápido",
    "em emergências." },
  { "Os bombeiros usam muitos equipamentos para",
    "ajudar as pessoas. No museu existem",
    "uniformes, ferramentas e objetos antigos",
    "que mostram como o trabalho deles foi",
    "ficando mais moderno.",
    "Também dá para aprender como a",
    "comunicação dos bombeiros mudou com o",
    "tempo, saindo de rádios mais antigos",
    "para tecnologias mais novas." },
  { "O CBMDF não trabalha só com fogo. Ele",
    "também ajuda em resgates, salvamentos",
    "e primeiros socorros.",
    "",
    "O museu tem uma exposição chamada",
    "\"Bravos Heróis - 50 Anos de História",
    "no Planalto Central\". Ele também tem",
    "visitas em Brasília para conhecer",
    "melhor a história dos bombeiros." },
};

static void DrawMedalha(int x,int y,int num,bool ligada){
    Color c=ligada?(Color){255,200,40,255}:(Color){80,80,70,255};
    DrawCircle(x,y,16,c);
    DrawCircle(x,y,11,ligada?(Color){255,235,140,255}:(Color){110,110,100,255});
    char n[6]; sprintf(n,"%d",num);
    DBold(n,x-MeasureText(n,14)/2,y-7,14,(Color){120,70,0,255});
    DrawRectangle(x-7,y+14,5,12,(Color){0,120,60,255});
    DrawRectangle(x+2,y+14,5,12,(Color){0,90,170,255});
}
/* desenha um caminhaozinho antigo (ilustracao da secao de veiculos) */
static void DrawCaminhaoAntigo(int x,int y){
    DrawRectangle(x,y,90,28,(Color){150,40,30,255});
    DrawRectangle(x+90,y-2,34,30,(Color){120,30,20,255});
    DrawRectangle(x+96,y+2,20,14,(Color){200,220,235,200});
    DrawRectangle(x+10,y-10,60,12,(Color){90,90,100,255});  /* tanque */
    DrawCircle(x+24,y+30,14,(Color){30,30,30,255}); DrawCircle(x+24,y+30,6,(Color){120,120,120,255});
    DrawCircle(x+96,y+30,14,(Color){30,30,30,255}); DrawCircle(x+96,y+30,6,(Color){120,120,120,255});
}

/* Desenha a pagina do museu. pag 0 = indice; 1..5 = secoes. */
static void DrawMuseuPagina(int pag,int estrelas,Vector2 mouse){
    ClearBackground((Color){28,24,40,255});
    DrawRectangle(0,40,W,66,(Color){90,20,10,255});
    DrawRectangle(0,43,W,4,(Color){255,200,0,255});
    DrawRectangle(0,102,W,4,(Color){255,200,0,255});
    const char*tit="MUSEU DOS BOMBEIROS";
    DBold(tit,W/2-MeasureText(tit,26)/2,62,26,(Color){255,230,60,255});

    if(pag==0){
        DBold("Escolha o que deseja conhecer:",
              W/2-MeasureText("Escolha o que deseja conhecer:",17)/2,140,17,(Color){210,215,235,255});
        for(int i=0;i<5;i++){
            Rectangle b={70,182+i*72,460,58};
            bool hov=CheckCollisionPointRec(mouse,b);
            DrawRectangleRounded(b,0.2f,6,hov?(Color){0,90,160,255}:(Color){40,55,90,255});
            DrawRectangleRoundedLines(b,0.2f,6,(Color){255,200,0,255});
            DBold(MUSEU_SEC[i],(int)b.x+22,(int)b.y+19,19,WHITE);
        }
        Rectangle bk={175,640,250,48};
        bool h2=CheckCollisionPointRec(mouse,bk);
        DrawRectangleRounded(bk,0.3f,8,h2?(Color){120,30,30,255}:(Color){70,20,20,255});
        DrawRectangleRoundedLines(bk,0.3f,8,(Color){255,140,60,255});
        DBold("VOLTAR AO MENU",(int)(bk.x+bk.width/2-MeasureText("VOLTAR AO MENU",19)/2),(int)(bk.y+14),19,WHITE);
    } else {
        int s=pag-1;
        DBold(MUSEU_SEC[s],W/2-MeasureText(MUSEU_SEC[s],22)/2,128,22,(Color){255,205,130,255});
        DrawRectangleLines(40,158,520,520,(Color){80,80,110,255});
        if(s<4){
            for(int i=0;i<9;i++)
                if(MUSEU_TXT[s][i][0])
                    DBold(MUSEU_TXT[s][i],62,188+i*42,19,(Color){222,228,242,255});
            if(s==1) DrawCaminhaoAntigo(230,590);     /* veiculos */
        } else {
            DBold("Você conquistou todas as 10 medalhas!",
                  W/2-MeasureText("Você conquistou todas as 10 medalhas!",18)/2,180,18,(Color){255,230,140,255});
            for(int i=0;i<10;i++)
                DrawMedalha(90+(i%5)*105, 270+(i/5)*120, i+1, i<estrelas || estrelas>=10);
            /* grande medalha de honra */
            DrawCircle(W/2,540,42,(Color){255,200,40,255});
            DrawCircle(W/2,540,30,(Color){255,235,140,255});
            DBold("DF",W/2-MeasureText("DF",24)/2,526,24,(Color){150,90,0,255});
            DrawRectangle(W/2-16,572,12,34,(Color){0,120,60,255});
            DrawRectangle(W/2+4,572,12,34,(Color){0,90,170,255});
            DBold("Bombeiro Honorário do DF",
                  W/2-MeasureText("Bombeiro Honorário do DF",18)/2,624,18,(Color){255,205,130,255});
        }
        Rectangle bk={185,710,230,44};
        bool h2=CheckCollisionPointRec(mouse,bk);
        DrawRectangleRounded(bk,0.3f,8,h2?(Color){0,120,60,255}:(Color){0,80,40,255});
        DrawRectangleRoundedLines(bk,0.3f,8,(Color){120,255,150,255});
        DBold("VOLTAR",(int)(bk.x+bk.width/2-MeasureText("VOLTAR",19)/2),(int)(bk.y+12),19,WHITE);
    }
}

/* =====================================================================
   PROGRAMA PRINCIPAL  (com tela cheia via RenderTexture)
   ===================================================================== */
int main(void){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(W,H,"Heróis do DF: Missão Bombeiro");
    int mon=GetCurrentMonitor();
    int mw=GetMonitorWidth(mon), mh=GetMonitorHeight(mon);
    if(mw>0 && mh>0) SetWindowSize(mw,mh);
    ToggleFullscreen();
    SetTargetFPS(60);

    RenderTexture2D target=LoadRenderTexture(W,H);
    SetTextureFilter(target.texture,TEXTURE_FILTER_POINT);

    Estado st=MENU;
    int fase=1, estrelas=0, chances=3, animFr=0;
    float timer=0, animT=0, celebraT=0, rocketT=0;
    bool faseIntro=true, faseConcluida=false;
    bool museuLiberado=false;   /* so abre apos concluir as 10 fases */
    int  museuPag=0;            /* 0 = indice; 1..5 = secoes         */

    ResetParts(); ResetRockets(); ResetJatos();

    while(!WindowShouldClose()){
        if(IsKeyPressed(KEY_F11)) ToggleFullscreen();

        float dt=GetFrameTime(); if(dt>0.05f) dt=0.05f;
        animT+=dt; if(animT>0.09f){ animFr++; animT=0; }

        /* ----- mouse virtual (converte da tela real p/ 600x800) ----- */
        float scale=fminf((float)GetScreenWidth()/W,(float)GetScreenHeight()/H);
        Vector2 rawm=GetMousePosition();
        Vector2 mouse={ (rawm.x-(GetScreenWidth()-W*scale)*0.5f)/scale,
                        (rawm.y-(GetScreenHeight()-H*scale)*0.5f)/scale };

        /* -------------------- UPDATE -------------------- */
        if(st==JOGO){
            if(faseIntro){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)||IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER)){
                    faseIntro=false; timer=0;
                }
            } else if(faseConcluida){
                celebraT+=dt; rocketT+=dt;
                if(rocketT>0.35f){ rocketT=0; LaunchRocket(); }
                UpdFoguetes();
                if(celebraT>0.6f && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)||IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))){
                    faseConcluida=false; celebraT=0; ResetRockets();
                    fase++;
                    if(fase>10){ st=FIMJOGO; museuLiberado=true; celebraT=0; }
                    else { faseIntro=true; chances=3; IniciarFase(fase); }
                }
            } else {
                timer+=dt;
                switch(fase){
                    case 1: UpdF1(mouse); if(f1_vestidos>=5){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 2: if(QuizUpdate(f2_q,f2_passo,mouse)==1) f2_passo++;
                            if(f2_passo>=F2_TOTAL){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 3: UpdF3(mouse); if(f3_carregados>=f3_total_certos){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 4: UpdF4(dt,&chances,&st); if(f4_prog>=100){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 5: UpdF5(dt); if(f5_apag>=f5_total){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 6: if(QuizUpdate(f6_q,f6_passo,mouse)==1) f6_passo++;
                            if(f6_passo>=F6_TOTAL){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 7: UpdF7(dt); if(f7_win){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 8: UpdF8(mouse); if(f8_count>=6){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 9: UpdF9(dt); if(f9_apag>=f9_total){ faseConcluida=true; estrelas++; celebraT=0; } break;
                    case 10: if(UpdF10(dt,mouse)){ faseConcluida=true; estrelas++; celebraT=0; } break;
                }
            }
        }

        /* -------------------- DRAW (na textura virtual) -------------------- */
        BeginTextureMode(target);

        if(st==MENU){
            ClearBackground((Color){15,18,40,255});
            for(int i=0;i<8;i++){
                float fx=40.f+i*75,fy=(float)(H-50)+sinf(GetTime()*2+i)*8;
                DrawCircle((int)fx,(int)fy,26,(Color){220,50,0,200});
                DrawCircle((int)fx,(int)(fy-20),15,(Color){255,140,0,180});
            }
            DrawRectangle(0,H-60,W,60,(Color){20,6,0,220});
            DrawRectangle(0,70,W,86,(Color){0,40,120,235});
            DrawRectangle(0,73,W,4,(Color){255,200,0,255});
            DrawRectangle(0,152,W,4,(Color){255,200,0,255});
            const char*t1="HERÓIS DO DF";
            DBold(t1,W/2-MeasureText(t1,40)/2+2,86,40,(Color){0,0,0,170});
            DBold(t1,W/2-MeasureText(t1,40)/2,84,40,(Color){255,230,50,255});
            const char*t2="MISSÃO BOMBEIRO";
            DBold(t2,W/2-MeasureText(t2,22)/2,128,22,(Color){255,170,90,255});
            DBold("193 Bombeiros   192 SAMU   190 Polícia",
                     W/2-MeasureText("193 Bombeiros   192 SAMU   190 Polícia",16)/2,180,16,(Color){255,230,120,255});
            DrawCapitao(70,260);   DBold("Cap. Almeida",48,316,12,WHITE);
            DrawClara(190,260,true,(int)(GetTime()*6),EQ_TUDO); DBold("Cadete Clara",168,316,12,WHITE);
            DrawAna(330,260);      DBold("Bombeira Ana",308,316,12,WHITE);
            DrawSilva(450,260);    DBold("Bombeiro Silva",430,316,12,WHITE);
            const char* col1[]={"1- Quartel","2- Emergência","3- Viatura","4- Deslocamento","5- Incêndio"};
            const char* col2[]={"6- Prim. Socorros","7- Resgate Animal","8- Acidente","9- Queimadas","10- Grande Missão"};
            for(int i=0;i<5;i++){
                DBold(col1[i],60,360+i*26,16,(Color){200,200,225,225});
                DBold(col2[i],320,360+i*26,15,(Color){200,200,225,225});
            }
            Rectangle bp={170,500,260,52}; bool hov=CheckCollisionPointRec(mouse,bp);
            DrawRectangleRounded(bp,0.3f,8,hov?(Color){0,150,40,255}:(Color){0,100,30,255});
            DrawRectangleRoundedLines(bp,0.3f,8,(Color){255,200,0,255});
            const char*bt="COMECAR FASE 1";
            DBold(bt,(int)(bp.x+bp.width/2-MeasureText(bt,20)/2),(int)(bp.y+16),20,WHITE);

            /* botao do Museu - so habilita apos concluir as 10 fases */
            Rectangle bmu={170,562,260,48}; bool hmu=CheckCollisionPointRec(mouse,bmu);
            if(museuLiberado){
                DrawRectangleRounded(bmu,0.3f,8,hmu?(Color){150,40,20,255}:(Color){100,30,15,255});
                DrawRectangleRoundedLines(bmu,0.3f,8,(Color){255,200,0,255});
                const char*bm="MUSEU DOS BOMBEIROS";
                DBold(bm,(int)(bmu.x+bmu.width/2-MeasureText(bm,17)/2),(int)(bmu.y+15),17,WHITE);
            } else {
                DrawRectangleRounded(bmu,0.3f,8,(Color){45,45,55,255});
                DrawRectangleRoundedLines(bmu,0.3f,8,(Color){90,90,110,255});
                /* cadeado */
                int lx=(int)bmu.x+40, ly=(int)bmu.y+14;
                DrawRectangle(lx,ly+8,18,14,(Color){180,180,190,255});
                DrawRectangleLines(lx,ly+8,18,14,(Color){90,90,100,255});
                DrawCircleLines(lx+9,ly+8,7,(Color){180,180,190,255});
                DrawRectangle(lx+7,ly+13,4,5,(Color){90,90,100,255});
                const char*bm="MUSEU (BLOQUEADO)";
                DBold(bm,(int)(bmu.x+bmu.width/2-MeasureText(bm,16)/2)+12,(int)(bmu.y+16),16,(Color){150,150,170,255});
            }

            DBold("Termine as 10 fases para virar Bombeiro Honorário!",
                     W/2-MeasureText("Termine as 10 fases para virar Bombeiro Honorário!",14)/2,628,14,(Color){180,190,220,220});
            DBold("F11: janela/tela cheia   ESC: sair",
                     W/2-MeasureText("F11: janela/tela cheia   ESC: sair",13)/2,650,13,(Color){140,150,180,200});
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                fase=1; estrelas=0; chances=3; faseIntro=true; faseConcluida=false;
                timer=0; IniciarFase(1); st=JOGO;
            }
            if(museuLiberado && hmu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                museuPag=0; st=MUSEU;
            }

        } else if(st==JOGO){
            if(faseIntro){
                DrawIntro(fase,INTRO[fase][0],INTRO[fase][1],INTRO[fase][2]);
            } else {
                switch(fase){
                    case 1: DrawF1(estrelas,chances,timer); break;
                    case 2: DrawF2(estrelas,chances,timer); break;
                    case 3: DrawF3(estrelas,chances,timer); break;
                    case 4: DrawF4(estrelas,chances,timer); break;
                    case 5: DrawF5(estrelas,chances,timer); break;
                    case 6: DrawF6(estrelas,chances,timer); break;
                    case 7: DrawF7(estrelas,chances,timer); break;
                    case 8: DrawF8(estrelas,chances,timer); break;
                    case 9: DrawF9(estrelas,chances,timer); break;
                    case 10: DrawF10(estrelas,chances,timer); break;
                }
                if(faseConcluida){
                    DrawFoguetes();
                    DrawFaseConcluida(fase,LICAO[fase][0],LICAO[fase][1]);
                }
            }

        } else if(st==FIMJOGO){
            ClearBackground((Color){8,16,45,255});
            celebraT+=dt; rocketT+=dt;
            if(rocketT>0.35f){ rocketT=0; LaunchRocket(); }
            UpdFoguetes(); DrawFoguetes();
            /* medalha de honra */
            DrawCircle(W/2,180,46,(Color){255,200,40,255});
            DrawCircle(W/2,180,34,(Color){255,230,120,255});
            DBold("DF",W/2-MeasureText("DF",26)/2,166,26,(Color){150,90,0,255});
            DrawRectangle(W/2-18,210,12,40,(Color){0,120,60,255});
            DrawRectangle(W/2+6,210,12,40,(Color){0,90,170,255});
            DrawRectangle(0,H/2-140,W,270,(Color){20,0,80,245});
            DrawRectangle(0,H/2-144,W,8,(Color){255,195,0,255});
            DrawRectangle(0,H/2+122,W,8,(Color){255,195,0,255});
            const char*vt="VOCÊ SE TORNOU UM";
            DBold(vt,W/2-MeasureText(vt,20)/2,H/2-126,20,WHITE);
            const char*v1="BOMBEIRO HONORÁRIO DO DF!";
            DBold(v1,W/2-MeasureText(v1,24)/2+2,H/2-98,24,(Color){0,0,0,160});
            DBold(v1,W/2-MeasureText(v1,24)/2,H/2-100,24,(Color){255,230,50,255});
            const char*v2="Parabéns, Cadete Clara!";
            DBold(v2,W/2-MeasureText(v2,19)/2,H/2-64,19,(Color){255,205,130,255});
            const char*v3="Você completou as 10 fases e aprendeu";
            const char*v4="a agir corretamente em emergências.";
            DBold(v3,W/2-MeasureText(v3,15)/2,H/2-34,15,(Color){200,210,255,255});
            DBold(v4,W/2-MeasureText(v4,15)/2,H/2-14,15,(Color){200,210,255,255});
            char ve[40]; sprintf(ve,"Estrelas de Segurança: %d/10",estrelas);
            DBold(ve,W/2-MeasureText(ve,18)/2,H/2+12,18,(Color){255,210,60,255});
            const char*v5="O Museu dos Bombeiros foi desbloqueado!";
            DBold(v5,W/2-MeasureText(v5,15)/2,H/2+42,15,(Color){255,200,120,255});
            const char*v6="Lembre-se: em emergências, ligue 193!";
            DBold(v6,W/2-MeasureText(v6,15)/2,H/2+64,15,(Color){255,200,120,255});

            Rectangle br={175,H/2+150,250,46}; bool hov=CheckCollisionPointRec(mouse,br);
            DrawRectangleRounded(br,0.3f,8,hov?(Color){150,40,20,255}:(Color){100,30,15,255});
            DrawRectangleRoundedLines(br,0.3f,8,(Color){255,200,0,255});
            DBold("VISITAR O MUSEU",(int)(br.x+br.width/2-MeasureText("VISITAR O MUSEU",19)/2),(int)(br.y+13),19,WHITE);
            Rectangle br2={175,H/2+206,250,46}; bool hov2=CheckCollisionPointRec(mouse,br2);
            DrawRectangleRounded(br2,0.3f,8,hov2?(Color){0,155,0,255}:(Color){0,95,0,255});
            DrawRectangleRoundedLines(br2,0.3f,8,(Color){90,255,90,255});
            DBold("VOLTAR AO MENU",(int)(br2.x+br2.width/2-MeasureText("VOLTAR AO MENU",19)/2),(int)(br2.y+13),19,WHITE);
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ museuPag=0; st=MUSEU; ResetRockets(); }
            if(hov2&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ st=MENU; ResetRockets(); }

        } else if(st==MUSEU){
            DrawMuseuPagina(museuPag,estrelas,mouse);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(museuPag==0){
                    for(int i=0;i<5;i++){
                        Rectangle b={70,182+i*72,460,58};
                        if(CheckCollisionPointRec(mouse,b)){ museuPag=i+1; break; }
                    }
                    Rectangle bk={175,640,250,48};
                    if(CheckCollisionPointRec(mouse,bk)) st=MENU;
                } else {
                    Rectangle bk={185,710,230,44};
                    if(CheckCollisionPointRec(mouse,bk)) museuPag=0;
                }
            }

        } else { /* GAMEOVER (apenas Fase 4) */
            ClearBackground((Color){12,6,6,255});
            for(int i=0;i<8;i++){ float fx=40+i*72,fy=(float)(H-45)+sinf(GetTime()*1.5f+i)*7;
                DrawCircle((int)fx,(int)fy,28,(Color){200,35,0,185});
                DrawCircle((int)fx,(int)(fy-20),16,(Color){255,110,0,165}); }
            DrawRectangle(0,180,W,100,(Color){140,0,0,230});
            DrawRectangle(0,183,W,4,(Color){255,90,0,255});
            DrawRectangle(0,276,W,4,(Color){255,90,0,255});
            char go[40]; sprintf(go,"OPS! TENTE DE NOVO - FASE %d",fase);
            DBold(go,W/2-MeasureText(go,24)/2,210,24,(Color){255,75,45,255});
            const char*sub="Dirija com calma e use a sirene!";
            DBold(sub,W/2-MeasureText(sub,17)/2,320,17,(Color){200,145,95,255});
            Rectangle bt2={175,400,250,52}; bool hov=CheckCollisionPointRec(mouse,bt2);
            DrawRectangleRounded(bt2,0.3f,8,hov?(Color){195,45,0,255}:(Color){120,25,0,255});
            DrawRectangleRoundedLines(bt2,0.3f,8,(Color){255,140,45,255});
            DBold("TENTAR NOVAMENTE",(int)(bt2.x+bt2.width/2-MeasureText("TENTAR NOVAMENTE",19)/2),(int)(bt2.y+16),19,WHITE);
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                chances=3; timer=0; faseIntro=true; faseConcluida=false;
                IniciarFase(fase); st=JOGO; ResetParts();
            }
            Rectangle bm={175,470,250,48}; hov=CheckCollisionPointRec(mouse,bm);
            DrawRectangleRounded(bm,0.3f,8,hov?(Color){55,55,85,255}:(Color){28,28,55,255});
            DrawRectangleRoundedLines(bm,0.3f,8,(Color){140,140,195,255});
            DBold("MENU PRINCIPAL",(int)(bm.x+bm.width/2-MeasureText("MENU PRINCIPAL",19)/2),(int)(bm.y+14),19,LIGHTGRAY);
            if(hov&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ st=MENU; ResetRockets(); }
        }

        EndTextureMode();

        /* -------------------- BLIT escalado p/ tela cheia -------------------- */
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src={0,0,(float)W,-(float)H};   /* altura negativa: textura vem invertida */
        Rectangle dst={(GetScreenWidth()-W*scale)*0.5f,(GetScreenHeight()-H*scale)*0.5f,W*scale,H*scale};
        DrawTexturePro(target.texture,src,dst,(Vector2){0,0},0,WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
