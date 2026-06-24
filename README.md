# 🚒 Heróis do DF: Missão Bombeiro

Jogo educativo em **C + Raylib** que ensina crianças (6 a 12 anos) a agir
corretamente em situações de emergência e a conhecer o trabalho do
**Corpo de Bombeiros Militar do Distrito Federal (CBMDF)**.

O jogador assume o papel da **Cadete Clara**, uma jovem que sonha em ser
bombeira, e passa por **10 fases** até se tornar **Bombeiro Honorário do DF**.

---

## 🎯 Objetivo educativo

Ao longo do jogo, a criança aprende a:

- Ligar **193** (Bombeiros), e diferenciar de **192** (SAMU) e **190** (Polícia);
- Nunca se aproximar de incêndios e nunca brincar com fogo;
- Sempre chamar um adulto responsável;
- Informar o endereço corretamente;
- Noções de primeiros socorros, resgate e preservação ambiental.

---

## 🕹️ Como jogar

- **Setas** ou **A / D / W / S**: mover / mirar
- **Espaço**: ação principal (água, sirene, descer a cesta, etc.)
- **Mouse**: clicar nos botões dos minigames e menus
- **F11**: alternar janela / tela cheia
- **ESC**: sair

---

## 🧑‍🚒 Personagens

| Personagem | Papel |
|---|---|
| **Capitão Almeida** | Mentor — explica todas as mecânicas |
| **Cadete Clara** | Personagem principal (jogadora) |
| **Bombeira Ana** | Especialista em resgates |
| **Bombeiro Silva** | Motorista da viatura |

---

## 🗺️ As 10 fases

| # | Fase | Local | Minigame |
|---|------|-------|----------|
| 1 | Conhecendo o Quartel | Quartel Central | Vestir o EPI |
| 2 | Chamada de Emergência | Bairro residencial | Ligar 193 e responder o atendente |
| 3 | Preparação da Viatura | Pátio do quartel | Carregar os itens certos |
| 4 | Deslocamento pela Cidade | Esplanada/Catedral | Dirigir a viatura com sirene |
| 5 | Incêndio Residencial | Casa em chamas | Apagar os focos com a mangueira |
| 6 | Primeiros Socorros | Rua | Quiz de socorro (chamar 192) |
| 7 | Resgate Animal | Parque da Cidade | Resgatar o cachorro com o guindaste |
| 8 | Acidente de Trânsito | Próximo ao estádio | Isolar a área com cones |
| 9 | Temporada de Queimadas | Área rural do DF | Conter os focos (com vento) |
| 10 | Grande Missão | Brasília | Tudo junto: ligação + equipamentos + fogo |

Ao concluir as 10 fases, o **Museu dos Bombeiros** é desbloqueado no menu,
com História do CBMDF, veículos antigos, equipamentos, curiosidades e a
galeria de medalhas.

---

## 🛠️ Como compilar e rodar

O jogo depende da biblioteca **[Raylib](https://www.raylib.com/)**.
Instale-a primeiro (veja o site oficial) e depois compile:

### Windows (MinGW)
```bash
gcc jogo-completo/HeroisDoDF_Bombeiro.c -o herois.exe -lraylib -lopengl32 -lgdi32 -lwinmm
herois.exe
```

### Linux
```bash
gcc jogo-completo/HeroisDoDF_Bombeiro.c -o herois -lraylib -lm -lpthread -ldl -lrt -lX11
./herois
```

### macOS
```bash
gcc jogo-completo/HeroisDoDF_Bombeiro.c -o herois -lraylib -framework OpenGL -framework Cocoa -framework IOKit
./herois
```

> ⚠️ **Codificação:** o arquivo está em **UTF-8** (necessário para os acentos).
> Ao abrir/salvar no editor, mantenha UTF-8 para não quebrar a acentuação.

---

## 📂 Organização do repositório

```
HeroisDoDF-MissaoBombeiro/
├── README.md                  ← este arquivo
├── LICENSE
├── .gitignore
│
├── jogo-completo/
│   ├── README.md
│   └── HeroisDoDF_Bombeiro.c  ← JOGO INTEIRO (este é o que compila!)
│
└── fases/
    ├── README.md
    ├── fase-01-conhecendo-o-quartel/   (fase1.c + explicação)
    ├── fase-02-chamada-de-emergencia/
    ├── fase-03-preparacao-da-viatura/
    ├── fase-04-deslocamento-pela-cidade/
    ├── fase-05-incendio-residencial/
    ├── fase-06-primeiros-socorros/
    ├── fase-07-resgate-animal/
    ├── fase-08-acidente-de-transito/
    ├── fase-09-temporada-de-queimadas/
    └── fase-10-grande-missao-em-brasilia/
```

A pasta **`fases/`** tem o código de cada fase **separado, para estudo e
explicação**. Esses arquivos **não compilam sozinhos** (dependem da Raylib e
de partes compartilhadas). O **jogo que compila e roda** é o arquivo único em
**`jogo-completo/`**.

---

## 👩‍💻 Autores

Projeto desenvolvido pelos alunos: **Amanda Soares, Cecilia Alemi,
Eduarda Moura, Isabelle Reis, Mariana Nogueira, Maria Jhuliana Brito
e Thiago Campêlo.**

Inspirado no trabalho do **Corpo de Bombeiros Militar do Distrito Federal**.

---

## 📜 Licença

Distribuído sob a licença **MIT** — veja o arquivo [LICENSE](LICENSE).
