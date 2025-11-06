#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* =========================================================
   DETECTIVE QUEST – NÍVEL MESTRE
   Mansão (Árvore Binária) + Pistas (BST) + Hash (pista→suspeito)
   ========================================================= */

/* -------------------------
   Configurações gerais
   ------------------------- */
#define HASH_SIZE 31

/* -------------------------
   Utilidades de string
   ------------------------- */
void str_tolower(char *s) {
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}
int str_ieq(const char *a, const char *b) {
    char ca[256], cb[256];
    strncpy(ca, a, sizeof(ca)-1); ca[sizeof(ca)-1] = '\0';
    strncpy(cb, b, sizeof(cb)-1); cb[sizeof(cb)-1] = '\0';
    str_tolower(ca); str_tolower(cb);
    return strcmp(ca, cb) == 0;
}

/* =========================================================
   1) ÁRVORE DA MANSÃO (mapa) – Binária
   ========================================================= */
typedef struct Sala {
    char nome[64];
    char pista[128];     // pista estática associada a este cômodo (pode estar vazia)
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* criarSala() – cria dinamicamente um cômodo. */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*)malloc(sizeof(Sala));
    if (!s) { printf("Falha ao alocar Sala.\n"); exit(1); }
    strncpy(s->nome, nome, sizeof(s->nome)-1); s->nome[sizeof(s->nome)-1] = '\0';
    if (pista) { strncpy(s->pista, pista, sizeof(s->pista)-1); s->pista[sizeof(s->pista)-1] = '\0'; }
    else s->pista[0] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

/* Libera toda a árvore da mansão */
void liberarMansao(Sala *r) {
    if (!r) return;
    liberarMansao(r->esq);
    liberarMansao(r->dir);
    free(r);
}

/* =========================================================
   2) ÁRVORE DE PISTAS – BST (ordenada alfabeticamente)
   ========================================================= */
typedef struct PistaNode {
    char pista[128];
    struct PistaNode *esq, *dir;
} PistaNode;

/* inserirPista() / adicionarPista() – insere a pista coletada na BST. */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (!pista || !*pista) return raiz;
    if (!raiz) {
        PistaNode *n = (PistaNode*)malloc(sizeof(PistaNode));
        if (!n) { printf("Falha ao alocar PistaNode.\n"); exit(1); }
        strncpy(n->pista, pista, sizeof(n->pista)-1);
        n->pista[sizeof(n->pista)-1] = '\0';
        n->esq = n->dir = NULL;
        return n;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) raiz->esq = inserirPista(raiz->esq, pista);
    else if (cmp > 0) raiz->dir = inserirPista(raiz->dir, pista);
    /* pistas iguais são ignoradas (sem duplicatas) */
    return raiz;
}

/* Percorre em ordem e imprime as pistas (ordem alfabética) */
void exibirPistas(PistaNode *r) {
    if (!r) return;
    exibirPistas(r->esq);
    printf(" - %s\n", r->pista);
    exibirPistas(r->dir);
}

/* Percorre BST e aplica callback em cada pista */
typedef void (*PistaCallback)(const char *pista, void *userdata);
void percorrerPistas(PistaNode *r, PistaCallback cb, void *ud) {
    if (!r) return;
    percorrerPistas(r->esq, cb, ud);
    cb(r->pista, ud);
    percorrerPistas(r->dir, cb, ud);
}

/* Libera BST */
void liberarBST(PistaNode *r) {
    if (!r) return;
    liberarBST(r->esq);
    liberarBST(r->dir);
    free(r);
}

/* =========================================================
   3) TABELA HASH – mapeia PISTA → SUSPEITO
   Encadeamento separado
   ========================================================= */
typedef struct HashNode {
    char key[128];       // pista
    char value[64];      // suspeito
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *buckets[HASH_SIZE];
} HashTable;

/* Hash simples (djb2) */
unsigned long hash_str(const char *s) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*s++)) h = ((h << 5) + h) + c;
    return h;
}

/* inicializa hash */
void initHash(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; ++i) ht->buckets[i] = NULL;
}

/* inserirNaHash() – insere associação pista/suspeito */
void inserirNaHash(HashTable *ht, const char *pista, const char *suspeito) {
    if (!pista || !*pista || !suspeito || !*suspeito) return;
    unsigned long h = hash_str(pista) % HASH_SIZE;
    HashNode *n = (HashNode*)malloc(sizeof(HashNode));
    if (!n) { printf("Falha ao alocar HashNode.\n"); exit(1); }
    strncpy(n->key, pista, sizeof(n->key)-1); n->key[sizeof(n->key)-1] = '\0';
    strncpy(n->value, suspeito, sizeof(n->value)-1); n->value[sizeof(n->value)-1] = '\0';
    n->next = ht->buckets[h];
    ht->buckets[h] = n;
}

/* encontrarSuspeito() – consulta o suspeito correspondente à pista */
const char* encontrarSuspeito(HashTable *ht, const char *pista) {
    if (!pista) return NULL;
    unsigned long h = hash_str(pista) % HASH_SIZE;
    for (HashNode *cur = ht->buckets[h]; cur; cur = cur->next) {
        if (strcmp(cur->key, pista) == 0) return cur->value;
    }
    return NULL;
}

/* libera hash */
void liberarHash(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode *cur = ht->buckets[i];
        while (cur) {
            HashNode *nx = cur->next;
            free(cur);
            cur = nx;
        }
        ht->buckets[i] = NULL;
    }
}

/* =========================================================
   4) Exploração da mansão (navegação + coleta de pistas)
   ========================================================= */
/* explorarSalas() – navega pela árvore e ativa o sistema de pistas. */
void explorarSalas(Sala *inicio, PistaNode **bstPistas) {
    Sala *atual = inicio;
    char op;

    printf("\n===== EXPLORAÇÃO INICIADA =====\n");
    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (atual->pista[0] != '\0') {
            printf("🕵️ Pista encontrada: \"%s\"\n", atual->pista);
            *bstPistas = inserirPista(*bstPistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("Escolha: (e) esquerda | (d) direita | (s) sair\n> ");
        if (scanf(" %c", &op) != 1) { printf("Entrada inválida.\n"); return; }

        if (op == 'e' || op == 'E') {
            if (atual->esq) atual = atual->esq;
            else printf("Não há caminho à esquerda.\n");
        } else if (op == 'd' || op == 'D') {
            if (atual->dir) atual = atual->dir;
            else printf("Não há caminho à direita.\n");
        } else if (op == 's' || op == 'S') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

/* =========================================================
   5) Julgamento final
   ========================================================= */
typedef struct {
    HashTable *ht;
    const char *acusado;
    int cont;
} CtxContagem;

/* callback: soma 1 se a pista aponta para o acusado na hash */
void contarPistasDoAcusadoCB(const char *pista, void *userdata) {
    CtxContagem *ctx = (CtxContagem*)userdata;
    const char *sus = encontrarSuspeito(ctx->ht, pista);
    if (sus && str_ieq(sus, ctx->acusado)) {
        ctx->cont++;
    }
}

/* verificarSuspeitoFinal() – conduz à fase de julgamento final. */
void verificarSuspeitoFinal(PistaNode *bstPistas, HashTable *ht) {
    char acusado[64];
    printf("\n===== JULGAMENTO FINAL =====\n");
    printf("Pistas coletadas (em ordem alfabética):\n");
    if (!bstPistas) {
        printf("(nenhuma)\n");
    } else {
        exibirPistas(bstPistas);
    }

    printf("\nDigite o NOME do suspeito que deseja acusar: ");
    // limpar buffer de linha anterior se necessário
    getchar(); // consome '\n' remanescente
    if (!fgets(acusado, sizeof(acusado), stdin)) {
        printf("Entrada inválida.\n");
        return;
    }
    // remover '\n'
    acusado[strcspn(acusado, "\r\n")] = '\0';

    if (strlen(acusado) == 0) {
        printf("Nenhum nome informado. Sem acusação.\n");
        return;
    }

    CtxContagem ctx = { ht, acusado, 0 };
    percorrerPistas(bstPistas, contarPistasDoAcusadoCB, &ctx);

    printf("\nPistas que apontam para \"%s\": %d\n", acusado, ctx.cont);
    if (ctx.cont >= 2) {
        printf("✅ Veredito: ACUSAÇÃO SUSTENTADA. \"%s\" é o culpado!\n", acusado);
    } else {
        printf("❌ Veredito: PROVAS INSUFICIENTES. \"%s\" não pode ser condenado.\n", acusado);
    }
}

/* =========================================================
   6) Programa principal
   ========================================================= */
int main(void) {
    /* -------------------------
       Mapa fixo da mansão (binária)
       ------------------------- 
                 [Hall]
                 /    \
          [Sala Estar] [Jardim]
            /     \        \
        [Cozinha] [Biblioteca] [Garagem]
    */
    Sala *hall       = criarSala("Hall de Entrada", "Pegadas de lama recentes no tapete.");
    Sala *estar      = criarSala("Sala de Estar", "Copo de cristal quebrado ao lado do sofá.");
    Sala *jardim     = criarSala("Jardim", "Marcas de pegadas até a cerca dos fundos.");
    Sala *cozinha    = criarSala("Cozinha", "Faca limpa fora do suporte, ainda úmida.");
    Sala *biblioteca = criarSala("Biblioteca", "Livro secreto aberto com anotações suspeitas.");
    Sala *garagem    = criarSala("Garagem", "Chaves do carro do mordomo ausentes.");

    hall->esq = estar;     hall->dir = jardim;
    estar->esq = cozinha;  estar->dir = biblioteca;
    jardim->dir = garagem;

    /* -------------------------
       Tabela Hash de pista → suspeito
       (mapeamento estático definido no código)
       ------------------------- */
    HashTable ht;
    initHash(&ht);

    /* Exemplos de suspeitos: "Mordomo", "Jardineiro", "Cozinheira", "Herdeiro" */
    inserirNaHash(&ht, "Pegadas de lama recentes no tapete.", "Jardineiro");
    inserirNaHash(&ht, "Copo de cristal quebrado ao lado do sofá.", "Herdeiro");
    inserirNaHash(&ht, "Marcas de pegadas até a cerca dos fundos.", "Jardineiro");
    inserirNaHash(&ht, "Faca limpa fora do suporte, ainda úmida.", "Cozinheira");
    inserirNaHash(&ht, "Livro secreto aberto com anotações suspeitas.", "Herdeiro");
    inserirNaHash(&ht, "Chaves do carro do mordomo ausentes.", "Mordomo");

    /* -------------------------
       Exploração + BST de pistas coletadas
       ------------------------- */
    PistaNode *pistasBST = NULL;
    printf("===== DETECTIVE QUEST – COLETA, ASSOCIAÇÃO E JULGAMENTO =====\n");
    printf("Navegue pela mansão a partir do Hall de Entrada.\n");
    explorarSalas(hall, &pistasBST);

    /* -------------------------
       Julgamento final
       ------------------------- */
    verificarSuspeitoFinal(pistasBST, &ht);

    /* -------------------------
       Limpeza de memória
       ------------------------- */
    liberarBST(pistasBST);
    liberarHash(&ht);
    liberarMansao(hall);

    return 0;
}
