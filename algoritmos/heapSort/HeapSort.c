#include <stdio.h>

/* ============================================================
   CONTROLE DE ESTATÍSTICAS
   ============================================================ */

int comparacoes = 0;
int trocas = 0;
int  i;

/* ============================================================
   FUNÇÃO PARA IMPRIMIR O VETOR
   ============================================================ */

void imprimirVetor(int vetor[], int tamanho)
{
    printf("    [ ");

    for (i = 0; i < tamanho; i++)
    {
        printf("%d", vetor[i]);

        if (i < tamanho - 1)
            printf(" | ");
    }

    printf(" ]\n");
}

/* ============================================================
   FUNÇÃO PARA TROCAR DOIS ELEMENTOS
   ============================================================ */

void trocar(int vetor[], int i, int j)
{
    int temp = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = temp;

    trocas++;
}

/* ============================================================
   HEAPIFY
   ============================================================ */

/*
                i
               / \
              /   \
             /     \
          esquerda direita

    Para um elemento na posição i:

        Filho esquerdo = 2 * i + 1
        Filho direito  = 2 * i + 2
*/

void heapify(int vetor[], int tamanho, int i)
{
    int maior = i;

    int esquerda = 2 * i + 1;
    int direita  = 2 * i + 2;

    printf("\n");
    printf("    ---------------------------------------------\n");
    printf("    HEAPIFY\n");
    printf("    ---------------------------------------------\n");

    printf("    No analisado:\n");
    printf("        indice = %d\n", i);
    printf("        valor  = %d\n", vetor[i]);

    /* --------------------------------------------------------
       Verificando filho esquerdo
       -------------------------------------------------------- */

    if (esquerda < tamanho)
    {
        printf("\n");
        printf("    Filho esquerdo:\n");
        printf("        indice = %d\n", esquerda);
        printf("        valor  = %d\n", vetor[esquerda]);

        comparacoes++;

        if (vetor[esquerda] > vetor[maior])
        {
            printf("        %d > %d\n",
                   vetor[esquerda],
                   vetor[maior]);

            printf("        -> O filho esquerdo e MAIOR.\n");

            maior = esquerda;
        }
        else
        {
            printf("        %d <= %d\n",
                   vetor[esquerda],
                   vetor[maior]);

            printf("        -> O filho esquerdo NAO e maior.\n");
        }
    }
    else
    {
        printf("\n");
        printf("    Nao existe filho esquerdo.\n");
    }

    /* --------------------------------------------------------
       Verificando filho direito
       -------------------------------------------------------- */

    if (direita < tamanho)
    {
        printf("\n");
        printf("    Filho direito:\n");
        printf("        indice = %d\n", direita);
        printf("        valor  = %d\n", vetor[direita]);

        comparacoes++;

        if (vetor[direita] > vetor[maior])
        {
            printf("        %d > %d\n",
                   vetor[direita],
                   vetor[maior]);

            printf("        -> O filho direito e MAIOR.\n");

            maior = direita;
        }
        else
        {
            printf("        %d <= %d\n",
                   vetor[direita],
                   vetor[maior]);

            printf("        -> O filho direito NAO e maior.\n");
        }
    }
    else
    {
        printf("\n");
        printf("    Nao existe filho direito.\n");
    }

    /* --------------------------------------------------------
       Verificando se é necessário trocar
       -------------------------------------------------------- */

    printf("\n");
    printf("    Resultado da analise:\n");

    if (maior != i)
    {
        printf("        O maior elemento esta no indice %d.\n",
               maior);

        printf("        Valor atual : %d\n", vetor[i]);
        printf("        Maior valor : %d\n", vetor[maior]);

        printf("\n");
        printf("        -> TROCA NECESSARIA!\n");

        trocar(vetor, i, maior);

        printf("\n");
        printf("        Vetor apos a troca:\n");
        imprimirVetor(vetor, tamanho);

        /*
           Depois da troca, o elemento que foi para a posição
           maior pode ter violado a propriedade do heap.

           Por isso fazemos heapify novamente nessa posição.
        */

        printf("\n");
        printf("        O elemento %d desceu para o indice %d.\n",
               vetor[maior],
               maior);

        printf("        -> Continuando o HEAPIFY nessa posicao...\n");

        heapify(vetor, tamanho, maior);
    }
    else
    {
        printf("        O elemento %d ja e o maior.\n",
               vetor[i]);

        printf("        -> Nenhuma troca necessaria.\n");
    }
}

/* ============================================================
   CONSTRUÇÃO DO MAX HEAP
   ============================================================ */

void construirMaxHeap(int vetor[], int tamanho)
{
    int i;
    printf("\n");
    printf("============================================================\n");
    printf("                 CONSTRUÇAO DO MAX HEAP\n");
    printf("============================================================\n");

    printf("\n");
    printf("Vetor inicial:\n");
    imprimirVetor(vetor, tamanho);

    /*
       As folhas já são heaps individuais.

       Portanto, começamos pelo último nó que possui filho.

       Fórmula:

           último pai = tamanho / 2 - 1
    */

    int ultimoPai = tamanho / 2 - 1;

    printf("\n");
    printf("Ultimo no que possui filhos:\n");
    printf("    indice = %d\n", ultimoPai);
    printf("    valor  = %d\n", vetor[ultimoPai]);

    printf("\n");
    printf("Vamos percorrer os nos de baixo para cima.\n");

    for (i = ultimoPai; i >= 0; i--)
    {
        printf("\n");
        printf("------------------------------------------------------------\n");
        printf("ANALISANDO NO %d\n", i);
        printf("------------------------------------------------------------\n");

        heapify(vetor, tamanho, i);

        printf("\n");
        printf("Heap apos analisar o indice %d:\n", i);

        imprimirVetor(vetor, tamanho);
    }

    printf("\n");
    printf("============================================================\n");
    printf("                 MAX HEAP CONSTRUIDO\n");
    printf("============================================================\n");

    imprimirVetor(vetor, tamanho);
}

/* ============================================================
   HEAP SORT
   ============================================================ */

void heapSort(int vetor[], int tamanho)
{
    /* --------------------------------------------------------
       ETAPA 1
       Construir o Max Heap
       -------------------------------------------------------- */

    construirMaxHeap(vetor, tamanho);

    /* --------------------------------------------------------
       ETAPA 2
       Ordenação
       -------------------------------------------------------- */

    int i, fim;

    printf("\n\n");
    printf("============================================================\n");
    printf("                    INICIO DO HEAP SORT\n");
    printf("============================================================\n");

    printf("\n");
    printf("Agora o maior elemento esta sempre na raiz.\n");

    printf("A raiz sera trocada com o ultimo elemento disponivel.\n");

    printf("\n");
    printf("Processo:\n");
    printf("    1. Pegar o maior elemento da raiz.\n");
    printf("    2. Coloca-lo no final da parte nao ordenada.\n");
    printf("    3. Diminuir o tamanho do heap.\n");
    printf("    4. Fazer HEAPIFY novamente.\n");

    for (fim = tamanho - 1; fim > 0; fim--)
    {
        printf("\n\n");
        printf("############################################################\n");
        printf("                 ETAPA DE ORDENACAO\n");
        printf("############################################################\n");

        printf("\n");
        printf("Tamanho atual do heap: %d\n", fim + 1);

        printf("\n");
        printf("Estado atual do vetor:\n");
        imprimirVetor(vetor, tamanho);

        printf("\n");
        printf("A raiz contem o maior elemento:\n");

        printf("    Raiz      = %d\n", vetor[0]);
        printf("    ultima posição disponivel = %d\n", fim);
        printf("    Valor da ultima posicao   = %d\n", vetor[fim]);

        printf("\n");
        printf("------------------------------------------------------------\n");
        printf("TROCANDO RAIZ COM ULTIMO ELEMENTO\n");
        printf("------------------------------------------------------------\n");

        printf("\n");
        printf("Antes da troca:\n");
        imprimirVetor(vetor, tamanho);

        trocar(vetor, 0, fim);

        printf("\n");
        printf("Depois da troca:\n");
        imprimirVetor(vetor, tamanho);

        printf("\n");
        printf("O elemento %d agora esta em sua posicao definitiva.\n",
               vetor[fim]);

        printf("Ele nao participara mais do heap.\n");

        printf("\n");
        printf("Parte do vetor ja ordenada:\n");

        printf("    [ ");

        for (i = fim; i < tamanho; i++)
        {
            printf("%d", vetor[i]);

            if (i < tamanho - 1)
                printf(" | ");
        }

        printf(" ]\n");

        printf("\n");
        printf("Parte do vetor que ainda e um heap:\n");

        printf("    [ ");

        for (i = 0; i < fim; i++)
        {
            printf("%d", vetor[i]);

            if (i < fim - 1)
                printf(" | ");
        }

        printf(" ]\n");

        /* ----------------------------------------------------
           RECONSTRUIR O HEAP
           ---------------------------------------------------- */

        printf("\n");
        printf("------------------------------------------------------------\n");
        printf("RECONSTRUINDO O MAX HEAP\n");
        printf("------------------------------------------------------------\n");

        printf("\n");
        printf("O elemento que ficou na raiz pode nao ser o maior.\n");

        printf("Precisamos executar HEAPIFY novamente.\n");

        heapify(vetor, fim, 0);

        printf("\n");
        printf("Heap reconstruido:\n");

        printf("    [ ");

        for (i = 0; i < fim; i++)
        {
            printf("%d", vetor[i]);

            if (i < fim - 1)
                printf(" | ");
        }

        printf(" ]\n");
    }

    /* --------------------------------------------------------
       RESULTADO FINAL
       -------------------------------------------------------- */

    printf("\n\n");
    printf("============================================================\n");
    printf("                    HEAP SORT FINALIZADO\n");
    printf("============================================================\n");

    printf("\n");
    printf("Vetor ordenado:\n");

    imprimirVetor(vetor, tamanho);

    printf("\n");
    printf("Estatisticas:\n");

    printf("    Comparacoes : %d\n", comparacoes);
    printf("    Trocas      : %d\n", trocas);

    printf("\n");
    printf("Complexidade:\n");
    printf("    Construcao do Heap : O(n)\n");
    printf("    Ordenacao          : O(n log n)\n");
    printf("    Heap Sort          : O(n log n)\n");
    printf("    Memoria adicional  : O(1)\n");

    printf("\n");
    printf("============================================================\n");
}

/* ============================================================
   MAIN
   ============================================================ */

int main()
{
    int vetor[] = {40, 10, 30, 50, 20, 60, 70};

    int tamanho = sizeof(vetor) / sizeof(vetor[0]);

    printf("\n");
    printf("============================================================\n");
    printf("                     ALGORITMO HEAP SORT\n");
    printf("============================================================\n");

    printf("\n");
    printf("Quantidade de elementos: %d\n", tamanho);

    printf("\n");
    printf("Vetor original:\n");

    imprimirVetor(vetor, tamanho);

    printf("\n");
    printf("Representacao inicial da arvore:\n\n");

    printf("                         %d\n", vetor[0]);
    printf("                       /   \\\n");
    printf("                      /     \\\n");
    printf("                    %d         %d\n",
           vetor[1], vetor[2]);

    printf("                  /  \\      /  \\\n");
    printf("                 /    \\    /    \\\n");
    printf("               %d      %d  %d      %d\n",
           vetor[3], vetor[4], vetor[5], vetor[6]);

    heapSort(vetor, tamanho);

     printf("Representacao Final da arvore:\n\n");

    printf("                         %d\n", vetor[0]);
    printf("                       /   \\\n");
    printf("                      /     \\\n");
    printf("                    %d         %d\n",
           vetor[1], vetor[2]);

    printf("                  /  \\      /  \\\n");
    printf("                 /    \\    /    \\\n");
    printf("               %d      %d  %d      %d\n",
           vetor[3], vetor[4], vetor[5], vetor[6]);

    printf("\n");
    printf("Programa finalizado.\n");

    return 0;
}