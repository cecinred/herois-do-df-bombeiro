/* ============================================================
   FASE 3 - Preparação da Viatura   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, struct Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

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

