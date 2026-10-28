# Trabalho 01 – Projeto e Análise de Algoritmos

**Universidade Federal do Ceará – Campus Crateús**
**Disciplinas:** Projeto e Análise de Algoritmos / Algoritmos e Complexidade Computacional (CRT0032 / CRT0407)
**Professor:** Francisco Antonio Ferreira de Almeida
**Semestre:** 2026.2
**Matrícula:** 554237
**Prazo de entrega:** 27/10/2026, 23h59 (via SIGAA, em PDF)

---

## 1. Definição das questões

O trabalho tem 5 problemas e cada aluno resolve 2, definidos pela matrícula `M`:

- `Q1 = (M % 5) + 1`
- `Q2 = ((Q1 + 2) % 5) + 1`

Para `M = 554237`:

| Cálculo | Resultado |
|---|---|
| `554237 % 5` | 2 |
| `Q1 = 2 + 1` | **3** |
| `Q2 = ((3 + 2) % 5) + 1 = (5 % 5) + 1` | **1** |

Questões a resolver:

- **Questão 3** – Elemento majoritário (divisão e conquista, sem memória adicional, O(n log n)).
- **Questão 1** – Contagem de inversões (algoritmo Θ(n log n)).

---

## 2. Estrutura sugerida do repositório

```
.
├── README.md
├── questao1_inversoes.py
├── questao3_majoritario.py
└── testes.py   (opcional)
```

Linguagem escolhida: **Python 3** (qualquer linguagem é permitida pelo enunciado).
Para executar: `python3 questao1_inversoes.py` e `python3 questao3_majoritario.py`.

---

## 3. Questão 3 – Elemento majoritário

### 3.1 Enunciado (resumo)

Um vetor `A[1..n]` possui **elemento majoritário** se mais da metade de seus elementos são iguais (ou seja, algum valor aparece **mais de ⌊n/2⌋ vezes**). Deve-se dizer se o vetor possui um e, em caso afirmativo, encontrá-lo, usando **divisão e conquista**, em tempo **O(n log n)** e **sem memória adicional** (nenhum outro vetor ou matriz).

### 3.2 Ideia central (a dica do enunciado)

Divida `A` em duas metades `A1` e `A2`.

**Afirmação:** se `x` é majoritário em `A`, então `x` é majoritário em `A1` **ou** em `A2` (ou em ambas).

**Prova (por contraposição / princípio da casa dos pombos).** Suponha que `x` **não** seja majoritário em `A1` nem em `A2`. Então `x` aparece no máximo `|A1|/2` vezes em `A1` e no máximo `|A2|/2` vezes em `A2`. Somando, `x` aparece no máximo `(|A1| + |A2|)/2 = n/2` vezes em `A`, logo não é majoritário em `A`. Contradição. ∎

Portanto, o único candidato possível a majoritário de `A` está entre:

- o majoritário de `A1` (se existir), e
- o majoritário de `A2` (se existir).

Assim, são no máximo **2 candidatos**. Para decidir qual (se algum) é de fato majoritário em `A`, basta **contar** as ocorrências de cada candidato em todo o intervalo, o que custa O(n) com uma varredura linear e usa apenas um contador (memória O(1)).

### 3.3 Algoritmo

```
MAJORITARIO(A, lo, hi):
    se lo == hi:
        devolva A[lo]                        // vetor de 1 elemento: ele mesmo é majoritário
    mid ← (lo + hi) / 2
    esq ← MAJORITARIO(A, lo, mid)            // candidato da metade esquerda (ou NENHUM)
    dir ← MAJORITARIO(A, mid + 1, hi)        // candidato da metade direita (ou NENHUM)
    tam ← hi - lo + 1
    se esq ≠ NENHUM e CONTA(A, lo, hi, esq) > tam / 2:
        devolva esq
    se dir ≠ NENHUM e CONTA(A, lo, hi, dir) > tam / 2:
        devolva dir
    devolva NENHUM

CONTA(A, lo, hi, x):
    c ← 0
    para i de lo até hi:
        se A[i] == x: c ← c + 1
    devolva c
```

Observações:

- A recursão trabalha **sobre índices** (`lo`, `hi`) do mesmo vetor, **sem copiar** subvetores. Isso cumpre a exigência de não usar memória adicional.
- Se `esq == dir` e ambos existirem, a contagem de `esq` já resolve; a segunda checagem é só redundância inofensiva.
- No máximo um valor pode ser majoritário, então devolver o primeiro que passar no teste é correto.

### 3.4 Implementação (Python)

Arquivo `questao3_majoritario.py`:

```python
def conta(A, lo, hi, x):
    """Conta quantas vezes x aparece em A[lo..hi] (inclusive). Memória O(1)."""
    c = 0
    for i in range(lo, hi + 1):
        if A[i] == x:
            c += 1
    return c


def majoritario_rec(A, lo, hi):
    """Devolve o elemento majoritário de A[lo..hi] ou None se não existir."""
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
            print(f"{A} -> não possui elemento majoritário")
        else:
            print(f"{A} -> elemento majoritário = {r}")
```

Saída esperada:

```
[3, 3, 4, 2, 3, 3, 1] -> elemento majoritário = 3
[1, 2, 3, 4] -> não possui elemento majoritário
[5] -> elemento majoritário = 5
[2, 2, 1, 1] -> não possui elemento majoritário
[7, 7, 7, 7] -> elemento majoritário = 7
[1, 2, 1, 2, 1] -> elemento majoritário = 1
```

> Nota: em Python, a lista de entrada `A` já existe; o algoritmo não cria nenhum outro vetor. As únicas estruturas extras são variáveis escalares e a pilha de recursão.

### 3.5 Exemplo passo a passo

`A = [3, 3, 4, 2, 3, 3, 1]` (índices 0..6)

```
MAJ(0..6)
├── MAJ(0..3) = [3,3,4,2]
│   ├── MAJ(0..1) = [3,3] → esq=3, dir=3 → conta(3)=2 > 1 → 3
│   └── MAJ(2..3) = [4,2] → esq=4, dir=2 → conta(4)=1 > 1? não; conta(2)=1 > 1? não → NENHUM
│   └── combinação em [3,3,4,2]: esq=3 → conta=2 > 2? não; dir=None → NENHUM
└── MAJ(4..6) = [3,3,1]
    ├── MAJ(4..5) = [3,3] → 3
    └── MAJ(6..6) = [1] → 1
    └── combinação: esq=3 → conta=2 > 1? sim → 3
Combinação final em [0..6]: esq=NENHUM; dir=3 → conta(3)=4 > 3? sim → 3
```

Resultado: **3** é o elemento majoritário.

### 3.6 Análise de complexidade

**Tempo.** Cada chamada com `n` elementos faz:

- 2 chamadas recursivas com `n/2` elementos cada;
- no máximo 2 contagens lineares, custo ≤ `2n`, ou seja, Θ(n).

Recorrência:

```
T(n) = 2·T(n/2) + Θ(n),   T(1) = Θ(1)
```

Pelo **Teorema Mestre** (`a = 2`, `b = 2`, `f(n) = Θ(n)`): como `n^(log_b a) = n^(log_2 2) = n` e `f(n) = Θ(n)`, estamos no **caso 2**, logo:

```
T(n) = Θ(n log n)   ⇒   em particular, O(n log n).
```

Verificação pela árvore de recursão: há `log₂ n` níveis e, em cada nível, o trabalho total das contagens é Θ(n) (a soma dos tamanhos dos subproblemas é n). Total: Θ(n · log n).

**Espaço.** Nenhum vetor ou matriz auxiliar é criado. O espaço extra é a pilha de recursão com profundidade `⌈log₂ n⌉` mais algumas variáveis, portanto **O(log n)** de pilha (sem memória adicional proporcional a `n`).

### 3.7 Corretude (resumo)

Por indução no tamanho do intervalo:

- **Base:** com 1 elemento, ele ocupa 100% do intervalo, sendo majoritário.
- **Passo:** pela afirmação da seção 3.2, qualquer majoritário do intervalo é majoritário de pelo menos uma das metades, que por hipótese de indução a recursão encontra corretamente. A etapa de combinação **verifica** cada candidato por contagem direta, então nunca devolve um falso positivo e nunca perde o verdadeiro majoritário.

---

## 4. Questão 1 – Contagem de inversões

### 4.1 Enunciado (resumo)

Seja `X[1..n]` um vetor de inteiros. Para `i < j`, o par `(i, j)` é uma **inversão** se `X[i] > X[j]`. Escrever um algoritmo **Θ(n log n)** que devolva o número de inversões.

### 4.2 Por que não usar força bruta?

A abordagem ingênua compara todos os pares `(i, j)` com `i < j`, o que dá Θ(n²). Para `n = 10⁶` isso é cerca de 5·10¹¹ comparações, inviável. A exigência de Θ(n log n) sugere adaptar o **Merge Sort**.

### 4.3 Ideia central

Divida o vetor ao meio. Toda inversão `(i, j)` cai em exatamente um destes três casos:

1. `i` e `j` estão na **metade esquerda** → contada recursivamente;
2. `i` e `j` estão na **metade direita** → contada recursivamente;
3. `i` está na esquerda e `j` na direita → **inversão cruzada**, contada durante o *merge*.

**Truque do merge.** Suponha que as duas metades já estão ordenadas (a recursão ordena como efeito colateral). Ao intercalar com ponteiros `i` (esquerda) e `j` (direita):

- se `esq[i] ≤ dir[j]`, copia `esq[i]`; não há inversão com `dir[j]`;
- se `esq[i] > dir[j]`, copia `dir[j]` e, como a metade esquerda está ordenada, **todos os elementos restantes da esquerda** (de `i` até o fim) são maiores que `dir[j]`. Soma-se `(tamanho_esq − i)` ao contador.

Cada inversão cruzada é contada uma única vez e sem comparar par a par.

### 4.4 Algoritmo

```
CONTA_INVERSOES(X, tmp, lo, hi):          // intervalo [lo, hi)
    se hi - lo ≤ 1: devolva 0
    mid ← (lo + hi) / 2
    inv ← CONTA_INVERSOES(X, tmp, lo, mid)
    inv ← inv + CONTA_INVERSOES(X, tmp, mid, hi)
    i ← lo; j ← mid; k ← lo
    enquanto i < mid e j < hi:
        se X[i] ≤ X[j]:
            tmp[k] ← X[i]; i ← i + 1
        senão:
            tmp[k] ← X[j]; j ← j + 1
            inv ← inv + (mid - i)          // inversões cruzadas
        k ← k + 1
    copie o que sobrou de X[i..mid) e de X[j..hi) para tmp
    copie tmp[lo..hi) de volta para X[lo..hi)
    devolva inv
```

Usa-se `≤` (e não `<`) para que elementos **iguais não sejam contados** como inversão, já que a definição exige `X[i] > X[j]` estritamente.

### 4.5 Implementação (Python)

Arquivo `questao1_inversoes.py`:

```python
import random


def conta_inversoes_rec(X, tmp, lo, hi):
    """Conta inversões em X[lo:hi] e deixa esse trecho ordenado."""
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
    """Devolve o número de inversões de X sem alterar o vetor original."""
    copia = list(X)
    tmp = [0] * len(copia)
    return conta_inversoes_rec(copia, tmp, 0, len(copia))


def conta_inversoes_forca_bruta(X):
    """O(n^2), usado apenas para validar a versão eficiente."""
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
        print(f"{X} -> {conta_inversoes(X)} inversões")

    # Validação cruzada com força bruta em vetores aleatórios
    for _ in range(500):
        n = random.randint(0, 40)
        X = [random.randint(-20, 20) for _ in range(n)]
        assert conta_inversoes(X) == conta_inversoes_forca_bruta(X), X
    print("Validação com força bruta: OK")
```

Saída esperada:

```
[2, 4, 1, 3, 5] -> 3 inversões
[1, 2, 3, 4, 5] -> 0 inversões
[5, 4, 3, 2, 1] -> 10 inversões
[1, 1, 1] -> 0 inversões
[3, 1, 2] -> 2 inversões
Validação com força bruta: OK
```

### 4.6 Exemplo passo a passo

`X = [2, 4, 1, 3, 5]`

```
[2, 4, 1, 3, 5]
├── [2, 4]            → 0 inversões → ordenado [2, 4]
└── [1, 3, 5]
    ├── [1]           → 0
    └── [3, 5]        → 0 → ordenado [3, 5]
    └── merge [1] e [3,5]: 0 inversões → [1, 3, 5]

Merge de esq=[2,4] com dir=[1,3,5]:
  compara 2 e 1 → 1 menor: copia 1, soma (2 - 0) = 2   (inversões: (2,1) e (4,1))
  compara 2 e 3 → copia 2
  compara 4 e 3 → 3 menor: copia 3, soma (2 - 1) = 1   (inversão: (4,3))
  compara 4 e 5 → copia 4
  sobra 5
Total = 0 + 0 + 2 + 1 = 3
```

### 4.7 Análise de complexidade

**Tempo.** O *merge* percorre cada elemento uma vez (cada passo avança `i` ou `j`), custando Θ(n). Recorrência:

```
T(n) = 2·T(n/2) + Θ(n),   T(1) = Θ(1)
```

Pelo Teorema Mestre (caso 2): **T(n) = Θ(n log n)**. Isso vale para melhor, pior e caso médio, pois o merge sempre percorre todos os elementos, e daí o Θ (e não apenas O).

**Espaço.** Θ(n) para o vetor auxiliar `tmp` (alocado uma única vez) mais O(log n) de pilha de recursão. O enunciado da Questão 1 não proíbe memória auxiliar (diferente das Questões 3 e 4).

### 4.8 Corretude (resumo)

- **Invariante do merge:** ao copiar `dir[j]` antes de `esq[i]`, como `esq[i..mid)` está ordenada de forma não decrescente e `esq[i] > dir[j]`, todos os `mid − i` elementos restantes de `esq` formam inversão com `dir[j]`. Esses pares ainda não foram contados anteriormente, pois `dir[j]` só é consumido uma vez.
- Pares com ambos na mesma metade são contados pelas chamadas recursivas.
- Como cada par `(i, j)` com `i < j` ou está na mesma metade ou cruza a divisão, a soma cobre todas as inversões, sem repetição.

---

## 5. Resumo comparativo

| Item | Questão 3 (majoritário) | Questão 1 (inversões) |
|---|---|---|
| Técnica | Divisão e conquista | Divisão e conquista (Merge Sort adaptado) |
| Recorrência | `T(n) = 2T(n/2) + Θ(n)` | `T(n) = 2T(n/2) + Θ(n)` |
| Tempo | O(n log n) | Θ(n log n) |
| Espaço extra | O(log n) (só pilha) | Θ(n) (vetor `tmp`) |
| Restrição de memória | Sem vetores auxiliares | Sem restrição |
| Ideia-chave | Majoritário do todo ⇒ majoritário de uma metade | Inversões cruzadas contadas no merge |

---
