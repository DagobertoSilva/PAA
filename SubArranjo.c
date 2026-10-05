#include <stdio.h>
#include <limits.h>

/*
============================================================
       SUBARRANJO CONTÍGUO DE SOMA MÁXIMA
       DIVISÃO E CONQUISTA
============================================================
*/


// ============================================================
// ESTRUTURA PARA GUARDAR O RESULTADO
// ============================================================
int i;

typedef struct
{
    int inicio;
    int fim;
    int soma;

} Resultado;


// ============================================================
// MOSTRA UM SUBARRANJO
// ============================================================

void mostrarSubarranjo(int vetor[], int inicio, int fim)
{
    

    printf("[");

    for (i = inicio; i <= fim; i++)
    {
        printf("%d", vetor[i]);

        if (i < fim)
        {
            printf(", ");
        }
    }

    printf("]");
}


// ============================================================
// ENCONTRA O MELHOR SUBARRANJO QUE ATRAVESSA O MEIO
// ============================================================

Resultado encontrarCruzamento(
    int vetor[],
    int inicio,
    int meio,
    int fim
)
{
    int i;
    int soma;

    int melhorSomaEsquerda = INT_MIN;
    int melhorInicio = meio;

    int melhorSomaDireita = INT_MIN;
    int melhorFim = meio + 1;


    // ========================================================
    // PROCURA A MELHOR PARTE À ESQUERDA
    // ========================================================

    soma = 0;

    for (i = meio; i >= inicio; i--)
    {
        soma = soma + vetor[i];

        if (soma > melhorSomaEsquerda)
        {
            melhorSomaEsquerda = soma;
            melhorInicio = i;
        }
    }


    // ========================================================
    // PROCURA A MELHOR PARTE À DIREITA
    // ========================================================

    soma = 0;

    for (i = meio + 1; i <= fim; i++)
    {
        soma = soma + vetor[i];

        if (soma > melhorSomaDireita)
        {
            melhorSomaDireita = soma;
            melhorFim = i;
        }
    }


    // ========================================================
    // MONTA O RESULTADO
    // ========================================================

    Resultado resultado;

    resultado.inicio = melhorInicio;
    resultado.fim = melhorFim;

    resultado.soma =
        melhorSomaEsquerda + melhorSomaDireita;

    return resultado;
}


// ============================================================
// ALGORITMO DE DIVISÃO E CONQUISTA
// ============================================================

Resultado subarranjoMaximo(
    int vetor[],
    int inicio,
    int fim,
    int nivel
)
{
    int i;


    // ========================================================
    // MOSTRA O NÍVEL DA RECURSÃO
    // ========================================================

    for (i = 0; i < nivel; i++)
    {
        printf("    ");
    }

    printf("ANALISANDO: ");

    mostrarSubarranjo(
        vetor,
        inicio,
        fim
    );

    printf("  [%d..%d]\n", inicio, fim);


    // ========================================================
    // CASO BASE
    // ========================================================

    if (inicio == fim)
    {
        for (i = 0; i < nivel; i++)
        {
            printf("    ");
        }

        printf(
            "CASO BASE -> valor = %d\n",
            vetor[inicio]
        );


        Resultado resultado;

        resultado.inicio = inicio;
        resultado.fim = fim;
        resultado.soma = vetor[inicio];

        return resultado;
    }


    // ========================================================
    // DIVISÃO
    // ========================================================

    int meio;

    meio = inicio + (fim - inicio) / 2;


    for (i = 0; i < nivel; i++)
    {
        printf("    ");
    }

    printf(
        "DIVIDINDO -> meio = %d\n",
        meio
    );


    // ========================================================
    // RESOLVE O LADO ESQUERDO
    // ========================================================

    Resultado esquerda;

    esquerda =
        subarranjoMaximo(
            vetor,
            inicio,
            meio,
            nivel + 1
        );


    // ========================================================
    // RESOLVE O LADO DIREITO
    // ========================================================

    Resultado direita;

    direita =
        subarranjoMaximo(
            vetor,
            meio + 1,
            fim,
            nivel + 1
        );


    // ========================================================
    // RESOLVE O SUBARRANJO QUE ATRAVESSA O MEIO
    // ========================================================

    Resultado cruzamento;

    cruzamento =
        encontrarCruzamento(
            vetor,
            inicio,
            meio,
            fim
        );


    // ========================================================
    // MOSTRA AS TRÊS POSSIBILIDADES
    // ========================================================

    printf("\n");

    for (i = 0; i < nivel; i++)
    {
        printf("    ");
    }

    printf("COMBINANDO:\n");


    // --------------------------------------------------------
    // ESQUERDA
    // --------------------------------------------------------

    for (i = 0; i < nivel; i++)
    {
        printf("    ");
    }

    printf("Esquerda  = ");

    mostrarSubarranjo(
        vetor,
        esquerda.inicio,
        esquerda.fim
    );

    printf(
        " -> soma = %d\n",
        esquerda.soma
    );


    // --------------------------------------------------------
    // DIREITA
    // --------------------------------------------------------

    for (i = 0; i < nivel; i++)
    {
        printf("    ");
    }

    printf("Direita   = ");

    mostrarSubarranjo(
        vetor,
        direita.inicio,
        direita.fim
    );

    printf(
        " -> soma = %d\n",
        direita.soma
    );


    // --------------------------------------------------------
    // CRUZAMENTO
    // --------------------------------------------------------

    for (i = 0; i < nivel; i++)
    {
        printf("    ");
    }

    printf("Cruzando  = ");

    mostrarSubarranjo(
        vetor,
        cruzamento.inicio,
        cruzamento.fim
    );

    printf(
        " -> soma = %d\n",
        cruzamento.soma
    );


    // ========================================================
    // ESCOLHE O MELHOR DOS TRÊS
    // ========================================================

    Resultado melhor;


    if (
        esquerda.soma >= direita.soma &&
        esquerda.soma >= cruzamento.soma
    )
    {
        melhor = esquerda;
    }
    else if (
        direita.soma >= esquerda.soma &&
        direita.soma >= cruzamento.soma
    )
    {
        melhor = direita;
    }
    else
    {
        melhor = cruzamento;
    }


    // ========================================================
    // MOSTRA O MELHOR
    // ========================================================

    for (i = 0; i < nivel; i++)
    {
        printf("    ");
    }

    printf("MELHOR -> ");

    mostrarSubarranjo(
        vetor,
        melhor.inicio,
        melhor.fim
    );

    printf(
        " = %d\n",
        melhor.soma
    );


    return melhor;
}


// ============================================================
// FUNÇÃO PRINCIPAL
// ============================================================

int main(void)
{
    int i;


    // ========================================================
    // VETOR
    // ========================================================

   // int vetor[] ={13,-3,-25,20,-3,-16,-23,18,20,-7,12,-5,-22,15,-4,7}; exemplo do slide pag 21
    int vetor[] ={-2, 5, -1, 7, -10, 4};


    // ========================================================
    // CALCULA O TAMANHO DO VETOR
    // ========================================================

    int tamanho;

    tamanho =
        sizeof(vetor) / sizeof(vetor[0]);


    // ========================================================
    // CABEÇALHO
    // ========================================================

    printf("\n");

    printf(
        "====================================================\n"
    );

    printf(
        "       SUBARRANJO CONTIGUO DE SOMA MAXIMA\n"
    );

    printf(
        "       DIVISAO E CONQUISTA\n"
    );

    printf(
        "====================================================\n"
    );


    // ========================================================
    // MOSTRA O VETOR
    // ========================================================

    printf("\nVETOR:\n\n");

    printf("Indice: ");

    for (i = 0; i < tamanho; i++)
    {
        printf("%4d ", i);
    }

    printf("\n");

    printf("Valor : ");

    for (i = 0; i < tamanho; i++)
    {
        printf("%4d ", vetor[i]);
    }

    printf("\n\n");


    printf(
        "Iniciando algoritmo...\n\n"
    );


    // ========================================================
    // EXECUTA O ALGORITMO
    // ========================================================

    Resultado resultado;

    resultado =
        subarranjoMaximo(
            vetor,
            0,
            tamanho - 1,
            0
        );


    // ========================================================
    // RESULTADO FINAL
    // ========================================================

    printf("\n");

    printf(
        "====================================================\n"
    );

    printf(
        "                  RESULTADO FINAL\n"
    );

    printf(
        "====================================================\n\n"
    );


    printf(
        "Maior subarranjo: "
    );

    mostrarSubarranjo(
        vetor,
        resultado.inicio,
        resultado.fim
    );

    printf("\n\n");


    printf(
        "Indice inicial : %d\n",
        resultado.inicio
    );

    printf(
        "Indice final   : %d\n",
        resultado.fim
    );

    printf(
        "Maior soma     : %d\n",
        resultado.soma
    );


    // ========================================================
    // MOSTRA O CÁLCULO DA SOMA
    // ========================================================

    printf("\n");

    printf(
        "Calculo da soma:\n\n"
    );


    for (
        i = resultado.inicio;
        i <= resultado.fim;
        i++
    )
    {
        printf("%d", vetor[i]);

        if (i < resultado.fim)
        {
            printf(" + ");
        }
    }

    printf(
        " = %d\n",
        resultado.soma
    );


    // ========================================================
    // FINAL
    // ========================================================

    printf("\n");

    printf(
        "====================================================\n"
    );

    printf(
        "                       FIM\n"
    );

    printf(
        "====================================================\n"
    );


    return 0;
}