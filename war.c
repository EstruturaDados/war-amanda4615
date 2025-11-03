#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MAX 5 // tamanho fixo da fila

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_MAX];
    int frente; // índice da primeira peça
    int tras;   // índice do último elemento inserido
    int qtd;    // quantidade atual de peças na fila
} Fila;

// ----------- Protótipos -----------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
Peca gerarPeca(int id);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void exibirFila(Fila *f);
void menu();

// ----------- Função principal -----------
int main() {
    Fila fila;
    inicializarFila(&fila);
    srand((unsigned) time(NULL));

    // Inicializa com 5 peças
    int idAtual = 0;
    for (int i = 0; i < TAM_MAX; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    int opcao;
    do {
        printf("\n===== TETRIS STACK: FILA DE PEÇAS FUTURAS =====\n");
        exibirFila(&fila);
        menu();

        printf("\nEscolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // limpa buffer
            printf("Entrada inválida.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1: // Jogar peça
                if (filaVazia(&fila)) {
                    printf("⚠️  Nenhuma peça disponível para jogar.\n");
                } else {
                    Peca jogada = desenfileirar(&fila);
                    printf("🧩 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                }
                break;

            case 2: // Inserir nova peça
                if (filaCheia(&fila)) {
                    printf("⚠️  Fila cheia! Não é possível adicionar nova peça.\n");
                } else {
                    Peca nova = gerarPeca(idAtual++);
                    enfileirar(&fila, nova);
                    printf("✅ Nova peça gerada e adicionada: [%c %d]\n", nova.nome, nova.id);
                }
                break;

            case 0:
                printf("👋 Encerrando simulação. Até a próxima partida!\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ----------- Implementação das funções -----------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->qtd == 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->qtd == TAM_MAX;
}

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// Insere uma nova peça ao final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Erro: fila cheia. Não foi possível enfileirar.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAM_MAX;
    f->pecas[f->tras] = p;
    f->qtd++;
}

// Remove a peça da frente da fila (dequeue)
Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1}; // valor padrão, caso esteja vazia
    if (filaVazia(f)) {
        printf("Erro: fila vazia. Não foi possível desenfileirar.\n");
        return p;
    }
    p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_MAX;
    f->qtd--;
    return p;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\nFila de peças (%d/%d): ", f->qtd, TAM_MAX);
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }

    int i = f->frente;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
        i = (i + 1) % TAM_MAX;
    }
    printf("\n");
}

// Exibe menu de ações
void menu() {
    printf("\n--- Opções de ação ---\n");
    printf("1 - Jogar peça (dequeue)\n");
    printf("2 - Inserir nova peça (enqueue)\n");
    printf("0 - Sair\n");
}
