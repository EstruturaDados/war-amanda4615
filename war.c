#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

// ============================================
// ======== ESTRUTURAS BÁSICAS DO JOGO ========
// ============================================

// Representa um item na mochila
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Nó da lista encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ============================================
// ======== VARIÁVEIS GLOBAIS AUXILIARES =====
// ============================================
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// ============================================
// ======== FUNÇÕES PARA O VETOR ==============
// ============================================

// Inserir item no vetor
void inserirItemVetor(Item vetor[], int *contador) {
    if (*contador >= MAX_ITENS) {
        printf("⚠️ Mochila (vetor) cheia!\n");
        return;
    }

    Item novo;
    printf("Nome: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    vetor[*contador] = novo;
    (*contador)++;
    printf("✅ Item adicionado ao vetor!\n");
}

// Remover item do vetor
void removerItemVetor(Item vetor[], int *contador) {
    if (*contador == 0) {
        printf("⚠️ Vetor vazio!\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int i, encontrado = -1;
    for (i = 0; i < *contador; i++) {
        if (strcmp(vetor[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("❌ Item não encontrado!\n");
        return;
    }

    for (i = encontrado; i < *contador - 1; i++)
        vetor[i] = vetor[i + 1];

    (*contador)--;
    printf("🗑️ Item removido do vetor!\n");
}

// Listar itens no vetor
void listarVetor(Item vetor[], int contador) {
    if (contador == 0) {
        printf("📦 Vetor vazio!\n");
        return;
    }

    printf("\n===== ITENS NO VETOR =====\n");
    for (int i = 0; i < contador; i++) {
        printf("%d) Nome: %-15s | Tipo: %-10s | Qtde: %d\n",
               i + 1, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
}

// Ordenar vetor por nome (Bubble Sort)
void ordenarVetor(Item vetor[], int contador) {
    Item temp;
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - i - 1; j++) {
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
    printf("📚 Vetor ordenado por nome!\n");
}

// Busca sequencial no vetor
int buscarSequencialVetor(Item vetor[], int contador, char nome[]) {
    comparacoesSequencial = 0;
    for (int i = 0; i < contador; i++) {
        comparacoesSequencial++;
        if (strcmp(vetor[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

// Busca binária no vetor (requer vetor ordenado)
int buscarBinariaVetor(Item vetor[], int contador, char nome[]) {
    comparacoesBinaria = 0;
    int inicio = 0, fim = contador - 1, meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoesBinaria++;
        int cmp = strcmp(vetor[meio].nome, nome);

        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// ============================================
// ======== FUNÇÕES PARA LISTA ENCADEADA =====
// ============================================

// Inserir item na lista
void inserirItemLista(No **inicio) {
    No *novo = (No *)malloc(sizeof(No));
    if (!novo) {
        printf("Erro de alocação!\n");
        return;
    }

    printf("Nome: ");
    fgets(novo->dados.nome, sizeof(novo->dados.nome), stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo->dados.tipo, sizeof(novo->dados.tipo), stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    getchar();

    novo->proximo = *inicio;
    *inicio = novo;

    printf("✅ Item adicionado à lista!\n");
}

// Remover item da lista
void removerItemLista(No **inicio) {
    if (*inicio == NULL) {
        printf("⚠️ Lista vazia!\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = *inicio, *anterior = NULL;

    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("❌ Item não encontrado!\n");
        return;
    }

    if (anterior == NULL)
        *inicio = atual->proximo;
    else
        anterior->proximo = atual->proximo;

    free(atual);
    printf("🗑️ Item removido da lista!\n");
}

// Listar itens da lista
void listarLista(No *inicio) {
    if (inicio == NULL) {
        printf("📦 Lista vazia!\n");
        return;
    }

    printf("\n===== ITENS NA LISTA =====\n");
    No *aux = inicio;
    int i = 1;
    while (aux != NULL) {
        printf("%d) Nome: %-15s | Tipo: %-10s | Qtde: %d\n",
               i++, aux->dados.nome, aux->dados.tipo, aux->dados.quantidade);
        aux = aux->proximo;
    }
}

// Busca sequencial na lista encadeada
int buscarSequencialLista(No *inicio, char nome[]) {
    comparacoesSequencial = 0;
    No *aux = inicio;
    int pos = 0;

    while (aux != NULL) {
        comparacoesSequencial++;
        if (strcmp(aux->dados.nome, nome) == 0)
            return pos;
        aux = aux->proximo;
        pos++;
    }
    return -1;
}

// ============================================
// ======== FUNÇÃO PRINCIPAL ==================
// ============================================
int main() {
    Item vetor[MAX_ITENS];
    int contadorVetor = 0;
    No *lista = NULL;

    int estrutura, opcao;
    char nomeBusca[30];

    printf("Escolha a estrutura: 1-Vetor | 2-Lista Encadeada: ");
    scanf("%d", &estrutura);
    getchar();

    do {
        printf("\n===== MENU MOCHILA =====\n");
        printf("1 - Inserir Item\n");
        printf("2 - Remover Item\n");
        printf("3 - Listar Itens\n");
        printf("4 - Buscar Item\n");
        if (estrutura == 1) printf("5 - Ordenar Vetor\n6 - Busca Binária\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                if (estrutura == 1) inserirItemVetor(vetor, &contadorVetor);
                else inserirItemLista(&lista);
                break;

            case 2:
                if (estrutura == 1) removerItemVetor(vetor, &contadorVetor);
                else removerItemLista(&lista);
                break;

            case 3:
                if (estrutura == 1) listarVetor(vetor, contadorVetor);
                else listarLista(lista);
                break;

            case 4:
                printf("Digite o nome do item: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                if (estrutura == 1) {
                    int pos = buscarSequencialVetor(vetor, contadorVetor, nomeBusca);
                    if (pos != -1)
                        printf("✅ Item encontrado no vetor (pos: %d)\n", pos + 1);
                    else
                        printf("❌ Item não encontrado!\n");
                    printf("🔎 Comparações (sequencial): %d\n", comparacoesSequencial);
                } else {
                    int pos = buscarSequencialLista(lista, nomeBusca);
                    if (pos != -1)
                        printf("✅ Item encontrado na lista (pos: %d)\n", pos + 1);
                    else
                        printf("❌ Item não encontrado!\n");
                    printf("🔎 Comparações (sequencial): %d\n", comparacoesSequencial);
                }
                break;

            case 5:
                if (estrutura == 1) ordenarVetor(vetor, contadorVetor);
                else printf("⚠️ Ordenação disponível apenas no vetor!\n");
                break;

            case 6:
                if (estrutura == 1) {
                    printf("Digite o nome para busca binária: ");
                    fgets(nomeBusca, sizeof(nomeBusca), stdin);
                    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                    int pos = buscarBinariaVetor(vetor, contadorVetor, nomeBusca);
                    if (pos != -1)
                        printf("✅ Item encontrado na busca binária (pos: %d)\n", pos + 1);
                    else
                        printf("❌ Item não encontrado!\n");
                    printf("⚡ Comparações (binária): %d\n", comparacoesBinaria);
                } else {
                    printf("⚠️ Busca binária não se aplica à lista encadeada!\n");
                }
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
