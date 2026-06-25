/* ============================================================
   FASE 8 - Acidente de Trânsito   (TRECHO DIDÁTICO / EXCERTO)
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
   FASE 8 - ACIDENTE DE TRANSITO
   ===================================================================== */
typedef struct { float x,y; bool posto; } Cone;
static Cone f8[6]; static int f8_count;
static void InitF8(void){
    float ang0=0;
    for(int i=0;i<6;i++){ float a=ang0 + i*(2*PI/6); f8[i].x=300+cosf(a)*150; f8[i].y=380+sinf(a)*120; f8[i].posto=false; }
    f8_count=0;
}
static void UpdF8(Vector2 m){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for(int i=0;i<6;i++) if(!f8[i].posto) if(fabsf(m.x-f8[i].x)<34 && fabsf(m.y-f8[i].y)<34){ f8[i].posto=true; f8_count++; break; }
}
static void DrawF8(int estrelas,int chances,float timer){
    ClearBackground((Color){150,200,235,255});
    DrawCloud(120,56,1.0f,(Color){255,255,255,200}); DrawCloud(470,40,1.1f,(Color){255,255,255,185});
    DrawEstadio(300,100,1.3f);
    DBold("Arena BRB Mané Garrincha",300-MeasureText("Arena BRB Mané Garrincha",13)/2,160,13,(Color){235,240,250,255});
    DrawRectangle(0,200,W,470,(Color){60,60,66,255}); DrawRectangle(0,200,W,6,(Color){90,90,96,255});
    for(int y=216;y<660;y+=60) DrawRectangle(W/2-4,y,8,30,(Color){230,210,80,255});
    DrawSidewalk(0,640,W,30); DrawLamppost(70,200); DrawLamppost(530,200);
    
    DrawRectangle(255,330,90,46,(Color){180,60,60,255}); DrawRectangle(265,318,60,16,(Color){120,140,180,220});
    DrawRectangle(250,360,100,8,(Color){120,40,40,255}); DrawCircle(275,380,10,(Color){25,25,25,255}); DrawCircle(325,380,10,(Color){25,25,25,255});
    DrawAna(160, 310);
    DrawCapitao(130, 330);
    DrawParkedCar(100, 220, DARKGREEN); DrawParkedCar(450, 500, BLUE);
    
    DrawSmokeColumn(300,300, 0.8f, 1.0f);
    for(int i=0;i<6;i++){
        if(f8[i].posto) DrawIcon(IC_CONE, (int)f8[i].x, (int)f8[i].y-4, 1.0f, false);
        else DrawCircleLines((int)f8[i].x,(int)f8[i].y,16,(Color){255,255,255,180});
    }
    DrawHUD(8,estrelas,chances,timer,"Clique nos pontos para isolar a área");
    char p[40]; sprintf(p,"Cones colocados: %d/6",f8_count); DBold(p,W/2-MeasureText(p,16)/2,H-50,16,(Color){40,40,60,255});
}

