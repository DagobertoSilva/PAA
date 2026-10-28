import random


def conta_inversoes_rec(X, tmp, lo, hi):
    """Conta inversoes em X[lo:hi] e deixa esse trecho ordenado."""
    if hi - lo <= 1:
        return 0

    mid = (lo + hi) // 2
    inv = conta_inversoes_rec(X, tmp, lo, mid)
    inv += conta_inversoes_rec(X, tmp, mid, hi)

    i, j, k = lo, mid, lo
    while i < mid and j < hi:
        if X[i] <= X[j]:
            tmp[k] = X[i]
            i += 1
        else:
            tmp[k] = X[j]
            j += 1
            inv += mid - i          # todos de X[i..mid) são maiores que X[j]
        k += 1

    while i < mid:
        tmp[k] = X[i]
        i += 1
        k += 1
    while j < hi:
        tmp[k] = X[j]
        j += 1
        k += 1

    X[lo:hi] = tmp[lo:hi]
    return inv


def conta_inversoes(X):
    """Devolve o numero de inversoes de X sem alterar o vetor original."""
    copia = list(X)
    tmp = [0] * len(copia)
    return conta_inversoes_rec(copia, tmp, 0, len(copia))


def conta_inversoes_forca_bruta(X):
    """O(n^2), usado apenas para validar a versao eficiente."""
    n = len(X)
    return sum(1 for i in range(n) for j in range(i + 1, n) if X[i] > X[j])


if __name__ == "__main__":
    exemplos = [
        [2, 4, 1, 3, 5],       # 3 inversões: (2,1), (4,1), (4,3)
        [1, 2, 3, 4, 5],       # 0
        [5, 4, 3, 2, 1],       # 10 (máximo: n(n-1)/2)
        [1, 1, 1],             # 0 (iguais não são inversão)
        [3, 1, 2],             # 2
    ]
    for X in exemplos:
        print(f"{X} -> {conta_inversoes(X)} inversoes")

    # Validação cruzada com força bruta em vetores aleatórios
    for _ in range(500):
        n = random.randint(0, 40)
        X = [random.randint(-20, 20) for _ in range(n)]
        assert conta_inversoes(X) == conta_inversoes_forca_bruta(X), X
    print("Validacoes com forca bruta: OK")
