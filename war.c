#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10  // capacidade máxima da mochila

// ======== ESTRUTURA DO ITEM ========
// Representa cada objeto dentro da mochila.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// ======== PROTÓTIPOS DE FUNÇÕES ========
void inserirItem(Item mochila[], int *contador);
void removerItem(Item mochila[], int *contador);
void listarItens(Item mochila[], int contador);
void buscarItem(Item mochila[], int contador);

// ======== FUNÇÃO PRINCIPAL ========
int main() {
    Item mochila[MAX_ITENS];
    int contador = 0;
    int opcao;

    do {
        printf("\n===== MOCHILA DE LOOT =====\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpa o buffer

        switch (opcao) {
            case 1:
                inserirItem(mochila, &contador);
                break;
            case 2:
                removerItem(mochila, &contador);
                break;
            case 3:
                listarItens(mochila, contador);
                break;
            case 4:
                buscarItem(mochila, contador);
                break;
            case 0:
                printf("Encerrando o sistema de inventário...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ======== FUNÇÕES ========

// Função para inserir um novo item na mochila
void inserirItem(Item mochila[], int *contador) {
    if (*contador >= MAX_ITENS) {
        printf("⚠️ Mochila cheia! Remova um item antes de adicionar outro.\n");
        return;
    }

    Item novo;

    printf("\n--- Cadastro de Item ---\n");
    printf("Nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // remove \n

    printf("Tipo (arma, munição, cura, ferramenta...): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[*contador] = novo;
    (*contador)++;

    printf("✅ Item adicionado com sucesso!\n");
    listarItens(mochila, *contador);
}

// Função para remover um item da mochila pelo nome
void removerItem(Item mochila[], int *contador) {
    if (*contador == 0) {
        printf("⚠️ A mochila está vazia!\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item que deseja remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < *contador; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("❌ Item não encontrado!\n");
        return;
    }

    // Move os itens seguintes uma posição para trás
    for (int i = encontrado; i < *contador - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    (*contador)--;
    printf("🗑️ Item '%s' removido da mochila!\n", nomeRemover);
    listarItens(mochila, *contador);
}

// Lista todos os itens da mochila
void listarItens(Item mochila[], int contador) {
    if (contador == 0) {
        printf("\n📦 Mochila vazia!\n");
        return;
    }

    printf("\n===== ITENS NA MOCHILA =====\n");
    for (int i = 0; i < contador; i++) {
        printf("%d) Nome: %-15s | Tipo: %-10s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Busca sequencial de um item pelo nome
void buscarItem(Item mochila[], int contador) {
    if (contador == 0) {
        printf("⚠️ Nenhum item para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < contador; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("❌ Item '%s' não encontrado!\n", nomeBusca);
    } else {
        printf("\n🔍 Item encontrado:\n");
        printf("Nome: %s\n", mochila[encontrado].nome);
        printf("Tipo: %s\n", mochila[encontrado].tipo);
        printf("Quantidade: %d\n", mochila[encontrado].quantidade);
    }
}
