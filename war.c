#include <stdio.h>
#include <string.h>

#define QTDE_TERRITORIOS 5

// Definição da struct
typedef struct {
    char nome[50];
    char corExercito[30];
    int tropas;
} Territorio;

int main() {
    Territorio territorios[QTDE_TERRITORIOS];
    int i;

    printf("=== Cadastro Inicial dos Territórios ===\n\n");

    // Leitura dos dados
    for (i = 0; i < QTDE_TERRITORIOS; i++) {
        printf("Território %d:\n", i + 1);

        printf("Nome: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // remove o '\n'

        printf("Cor do Exército: ");
        fgets(territorios[i].corExercito, sizeof(territorios[i].corExercito), stdin);
        territorios[i].corExercito[strcspn(territorios[i].corExercito, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // consome o '\n' deixado pelo scanf

        printf("\n");
    }

    // Exibição dos dados
    printf("\n=== Estado Atual do Mapa ===\n");
    for (i = 0; i < QTDE_TERRITORIOS; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exército: %s\n", territorios[i].corExercito);
        printf("Número de Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}
