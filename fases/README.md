# 📚 Fases (código separado para estudo)

Esta pasta contém o código de **cada fase separado em sua própria pasta**,
para facilitar o estudo e a explicação de como cada minigame foi feito.

> ⚠️ **Importante:** estes arquivos `faseN.c` são **trechos didáticos** e
> **NÃO compilam sozinhos**. Eles dependem da Raylib e de partes
> **compartilhadas** que ficam no jogo completo, como:
>
> - `DBold(...)` — desenho de texto com contraste
> - `DrawHUD(...)` — barra de status (fase, estrelas, vidas)
> - `QuizDraw / QuizUpdate` — motor de perguntas (fases 2, 6 e 10)
> - `HoseControls / HoseFire / HoseMove / DrawFoco` — mangueira (fases 5, 9 e 10)
> - `DrawClara / DrawCapitao / DrawAna / DrawSilva` — personagens
> - sistema de partículas e foguetes, structs `Foco` e `Quiz`, etc.
>
> O **jogo completo e compilável** está em **[`../jogo-completo/`](../jogo-completo/)**.

## Como cada fase é estruturada no código

Quase toda fase segue o mesmo padrão de **3 funções**:

| Função | O que faz |
|---|---|
| `InitFx()` | prepara a fase (posições, contadores, reseta o estado) |
| `UpdFx(...)` | lógica por quadro (entrada do jogador, colisões, regras) |
| `DrawFx(...)` | desenha tudo na tela daquela fase |

No jogo, o `main()` chama essas funções conforme a fase atual, e detecta a
condição de vitória de cada uma para avançar.

## Índice

1. [Conhecendo o Quartel](fase-01-conhecendo-o-quartel/)
2. [Chamada de Emergência](fase-02-chamada-de-emergencia/)
3. [Preparação da Viatura](fase-03-preparacao-da-viatura/)
4. [Deslocamento pela Cidade](fase-04-deslocamento-pela-cidade/)
5. [Incêndio Residencial](fase-05-incendio-residencial/)
6. [Primeiros Socorros](fase-06-primeiros-socorros/)
7. [Resgate Animal](fase-07-resgate-animal/)
8. [Acidente de Trânsito](fase-08-acidente-de-transito/)
9. [Temporada de Queimadas](fase-09-temporada-de-queimadas/)
10. [Grande Missão em Brasília](fase-10-grande-missao-em-brasilia/)
