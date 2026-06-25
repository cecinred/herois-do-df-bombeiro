# Fase 3 — Preparação da Viatura 🚒

**Local:** Pátio do quartel.
**Personagem:** Bombeiro Silva (motorista) supervisiona.

## Objetivo
Carregar **só os itens corretos** da viatura — **mangueira, extintor, kit de
socorros, escada e machado** — e **ignorar os distratores** (videogame, skate,
pipoca).

## O que ensina
Quais equipamentos uma viatura de bombeiros leva e a importância de escolher
o material certo.

## Controles
- **Mouse:** clicar nos itens.

## Como o código funciona (`fase3.c`)
- `ItemVia f3[8]` — cada item tem `nome`, se é o `certo`, se foi `carregado`,
  um `icon` (qual desenho mostrar) e o retângulo do botão. A ordem dos itens é
  embaralhada para o jogo não ficar previsível.
- Cada item é desenhado como um **ícone** pela função compartilhada `DrawIcon`
  (mangueira, extintor, machado, escada, etc.), em vez de só texto.
- `f3_carregados` / `f3_total_certos` — contadores.
- `UpdF3(mouse)` — ao clicar: se o item é certo, carrega e conta; se for errado,
  mostra um aviso e perde uma vida; se zerar, vai para **GAME OVER**.
- `DrawF3(...)` — desenha o caminhão, o Bombeiro Silva e os botões com ícones.
- **Vitória:** quando `f3_carregados >= f3_total_certos`.

> Depende do jogo completo: `DrawIcon`, `DrawSilva`, `DrawHUD`, `DBold`.
