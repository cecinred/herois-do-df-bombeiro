# Fase 3 — Preparação da Viatura 🚒

**Local:** Pátio do quartel.
**Personagem:** Bombeiro Silva (motorista) supervisiona.

## Objetivo
Carregar **só os itens corretos** da viatura (mangueiras, escadas, extintores,
kit de primeiros socorros e equipamentos de resgate) e **ignorar os
distratores** (videogame, skate, pipoca).

## O que ensina
Quais equipamentos uma viatura de bombeiros leva e a importância de escolher
o material certo.

## Controles
- **Mouse:** clicar nos itens.

## Como o código funciona (`fase3.c`)
- `ItemVia f3[8]` — cada item tem `nome`, se é o `certo`, se foi `carregado` e
  o retângulo do botão. (Na versão atual a ordem dos itens é embaralhada para
  o jogo não ficar previsível.)
- `f3_carregados` / `f3_total_certos` — contadores.
- `UpdF3(mouse)` — ao clicar: se o item é certo, carrega e conta; se for errado,
  mostra um aviso (não soma).
- `DrawF3(...)` — desenha o caminhão, o Bombeiro Silva e os botões dos itens.
- **Vitória:** quando `f3_carregados >= f3_total_certos`.

> Depende do jogo completo: `DrawSilva`, `DrawHUD`, `DBold`.
