# Fase 1 — Conhecendo o Quartel 🧯

**Local:** Quartel Central dos Bombeiros (Brasília)
**Mentor:** Capitão Almeida apresenta o quartel à Cadete Clara.

## Objetivo
Vestir corretamente o **EPI** (Equipamento de Proteção Individual) clicando
nos 5 itens: **botas, cilindro de oxigênio, máscara, capacete e luvas**.

## O que ensina
O que cada equipamento de proteção faz e por que o bombeiro precisa dele.

## Controles
- **Mouse:** clicar nos botões dos equipamentos.

## Como o código funciona (`fase1.c`)
- `ItemEPI epi[5]` — cada item tem `nome`, um `bit` (máscara de bits), o
  retângulo do `btn` e se já foi `usado`.
- `f1_equip` — máscara de bits do que já está vestido; `f1_vestidos` conta as peças.
- `InitF1()` — monta os botões e zera o estado.
- `UpdF1(mouse)` — ao clicar num item ainda não usado, liga o bit em `f1_equip`.
- `DrawF1(...)` — desenha o quartel e a **Cadete Clara** recebendo cada peça
  conforme o bitmask (a função `DrawClara` mostra o item só se o bit estiver ligado).
- **Vitória:** quando `f1_vestidos >= 5`.

> Depende do jogo completo: `DrawClara`, `DrawHUD`, `DBold`, constantes `EQ_*`.
