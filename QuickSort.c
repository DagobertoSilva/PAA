#include <stdio.h>

// ============================================================
// FUNÇÃO PARA MOSTRAR O VETOR
// ============================================================
void ShowVector(int *vector, int size)
{
    int i;

    printf("[ ");

    for (i = 0; i < size; i++)
    {
        printf("%d", vector[i]);

        if (i < size - 1)
            printf(", ");
    }

    printf(" ]\n");
}


// ============================================================
// FUNÇÃO DE PARTIÇÃO
// ============================================================
int Partition(int *vector, int left, int right)
{
    int pivot;
    int i, j;
    int temp;

    // --------------------------------------------------------
    // Escolhemos o elemento do meio como pivô
    // --------------------------------------------------------
    pivot = vector[(left + right) / 2];

    i = left;
    j = right;

    printf("\n--------------------------------------------------\n");
    printf("NOVA PARTICAO\n");
    printf("Intervalo: [%d ... %d]\n", left, right);
    printf("Pivo escolhido: %d\n", pivot);
    printf("--------------------------------------------------\n");

    while (i <= j)
    {
        // ----------------------------------------------------
        // Procura pela esquerda um elemento >= pivô
        // ----------------------------------------------------
        while (vector[i] < pivot)
        {
            printf("  i=%d -> %d < pivo %d, avancando i\n",
                   i, vector[i], pivot);

            i++;
        }

        // ----------------------------------------------------
        // Procura pela direita um elemento <= pivô
        // ----------------------------------------------------
        while (vector[j] > pivot)
        {
            printf("  j=%d -> %d > pivo %d, diminuindo j\n",
                   j, vector[j], pivot);

            j--;
        }

        // ----------------------------------------------------
        // Se i <= j, podemos trocar os elementos
        // ----------------------------------------------------
        if (i <= j)
        {
            printf("\n  Elementos encontrados para troca:\n");
            printf("  vector[%d] = %d\n", i, vector[i]);
            printf("  vector[%d] = %d\n", j, vector[j]);

            temp = vector[i];
            vector[i] = vector[j];
            vector[j] = temp;

            printf("  -> TROCA REALIZADA!\n");
            printf("  Vetor: ");
            ShowVector(vector, right + 1);

            i++;
            j--;
        }
    }

    printf("\nParticao terminada.\n");
    printf("Proxima posicao de i: %d\n", i);
    printf("Proxima posicao de j: %d\n", j);

    return i;
}


// ============================================================
// QUICKSORT
// ============================================================
void QuickSort(int *vector, int left, int right)
{
    int index;

    // --------------------------------------------------------
    // Caso base:
    // se left >= right, temos 0 ou 1 elemento.
    // Portanto, ja esta ordenado.
    // --------------------------------------------------------
    if (left >= right)
    {
        printf("\n[CASO BASE] Intervalo [%d ... %d]\n",
               left, right);

        return;
    }

    printf("\n==================================================\n");
    printf("QUICKSORT(%d, %d)\n", left, right);
    printf("==================================================\n");

    printf("Vetor atual: ");
    ShowVector(vector, right + 1);

    // --------------------------------------------------------
    // Faz a particao
    // --------------------------------------------------------
    index = Partition(vector, left, right);

    // --------------------------------------------------------
    // Ordena a parte esquerda
    // --------------------------------------------------------
    printf("\n>>> CHAMADA RECURSIVA ESQUERDA\n");

    if (left < index - 1)
    {
        printf("QuickSort(%d, %d)\n", left, index - 1);

        QuickSort(vector, left, index - 1);
    }
    else
    {
        printf("Nao ha subvetor esquerdo para ordenar.\n");
    }

    // --------------------------------------------------------
    // Ordena a parte direita
    // --------------------------------------------------------
    printf("\n>>> CHAMADA RECURSIVA DIREITA\n");

    if (index < right)
    {
        printf("QuickSort(%d, %d)\n", index, right);

        QuickSort(vector, index, right);
    }
    else
    {
        printf("Nao ha subvetor direito para ordenar.\n");
    }
}


// ============================================================
// FUNÇÃO PRINCIPAL
// ============================================================
int main()
{
    int vector[] = {10, 7, 8, 9, 1, 5};
    int size = sizeof(vector) / sizeof(vector[0]);

    printf("==================================================\n");
    printf("             QUICK SORT - TRACE\n");
    printf("==================================================\n");

    printf("\nVetor inicial:\n");
    ShowVector(vector, size);

    printf("\nIniciando Quick Sort...\n");

    QuickSort(vector, 0, size - 1);

    printf("\n\n==================================================\n");
    printf("             ORDENACAO FINALIZADA\n");
    printf("==================================================\n");

    printf("\nVetor ordenado:\n");
    ShowVector(vector, size);

    return 0;
}