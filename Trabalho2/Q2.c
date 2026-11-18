/*
 * Trabalho 02 - Projeto e Analise de Algoritmos (UFC Crateus, 2026.2)
 * Questao 2 - Tarefas com prazo e multa (Algoritmo Guloso)
 *
 * Ideia:
 *   1. Ordena as tarefas por multa decrescente.
 *   2. Para cada tarefa, coloca no ULTIMO dia livre <= min(P, n).
 *      O "ultimo dia livre <= d" e encontrado com Union-Find
 *      (pai[d] = d se o dia d esta livre; senao aponta para d-1).
 *   3. Se nao ha dia livre, a tarefa sera atrasada (paga a multa).
 *   4. No fim, as atrasadas ocupam os dias livres que restaram.
 *
 * Tempo: O(n log n) (ordenacao) + O(n alpha(n)) (Union-Find).
 *
 * Uso:
 *   q2_tarefas                  executa o exemplo dos slides
 *   q2_tarefas arquivo.txt      le a instancia de um arquivo ("-" = teclado)
 *   q2_tarefas --teste          valida o guloso contra forca bruta
 *
 * Formato do arquivo de entrada:
 *   n
 *   P_1 M_1
 *   ...
 *   P_n M_n          (prazo >= 1, multa >= 0)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int n;
    int *P;        /* prazos, indices 1..n */
    long long *M;  /* multas, indices 1..n */
} Instancia;

/* variavel global usada pelo qsort para comparar por multa */
static const long long *g_multa;

static int cmp_multa_desc(const void *a, const void *b)
{
    int i = *(const int *)a, j = *(const int *)b;
    if (g_multa[i] != g_multa[j]) return g_multa[i] < g_multa[j] ? 1 : -1;
    return i - j;                      /* desempate estavel pelo indice */
}

/* Union-Find com compressao de caminho: ultimo dia livre <= d (0 = nenhum) */
static int achar(int *pai, int d)
{
    int raiz = d, prox;
    while (pai[raiz] != raiz) raiz = pai[raiz];
    while (pai[d] != raiz) {           /* compressao de caminho */
        prox = pai[d];
        pai[d] = raiz;
        d = prox;
    }
    return raiz;
}

/*
 * dia[i]      = dia (1..n) em que a tarefa i sera executada
 * atrasada[i] = 1 se dia[i] > P[i] (paga multa)
 * Retorna a multa total. Se verbose, imprime o passo a passo.
 */
static long long escalona(const Instancia *in, int *dia, int *atrasada, int verbose)
{
    int n = in->n, i, d, k;
    int *ordem = malloc((size_t)n * sizeof(int));
    int *pai = malloc((size_t)(n + 1) * sizeof(int));
    int *dono = calloc((size_t)(n + 1), sizeof(int));      /* dono[d] = tarefa do dia d */
    int *fila_atrasadas = malloc((size_t)n * sizeof(int));
    int qtd_atrasadas = 0;
    long long multa = 0;

    for (i = 1; i <= n; i++) { ordem[i - 1] = i; dia[i] = 0; atrasada[i] = 0; }
    g_multa = in->M;
    qsort(ordem, (size_t)n, sizeof(int), cmp_multa_desc);
    for (d = 0; d <= n; d++) pai[d] = d;

    for (k = 0; k < n; k++) {
        i = ordem[k];
        int limite = in->P[i] < n ? in->P[i] : n;
        d = achar(pai, limite);
        if (d > 0) {
            dia[i] = d;
            dono[d] = i;
            pai[d] = d - 1;                                /* dia d agora ocupado */
            if (verbose)
                printf("  T%d (multa %lld, prazo %d): ultimo dia livre <= %d e %d -> dia %d\n",
                       i, in->M[i], in->P[i], limite, d, d);
        } else {
            fila_atrasadas[qtd_atrasadas++] = i;
            atrasada[i] = 1;
            multa += in->M[i];
            if (verbose)
                printf("  T%d (multa %lld, prazo %d): nenhum dia livre <= %d -> ATRASADA\n",
                       i, in->M[i], in->P[i], limite);
        }
    }

    /* atrasadas ocupam os dias livres restantes (todos apos seus prazos) */
    d = 1;
    for (k = 0; k < qtd_atrasadas; k++) {
        while (dono[d] != 0) d++;
        i = fila_atrasadas[k];
        dia[i] = d;
        dono[d] = i;
        if (verbose)
            printf("  T%d (atrasada) -> dia livre %d\n", i, d);
    }

    free(ordem);
    free(pai);
    free(dono);
    free(fila_atrasadas);
    return multa;
}

/* ------------------------------------------------------------------ */
/* Forca bruta: testa todos os subconjuntos S e usa o Lema 1           */
/* (S viavel <=> N_t(S) <= t para todo t). So para n pequeno.          */
/* ------------------------------------------------------------------ */

static long long multa_otima_forca_bruta(const Instancia *in)
{
    int n = in->n, i, t;
    long long total = 0, melhor_soma = 0;
    unsigned long mascara;

    for (i = 1; i <= n; i++) total += in->M[i];
    for (mascara = 0; mascara < (1UL << n); mascara++) {
        int cont[64] = {0};
        long long soma = 0;
        int viavel = 1, acum = 0;
        for (i = 1; i <= n; i++)
            if (mascara & (1UL << (i - 1))) {
                int p = in->P[i] < n ? in->P[i] : n;
                cont[p]++;
                soma += in->M[i];
            }
        for (t = 1; t <= n; t++) {
            acum += cont[t];
            if (acum > t) { viavel = 0; break; }
        }
        if (viavel && soma > melhor_soma) melhor_soma = soma;
    }
    return total - melhor_soma;
}

/* ------------------------------------------------------------------ */
/* Entrada e saida                                                     */
/* ------------------------------------------------------------------ */

static int ler_instancia(FILE *f, Instancia *in)
{
    int i;
    if (fscanf(f, "%d", &in->n) != 1 || in->n < 1) {
        fprintf(stderr, "Erro: n invalido ou ausente.\n");
        return 0;
    }
    in->P = malloc((size_t)(in->n + 1) * sizeof(int));
    in->M = malloc((size_t)(in->n + 1) * sizeof(long long));
    if (!in->P || !in->M) return 0;
    for (i = 1; i <= in->n; i++) {
        if (fscanf(f, "%d %lld", &in->P[i], &in->M[i]) != 2 ||
            in->P[i] < 1 || in->M[i] < 0) {
            fprintf(stderr, "Erro: linha %d invalida (prazo >= 1, multa >= 0).\n", i);
            return 0;
        }
    }
    return 1;
}

static void imprimir_escala(const Instancia *in, const int *dia, const int *atrasada,
                            long long multa)
{
    int n = in->n, d, i;
    int *tarefa_do_dia = calloc((size_t)(n + 1), sizeof(int));
    for (i = 1; i <= n; i++) tarefa_do_dia[dia[i]] = i;

    printf("\n%-5s %-7s %-6s %-8s %s\n", "Dia", "Tarefa", "Prazo", "Multa", "Situacao");
    for (d = 1; d <= n; d++) {
        i = tarefa_do_dia[d];
        printf("%-5d T%-6d %-6d %-8lld %s\n", d, i, in->P[i], in->M[i],
               atrasada[i] ? "atrasada (paga multa)" : "no prazo");
    }
    printf("\nMulta total minima = %lld\n", multa);
    free(tarefa_do_dia);
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
        int dia[32], atrasada[32], visto[32] = {0}, ok = 1;
        long long multa, esperada, recalculada = 0;

        in.n = 1 + rand() % 12;
        in.P = malloc((size_t)(in.n + 1) * sizeof(int));
        in.M = malloc((size_t)(in.n + 1) * sizeof(long long));
        for (i = 1; i <= in.n; i++) {
            in.P[i] = 1 + rand() % (in.n + 3);   /* inclui prazos maiores que n */
            in.M[i] = rand() % 51;               /* inclui multa zero e empates */
        }

        multa = escalona(&in, dia, atrasada, 0);
        esperada = multa_otima_forca_bruta(&in);

        for (i = 1; i <= in.n; i++) {            /* cada dia usado exatamente uma vez */
            if (dia[i] < 1 || dia[i] > in.n || visto[dia[i]]) ok = 0;
            else visto[dia[i]] = 1;
            if (dia[i] > in.P[i]) recalculada += in.M[i];
        }
        if (multa != esperada || recalculada != multa) ok = 0;

        if (!ok) {
            falhas++;
            printf("FALHA caso %d: n=%d guloso=%lld otimo=%lld recalculada=%lld\n",
                   c, in.n, multa, esperada, recalculada);
        }
        free(in.P);
        free(in.M);
    }
    printf("Autoteste: %d casos aleatorios, %d falha(s).\n", casos, falhas);
    return falhas ? 1 : 0;
}

/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    Instancia in;
    int i, *dia, *atrasada;
    const char *arquivo = NULL;
    long long multa;
    static int exP[] = {0, 4, 2, 4, 3, 1, 4, 6};                 /* P = [4,2,4,3,1,4,6] */
    static long long exM[] = {0, 70, 60, 50, 40, 30, 20, 10};     /* M = [70,...,10]     */

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--teste") == 0) return autoteste();
        if (strcmp(argv[i], "--ajuda") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Uso: %s [arquivo|-]\n     %s --teste\n", argv[0], argv[0]);
            return 0;
        }
        arquivo = argv[i];
    }

    if (arquivo == NULL) {
        in.n = 7;
        in.P = exP;
        in.M = exM;
    } else {
        FILE *f = strcmp(arquivo, "-") == 0 ? stdin : fopen(arquivo, "r");
        if (!f) {
            fprintf(stderr, "Nao foi possivel abrir '%s'.\n", arquivo);
            return 1;
        }
        if (!ler_instancia(f, &in)) return 1;
        if (f != stdin) fclose(f);
    }

    dia = malloc((size_t)(in.n + 1) * sizeof(int));
    atrasada = malloc((size_t)(in.n + 1) * sizeof(int));
    if (!dia || !atrasada) return 1;

    printf("Instancia: n = %d tarefas\n", in.n);
    if (in.n <= 30) printf("\nPasso a passo (tarefas por multa decrescente):\n");
    multa = escalona(&in, dia, atrasada, in.n <= 30);
    imprimir_escala(&in, dia, atrasada, multa);

    free(dia);
    free(atrasada);
    if (arquivo != NULL) { free(in.P); free(in.M); }
    return 0;
}
