# 🧩 Simulador 8-Puzzle (em C) - Inteligência Atificial

Projeto desenvolvido em **linguagem C** para implementação e estudo de algoritmos de **Inteligência Artificial aplicados ao problema do 8-Puzzle**.

O sistema permite jogar manualmente ou utilizar algoritmos de busca para encontrar automaticamente uma solução para o tabuleiro.

---

## 📌 Sobre o Projeto

O **8-Puzzle** é um problema clássico de Inteligência Artificial composto por um tabuleiro **3×3**, contendo oito peças numeradas e um espaço vazio representado pelo número `0`.

O objetivo é movimentar as peças até alcançar o estado final:

```text
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 7 | 8 |   |
+---+---+---+
```

Neste projeto, o usuário pode:

* 🎮 Jogar manualmente;
* 🤖 Resolver o puzzle utilizando **A***;
* 🧠 Resolver utilizando **IDDFS**;
* 🔀 Gerar tabuleiros embaralhados automaticamente;
* 📊 Visualizar a quantidade de estados visitados;
* 🏁 Visualizar a sequência de movimentos utilizada para chegar à solução.

---

## 🚀 Funcionalidades

### 🎮 Modo Jogador

O usuário pode controlar o tabuleiro através dos comandos:

| Comando | Movimento      |
| ------- | -------------- |
| `U`     | Cima           |
| `D`     | Baixo          |
| `L`     | Esquerda       |
| `R`     | Direita        |
| `Q`     | Voltar ao menu |

Os comandos são convertidos para maiúsculas automaticamente, permitindo também a utilização de letras minúsculas.

---

### 🤖 Algoritmo A*

O projeto implementa o algoritmo **A*** para encontrar uma solução.

O custo de cada estado é calculado através da fórmula:

```text
f(n) = g(n) + h(n)
```

Onde:

* `g(n)` → custo do caminho percorrido;
* `h(n)` → estimativa do custo restante;
* `f(n)` → custo total estimado.

A heurística utilizada é a **Distância de Manhattan**.

```text
h(n) = Σ |linha_atual - linha_objetivo|
             + |coluna_atual - coluna_objetivo|
```

A implementação mantém a fronteira ordenada pelo menor `f_custo`, fazendo com que os estados mais promissores sejam explorados primeiro.

---

### 🧠 IDDFS

Também foi implementado o algoritmo **IDDFS — Iterative Deepening Depth-First Search**.

O algoritmo combina características da:

* Busca em Profundidade (DFS);
* Busca em Largura (BFS).

A profundidade máxima utilizada pelo programa é:

```c
#define MAX_PROFUNDIDADE 30
```

O algoritmo realiza sucessivas buscas aumentando gradualmente o limite de profundidade.

---

## 🏗️ Estrutura do Projeto

O programa possui diferentes estruturas e funções responsáveis pelo funcionamento do simulador.

### Tabuleiro

O tabuleiro é representado por uma matriz:

```c
typedef int Tabuleiro[TAMANHO][TAMANHO];
```

Com:

```c
#define TAMANHO 3
```

---

### Estrutura de um Estado

Cada estado do problema é representado pela estrutura `No`:

```c
typedef struct No {
    Tabuleiro estado;
    int g_custo;
    int h_custo;
    int f_custo;
    struct No *pai;
    char movimento;
    int profundidade;
} No;
```

Ela armazena informações como:

* Estado atual do tabuleiro;
* Custo `g`;
* Heurística `h`;
* Custo total `f`;
* Estado pai;
* Movimento realizado;
* Profundidade da busca.

---

### Lista de Busca

O projeto utiliza uma lista encadeada para representar a fronteira dos algoritmos:

```c
typedef struct ListaBusca {
    No *no;
    struct ListaBusca *proximo;
} ListaBusca;
```

Dependendo do algoritmo utilizado, a lista funciona como:

* **Fila FIFO** → BFS;
* **Pilha LIFO** → DFS/IDDFS;
* **Fila de prioridade** → A*.

---

## 🔀 Embaralhamento

Antes de iniciar um jogo ou uma resolução pela IA, o tabuleiro é embaralhado automaticamente.

O programa parte do estado objetivo e realiza **50 movimentos aleatórios válidos**:

```c
embaralhar_tabuleiro(tabuleiro_jogo, 50);
```

Isso garante que o estado inicial seja obtido através de movimentos válidos a partir da solução.

---

## 🔍 Controle de Estados Visitados

Para evitar que o algoritmo fique retornando imediatamente para estados anteriores do mesmo caminho, o programa verifica os estados presentes nos ancestrais do nó atual.

A função responsável por isso é:

```c
ja_visitado()
```

Ela percorre os estados pais e verifica se o novo estado já apareceu anteriormente no caminho.

---

## 🏁 Exibição da Solução

Quando uma solução é encontrada, o programa apresenta:

* Quantidade de estados visitados;
* Profundidade da solução;
* Sequência de movimentos;
* Tabuleiro correspondente a cada passo.

Exemplo:

```text
--- 🏁 Solução Encontrada! 🏁 ---

Estados visitados: 125
Profundidade da Solução: 18 passos

Passos para a Solução:

Passo 0
+---+---+---+
| 2 | 8 | 3 |
+---+---+---+
| 1 | 6 | 4 |
+---+---+---+
| 7 | 5 |   |
+---+---+---+
```

---

## 🖥️ Menu Principal

Ao executar o programa, o usuário encontra:

```text
==================================
  🧩 SIMULADOR 8-PUZZLE (IA/JOGAR)
==================================

1. 🏃 Iniciar Novo Jogo (Tabuleiro Embaralhado)
2. 🤖 Mostrar Solução (Usando IA)
3. 🚪 Sair
```

Ao selecionar a opção de IA:

```text
--- Escolha a IA Solucionadora ---

1. Algoritmo A* (Heurística Distância de Manhattan)
2. Busca em Profundidade Limitada Iterativa (IDDFS)
```

---

## 🛠️ Tecnologias Utilizadas

* **C**
* Estruturas de dados
* Ponteiros
* Alocação dinâmica de memória
* Listas encadeadas
* Recursividade/estratégias de busca
* Algoritmos de Inteligência Artificial
* Heurística de Distância de Manhattan

Bibliotecas utilizadas:

```c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
```

---

## ▶️ Como Executar

### 1. Clone o repositório

```bash
git clone https://github.com/Rouxxz/Simulador-8-Puzzle-Em-C-.git
```

### 2. Entre na pasta

```bash
cd nome-do-projeto
```

### 3. Compile o programa

Utilizando o GCC:

```bash
gcc main.c -o puzzle
```

### 4. Execute

No Linux/macOS:

```bash
./puzzle
```

No Windows:

```bash
puzzle.exe
```

---

## 📚 Conceitos de Inteligência Artificial

Este projeto permite aplicar conceitos importantes de **Busca em Espaço de Estados**, incluindo:

* Busca em profundidade;
* Busca em profundidade iterativa;
* Busca heurística;
* Algoritmo A*;
* Função de custo;
* Heurísticas;
* Estados e transições;
* Árvore de busca;
* Controle de estados repetidos.

---

## 📈 Comparação dos Algoritmos

| Característica               | A*                          | IDDFS         |
| ---------------------------- | --------------------------- | ------------- |
| Utiliza heurística           | ✅                           | ❌             |
| Distância de Manhattan       | ✅                           | ❌             |
| Busca em profundidade        | ❌                           | ✅             |
| Limite de profundidade       | Não como critério principal | ✅             |
| Utiliza custo `g(n)`         | ✅                           | Indiretamente |
| Prioriza estados promissores | ✅                           | ❌             |
| Implementado no projeto      | ✅                           | ✅             |

---

## 🎯 Objetivos Acadêmicos

O projeto foi desenvolvido com o objetivo de aplicar, na prática, conceitos de **Inteligência Artificial e Algoritmos de Busca**, utilizando o problema clássico do 8-Puzzle como ambiente de experimentação.

Entre os principais objetivos estão:

* Compreender a representação de estados;
* Implementar algoritmos de busca;
* Trabalhar com estruturas de dados em C;
* Aplicar heurísticas;
* Comparar diferentes estratégias de resolução;
* Analisar a quantidade de estados explorados até encontrar uma solução.

---

## 👨‍💻 Autor

**Arthur Bergamasco Constantino**

Estudante de **Ciência de Dados e Inteligência Artificial**.

---

## 📄 Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo LICENSE para obter detalhes.
