def conta(A, lo, hi, x):
    """Conta quantas vezes x aparece em A[lo..hi] (inclusive). Memoria O(1)."""
    c = 0
    for i in range(lo, hi + 1):
        if A[i] == x:
            c += 1
    return c


def majoritario_rec(A, lo, hi):
    """Devolve o elemento majoritario de A[lo..hi] ou None se nao existir."""
    if lo == hi:
        return A[lo]

    mid = (lo + hi) // 2
    esq = majoritario_rec(A, lo, mid)
    dir_ = majoritario_rec(A, mid + 1, hi)

    tam = hi - lo + 1
    if esq is not None and conta(A, lo, hi, esq) > tam // 2:
        return esq
    if dir_ is not None and conta(A, lo, hi, dir_) > tam // 2:
        return dir_
    return None


def majoritario(A):
    if not A:
        return None
    return majoritario_rec(A, 0, len(A) - 1)


if __name__ == "__main__":
    testes = [
        [3, 3, 4, 2, 3, 3, 1],      # majoritário: 3 (4 de 7)
        [1, 2, 3, 4],               # não tem
        [5],                        # majoritário: 5
        [2, 2, 1, 1],               # não tem (exatamente metade não basta)
        [7, 7, 7, 7],               # majoritário: 7
        [1, 2, 1, 2, 1],            # majoritário: 1
    ]
    for A in testes:
        r = majoritario(A)
        if r is None:
            print(f"{A} -> nao possui elemento majoritario")
        else:
            print(f"{A} -> elemento majoritario = {r}")
