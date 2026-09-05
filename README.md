# 🧩 8-Puzzle em C com Inteligência Artificial

---

Este projeto implementa o clássico **quebra-cabeça 8-Puzzle**, onde o objetivo é organizar os números de **1 a 8** em ordem crescente, deixando o espaço vazio (0) na última posição.

O projeto foi totalmente desenvolvido em C e permite tanto **jogo manual** quanto **resolução automática** utilizando algoritmos de busca (IA).

---

## 🎮 Modos de Execução

Ao iniciar o programa, o usuário pode escolher entre:

1. 🏃 **Iniciar Novo Jogo** (Tabuleiro embaralhado para jogar manualmente)
2. 🤖 **Mostrar Solução** (Resolução automática pela IA)
3. 🚪 **Sair**

---

## 🧠 Algoritmos Utilizados

🔹 **Algoritmo A* (A-Star)**
* Garante encontrar a solução ótima (caminho mais curto)
* Utiliza a heurística da **Distância de Manhattan** para guiar a busca
* Usa uma fila de prioridade para controle dos nós, baseada no menor custo total

🔹 **Busca em Profundidade Limitada Iterativa (IDDFS)**
* Explora caminhos em profundidade até um limite definido, aumentando-o a cada iteração
* Usa estrutura de pilha (LIFO)
* Combina a eficiência de memória da busca em profundidade com a garantia de encontrar a solução da busca em largura

---

## 📁 Estrutura do Projeto

```text
📦 8-puzzle-c
├── 8puzzle.c   # Contém toda a implementação do jogo, estruturas e algoritmos de busca
└── README.md   # Documentação do projeto

## ▶️ Como Compilar e Executar
1️⃣ Compilar
Abra o terminal na pasta do projeto e execute:

Bash
gcc 8puzzle.c -o puzzle

2️⃣ Executar
No Linux ou macOS:

Bash
./puzzle

No Windowns:
DOS
puzzle.exe

⚠️ Observação: Certifique-se de ter um compilador C (como o GCC) devidamente instalado e configurado nas variáveis de ambiente do seu sistema operacional.

📄 Licença
Este projeto está licenciado sob a Licença MIT - veja o arquivo LICENSE para obter detalhes.
