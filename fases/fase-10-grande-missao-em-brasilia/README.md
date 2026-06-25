# Fase 10 — Grande Missão em Brasília 🏅

**Local:** Brasília — Esplanada, Catedral, Congresso e Estádio (ocorrência combinada).

## Objetivo
Usar **tudo o que aprendeu** em etapas seguidas. Ao concluir, a Cadete Clara
se torna **Bombeiro Honorário do DF** (e o **Museu** é desbloqueado no menu).

## As etapas
1. **Ligação:** quiz rápido (qual número ligar, pode entrar sozinho?).
2. **Equipamentos:** escolher os 3 itens certos (mangueira, extintor, kit) entre
   os disponíveis, mostrados como ícones.
3. **Incêndio + resgate:** apagar os focos com a mangueira **e resgatar as 3
   vítimas**.

## Controles
- **Mouse:** etapas 1 e 2 (responder e escolher).
- **Setas + Espaço:** etapa 3 (mover, mirar e jogar água).

## Como o código funciona (`fase10.c`)
- `f10_etapa` — controla em qual etapa a fase está (`0`, `1`, `2`).
- **Etapa 0:** reaproveita o **quiz genérico** com `f10_q[]`.
- **Etapa 1:** `ItemVia f10_it[6]` (3 certos, 3 distratores), desenhados com
  `DrawIcon`; conta `f10_load` até `f10_need`.
- **Etapa 2:** `Foco f10_foco[4]` (mangueira) **e** `f10_vits[3]` (vítimas a
  resgatar); `f10_apag` conta focos apagados e `f10_vsalvas` conta as vítimas salvas.
- `UpdF10(...)` — devolve `true` quando a missão termina (todos os focos
  apagados e as vítimas salvas), sinalizando o fim do jogo.
- `DrawF10` desenha o cenário de Brasília ao fundo e a etapa atual.

> Depende do jogo completo: `QuizUpdate/QuizDraw`, `HoseControls/HoseMove/DrawFoco`,
> `DrawIcon`, `DrawViaturaMangueira`, cenários de Brasília, `DrawHUD`, `DBold`.
