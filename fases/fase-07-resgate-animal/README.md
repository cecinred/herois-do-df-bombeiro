# Fase 7 — Resgate Animal 🐶

**Local:** Parque da Cidade — um cachorro ficou preso.

## Objetivo
Mover a **cesta do guindaste** até ficar alinhada com o cachorro e descê-la
para resgatá-lo com segurança.

## O que ensina
Respeito aos animais e a importância de um resgate **calmo e seguro**.

## Controles
- **Setas / A,D:** mover a cesta na horizontal.
- **Espaço:** descer a cesta.

## Como o código funciona (`fase7.c`)
- `f7_hx`, `f7_hy` — posição do gancho/cesta.
- `f7_dx`, `f7_dy` — posição do cachorro (sorteada).
- `f7_st` — **máquina de estados** da cesta: `0` mover, `1` descendo, `2` subindo.
- `f7_got` — se a cesta pegou o cão; `f7_win` — se o resgate foi concluído.
- `UpdF7(...)`:
  - estado **0**: o jogador move a cesta e aperta Espaço para descer;
  - estado **1**: a cesta desce; ao chegar na altura do cão, verifica se está
    **alinhada** (tolerância em x); se sim, pega;
  - estado **2**: a cesta sobe (com o cão, se pegou). Se voltou vazia, tenta de novo.
- **Vitória:** quando `f7_win` fica verdadeiro.

> Depende do jogo completo: `DrawDog`, `DrawHUD`, `DBold`.
