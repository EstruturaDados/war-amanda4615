#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20

// Estrutura que representa um componente da torre
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // 1 (mais alta) a 10 (mais baixa)
} Componente;

// Protótipos
void cadastroComponentes(Componente comp[], int *n);
void mostrarComponentes(Componente comp[], int n);
void bubbleSortNome(Componente comp[], int n, long *comparacoes);
void insertionSortTipo(Componente comp[], int n, long *comparacoes);
void selectionSortPrioridade(Componente comp[], int n, long *comparacoes);
int buscaBinariaPorNome(Componente comp[], int n, const char chave[], long *comparacoes);
double medirTempo(void (*algoritmo)(Componente*, int, long*), Componente comp[], int n, long *comparacoes);
void copiarVetor(Componente dest[], Componente src[], int n);
void limparString(char s[], int tamanho);

// ------------------- Função main -------------------
int main() {
    Componente componentes[MAX_COMPONENTES];
    int n = 0;
    int opcao;
    srand((unsigned)time(NULL));

    printf("=== Módulo de Priorização e Montagem da Torre de Fuga ===\n");

    // Cadastro inicial (opcional) — o jogador pode cadastrar até 20 componentes
    cadastroComponentes(componentes, &n);

    // Vetor auxiliar para permitir ordenar sem perder entrada original
    Componente aux[MAX_COMPONENTES];

    do {
        printf("\n--- MENU ---\n");
        printf("1 - Listar componentes\n");
        printf("2 - Adicionar componentes (mais)\n");
        printf("3 - Ordenar por NOME (Bubble Sort)  -> rápido para estudar comparações\n");
        printf("4 - Ordenar por TIPO (Insertion Sort)\n");
        printf("5 - Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6 - Busca binária por NOME (só após ordenar por NOME)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) { // valida entrada numérica
            while (getchar() != '\n'); // limpa
            printf("Entrada inválida.\n");
            opcao = -1;
            continue;
        }
        getchar(); // limpa \n

        if (opcao == 1) {
            mostrarComponentes(componentes, n);
        }
        else if (opcao == 2) {
            cadastroComponentes(componentes, &n);
        }
        else if (opcao == 3) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            copiarVetor(aux, componentes, n);
            long compar = 0;
            double tempo = medirTempo(bubbleSortNome, aux, n, &compar);
            printf("\n=> Resultado da ordenação por NOME (Bubble Sort):\n");
            mostrarComponentes(aux, n);
            printf("Comparações: %ld | Tempo: %.3f ms\n", compar, tempo * 1000.0);
            // copia o resultado ordenado de volta para componentes (opcional)
            copiarVetor(componentes, aux, n);
        }
        else if (opcao == 4) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            copiarVetor(aux, componentes, n);
            long compar = 0;
            double tempo = medirTempo(insertionSortTipo, aux, n, &compar);
            printf("\n=> Resultado da ordenação por TIPO (Insertion Sort):\n");
            mostrarComponentes(aux, n);
            printf("Comparações: %ld | Tempo: %.3f ms\n", compar, tempo * 1000.0);
            copiarVetor(componentes, aux, n);
        }
        else if (opcao == 5) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            copiarVetor(aux, componentes, n);
            long compar = 0;
            double tempo = medirTempo(selectionSortPrioridade, aux, n, &compar);
            printf("\n=> Resultado da ordenação por PRIORIDADE (Selection Sort):\n");
            mostrarComponentes(aux, n);
            printf("Comparações: %ld | Tempo: %.3f ms\n", compar, tempo * 1000.0);
            copiarVetor(componentes, aux, n);
        }
        else if (opcao == 6) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            // Para busca binária precisa estar ordenado por nome.
            printf("ATENÇÃO: a busca binária exige que o vetor esteja ordenado por NOME.\n");
            printf("Deseja ordenar por NOME agora com Bubble Sort antes da busca? (s/n): ");
            char resp = getchar();
            while (getchar() != '\n');
            if (resp == 's' || resp == 'S') {
                // ordena o vetor principal
                long comparSort = 0;
                double t = medirTempo(bubbleSortNome, componentes, n, &comparSort);
                printf("Vetor ordenado por NOME. (Comparações: %ld | Tempo: %.3f ms)\n", comparSort, t*1000.0);
            } else {
                printf("Preservando ordem atual — se não estiver ordenado, busca pode falhar.\n");
            }

            char chave[30];
            printf("Digite o nome do componente-chave a buscar: ");
            fgets(chave, sizeof(chave), stdin);
            limparString(chave, sizeof(chave));

            long comparBin = 0;
            int pos = buscaBinariaPorNome(componentes, n, chave, &comparBin);
            if (pos != -1) {
                printf("✅ Componente-chave encontrado na posição %d:\n", pos + 1);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
            } else {
                printf("❌ Componente-chave NÃO encontrado.\n");
            }
            printf("Comparações (busca binária): %ld\n", comparBin);
        }
        else if (opcao == 0) {
            printf("Encerrando módulo. Boa sorte na fuga!\n");
        }
        else {
            printf("Opção inválida. Tente de novo.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ------------------- Funções auxiliares -------------------

// Cadastro de componentes (adiciona até MAX_COMPONENTES)
void cadastroComponentes(Componente comp[], int *n) {
    int restantes = MAX_COMPONENTES - *n;
    if (restantes <= 0) {
        printf("Limite de componentes atingido (%d).\n", MAX_COMPONENTES);
        return;
    }

    int qtd;
    printf("Quantos componentes deseja cadastrar? (máx %d): ", restantes);
    if (scanf("%d", &qtd) != 1) {
        while (getchar() != '\n');
        printf("Entrada inválida.\n");
        return;
    }
    getchar();

    if (qtd < 1) { printf("Nenhum componente cadastrado.\n"); return; }
    if (qtd > restantes) qtd = restantes;

    for (int i = 0; i < qtd; i++) {
        printf("\n--- Componente %d ---\n", (*n) + 1);
        printf("Nome: ");
        fgets(comp[*n].nome, sizeof(comp[*n].nome), stdin);
        limparString(comp[*n].nome, sizeof(comp[*n].nome));

        printf("Tipo (ex: controle, suporte, propulsao): ");
        fgets(comp[*n].tipo, sizeof(comp[*n].tipo), stdin);
        limparString(comp[*n].tipo, sizeof(comp[*n].tipo));

        int pri;
        do {
            printf("Prioridade (1 = mais alta ... 10 = mais baixa): ");
            if (scanf("%d", &pri) != 1) { while (getchar() != '\n'); pri = -1; }
            getchar();
            if (pri < 1 || pri > 10) printf("Valor inválido. Insira entre 1 e 10.\n");
        } while (pri < 1 || pri > 10);

        comp[*n].prioridade = pri;
        (*n)++;
    }
    printf("\n✅ Cadastro concluído. Total de componentes: %d\n", *n);
}

// Mostra vetor de componentes formatado
void mostrarComponentes(Componente comp[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("\n--- Componentes (total: %d) ---\n", n);
    printf("%-3s | %-25s | %-12s | %-9s\n", "N", "NOME", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-25s | %-12s | %-9d\n",
               i + 1, comp[i].nome, comp[i].tipo, comp[i].prioridade);
    }
}

// Bubble sort por nome (string). Conta comparações via ponteiro comparacoes.
void bubbleSortNome(Componente comp[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(comp[j].nome, comp[j+1].nome) > 0) {
                Componente tmp = comp[j];
                comp[j] = comp[j+1];
                comp[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

// Insertion sort por tipo (string). Conta comparações via ponteiro comparacoes.
void insertionSortTipo(Componente comp[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = comp[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(comp[j].tipo, chave.tipo) > 0) {
                comp[j+1] = comp[j];
                j--;
            } else break;
        }
        comp[j+1] = chave;
    }
}

// Selection sort por prioridade (int). Conta comparações via ponteiro comparacoes.
void selectionSortPrioridade(Componente comp[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int idxMin = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (comp[j].prioridade < comp[idxMin].prioridade) {
                idxMin = j;
            }
        }
        if (idxMin != i) {
            Componente tmp = comp[i];
            comp[i] = comp[idxMin];
            comp[idxMin] = tmp;
        }
    }
}

// Busca binária por nome (assume vetor ordenado por nome).
// Retorna índice ou -1. Também conta comparações por ponteiro comparacoes.
int buscaBinariaPorNome(Componente comp[], int n, const char chave[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(comp[meio].nome, chave);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// Mede o tempo (em segundos) de execução do algoritmo de ordenação passado.
// O algoritmo tem assinatura void alg(Componente*, int, long*).
double medirTempo(void (*algoritmo)(Componente*, int, long*), Componente comp[], int n, long *comparacoes) {
    clock_t inicio = clock();
    algoritmo(comp, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / (double)CLOCKS_PER_SEC;
}

// Copia vetor (útil para preservar original)
void copiarVetor(Componente dest[], Componente src[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

// Remove \n final de fgets e protege contra overflow
void limparString(char s[], int tamanho) {
    s[tamanho-1] = '\0'; // garante terminação
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}
