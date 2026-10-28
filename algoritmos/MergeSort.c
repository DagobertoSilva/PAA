
#include <stdio.h>

/*
    Função responsável por imprimir uma parte do vetor.
*/
void imprimirVetor(int vetor[], int inicio, int fim)
{
    int i;

    printf("[ ");

    for (i = inicio; i <= fim; i++)
    {
        printf("%d ", vetor[i]);
    }

    printf("]\n");
}


/*
    Função responsável por realizar o MERGE.

    Ela recebe duas partes já ordenadas:

        esquerda: inicio até meio
        direita:  meio + 1 até fim

    e junta as duas partes em ordem crescente.
*/
void merge(int vetor[], int inicio, int meio, int fim)
{
    int tamanhoEsquerda;
    int tamanhoDireita;

    int *esquerda;
    int *direita;

    int i;
    int j;
    int k;


    /* Calcula o tamanho das duas partes */
    tamanhoEsquerda = meio - inicio + 1;
    tamanhoDireita = fim - meio;


    /*
        Cria os vetores temporários.
    */
    esquerda = (int *) malloc(tamanhoEsquerda * sizeof(int));
    direita = (int *) malloc(tamanhoDireita * sizeof(int));


    /*
        Copia os elementos da parte esquerda.
    */
    for (i = 0; i < tamanhoEsquerda; i++)
    {
        esquerda[i] = vetor[inicio + i];
    }


    /*
        Copia os elementos da parte direita.
    */
    for (j = 0; j < tamanhoDireita; j++)
    {
        direita[j] = vetor[meio + 1 + j];
    }


    /*
        Índices utilizados para percorrer:

        i → vetor esquerda
        j → vetor direita
        k → vetor original
    */
    i = 0;
    j = 0;
    k = inicio;


    printf("\nComparando:\n");

    printf("Esquerda: ");
    imprimirVetor(esquerda, 0, tamanhoEsquerda - 1);

    printf("Direita:  ");
    imprimirVetor(direita, 0, tamanhoDireita - 1);


    /*
        Compara os elementos das duas partes.
    */
    while (i < tamanhoEsquerda && j < tamanhoDireita)
    {
        if (esquerda[i] <= direita[j])
        {
            vetor[k] = esquerda[i];

            i++;
        }
        else
        {
            vetor[k] = direita[j];

            j++;
        }

        k++;
    }


    /*
        Caso ainda existam elementos
        na parte esquerda.
    */
    while (i < tamanhoEsquerda)
    {
        vetor[k] = esquerda[i];

        i++;
        k++;
    }


    /*
        Caso ainda existam elementos
        na parte direita.
    */
    while (j < tamanhoDireita)
    {
        vetor[k] = direita[j];

        j++;
        k++;
    }


    printf("Resultado do merge: ");
    imprimirVetor(vetor, inicio, fim);


    /*
        Libera a memória utilizada
        pelos vetores temporários.
    */
    free(esquerda);
    free(direita);
}


/*
    Função principal do Merge Sort.

    Divide o vetor recursivamente
    até chegar a partes com apenas
    um elemento.
*/
void mergeSort(int vetor[], int inicio, int fim)
{
    int meio;


    /*
        Enquanto inicio < fim,
        ainda podemos dividir o vetor.
    */
    if (inicio < fim)
    {
        /*
            Calcula o meio do vetor.
        */
        meio = inicio + (fim - inicio) / 2;


        printf("\n=====================================\n");
        printf("DIVIDINDO\n");
        printf("=====================================\n");

        printf("Vetor:    ");
        imprimirVetor(vetor, inicio, fim);

        printf("Esquerda: ");
        imprimirVetor(vetor, inicio, meio);

        printf("Direita:  ");
        imprimirVetor(vetor, meio + 1, fim);


        /*
            Ordena a metade esquerda.
        */
        mergeSort(vetor, inicio, meio);


        /*
            Ordena a metade direita.
        */
        mergeSort(vetor, meio + 1, fim);


        /*
            Junta as duas partes ordenadas.
        */
        merge(vetor, inicio, meio, fim);
    }
}


int main()
{
    int vetor[] = {2,13,100,1};

    int tamanho;


    /*
        Calcula a quantidade de elementos
        do vetor.
    */
    tamanho = sizeof(vetor) / sizeof(vetor[0]);


    printf("=====================================\n");
    printf("           MERGE SORT\n");
    printf("=====================================\n");


    printf("\nVetor inicial: ");

    imprimirVetor(vetor, 0, tamanho - 1);


    /*
        Chama o Merge Sort.
    */
    mergeSort(vetor, 0, tamanho - 1);


    printf("\n=====================================\n");
    printf("          RESULTADO FINAL\n");
    printf("=====================================\n");


    printf("\nVetor ordenado: ");

    imprimirVetor(vetor, 0, tamanho - 1);


    return 0;
}

