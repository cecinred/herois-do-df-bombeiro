# Fase 10 — Grande Missão em Brasília 🏅

**Local:** Brasília — Esplanada, Catedral e Estádio (ocorrência combinada).

## Objetivo
Usar **tudo o que aprendeu** em 3 etapas seguidas. Ao concluir, a Cadete Clara
se torna **Bombeiro Honorário do DF** (e o **Museu** é desbloqueado no menu).

## As 3 etapas
1. **Ligação:** quiz rápido (qual número ligar, pode entrar sozinho?).
2. **Equipamentos:** escolher os 3 itens certos entre os disponíveis.
3. **Incêndio:** apagar os focos com a mangueira.

## Controles
- **Mouse:** etapas 1 e 2 (responder e escolher).
- **Setas + Espaço:** etapa 3 (mover, mirar e jogar água).

## Como o código funciona (`fase10.c`)
- `f10_etapa` — controla em qual das 3 etapas a fase está (`0`, `1`, `2`).
- **Etapa 0:** reaproveita o **quiz genérico** com `f10_q[]`.
- **Etapa 1:** `ItemVia f10_it[6]` (3 certos, 3 distratores); conta `f10_load`
  até `f10_need`.
- **Etapa 2:** `Foco f10_foco[4]` com as mesmas funções da mangueira.
- `UpdF10(...)` — devolve `true` quando a última etapa termina, sinalizando a
  conclusão da fase (e do jogo).
- `DrawF10BG()` desenha o cenário de Brasília ao fundo; `DrawF10` mostra a etapa atual.

> Depende do jogo completo: `QuizUpdate/QuizDraw`, `HoseControls/HoseMove/DrawFoco`,
> `DrawViaturaMangueira`, `DrawHUD`, `DBold`, structs `Quiz`, `Foco`, `ItemVia`.
