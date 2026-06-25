/* ============================================================
   FASE 6 - Primeiros Socorros   (TRECHO DIDÁTICO / EXCERTO)
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
   FASE 6 - PRIMEIROS SOCORROS
   ===================================================================== */
static Quiz f6_q[]={
  /* Alternativas embaralhadas para variar a posicao correta */
  {"Você vê uma pessoa caída no chão. Devo primeiro:",{"Chamar e ver se ela responde","Sacudir com força","Sair correndo"},0},
  {"Ela não acorda. Para quem devo ligar?",{"190","Para um conhecido","192"},2},
  {"Posso mover a vítima de lugar?",{"Sim, arrasto rápido","Só se houver perigo por perto","Sim, para ela sentar"},1},
  {"Enquanto a ajuda não chega, eu:",{"Dou água para ela","Vou embora","Fico por perto e a observo"},2},
};
#define F6_TOTAL 4
static int f6_passo;
static void InitF6(void){ f6_passo=0; QuizInit(400); }
static void DrawF6(int estrelas,int chances,float timer){
    ClearBackground((Color){150,200,235,255});
    DrawCloud(110,70,1.0f,(Color){255,255,255,210}); DrawCloud(440,52,1.2f,(Color){255,255,255,190});
    DrawCongresso(450, 150, 0.5f);
    DrawRectangle(60,120,150,150,(Color){120,135,160,255});
    for(int r=0;r<3;r++) for(int c=0;c<3;c++) DrawRectangle(78+c*42,134+r*44,28,30,(Color){180,210,235,220});
    
    DrawSidewalk(0,250,W,42); DrawLamppost(110,250); DrawTree(515,250,1.0f); DrawFlowerBed(250,250,150);
    DrawRectangle(150,256,90,16,(Color){70,120,200,255}); DrawRectangle(232,254,16,16,(Color){255,205,150,255});
    DrawAna(280,216); DBold("Bombeira Ana",270,278,12,(Color){40,50,80,255});
    DrawRectangle(40,320,520,32,(Color){0,40,90,255});
    DBold("Emergência médica? Ligue 192 (SAMU)", 52,328,16,(Color){255,230,120,255});
    int qp6=(f6_passo<F6_TOTAL)?f6_passo:F6_TOTAL-1;
    QuizDraw(f6_q,qp6);
    DrawHUD(6,estrelas,chances,timer,"Escolha a ação correta");
    char p[32]; sprintf(p,"Pergunta %d/%d",(f6_passo<F6_TOTAL)?f6_passo+1:F6_TOTAL,F6_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,670,18,(Color){255,220,120,255});
}

