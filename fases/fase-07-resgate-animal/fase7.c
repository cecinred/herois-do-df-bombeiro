/* ============================================================
   FASE 7 - Resgate Animal   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, struct Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

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

