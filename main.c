#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// --- Definições Globais ---
#define TAMANHO 3
#define N_PECAS (TAMANHO * TAMANHO)
#define MAX_PROFUNDIDADE 30 

typedef int Tabuleiro[TAMANHO][TAMANHO];

// Estado meta
Tabuleiro estado_meta = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

// Estrutura para um estado do tabuleiro (Nó)
typedef struct No {
    Tabuleiro estado;
    int g_custo;            
    int h_custo;            
    int f_custo;            
    struct No *pai;         
    char movimento;         
    int profundidade;       
} No;

// Estrutura genérica para a FILA/PILHA
typedef struct ListaBusca {
    No *no;
    struct ListaBusca *proximo;
} ListaBusca;

// --- Funções de Manipulação da Lista Genérica ---

ListaBusca *adicionar_estado(ListaBusca *cabeca, No *novo_no, char tipo_busca) {
    ListaBusca *novo_elemento = (ListaBusca *)malloc(sizeof(ListaBusca));
    if (!novo_elemento) {
        perror("Erro de alocação de memória (adicionar_estado)");
        exit(EXIT_FAILURE);
    }
    novo_elemento->no = novo_no;
    novo_elemento->proximo = NULL;

    if (cabeca == NULL) {
        return novo_elemento;
    }

    if (tipo_busca == 'D') {
        // PILHA (LIFO) para DFS/IDDFS: Adiciona no início
        novo_elemento->proximo = cabeca;
        return novo_elemento;
    } else if (tipo_busca == 'A') {
        // A* (Fila de Prioridade): Inserir ordenado pelo menor f_custo
        if (novo_no->f_custo < cabeca->no->f_custo) {
            novo_elemento->proximo = cabeca;
            return novo_elemento;
        }
        ListaBusca *atual = cabeca;
        while (atual->proximo != NULL && atual->proximo->no->f_custo <= novo_no->f_custo) {
            atual = atual->proximo;
        }
        novo_elemento->proximo = atual->proximo;
        atual->proximo = novo_elemento;
        return cabeca;
    } 
    // Para BFS (Fila FIFO): Adiciona no final
    ListaBusca *atual = cabeca;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novo_elemento;
    return cabeca;
}

ListaBusca *remover_proximo_estado(ListaBusca *cabeca, No **removido) {
    if (cabeca == NULL) {
        *removido = NULL;
        return NULL;
    }

    ListaBusca *temp = cabeca;
    *removido = temp->no;
    cabeca = cabeca->proximo;
    free(temp);
    return cabeca;
}

void liberar_lista(ListaBusca *cabeca) {
    ListaBusca *temp;
    while (cabeca != NULL) {
        temp = cabeca;
        cabeca = cabeca->proximo;
        free(temp->no); 
        free(temp);     
    }
}

// --- Funções Básicas do Tabuleiro ---

void copiar_tabuleiro(Tabuleiro origem, Tabuleiro destino) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            destino[i][j] = origem[i][j];
        }
    }
}

void imprimir_tabuleiro(Tabuleiro t) {
    printf("+---+---+---+\n");
    for (int i = 0; i < TAMANHO; i++) {
        printf("|");
        for (int j = 0; j < TAMANHO; j++) {
            if (t[i][j] == 0) {
                printf("   |");
            } else {
                printf(" %d |", t[i][j]);
            }
        }
        printf("\n+---+---+---+\n");
    }
}

void encontrar_vazio(Tabuleiro t, int *linha, int *coluna) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (t[i][j] == 0) {
                *linha = i;
                *coluna = j;
                return;
            }
        }
    }
}

int mover_peca(Tabuleiro t, int r_vazio, int c_vazio, int r_destino, int c_destino) {
    if (r_destino >= 0 && r_destino < TAMANHO && c_destino >= 0 && c_destino < TAMANHO) {
        int temp = t[r_vazio][c_vazio];
        t[r_vazio][c_vazio] = t[r_destino][c_destino];
        t[r_destino][c_destino] = temp;
        return 1;
    }
    return 0;
}

int sao_iguais(Tabuleiro t1, Tabuleiro t2) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (t1[i][j] != t2[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

int ja_visitado(No *no, Tabuleiro estado) {
    No *atual = no->pai;
    while (atual != NULL) {
        if (sao_iguais(atual->estado, estado)) {
            return 1;
        }
        atual = atual->pai;
    }
    return 0;
}


// --- Funções de Heurística (Para A*) ---

int distancia_manhattan(Tabuleiro t) {
    int distancia = 0;
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            int valor = t[i][j];
            if (valor != 0) {
                int linha_meta = (valor - 1) / TAMANHO;
                int col_meta = (valor - 1) % TAMANHO;
                
                if (valor == N_PECAS - 1 && estado_meta[TAMANHO - 1][TAMANHO - 1] == 0) {
                    linha_meta = TAMANHO - 1;
                    col_meta = TAMANHO - 1;
                }
                
                distancia += abs(i - linha_meta) + abs(j - col_meta);
            }
        }
    }
    return distancia;
}


// --- Funções de Solução ---

No *criar_novo_no(No *pai, Tabuleiro novo_estado, char movimento, char tipo_busca) {
    No *novo = (No *)malloc(sizeof(No));
    if (!novo) {
        perror("Erro de alocação de memória (criar_novo_no)");
        exit(EXIT_FAILURE);
    }
    
    copiar_tabuleiro(novo_estado, novo->estado);
    novo->pai = pai;
    novo->movimento = movimento;
    novo->g_custo = pai->g_custo + 1;
    novo->profundidade = novo->g_custo;
    
    if (tipo_busca == 'A') {
        novo->h_custo = distancia_manhattan(novo->estado);
        novo->f_custo = novo->g_custo + novo->h_custo;
    } else {
        novo->h_custo = 0;
        novo->f_custo = 0;
    }
    
    return novo;
}

void mostrar_solucao(No *final, int visitados) {
    printf("\n--- 🏁 Solução Encontrada! 🏁 ---\n");
    printf("Estados visitados: %d\n", visitados);
    printf("Profundidade da Solução: %d passos\n", final->g_custo);

    int passos = final->g_custo;
    No **caminho = (No **)malloc(sizeof(No *) * (passos + 1));
    if (!caminho) {
        perror("Erro de alocação de memória (caminho)");
        return;
    }
    
    No *atual = final;
    for (int i = passos; i >= 0; i--) {
        caminho[i] = atual;
        atual = atual->pai;
    }

    printf("\nPassos para a Solução:\n");
    for (int i = 0; i <= passos; i++) {
        printf("\nPasso %d (Movimento: %c):\n", i, caminho[i]->movimento);
        imprimir_tabuleiro(caminho[i]->estado);
    }
    
    free(caminho);
}


void IA_solucionar(Tabuleiro estado_inicial, char tipo_busca) {
    ListaBusca *fronteira = NULL;
    int visitados = 0;
    int r_vazio, c_vazio;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char movimentos[] = {'D', 'U', 'R', 'L'};

    for (int limite = 0; limite < MAX_PROFUNDIDADE; limite++) {
        
        if (tipo_busca == 'I') {
            liberar_lista(fronteira);
            fronteira = NULL;
            visitados = 0;
        } else if (limite > 0 && tipo_busca == 'A') {
            break;
        }

        printf("\n--- Buscando: %s (Limite Profundidade: %d) ---\n", 
               (tipo_busca == 'A' ? "Algoritmo A*" : "IDDFS"), limite);

        No *raiz = (No *)malloc(sizeof(No));
        if (!raiz) {
            perror("Erro de alocação de memória (raiz)");
            return;
        }
        copiar_tabuleiro(estado_inicial, raiz->estado);
        raiz->pai = NULL;
        raiz->g_custo = 0;
        raiz->movimento = 'S';
        raiz->profundidade = 0;
        if (tipo_busca == 'A') {
            raiz->h_custo = distancia_manhattan(raiz->estado);
            raiz->f_custo = raiz->g_custo + raiz->h_custo;
        } else {
            raiz->h_custo = 0;
            raiz->f_custo = 0;
        }
        
        fronteira = adicionar_estado(fronteira, raiz, (tipo_busca == 'A' ? 'A' : 'D'));

        No *estado_atual;
        while (fronteira != NULL) {
            
            fronteira = remover_proximo_estado(fronteira, &estado_atual);
            visitados++;

            if (sao_iguais(estado_atual->estado, estado_meta)) {
                mostrar_solucao(estado_atual, visitados);
                liberar_lista(fronteira);
                return;
            }
            
            if (tipo_busca == 'I' && estado_atual->profundidade >= limite) {
                continue;
            }

            encontrar_vazio(estado_atual->estado, &r_vazio, &c_vazio);

            for (int i = 0; i < 4; i++) {
                int r_novo = r_vazio + dr[i];
                int c_novo = c_vazio + dc[i];

                Tabuleiro proximo_estado;
                copiar_tabuleiro(estado_atual->estado, proximo_estado);

                if (mover_peca(proximo_estado, r_vazio, c_vazio, r_novo, c_novo)) {
                    
                    if (!ja_visitado(estado_atual, proximo_estado)) {
                        No *novo_no = criar_novo_no(estado_atual, proximo_estado, movimentos[i], tipo_busca);
                        fronteira = adicionar_estado(fronteira, novo_no, (tipo_busca == 'A' ? 'A' : 'D'));
                    }
                }
            }
        }
        
        if (tipo_busca == 'A') {
            printf("\nNão foi possível encontrar a solução dentro dos limites de memória/tempo.\n");
            liberar_lista(fronteira);
            return;
        }
        if (tipo_busca == 'I' && limite == MAX_PROFUNDIDADE - 1) {
            printf("\nBusca em Profundidade Limitada Iterativa não encontrou solução até a profundidade %d.\n", MAX_PROFUNDIDADE);
            return;
        }
    }
}


// --- Lógica do Jogo (Opção 1) ---

void modo_jogo(Tabuleiro jogo) {
    char direcao;
    int r_vazio, c_vazio;
    int movimento_valido;

    printf("\n--- 🕹️ MODO JOGADOR ---\n");
    printf("Comandos: U (Cima), D (Baixo), L (Esquerda), R (Direita). Q para voltar ao menu.\n");

    while (1) {
        printf("\n--------------------------\n");
        printf("Tabuleiro Atual:\n");
        imprimir_tabuleiro(jogo);

        if (sao_iguais(jogo, estado_meta)) {
            printf("\n🎉 PARABÉNS! Você resolveu o Puzzle! 🎉\n");
            break;
        }

        printf("\nSua jogada (U/D/L/R/Q): ");
        if (scanf(" %c", &direcao) != 1) continue;
        direcao = toupper(direcao);

        if (direcao == 'Q') {
            printf("Voltando ao menu principal.\n");
            break;
        }

        encontrar_vazio(jogo, &r_vazio, &c_vazio);
        movimento_valido = 0;

        switch (direcao) {
            case 'U': 
                movimento_valido = mover_peca(jogo, r_vazio, c_vazio, r_vazio + 1, c_vazio);
                break;
            case 'D': 
                movimento_valido = mover_peca(jogo, r_vazio, c_vazio, r_vazio - 1, c_vazio);
                break;
            case 'L': 
                movimento_valido = mover_peca(jogo, r_vazio, c_vazio, r_vazio, c_vazio + 1);
                break;
            case 'R': 
                movimento_valido = mover_peca(jogo, r_vazio, c_vazio, r_vazio, c_vazio - 1);
                break;
            default:
                printf("\nComando desconhecido.\n");
                break;
        }

        if (!movimento_valido && (direcao == 'U' || direcao == 'D' || direcao == 'L' || direcao == 'R')) {
             printf("\n🚫 Movimento inválido! O espaço vazio está no limite.\n");
        }
    }
}

// --- Menu Principal e Inicialização ---

void embaralhar_tabuleiro(Tabuleiro t, int movimentos) {
    srand(time(NULL));
    int r_vazio, c_vazio;
    
    copiar_tabuleiro(estado_meta, t); 

    for (int i = 0; i < movimentos; i++) {
        encontrar_vazio(t, &r_vazio, &c_vazio);
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        int direcao_aleatoria;
        
        do {
            direcao_aleatoria = rand() % 4;
        } while (mover_peca(t, r_vazio, c_vazio, r_vazio + dr[direcao_aleatoria], c_vazio + dc[direcao_aleatoria]) == 0);
    }
}

int main() {
    int opcao;
    Tabuleiro tabuleiro_jogo;
    
    copiar_tabuleiro(estado_meta, tabuleiro_jogo);

    while (1) {
        printf("\n==================================\n");
        printf("  🧩 SIMULADOR 8-PUZZLE (IA/JOGAR) \n");
        printf("==================================\n");
        printf("1. 🏃 Iniciar Novo Jogo (Tabuleiro Embaralhado)\n");
        printf("2. 🤖 Mostrar Solução (Usando IA)\n");
        printf("3. 🚪 Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            opcao = 0;
            printf("\nOpção inválida. Por favor, digite um número.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                embaralhar_tabuleiro(tabuleiro_jogo, 50);
                printf("\n✨ Novo jogo embaralhado! Comece a jogar. ✨\n");
                modo_jogo(tabuleiro_jogo);
                break;
            
            case 2:
                // AGORA: Embaralha o tabuleiro sempre que a IA for acionada
                embaralhar_tabuleiro(tabuleiro_jogo, 50);
                printf("\n✨ Tabuleiro embaralhado para a IA resolver: ✨\n");
                imprimir_tabuleiro(tabuleiro_jogo);
                
                printf("\n--- Escolha a IA Solucionadora ---\n");
                printf("1. Algoritmo A* (Heurística Distância de Manhattan)\n");
                printf("2. Busca em Profundidade Limitada Iterativa (IDDFS)\n");
                printf("Escolha (1 ou 2): ");
                int ia_opcao;
                if (scanf(" %d", &ia_opcao) != 1) ia_opcao = 0; 
                
                if (ia_opcao == 1) {
                    IA_solucionar(tabuleiro_jogo, 'A');
                } else if (ia_opcao == 2) {
                    IA_solucionar(tabuleiro_jogo, 'I');
                } else {
                    printf("Opção de IA inválida.\n");
                }
                break;
                
            case 3:
                printf("Obrigado por jogar! Encerrando o simulador.\n");
                return 0;
                
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }
    }
    
    return 0;
}
