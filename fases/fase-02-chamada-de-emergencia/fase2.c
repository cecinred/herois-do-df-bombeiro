/* ============================================================
   FASE 2 - Chamada de Emergência   (TRECHO DIDÁTICO / EXCERTO)
   ------------------------------------------------------------
   Este arquivo contém APENAS o código desta fase, separado do
   jogo para fins de estudo e explicação. Ele NÃO compila
   sozinho: depende da Raylib e de partes compartilhadas que
   ficam no jogo completo (ex.: DBold, DrawHUD, QuizDraw,
   HoseControls, struct Foco/Quiz, sistema de partículas...).

   O código completo e compilável está em:  jogo-completo/
   ============================================================ */

/* =====================================================================
   FASE 2 - CHAMADA DE EMERGENCIA (ligar 193)
   ===================================================================== */
static Quiz f2_q[]={
  {"Você vê fumaça saindo de uma casa. O que fazer?",
     {"Entrar para apagar","Manter distância e chamar um adulto","Tirar foto e postar"},1},
  {"Para chamar os Bombeiros, qual número devo ligar?",
     {"190","192","193"},2},
  {"O atendente pergunta o endereço. O que devo responder?",
     {"Não sei, desligo","Informo a rua e o número da casa","Mando ele descobrir"},1},
  {"Ele pergunta: o que aconteceu?",
     {"Explico que tem fumaça e fogo na casa","Fico em silêncio","Conto uma piada"},0},
  {"Ele pergunta: se há vítimas?",
     {"Minto que não","Informo se tem pessoas no local","Desligo o telefone"},1},
};
#define F2_TOTAL 5
static int f2_passo;
static void InitF2(void){ f2_passo=0; QuizInit(); }
static void DrawF2(int estrelas,int chances,float timer){
    ClearBackground((Color){30,30,55,255});
    DrawRectangle(60,120,180,150,(Color){150,120,90,255});
    DrawTriangle((Vector2){50,120},(Vector2){250,120},(Vector2){150,70},(Color){120,40,30,255});
    DrawRectangle(100,180,40,40,(Color){70,90,130,255});
    DrawRectangle(170,200,45,70,(Color){80,50,30,255});
    for(int i=0;i<5;i++){
        float fx=120+i*22;
        Color sm={120,120,120,(unsigned char)(150-i*20)};
        DrawCircle((int)fx,100-((int)(GetTime()*30)%40),12+i*2,sm);
    }
    DrawRectangle(420,210,12,22,(Color){60,160,90,255});
    DrawRectangle(421,196,10,12,(Color){255,205,150,255});
    DBold("Você",414,236,12,WHITE);
    DrawRectangle(40,290,520,46,(Color){0,40,90,255});
    DBold("193 Bombeiros | 192 SAMU | 190 Polícia",
             52,302,16,(Color){255,230,120,255});
    int qp2=(f2_passo<F2_TOTAL)?f2_passo:F2_TOTAL-1;
    QuizDraw(f2_q,qp2);
    DrawHUD(2,estrelas,chances,timer,"Escolha a ação correta");
    int pn2=(f2_passo<F2_TOTAL)?f2_passo+1:F2_TOTAL;
    char p[32]; sprintf(p,"Pergunta %d/%d",pn2,F2_TOTAL);
    DBold(p,W/2-MeasureText(p,18)/2,632,18,(Color){255,220,120,255});
}

