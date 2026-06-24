# Fase 5 — Incêndio Residencial 🔥

**Local:** Casa em chamas (a ocorrência).

## Objetivo
Apagar os **6 focos de incêndio** (nos dois andares da casa) com a mangueira
da viatura.

## O que ensina
Nunca entrar sozinho, nunca brincar com fogo e manter distância.

## Controles
- **Setas / A,D:** mover a viatura.
- **Setas ↑↓ / W,S:** ajustar o ângulo da mangueira.
- **Espaço:** lançar o jato de água.

## Como o código funciona (`fase5.c`)
- `Foco f5_foco[6]` — cada foco tem posição e intensidade de `fogo` (0 a 1),
  posicionado no **centro de cada janela**.
- `f5_cx` (posição da viatura) e `f5_ang` (ângulo da mangueira).
- Usa as funções **compartilhadas da mangueira**: `HoseControls` (mover/mirar/
  atirar), `HoseMove` (física do jato e colisão com os focos) e `DrawFoco`.
- O jato é um projétil com gravidade; a força foi ajustada para **alcançar os
  dois andares** (dá para parar embaixo de uma janela e atirar reto para cima).
- **Vitória:** quando todos os focos são apagados (`f5_apag >= f5_total`).

> Depende do jogo completo: `HoseControls/HoseMove/HoseFire/DrawFoco`,
> `DrawViaturaMangueira`, `DrawHUD`, `DBold`, struct `Foco`.
