/*
 * Trabalho 02 - Projeto e Analise de Algoritmos (UFC Crateus, 2026.2)
 * Questao 1 - Balsa com duas pistas (Programacao Dinamica)
 *
 * M[k][A][B] = maior numero de carros de C_k..C_n que podem ser embarcados
 *              com a pista 1 tendo A metros livres e a pista 2 tendo B.
 *
 * Recorrencia:
 *   M[k][A][B] = 0                                   se k > n ou T_k > max(A,B)
 *   M[k][A][B] = max{ 1 + M[k+1][A-T_k][B]   (se T_k <= A),
 *                     1 + M[k+1][A][B-T_k]   (se T_k <= B) }
 *
 * Resposta: M[1][L][L].
 *
 * Duas versoes sao implementadas:
 *   - balsa_pd         : tabela completa, Theta(n L^2) tempo e espaco (padrao)
 *   - balsa_otimizada  : usa o fato de A + B = 2L - (T_1 + ... + T_{k-1}),
 *                        guardando so (k, A): Theta(n L) tempo e espaco
 *
 * Uso:
 *   q1_balsa                  executa o exemplo dos slides
 *   q1_balsa arquivo.txt      le a instancia de um arquivo ("-" = teclado)
 *   q1_balsa --otimizado ...  usa a versao O(nL)
 *   q1_balsa --teste          valida as duas versoes contra forca bruta
 *
 * Formato do arquivo de entrada:
 *   L
 *   n
 *   T_1 T_2 ... T_n
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int j ;
typedef struct {
    int L;   /* comprimento de cada pista */
    int n;   /* numero de carros na fila */
    int *T;  /* tamanhos, indexados de 1 a n */
} Instancia;

#define LIMITE_CELULAS 300000000.0 /* protecao de memoria da versao completa */

#define IDX3(k, A, B, L) \
    (((size_t)(k) * ((size_t)(L) + 1) + (size_t)(A)) * ((size_t)(L) + 1) + (size_t)(B))

/* ------------------------------------------------------------------ */
/* Versao 1: PD bottom-up com tabela M[k][A][B] e tabela de decisao    */
/* ------------------------------------------------------------------ */

/* Retorna M[1][L][L] (ou -1 se faltar memoria).
 * pista[1..n] recebe 0 (nao embarcou), 1 ou 2 (rastreio da parte (d)). */
static int balsa_pd(const Instancia *in, int *pista)
{
    int L = in->L, n = in->n, k, A, B;
    size_t lado = (size_t)L + 1;
    int *M;
    unsigned char *esc;

    if ((double)(n + 2) * (double)lado * (double)lado > LIMITE_CELULAS) {
        fprintf(stderr, "Instancia grande demais para a tabela completa; "
                        "use --otimizado.\n");
        return -1;
    }
    size_t total = (size_t)(n + 2) * lado * lado;
    M = calloc(total, sizeof(int));          /* k = n+1 e caso base (zeros) */
    esc = calloc(total, sizeof(unsigned char));
    if (!M || !esc) {
        fprintf(stderr, "Memoria insuficiente; use --otimizado.\n");
        free(M);
        free(esc);
        return -1;
    }

    for (k = n; k >= 1; k--) {
        int t = in->T[k];
        for (A = 0; A <= L; A++) {
            for (B = 0; B <= L; B++) {
                int melhor = 0;
                unsigned char e = 0;               /* 0 = nenhuma pista */
                if (t <= A) {
                    int v = 1 + M[IDX3(k + 1, A - t, B, L)];
                    if (v > melhor) { melhor = v; e = 1; }
                }
                if (t <= B) {
                    int v = 1 + M[IDX3(k + 1, A, B - t, L)];
                    if (v > melhor) { melhor = v; e = 2; }
                }
                M[IDX3(k, A, B, L)] = melhor;
                esc[IDX3(k, A, B, L)] = e;
            }
        }
    }

    /* Rastreio (parte d): percorre as decisoes a partir de (1, L, L). */
    A = L;
    B = L;
    for (k = 1; k <= n; k++) pista[k] = 0;
    for (k = 1; k <= n; k++) {
        unsigned char e = esc[IDX3(k, A, B, L)];
        if (e == 0) break;                         /* C_k nao cabe */
        pista[k] = e;
        if (e == 1) A -= in->T[k]; else B -= in->T[k];
    }

    int resposta = M[IDX3(1, L, L, L)];
    free(M);
    free(esc);
    return resposta;
}

/* ------------------------------------------------------------------ */
/* Versao 2: PD otimizada, estado (k, A); B = 2L - S_k - A             */
/* ------------------------------------------------------------------ */

static int balsa_otimizada(const Instancia *in, int *pista)
{
    int L = in->L, n = in->n, k, A;
    size_t lado = (size_t)L + 1;
    long long *S = malloc((size_t)(n + 2) * sizeof(long long)); /* S[k] = T_1+..+T_{k-1} */
    int *M = calloc((size_t)(n + 2) * lado, sizeof(int));
    unsigned char *esc = calloc((size_t)(n + 2) * lado, sizeof(unsigned char));
    int resposta = -1;

    if (!S || !M || !esc) {
        fprintf(stderr, "Memoria insuficiente.\n");
        goto fim;
    }
    S[1] = 0;
    for (k = 1; k <= n; k++) S[k + 1] = S[k] + in->T[k];

    for (k = n; k >= 1; k--) {
        int t = in->T[k];
        for (A = 0; A <= L; A++) {
            long long B = 2LL * L - S[k] - A;
            if (B < 0 || B > L) continue;          /* estado inalcancavel */
            int melhor = 0;
            unsigned char e = 0;
            if (t <= A) {                          /* pista 1: (k+1, A-t), B igual */
                int v = 1 + M[(size_t)(k + 1) * lado + (size_t)(A - t)];
                if (v > melhor) { melhor = v; e = 1; }
            }
            if (t <= B) {                          /* pista 2: (k+1, A), B-t */
                int v = 1 + M[(size_t)(k + 1) * lado + (size_t)A];
                if (v > melhor) { melhor = v; e = 2; }
            }
            M[(size_t)k * lado + (size_t)A] = melhor;
            esc[(size_t)k * lado + (size_t)A] = e;
        }
    }

    A = L;
    for (k = 1; k <= n; k++) pista[k] = 0;
    for (k = 1; k <= n; k++) {
        unsigned char e = esc[(size_t)k * lado + (size_t)A];
        if (e == 0) break;
        pista[k] = e;
        if (e == 1) A -= in->T[k];
    }
    resposta = M[(size_t)1 * lado + (size_t)L];

fim:
    free(S);
    free(M);
    free(esc);
    return resposta;
}

/* ------------------------------------------------------------------ */
/* Forca bruta (so para validar com n pequeno): recursao sem memoizar  */
/* ------------------------------------------------------------------ */

static int forca_bruta(const int *T, int n, int k, int A, int B)
{
    int melhor = 0;
    if (k > n) return 0;
    if (T[k] <= A) {
        int v = 1 + forca_bruta(T, n, k + 1, A - T[k], B);
        if (v > melhor) melhor = v;
    }
    if (T[k] <= B) {
        int v = 1 + forca_bruta(T, n, k + 1, A, B - T[k]);
        if (v > melhor) melhor = v;
    }
    return melhor;
}

/* ------------------------------------------------------------------ */
/* Entrada e saida                                                     */
/* ------------------------------------------------------------------ */

static int ler_instancia(FILE *f, Instancia *in)
{
    int i;
    if (fscanf(f, "%d", &in->L) != 1 || fscanf(f, "%d", &in->n) != 1) {
        fprintf(stderr, "Erro: esperado 'L' e 'n' no inicio da entrada.\n");
        return 0;
    }
    if (in->L < 1 || in->n < 1) {
        fprintf(stderr, "Erro: L e n devem ser positivos.\n");
        return 0;
    }
    in->T = malloc((size_t)(in->n + 2) * sizeof(int));
    if (!in->T) return 0;
    for (i = 1; i <= in->n; i++) {
        if (fscanf(f, "%d", &in->T[i]) != 1 || in->T[i] < 1) {
            fprintf(stderr, "Erro: tamanho do carro %d invalido ou ausente.\n", i);
            free(in->T);
            in->T = NULL;
            return 0;
        }
    }
    return 1;
}

static void imprimir_resultado(const Instancia *in, const int *pista, int total,
                               const char *versao)
{
    int k, uso1 = 0, uso2 = 0, encerrou = 0;
    printf("Versao: %s\n", versao);
    printf("Instancia: L = %d m (por pista), n = %d carros\n\n", in->L, in->n);
    printf("%-7s %-9s %s\n", "Carro", "Tamanho", "Pista");
    for (k = 1; k <= in->n; k++) {
        if (pista[k] != 0) {
            printf("C%-6d %-9d %d\n", k, in->T[k], pista[k]);
            if (pista[k] == 1) uso1 += in->T[k]; else uso2 += in->T[k];
        } else if (!encerrou) {
            printf("C%-6d %-9d nao cabe (embarque encerrado)\n", k, in->T[k]);
            encerrou = 1;
        } else {
            printf("C%-6d %-9d nao embarcou\n", k, in->T[k]);
        }
    }
    printf("\nM[1,%d,%d] = %d carros embarcados\n", in->L, in->L, total);
    printf("Pista 1: %d/%d m ocupados\n", uso1, in->L);
    printf("Pista 2: %d/%d m ocupados\n", uso2, in->L);
}

/* Confere se o rastreio e valido: prefixo da fila, capacidades respeitadas */
static int rastreio_valido(const Instancia *in, const int *pista, int total)
{
    int k, A = in->L, B = in->L, cont = 0;
    for (k = 1; k <= in->n; k++) {
        if (pista[k] == 0) {
            for (j= k; j <= in->n; j++)
                if (pista[j] != 0) return 0;       /* embarcou depois de parar */
            break;
        }
        if (pista[k] == 1) A -= in->T[k]; else B -= in->T[k];
        if (A < 0 || B < 0) return 0;
        cont++;
    }
    return cont == total;
}

/* ------------------------------------------------------------------ */
/* Autoteste                                                           */
/* ------------------------------------------------------------------ */

static int autoteste(void)
{
    int casos = 3000, falhas = 0, c, i;
    srand(2026);
    for (c = 0; c < casos; c++) {
        Instancia in;
        int pista1[32], pista2[32];
        in.n = 1 + rand() % 14;
        in.L = 1 + rand() % 20;
        in.T = malloc((size_t)(in.n + 2) * sizeof(int));
        for (i = 1; i <= in.n; i++) in.T[i] = 1 + rand() % (in.L + 3);

        int bruto = forca_bruta(in.T, in.n, 1, in.L, in.L);
        int r1 = balsa_pd(&in, pista1);
        int r2 = balsa_otimizada(&in, pista2);
        int ok = (r1 == bruto) && (r2 == bruto) &&
                 rastreio_valido(&in, pista1, r1) &&
                 rastreio_valido(&in, pista2, r2);
        if (!ok) {
            falhas++;
            printf("FALHA caso %d: L=%d n=%d bruto=%d pd=%d otimizada=%d\n",
                   c, in.L, in.n, bruto, r1, r2);
        }
        free(in.T);
    }
    printf("Autoteste: %d casos aleatorios, %d falha(s).\n", casos, falhas);
    return falhas ? 1 : 0;
}

/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    int otimizado = 0, i, total;
    const char *arquivo = NULL;
    Instancia in;
    int *pista;
    static int exemplo[] = {0, 4, 6, 3, 5, 2, 7, 1};   /* T = [4,6,3,5,2,7,1] */

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--otimizado") == 0) otimizado = 1;
        else if (strcmp(argv[i], "--teste") == 0) return autoteste();
        else if (strcmp(argv[i], "--ajuda") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Uso: %s [--otimizado] [arquivo|-]\n"
                   "     %s --teste\n", argv[0], argv[0]);
            return 0;
        } else arquivo = argv[i];
    }

    if (arquivo == NULL) {                         /* exemplo dos slides */
        in.L = 10;
        in.n = 7;
        in.T = exemplo;
    } else {
        FILE *f = strcmp(arquivo, "-") == 0 ? stdin : fopen(arquivo, "r");
        if (!f) {
            fprintf(stderr, "Nao foi possivel abrir '%s'.\n", arquivo);
            return 1;
        }
        if (!ler_instancia(f, &in)) return 1;
        if (f != stdin) fclose(f);
    }

    pista = calloc((size_t)(in.n + 2), sizeof(int));
    if (!pista) return 1;
    total = otimizado ? balsa_otimizada(&in, pista) : balsa_pd(&in, pista);
    if (total < 0) return 1;

    imprimir_resultado(&in, pista, total,
                       otimizado ? "otimizada, Theta(nL)" : "completa, Theta(nL^2)");
    free(pista);
    if (arquivo != NULL) free(in.T);
    return 0;
}