# Fase 6 — Primeiros Socorros 🩹

**Local:** Rua — a criança encontra uma pessoa caída.
**Personagem:** Bombeira Ana ajuda na cena.

## Objetivo
Escolher as ações corretas de socorro respondendo a um pequeno quiz.

## O que ensina
- Verificar a **consciência** da vítima com cuidado;
- Chamar ajuda (**192 — SAMU**);
- **Não movimentar** a vítima sem necessidade.

## Controles
- **Mouse:** clicar na resposta correta.

## Como o código funciona (`fase6.c`)
- Reaproveita o **motor de quiz genérico** (igual à Fase 2), com seu próprio
  conjunto de perguntas `f6_q[]` e total `F6_TOTAL`.
- `f6_passo` — pergunta atual; avança a cada acerto.
- `DrawF6(...)` — desenha a vítima caída, a Bombeira Ana e a faixa lembrando o
  número 192, e chama `QuizDraw`.
- **Vitória:** quando `f6_passo >= F6_TOTAL`.

> Depende do jogo completo: `QuizInit/QuizUpdate/QuizDraw`, `DrawAna`,
> `DrawHUD`, `DBold`.
