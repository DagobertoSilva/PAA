#include <stdio.h>

void ShowVector(int *vector, int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        printf("%d\t", vector[i]);
    }

    printf("\n");
}


void InsertionSort(int *vector, int size)
{
    int j, key, i;

    // Percorre o vetor a partir do segundo elemento
    for(j = 1; j < size; j++)
    {
        key = vector[j];
        i = j - 1;

        printf("\n");
        printf("==============================================\n");
        printf("             ITERACAO j = %d\n", j);
        printf("==============================================\n");

        printf("Key escolhido: %d\n", key);

        printf("\nVetor antes da iteracao:\n");
        ShowVector(vector, size);

        printf("\n");

        // Compara o key com os elementos anteriores
        while(i >= 0 && vector[i] > key)
        {
            printf("Comparacao:\n");
            printf("vector[%d] = %d > key = %d\n", 
                   i, vector[i], key);

            printf("Deslocando %d:\n", vector[i]);

            // Desloca o elemento uma posição para a direita
            vector[i + 1] = vector[i];

            printf("Vetor depois do deslocamento:\n");
            ShowVector(vector, size);

            printf("\n");

            // Volta uma posição
            i = i - 1;
        }

        // Coloca o key na posição correta
        vector[i + 1] = key;

        printf("Inserindo key = %d na posicao %d\n", 
               key, i + 1);

        printf("\nVetor depois da iteracao:\n");
        ShowVector(vector, size);

        printf("==============================================\n");
    }
}


int main()
{
    int vector[] = {5, 7, 8, 19, 24, 0, 1};

    int sizeOfVector = sizeof(vector) / sizeof(vector[0]);

    printf("==============================================\n");
    printf("              INSERTION SORT\n");
    printf("==============================================\n");

    printf("\nVetor original:\n");
    ShowVector(vector, sizeOfVector);

    printf("\n");

    InsertionSort(vector, sizeOfVector);

    printf("\n");
    printf("==============================================\n");
    printf("              VETOR ORDENADO\n");
    printf("==============================================\n");

    ShowVector(vector, sizeOfVector);

    printf("\n");

    return 0;
}