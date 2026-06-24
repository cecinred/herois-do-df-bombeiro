# Fase 9 — Temporada de Queimadas 🌳🔥

**Local:** Área rural próxima ao DF.

## Objetivo
Conter **9 focos** de incêndio que se espalham, agora com **vento**
atrapalhando a mira da água.

## O que ensina
Os riscos das queimadas e a importância da preservação ambiental.

## Controles
- **Setas / A,D:** mover a viatura.
- **Setas ↑↓ / W,S:** ajustar o ângulo da mangueira.
- **Espaço:** lançar o jato de água.

## Como o código funciona (`fase9.c`)
- `Foco f9_foco[9]` — focos espalhados pela área rural.
- `f9_wind` — valor do **vento**, sorteado no início, que empurra o jato de
  água para o lado (passado para `HoseMove`, exigindo correção da mira).
- `f9_espalha` — temporizador que faz os focos não apagados crescerem de novo,
  deixando a fase mais desafiadora que a Fase 5.
- Usa as mesmas funções compartilhadas da mangueira (`HoseControls`, `HoseMove`,
  `DrawFoco`, `DrawViaturaMangueira`).
- **Vitória:** quando todos os focos são apagados (`f9_apag >= f9_total`).

> Depende do jogo completo: `HoseControls/HoseMove/HoseFire/DrawFoco`,
> `DrawViaturaMangueira`, `DrawHUD`, `DBold`, struct `Foco`.
