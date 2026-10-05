#include <stdio.h>
#include <math.h>

/*
============================================================
              MÉTODO MESTRE - PASSO A PASSO
============================================================

Modelo:

        T(n) = aT(n/b) + f(n)

Exemplo:

        T(n) = 2T(n/2) + n

Resultado:

        T(n) = Theta(n log n)
============================================================
*/

int main() {

    // =====================================================
    // 1. DEFININDO A RECORRÊNCIA
    // =====================================================

    double a = 2;
    double b = 2;

    printf("====================================================\n");
    printf("              METODO MESTRE EM C\n");
    printf("====================================================\n\n");

    printf("Recorrencia analisada:\n");
    printf("\n");
    printf("        T(n) = 2T(n/2) + n\n\n");

    printf("Modelo geral:\n");
    printf("\n");
    printf("        T(n) = aT(n/b) + f(n)\n\n");

    // =====================================================
    // 2. IDENTIFICANDO a, b E f(n)
    // =====================================================

    printf("----------------------------------------------------\n");
    printf("PASSO 1 - IDENTIFICAR a, b E f(n)\n");
    printf("----------------------------------------------------\n\n");

    printf("a = %.0f\n", a);
    printf("b = %.0f\n", b);
    printf("f(n) = n\n\n");

    printf("Interpretacao:\n");
    printf("- %.0f chamadas recursivas sao realizadas.\n", a);
    printf("- Cada chamada recebe n/%.0f elementos.\n", b);
    printf("- Fora da recursao, fazemos trabalho proporcional a n.\n\n");

    // =====================================================
    // 3. CALCULAR n^(log_b(a))
    // =====================================================

    printf("----------------------------------------------------\n");
    printf("PASSO 2 - CALCULAR n^(log_b(a))\n");
    printf("----------------------------------------------------\n\n");

    printf("Precisamos calcular:\n\n");

    printf("        n^(log_b(a))\n\n");

    printf("Substituindo a = %.0f e b = %.0f:\n\n", a, b);

    printf("        n^(log_%.0f(%.0f))\n\n", b, a);

    double expoente = log(a) / log(b);

    printf("Agora calculamos o logaritmo:\n\n");

    printf("        log_%.0f(%.0f) = %.2f\n\n", b, a, expoente);

    printf("Portanto:\n\n");

    printf("        n^(%.2f)\n", expoente);
    printf("        = n\n\n");

    // =====================================================
    // 4. COMPARAR f(n) COM n^(log_b(a))
    // =====================================================

    printf("----------------------------------------------------\n");
    printf("PASSO 3 - COMPARAR OS CRESCIMENTOS\n");
    printf("----------------------------------------------------\n\n");

    printf("Temos:\n\n");

    printf("        f(n) = n\n");
    printf("        n^(log_b(a)) = n\n\n");

    printf("Os dois possuem o mesmo crescimento!\n\n");

    printf("        f(n) = Theta(n^(log_b(a)))\n\n");

    // =====================================================
    // 5. IDENTIFICAR O CASO
    // =====================================================

    printf("----------------------------------------------------\n");
    printf("PASSO 4 - IDENTIFICAR O CASO DO METODO MESTRE\n");
    printf("----------------------------------------------------\n\n");

    printf("CASO 1:\n");
    printf("f(n) cresce mais devagar.\n\n");

    printf("CASO 2:\n");
    printf("f(n) possui o mesmo crescimento.\n\n");

    printf("CASO 3:\n");
    printf("f(n) cresce mais rapidamente.\n\n");

    printf("Neste problema:\n\n");

    printf("        f(n) = n\n");
    printf("        n^(log_b(a)) = n\n\n");

    printf("Portanto, estamos no:\n\n");

    printf("        >>> CASO 2 <<<\n\n");

    // =====================================================
    // 6. RESULTADO
    // =====================================================

    printf("----------------------------------------------------\n");
    printf("PASSO 5 - RESULTADO FINAL\n");
    printf("----------------------------------------------------\n\n");

    printf("Para o Caso 2:\n\n");

    printf("        T(n) = Theta(n^(log_b(a)) * log n)\n\n");

    printf("Substituindo:\n\n");

    printf("        T(n) = Theta(n * log n)\n\n");

    printf("====================================================\n");
    printf("                  RESPOSTA FINAL\n");
    printf("====================================================\n\n");

    printf("        T(n) = Theta(n log n)\n\n");

    return 0;
}