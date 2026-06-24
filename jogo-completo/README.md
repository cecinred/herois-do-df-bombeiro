# 🎮 Jogo completo

Aqui está o **jogo inteiro em um único arquivo**: `HeroisDoDF_Bombeiro.c`.

**Este é o arquivo que você compila e executa.** (Os arquivos da pasta
`../fases/` são apenas trechos separados para explicação e não compilam
sozinhos.)

## Compilar

### Windows (MinGW)
```bash
gcc HeroisDoDF_Bombeiro.c -o herois.exe -lraylib -lopengl32 -lgdi32 -lwinmm
herois.exe
```

### Linux
```bash
gcc HeroisDoDF_Bombeiro.c -o herois -lraylib -lm -lpthread -ldl -lrt -lX11
./herois
```

### macOS
```bash
gcc HeroisDoDF_Bombeiro.c -o herois -lraylib -framework OpenGL -framework Cocoa -framework IOKit
./herois
```

## Visão geral da estrutura interna

```
includes / #defines / enum Estado / structs
│
├── Sistema de partículas e foguetes (celebração)
├── Personagens (DrawClara, DrawCapitao, DrawAna, DrawSilva, DrawDog)
├── HUD e telas padrão (DrawHUD, DrawFaseConcluida, DrawIntro)
├── Quiz genérico  (QuizInit / QuizUpdate / QuizDraw)      → fases 2, 6, 10
├── Mangueira genérica (HoseControls / HoseFire / HoseMove)→ fases 5, 9, 10
│
├── FASE 1 .. FASE 10   (InitFx / UpdFx / DrawFx)
│
├── Textos do mentor e lições
├── Museu dos Bombeiros
└── main()  → máquina de estados (MENU, JOGO, GAMEOVER, FIMJOGO, MUSEU)
              + tela cheia via RenderTexture (mouse virtual)
```

> ⚠️ Mantenha o arquivo em **UTF-8** para preservar a acentuação.
