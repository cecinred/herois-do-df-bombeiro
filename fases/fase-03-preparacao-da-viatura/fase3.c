/* ============================================================
   FASE 3 - Preparação da Viatura   (TRECHO DIDÁTICO / EXCERTO)
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

