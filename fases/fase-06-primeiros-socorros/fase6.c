/* ============================================================
   FASE 6 - Primeiros Socorros   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, struct Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

/* =====================================================================
   FASE 6 - PRIMEIROS SOCORROS (quiz educativo)
   ===================================================================== */
static Quiz f6_q[]={
  {"Você vê uma pessoa caída no chão. Devo primeiro:",
     {"Sacudir com força","Chamar e ver se ela responde","Sair correndo"},1},
  {"Ela não acorda. Para quem devo ligar?",
     {"190","192","Para um conhecido"},1},
  {"Posso mover a vítima de lugar?",
     {"Sim, arrasto rápido","Só se houver perigo por perto","Sim, para ela sentar"},1},
  {"Enquanto a ajuda não chega, eu:",
     {"Vou embora","Fico por perto e a observo","Dou água para ela"},1},
};
#define F6_TOTAL 4
static int f6_passo;
static void InitF6(void){ f6_passo=0; QuizInit(); }
static void DrawF6(int estrelas,int chances,float timer){
    ClearBackground((Color){35,45,55,255});
    DrawRectangle(0,250,W,40,(Color){70,75,80,255});           /* calcada */
    /* vitima caida */
    DrawRectangle(150,250,90,16,(Color){70,120,200,255});
    DrawRectangle(232,248,16,16,(Color){255,205,150,255});
    DrawRectangle(150,250,16,16,(Color){40,40,60,255});
    /* Bombeira Ana ajoelhada ao lado */
    DrawAna(280,210);
    DBold("Bombeira Ana",270,272,12,(Color){200,220,255,255});
    DrawRectangle(40,300,520,32,(Color){0,40,90,255});
    DBold("Emergência médica? Ligue 192 (SAMU)",
             52,308,16,(Color){255,230,120,255});
    int qp6=(f6_passo<F6_TOTAL)?f6_passo:F6_TOTAL-1;
    QuizDraw(f6_q,qp6);
    DrawHUD(6,estrelas,chances,timer,"Escolha a ação correta");
    int pn6=(f6_passo<F6_TOTAL)?f6_passo+1:F6_TOTAL;
    char p[32]; sprintf(p,"Pergunta %d/%d",pn6,F6_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,632,18,(Color){255,220,120,255});
}

