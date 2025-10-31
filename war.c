#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ======== ESTRUTURA BÁSICA DO JOGO ========
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ======== PROTÓTIPOS ========
Territorio* cadastrarTerritorios(int *qtd);
void exibirMapa(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missao1, char* missao2);

// ======== FUNÇÃO PRINCIPAL ========
int main() {
    srand(time(NULL));

    int qtd;
    printf("Digite o número de territórios no mapa: ");
    scanf("%d", &qtd);
    getchar();

    // Criação e cadastro do mapa
    Territorio* mapa = cadastrarTerritorios(&qtd);

    // ----- Sistema de missões -----
    char* missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar metade do mapa",
        "Reduzir as tropas inimigas abaixo de 5 em todos os territórios",
        "Dominar todos os territórios azuis"
    };
    int totalMissoes = 5;

    // Alocação dinâmica para armazenar a missão dos jogadores
    char* missaoJogador1 = (char*) malloc(100 * sizeof(char));
    char* missaoJogador2 = (char*) malloc(100 * sizeof(char));

    // Sorteia e atribui missões
    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);

    printf("\n🎯 Missão do Jogador 1: %s\n", missaoJogador1);
    printf("🎯 Missão do Jogador 2: %s\n\n", missaoJogador2);

    // ----- Loop principal -----
    int opcao;
    do {
        printf("===== MENU WAR =====\n");
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
            exibirMapa(mapa, qtd);

            int at, def;
            printf("\nNúmero do território atacante: ");
            scanf("%d", &at);
            printf("Número do território defensor: ");
            scanf("%d", &def);
            getchar();

            if (at < 1 || at > qtd || def < 1 || def > qtd) {
                printf("❌ Territórios inválidos!\n");
            } 
            else if (strcmp(mapa[at-1].cor, mapa[def-1].cor) == 0) {
                printf("⚠️ Não é possível atacar um território da mesma cor!\n");
            } 
            else {
                atacar(&mapa[at-1], &mapa[def-1]);

                // Verificação automática das missões
                if (verificarMissao(missaoJogador1, mapa, qtd)) {
                    printf("\n🏆 JOGADOR 1 CUMPRIU SUA MISSÃO: %s\n", missaoJogador1);
                    break;
                }
                if (verificarMissao(missaoJogador2, mapa, qtd)) {
                    printf("\n🏆 JOGADOR 2 CUMPRIU SUA MISSÃO: %s\n", missaoJogador2);
                    break;
                }
            }
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missaoJogador1, missaoJogador2);
    printf("\nMemória liberada. Fim do jogo!\n");

    return 0;
}

// ======== FUNÇÕES AUXILIARES ========

// Cadastra os territórios
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

// Exibe todos os territórios
void exibirMapa(Territorio* mapa, int qtd) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d) %-15s | Exército: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula o ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 1) {
        printf("⚠️ O território atacante precisa ter mais de 1 tropa!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n🎲 Rolagem de dados...\n");
    printf("%s (Atacante): %d\n", atacante->nome, dadoAtacante);
    printf("%s (Defensor): %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("🏆 Ataque bem-sucedido!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("💥 Ataque fracassado!\n");
        atacante->tropas--;
    }

    printf("📊 Tropas após batalha:\n");
    printf("Atacante: %s (%d tropas)\n", atacante->nome, atacante->tropas);
    printf("Defensor: %s (%d tropas, cor: %s)\n\n",
           defensor->nome, defensor->tropas, defensor->cor);
}

// Sorteia e atribui missão a um jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Verifica se a missão foi cumprida (lógica simples inicial)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    // Exemplo de verificação simples: se todos territórios têm a mesma cor
    if (strstr(missao, "Controlar metade")) {
        int verdes = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) verdes++;
        }
        if (verdes >= tamanho / 2) return 1;
    }

    // Exemplo genérico (pode expandir conforme jogo evolui)
    return 0;
}

// Libera memória alocada
void liberarMemoria(Territorio* mapa, char* missao1, char* missao2) {
    free(mapa);
    free(missao1);
    free(missao2);
}
