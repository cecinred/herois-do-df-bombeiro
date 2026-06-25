/* ============================================================
   FASE 1 - Conhecendo o Quartel   (TRECHO DIDÁTICO / EXCERTO)
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

