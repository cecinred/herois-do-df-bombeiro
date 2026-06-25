/* ============================================================
   FASE 9 - Temporada de Queimadas   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, DrawIcon, os cenários de Brasília, structs
   Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

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

