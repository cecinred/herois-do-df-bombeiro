/* ============================================================
   FASE 10 - Grande Missão em Brasília   (TRECHO DIDÁTICO / EXCERTO)
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

