#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =======================
//   ESTRUTURA DA SALA
// =======================
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ----------------------------------------------------------
// criarSala()
// Cria dinamicamente uma sala com o nome fornecido.
// ----------------------------------------------------------
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ----------------------------------------------------------
// explorarSalas()
// Permite ao jogador caminhar pela mansão usando:
//  e - ir para a esquerda
//  d - ir para a direita
//  s - sair
// A exploração termina ao chegar em uma sala sem caminhos.
// ----------------------------------------------------------
void explorarSalas(Sala *atual) {

    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: **%s**\n", atual->nome);

        // Sala sem caminhos → fim da exploração
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou a um cômodo sem saídas. Fim da exploração!\n");
            return;
        }

        printf("Para onde deseja ir? (e = esquerda | d = direita | s = sair): ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            atual = atual->esquerda;
        }
        else if (opcao == 'd' || opcao == 'D') {
            atual = atual->direita;
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("Exploração encerrada pelo jogador.\n");
            return;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ----------------------------------------------------------
// liberarArvore()
// Libera memória de todas as salas da mansão.
// ----------------------------------------------------------
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// ----------------------------------------------------------
// main()
// Monta o mapa FIXO da mansão e inicia a exploração.
// ----------------------------------------------------------
int main() {

    // ---------- MONTANDO O MAPA DA MANSÃO ----------
    // Estrutura definida manualmente, como solicitado.

    /*
                       [Hall de Entrada]
                           /        \
                 [Sala de Estar]    [Jardim]
                     /      \           \
           [Cozinha]   [Biblioteca]   [Garagem]
    */

    Sala *hall         = criarSala("Hall de Entrada");
    Sala *salaEstar    = criarSala("Sala de Estar");
    Sala *jardim       = criarSala("Jardim");
    Sala *cozinha      = criarSala("Cozinha");
    Sala *biblioteca   = criarSala("Biblioteca");
    Sala *garagem      = criarSala("Garagem");

    // Conectando os nós
    hall->esquerda = salaEstar;
    hall->direita  = jardim;

    salaEstar->esquerda = cozinha;
    salaEstar->direita  = biblioteca;

    jardim->direita = garagem;

    // ---------- INÍCIO DO JOGO ----------
    printf("===== DETECTIVE QUEST – EXPLORAÇÃO DA MANSÃO =====\n");
    printf("Você iniciará sua jornada pelo Hall de Entrada.\n");

    explorarSalas(hall);

    // ---------- LIBERAR MEMÓRIA ----------
    liberarArvore(hall);

    return 0;
}
