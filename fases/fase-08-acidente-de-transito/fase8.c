/* ============================================================
   FASE 8 - Acidente de Trânsito   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, struct Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

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

