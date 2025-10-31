#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura do território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ==== PROTÓTIPOS DAS FUNÇÕES ====
Territorio* cadastrarTerritorios(int *qtd);
void exibirMapa(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

// ==== FUNÇÃO PRINCIPAL ====
int main() {
    srand(time(NULL)); // garante aleatoriedade nas batalhas

    int qtd;
    printf("Digite o número de territórios: ");
    scanf("%d", &qtd);
    getchar(); // limpa o buffer

    // Alocação dinâmica dos territórios
    Territorio* mapa = cadastrarTerritorios(&qtd);

    int opcao;
    do {
        printf("\n===== MENU WAR =====\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Atacar\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            exibirMapa(mapa, qtd);
        } 
        else if (opcao == 2) {
            int at, def;
            exibirMapa(mapa, qtd);

            printf("\nEscolha o número do território ATACANTE: ");
            scanf("%d", &at);
            printf("Escolha o número do território DEFENSOR: ");
            scanf("%d", &def);

            if (at < 1 || at > qtd || def < 1 || def > qtd) {
                printf("❌ Territórios inválidos!\n");
            } 
            else if (strcmp(mapa[at-1].cor, mapa[def-1].cor) == 0) {
                printf("⚠️ Você não pode atacar um território da mesma cor!\n");
            } 
            else {
                atacar(&mapa[at-1], &mapa[def-1]);
            }
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    printf("\nMemória liberada. Fim do jogo!\n");

    return 0;
}

// ==== FUNÇÕES AUXILIARES ====

// Cadastra os territórios dinamicamente
Territorio* cadastrarTerritorios(int *qtd) {
    Territorio* mapa = (Territorio*) calloc(*qtd, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    for (int i = 0; i < *qtd; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }

    return mapa;
}

// Exibe o estado atual do mapa
void exibirMapa(Territorio* mapa, int qtd) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d) %-15s | Exército: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula um ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 1) {
        printf("⚠️ O território atacante precisa ter mais de 1 tropa para atacar!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n🎲 Rolagem de dados...\n");
    printf("%s (Atacante) tirou: %d\n", atacante->nome, dadoAtacante);
    printf("%s (Defensor) tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n🏆 O atacante venceu a batalha!\n");

        // Transferência de cor e metade das tropas
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas = atacante->tropas / 2;

    } else {
        printf("\n💥 O defensor resistiu ao ataque!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }

    printf("\n📊 Estado pós-batalha:\n");
    printf("Atacante: %s | Tropas: %d\n", atacante->nome, atacante->tropas);
    printf("Defensor: %s | Tropas: %d | Cor: %s\n", defensor->nome, defensor->tropas, defensor->cor);
}

// Libera memória alocada dinamicamente
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}
