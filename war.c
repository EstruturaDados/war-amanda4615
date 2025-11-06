#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===============================================
//   ESTRUTURA DA MANSÃO (ÁRVORE BINÁRIA NORMAL)
// ===============================================
typedef struct Sala {
    char nome[50];
    char pista[100];  // pode estar vazia
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Cria uma sala dinamicamente
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0';

    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

// ===============================================
//      ÁRVORE DE PISTAS (BST ORDENADA)
// ===============================================
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// Insere nova pista na árvore BST (ordenada por ordem alfabética)
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return raiz;

    if (raiz == NULL) {
        PistaNode *nova = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        
        nova->esq = NULL;
        nova->dir = NULL;
        return nova;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

// Exibe as pistas em ordem alfabética (in-order)
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

// Libera memória da árvore BST
void liberarBST(PistaNode *raiz) {
    if (!raiz) return;
    liberarBST(raiz->esq);
    liberarBST(raiz->dir);
    free(raiz);
}

// Libera memória da árvore da mansão
void liberarMansao(Sala *raiz) {
    if (!raiz) return;
    liberarMansao(raiz->esq);
    liberarMansao(raiz->dir);
    free(raiz);
}

// ===============================================
//   EXPLORAÇÃO DA MANSÃO E COLETA DE PISTAS
// ===============================================
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: **%s**\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("🕵️ Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("\nEscolha seu caminho:\n");
        printf(" e - seguir para a esquerda\n");
        printf(" d - seguir para a direita\n");
        printf(" s - sair da mansão\n");

        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esq == NULL) {
                printf("Não há caminho à esquerda.\n");
            } else {
                atual = atual->esq;
            }
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->dir == NULL) {
                printf("Não há caminho à direita.\n");
            } else {
                atual = atual->dir;
            }
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("\nExploração encerrada.\n");
            return;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

// ===============================================
//   PROGRAMA PRINCIPAL
// ===============================================
int main() {
    // -------------- MONTANDO A MANSÃO --------------
    /*
                [Hall de Entrada]
                 /             \
            [Sala Estar]     [Jardim]
             /       \            \
      [Cozinha]   [Biblioteca]   [Garagem]

    Cada sala tem uma pista opcional.
    */

    Sala *hall         = criarSala("Hall de Entrada", "Pegadas suspeitas no tapete.");
    Sala *estar        = criarSala("Sala de Estar", "Copo quebrado próximo ao sofá.");
    Sala *jardim       = criarSala("Jardim", "Pegadas indo em direção à cerca.");
    Sala *cozinha      = criarSala("Cozinha", "");
    Sala *biblioteca   = criarSala("Biblioteca", "Livro fora do lugar contendo anotações.");
    Sala *garagem      = criarSala("Garagem", "Chaves de carro faltando.");

    hall->esq = estar;
    hall->dir = jardim;
    estar->esq = cozinha;
    estar->dir = biblioteca;
    jardim->dir = garagem;

    // Árvore de pistas coletadas (BST)
    PistaNode *pistas = NULL;

    printf("===== DETECTIVE QUEST – COLETA DE PISTAS =====\n");
    printf("Você iniciará sua exploração pelo Hall de Entrada.\n");

    explorarSalasComPistas(hall, &pistas);

    // -------------- EXIBIR PISTAS COLETADAS --------------
    printf("\n===== PISTAS COLETADAS =====\n");
    if (pistas == NULL) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistas(pistas);
    }

    // -------------- LIBERAR MEMÓRIA --------------
    liberarBST(pistas);
    liberarMansao(hall);

    return 0;
}
