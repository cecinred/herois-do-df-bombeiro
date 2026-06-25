/* ============================================================
   FASE 4 - Deslocamento pela Cidade   (TRECHO DIDÁTICO / EXCERTO)
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
   FASE 4 - DESLOCAMENTO PELA CIDADE
   ===================================================================== */
typedef struct { float x,y; int lane; bool on; Color c; } Carro;
static Carro f4_cars[8];
static float f4_truckx, f4_prog, f4_inv, f4_spawn; static bool f4_sirene;
static const float LANE_X[3]={170,300,430};
static void InitF4(void){
    for(int i=0;i<8;i++) f4_cars[i].on=false;
    f4_truckx=LANE_X[1]; f4_prog=0; f4_sirene=false; f4_inv=0; f4_spawn=0;
}
static void SpawnCarro(void){
    for(int i=0;i<8;i++) if(!f4_cars[i].on){
        int l=GetRandomValue(0,2);
        Color cs[]={(Color){200,200,210,255},(Color){80,120,200,255},(Color){60,160,90,255},(Color){220,180,40,255}};
        f4_cars[i]=(Carro){LANE_X[l],-60.f,l,true,cs[GetRandomValue(0,3)]}; break;
    }
}
static void UpdF4(float dt,int *chances,Estado *st){
    if(f4_inv>0) f4_inv-=dt;
    if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A))  f4_truckx-=4;
    if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) f4_truckx+=4;
    if(f4_truckx<150) f4_truckx=150; if(f4_truckx>450) f4_truckx=450;
    f4_sirene = IsKeyDown(KEY_SPACE);
    f4_prog += dt*6.0f; if(f4_prog>100) f4_prog=100;
    f4_spawn+=dt; if(f4_spawn>0.7f){ f4_spawn=0; SpawnCarro(); }
    float vel = f4_sirene?6.5f:4.5f;
    for(int i=0;i<8;i++){ if(!f4_cars[i].on) continue;
        f4_cars[i].y += vel*60*dt;
        if(f4_sirene && f4_cars[i].y<H-180 && f4_cars[i].y>120){
            if(f4_cars[i].x<W/2) f4_cars[i].x-=2; else f4_cars[i].x+=2;
            if(f4_cars[i].x < 155) f4_cars[i].x = 155;
            if(f4_cars[i].x > 445) f4_cars[i].x = 445;
        }
        if(f4_cars[i].y>H+60) f4_cars[i].on=false;
        if(f4_inv<=0){
            Rectangle rt={f4_truckx-26,(float)(H-150),52,90}; Rectangle rc={f4_cars[i].x-22,f4_cars[i].y-40,44,80};
            if(CheckCollisionRecs(rt,rc)){
                (*chances)--; f4_inv=1.2f; f4_cars[i].on=false;
                for(int p=0;p<10;p++) EmitPart(f4_truckx,H-150);
                if(*chances<=0) *st=GAMEOVER;
            }
        }
    }
    UpdParts();
}
static void DrawCaminhao(int cx,int by,bool sirene){
    DrawRectangle(cx-26,by-90,52,92,(Color){200,30,30,255}); DrawRectangle(cx-26,by-60,52,12,(Color){255,255,255,255});
    DrawRectangle(cx-22,by-86,44,22,(Color){180,220,255,220}); DrawRectangle(cx-16,by-100,32,8,(Color){255,255,0,255});
    DrawCircle(cx-16,by+4,12,(Color){25,25,25,255}); DrawCircle(cx+16,by+4,12,(Color){25,25,25,255});
    if(sirene){
        DrawCircle(cx-8,by-96,5,(int)(GetTime()*10)%2?(Color){255,40,40,255}:(Color){40,40,255,255});
        DrawCircle(cx+8,by-96,5,(int)(GetTime()*10)%2?(Color){40,40,255,255}:(Color){255,40,40,255});
    }
}
static void DrawF4(int estrelas,int chances,float timer){
    ClearBackground((Color){135,185,225,255});
    DrawEsplanada(W/2,150,0.7f); DrawCongresso(300,150,0.55f);
    DrawRectangle(140,0,320,H,(Color){55,55,60,255}); DrawRectangleLines(140,0,320,H,(Color){30,30,35,255});
    int off=(int)(GetTime()*300)%60;
    for(int l=0;l<2;l++){ int lx=235+l*130; for(int y=-60;y<H;y+=60) DrawRectangle(lx-3,y+off,6,30,(Color){230,210,80,255}); }
    DrawSidewalk(120,0,20,H); DrawSidewalk(460,0,20,H);
    for(int i=0;i<8;i++){ if(!f4_cars[i].on) continue;
        DrawRectangle((int)f4_cars[i].x-22,(int)f4_cars[i].y-40,44,80,f4_cars[i].c);
        DrawRectangle((int)f4_cars[i].x-18,(int)f4_cars[i].y-34,36,18,(Color){40,50,70,200});
        DrawRectangle((int)f4_cars[i].x-18,(int)f4_cars[i].y+18,36,16,(Color){40,50,70,200});
    }
    DrawParts();
    if(f4_inv<=0 || ((int)(GetTime()*12))%2) DrawCaminhao((int)f4_truckx,H-58,f4_sirene);
    DrawRectangle(40,70,520,18,(Color){0,0,0,160}); DrawRectangle(42,72,(int)(516*f4_prog/100),14,(Color){60,200,90,255});
    const char*marcos[]={"Quartel","Esplanada","Catedral","Local"};
    for(int i=0;i<4;i++) DBold(marcos[i],42+i*168,92,12,(Color){240,240,240,255});
    DrawHUD(4,estrelas,chances,timer,f4_sirene?"SIRENE LIGADA - corredor aberto!":"SETAS: desviar | ESPACO: sirene");
}

