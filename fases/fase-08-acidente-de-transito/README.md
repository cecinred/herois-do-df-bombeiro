# Fase 8 — Acidente de Trânsito 🚧

**Local:** Próximo ao estádio (Arena BRB Mané Garrincha).

## Objetivo
**Isolar a área** do acidente colocando 6 cones nos pontos marcados, sem se
aproximar das vítimas.

## O que ensina
- **Não se aproximar** das vítimas;
- **Isolar a área** para proteger todo mundo;
- Acionar a emergência.

## Controles
- **Mouse:** clicar nos pontos marcados para colocar cada cone.

## Como o código funciona (`fase8.c`)
- `Cone f8[6]` — seis posições dispostas em **anel** ao redor do acidente; cada
  uma guarda se o cone já foi `posto`.
- `f8_count` — quantos cones já foram colocados.
- `InitF8()` — calcula as posições do anel usando seno/cosseno.
- `UpdF8(mouse)` — ao clicar perto de um ponto ainda vazio, coloca o cone ali.
- `DrawF8(...)` — desenha o estádio ao fundo, o carro acidentado e os pontos
  (círculo tracejado quando vazio, cone quando preenchido).
- **Vitória:** quando `f8_count >= 6`.

> Depende do jogo completo: `DrawHUD`, `DBold`.
