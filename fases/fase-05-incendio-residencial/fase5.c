/* ============================================================
   FASE 5 - Incêndio Residencial   (TRECHO DIDÁTICO / EXCERTO)
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

