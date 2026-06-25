# Fase 4 — Deslocamento pela Cidade 🚦

**Local:** Percurso Quartel → Esplanada → Catedral → ocorrência.
**Personagem:** Bombeiro Silva dirige.

## Objetivo
Dirigir a viatura até o local da ocorrência **desviando dos carros**, usando
a **sirene** para abrir o corredor de emergência.

## O que ensina
A importância da sirene, do **corredor de emergência** e do trânsito seguro
(dar passagem para a viatura).

## Controles
- **Setas / A,D:** mover a viatura para os lados.
- **Espaço:** ligar a sirene (afasta os carros da frente).

## Como o código funciona (`fase4.c`)
- `Carro f4_cars[8]` — carros civis que descem pela pista.
- `f4_truckx` (posição), `f4_prog` (progresso 0–100), `f4_sirene`, `f4_inv`
  (tempo de invulnerabilidade após uma batida).
- `UpdF4(...)` — move os carros; com a **sirene** ligada eles se afastam para
  as laterais (corredor de emergência). Em colisão, perde uma vida; se zerar,
  vai para **GAME OVER**.
- `DrawF4(...)` — pista com faixas animadas, prédios, carros e a barra de
  progresso com os marcos do percurso.
- **Vitória:** quando `f4_prog >= 100`.

> Depende do jogo completo: `DrawHUD`, `DBold`, sistema de partículas, `enum Estado`.
