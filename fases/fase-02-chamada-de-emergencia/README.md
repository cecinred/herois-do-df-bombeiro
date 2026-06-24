# Fase 2 — Chamada de Emergência ☎️

**Local:** Bairro residencial — uma criança vê fumaça saindo de uma casa.

## Objetivo
Responder corretamente à sequência de perguntas: manter distância e chamar
um adulto → ligar **193** → informar endereço, o que aconteceu e se há vítimas.

## O que ensina
Os números de emergência (**193** Bombeiros, **192** SAMU, **190** Polícia) e
como agir e informar numa ligação de emergência.

## Controles
- **Mouse:** clicar na resposta correta.

## Como o código funciona (`fase2.c`)
- Usa o **motor de quiz genérico** do jogo (`QuizInit / QuizUpdate / QuizDraw`).
- `Quiz f2_q[]` — vetor de perguntas; cada uma tem o enunciado, 3 opções e o
  índice da `correta`. `F2_TOTAL` é o número de perguntas.
- `f2_passo` — pergunta atual; avança a cada acerto (o feedback verde/vermelho
  é controlado pelo quiz genérico).
- `DrawF2(...)` — desenha a casa com fumaça e a barra com os números de
  emergência, e chama `QuizDraw`.
- **Vitória:** quando `f2_passo >= F2_TOTAL`.

> Depende do jogo completo: `QuizInit/QuizUpdate/QuizDraw`, `DrawHUD`, `DBold`.
