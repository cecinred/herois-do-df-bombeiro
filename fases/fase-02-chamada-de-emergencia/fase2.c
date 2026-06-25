/* ============================================================
   FASE 2 - Chamada de Emergência   (TRECHO DIDÁTICO / EXCERTO)
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
   FASE 2 - CHAMADA DE EMERGENCIA
   ===================================================================== */
static Quiz f2_q[]={
  {"Você vê fumaça saindo de uma casa. O que fazer?",{"Entrar para apagar","Manter distância e chamar adulto","Tirar foto e postar"},1},
  {"Para chamar os Bombeiros, qual número devo ligar?",{"190","192","193"},2},
  {"Qual o endereço da emergência?",{"Rua das Flores, 102","Av. Central, 50","Não sei"},0},
  {"O atendente pergunta: o que aconteceu?",{"Explico que tem fumaça e fogo na casa","Fico em silêncio","Conto uma piada"},0},
  {"Ele pergunta: há vítimas?",{"Minto que não","Informo se tem pessoas no local","Desligo o telefone"},1},
};
#define F2_TOTAL 5
static int f2_passo;
static void InitF2(void){ f2_passo=0; QuizInit(400); }
static void DrawF2(int estrelas,int chances,float timer){
    ClearBackground((Color){120,170,215,255});
    DrawCloud(380,40,1.0f,(Color){255,255,255,180});
    DrawRectangle(0,300,W,H-300,(Color){95,140,80,255}); DrawSidewalk(0,300,W,28);
    /* Casa */
    DrawRectangle(60,140,190,160,(Color){175,150,120,255});
    DrawTriangle((Vector2){48,140},(Vector2){262,140},(Vector2){155,80},(Color){130,50,40,255});
    DrawRectangle(100,180,44,44,(Color){80,100,140,255}); DrawRectangleLines(100,180,44,44,(Color){50,40,30,255});
    DrawRectangle(180,210,46,90,(Color){90,55,30,255}); /* Porta */
    
    /* Placa do número na casa (acima da porta) */
    DrawRectangle(183, 185, 40, 20, (Color){240,240,240,255});
    DrawRectangleLines(183, 185, 40, 20, BLACK);
    DBold("102", 191, 188, 14, BLACK);
    
    DrawFoco(150,150,0.7f); DrawSmokeColumn(150,150,0.7f,1.3f);
    DrawFlowerBed(330,300,120); 
    
    /* Placa de Rua Ajustada */
    DrawRectangle(420, 240, 4, 60, GRAY); 
    DrawRectangle(370, 245, 104, 24, BLUE);
    DBold("RUA DAS FLORES", 376, 252, 10, WHITE);
    
    DrawRectangle(500,250,12,40,(Color){60,160,90,255}); DrawRectangle(501,232,10,16,(Color){255,205,150,255});
    DrawRectangle(512,244,5,9,(Color){40,40,50,255}); DBold("Você",494,292,12,(Color){30,40,30,255});
    DrawRectangle(40,340,520,30,(Color){0,40,90,235});
    DBold("193 Bombeiros   |   192 SAMU   |   190 Polícia", 58,348,16,(Color){255,230,120,255});
    
    int qp2=(f2_passo<F2_TOTAL)?f2_passo:F2_TOTAL-1;
    QuizDraw(f2_q,qp2);
    DrawHUD(2,estrelas,chances,timer,"Escolha a ação correta");
    char p[32]; sprintf(p,"Pergunta %d/%d", (f2_passo<F2_TOTAL)?f2_passo+1:F2_TOTAL, F2_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,660,18,(Color){255,220,120,255});
}

