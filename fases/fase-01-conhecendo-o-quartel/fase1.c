/* ============================================================
   FASE 1 - Conhecendo o Quartel   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, struct Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

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

