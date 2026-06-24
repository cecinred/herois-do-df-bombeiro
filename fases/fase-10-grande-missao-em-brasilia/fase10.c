/* ============================================================
   FASE 10 - Grande Missão em Brasília   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, struct Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

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

