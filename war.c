#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // tipo: 'I', 'O', 'T', 'L'
    int id;     // identificador único
} Peca;

// ==================== FILA CIRCULAR ====================

typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Insere uma peça no final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("A fila está cheia!\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
}

// Remove uma peça do início da fila (dequeue)
Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) {
        printf("A fila está vazia!\n");
        return removida;
    }
    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return removida;
}

// Exibe o conteúdo atual da fila
void exibirFila(Fila *f) {
    printf("Fila de peças: ");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }

    int i = f->inicio;
    for (int c = 0; c < f->tamanho; c++) {
        printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

// ==================== PILHA ====================

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// Inicializa a pilha vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Adiciona uma peça no topo da pilha (push)
void empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("A pilha de reserva está cheia!\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = x;
}

// Remove a peça do topo da pilha (pop)
Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) {
        printf("A pilha de reserva está vazia!\n");
        return removida;
    }
    removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// Exibe o conteúdo atual da pilha
void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// ==================== GERAÇÃO DE PEÇAS ====================

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ==================== PROGRAMA PRINCIPAL ====================

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorId = 0; // controla o ID único das peças

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorId++));
    }

    do {
        printf("\n===== ESTADO ATUAL =====\n");
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("=========================\n");

        printf("\n1 - Jogar peça");
        printf("\n2 - Reservar peça");
        printf("\n3 - Usar peça reservada");
        printf("\n0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                enfileirar(&fila, gerarPeca(contadorId++));
                break;
            }
            case 2: { // Reservar peça
                if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    if (!pilhaCheia(&pilha)) {
                        empilhar(&pilha, reservada);
                        printf("Peça [%c %d] movida para a pilha de reserva!\n", reservada.nome, reservada.id);
                    } else {
                        printf("Não há espaço na pilha de reserva! A peça será descartada.\n");
                    }
                    enfileirar(&fila, gerarPeca(contadorId++));
                }
                break;
            }
            case 3: { // Usar peça reservada
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("Peça [%c %d] usada!\n", usada.nome, usada.id);
                break;
            }
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
