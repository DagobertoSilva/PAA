# Trabalho 02 – Projeto e Análise de Algoritmos (2026.2)

Implementação em **C** das duas questões do Trabalho 02:

| Questão | Problema | Técnica | Arquivo |
|---|---|---|---|
| **Q1** | Balsa com duas pistas | Programação Dinâmica (bottom-up) | `src/q1_balsa.c` |
| **Q2** | Tarefas com prazo e multa | Algoritmo Guloso + Union-Find | `src/q2_tarefas.c` |

- **Discente:** Dagoberto – **Matrícula:** 554237 (ímpar ⇒ Q1 = 1 e Q2 = 2)
- **Disciplina:** Projeto e Análise de Algoritmos – UFC Campus Crateús
- **Professor:** Francisco Antonio Ferreira de Almeida

---

## Sumário

1. [Estrutura do repositório](#1-estrutura-do-repositório)
2. [Requisitos, compilação e execução](#2-requisitos-compilação-e-execução)
3. [Questão 1 – Balsa (Programação Dinâmica)](#3-questão-1--balsa-programação-dinâmica)
4. [Questão 2 – Tarefas com multa (Guloso)](#4-questão-2--tarefas-com-multa-guloso)
5. [Testes automáticos](#5-testes-automáticos)
6. [Roteiro sugerido para o vídeo](#6-roteiro-sugerido-para-o-vídeo)
7. [Limitações e observações](#7-limitações-e-observações)

---

## 1. Estrutura do repositório

```
trabalho02-paa/
├── README.md
├── Makefile
├── src/
│   ├── q1_balsa.c            # Q1: PD completa + PD otimizada + força bruta + autoteste
│   └── q2_tarefas.c          # Q2: guloso + Union-Find + força bruta + autoteste
└── exemplos/
    ├── q1_exemplo_slides.txt # L=10, T=[4,6,3,5,2,7,1]
    ├── q1_exemplo_medio.txt  # L=20, 12 carros
    ├── q2_exemplo_slides.txt # 7 tarefas dos slides
    └── q2_exemplo_medio.txt  # 10 tarefas
```

---

## 2. Requisitos, compilação e execução

Requisito: um compilador C com suporte a C99 (GCC ou Clang). Nenhuma biblioteca externa é usada, apenas `stdio.h`, `stdlib.h` e `string.h`.

### Linux / macOS (com `make`)

```bash
make            # gera bin/q1_balsa e bin/q2_tarefas
make teste      # compila e roda os autotestes das duas questões
make clean
```

### Qualquer sistema (somente `gcc`)

```bash
gcc -std=c99 -O2 -Wall -Wextra -pedantic -o q1_balsa    src/q1_balsa.c
gcc -std=c99 -O2 -Wall -Wextra -pedantic -o q2_tarefas  src/q2_tarefas.c
```

No **Windows** (MinGW/TDM-GCC), use os mesmos comandos e execute com `q1_balsa.exe` e `q2_tarefas.exe`. As mensagens dos programas foram escritas **sem acentos** de propósito, para não aparecerem quebradas no terminal do Windows.

### Opções de execução

| Comando | O que faz |
|---|---|
| `./q1_balsa` | roda o exemplo dos slides (L = 10, T = [4,6,3,5,2,7,1]) |
| `./q1_balsa arquivo.txt` | lê a instância de um arquivo (`-` lê do teclado) |
| `./q1_balsa --otimizado arquivo.txt` | usa a versão Θ(nL) em vez de Θ(nL²) |
| `./q1_balsa --teste` | valida as duas versões contra força bruta |
| `./q2_tarefas` | roda o exemplo dos slides (7 tarefas) |
| `./q2_tarefas arquivo.txt` | lê a instância de um arquivo (`-` lê do teclado) |
| `./q2_tarefas --teste` | valida o guloso contra força bruta |

### Formato dos arquivos de entrada

**Q1** (`L`, depois `n`, depois os tamanhos dos carros na ordem da fila):

```
10
7
4 6 3 5 2 7 1
```

**Q2** (`n`, depois uma linha `prazo multa` por tarefa; prazo ≥ 1 e multa ≥ 0):

```
7
4 70
2 60
4 50
3 40
1 30
4 20
6 10
```

---

## 3. Questão 1 – Balsa (Programação Dinâmica)

### 3.1 Enunciado resumido

Uma balsa tem duas pistas de `L` metros cada. Uma fila com `n` carros `C1..Cn` (tamanhos `T1..Tn`) deve ser embarcada **na ordem da fila**. Deseja-se embarcar o **maior número de carros**, escolhendo a pista de cada um.

### 3.2 Por que `M[1, L, L]` resolve?

`M[k, A, B]` é o maior número de carros de `Ck..Cn` que cabem com a pista 1 tendo `A` metros e a pista 2 tendo `B` metros. Antes de embarcar qualquer carro, a fila inteira está disponível e as duas pistas estão vazias (`A = B = L`). Esse é exatamente o problema original.

### 3.3 (a) Subestrutura ótima

Considere uma solução ótima para o estado `(k, A, B)` e o carro `Ck`:

- se `Ck` vai para a **pista 1** (exige `Tk ≤ A`), os carros restantes formam uma solução de `(k+1, A−Tk, B)`;
- se `Ck` vai para a **pista 2** (exige `Tk ≤ B`), os restantes formam uma solução de `(k+1, A, B−Tk)`;
- se `Ck` não cabe em nenhuma, nenhum carro de `Ck..Cn` embarca (a ordem da fila deve ser respeitada).

**Cortar e colar:** se a parte restante não fosse ótima para o subproblema, trocá-la por uma solução ótima aumentaria o total, contradizendo a otimalidade da solução original.

### 3.4 (b) Recorrência

```
M[k,A,B] = 0                                       se k > n  ou  Tk > max(A,B)
M[k,A,B] = max{ 1 + M[k+1, A−Tk, B]   (se Tk ≤ A),
                1 + M[k+1, A, B−Tk]   (se Tk ≤ B) }   caso contrário
```

Opções inviáveis são descartadas do `max`.

### 3.5 (c) Algoritmo (bottom-up)

```
BALSA(T[1..n], L):
  M[k][A][B] = 0 para todo k, A, B        // k = n+1 é o caso base
  para k = n até 1:
    para A = 0 até L:
      para B = 0 até L:
        melhor = 0
        se T[k] <= A e 1 + M[k+1][A-T[k]][B] > melhor: melhor = ...; esc[k][A][B] = 1
        se T[k] <= B e 1 + M[k+1][A][B-T[k]] > melhor: melhor = ...; esc[k][A][B] = 2
        M[k][A][B] = melhor
  retorne M[1][L][L]
```

Implementado em `balsa_pd()`. **Complexidade:** Θ(n·L²) tempo e espaço.

### 3.6 (d) Rastreio: em qual pista cada carro vai

A tabela `esc[k][A][B]` guarda a decisão tomada (0 = nenhuma, 1 = pista 1, 2 = pista 2). O rastreio parte de `(1, L, L)`:

```
A = L; B = L
para k = 1 até n:
  se esc[k][A][B] == 0: pare            // Ck não cabe
  pista[k] = esc[k][A][B]
  se pista[k] == 1: A = A − T[k]  senão: B = B − T[k]
```

Custo adicional: O(n). O rastreio está no final de `balsa_pd()` e o resultado é impresso por `imprimir_resultado()`.

### 3.7 Melhoria: versão Θ(n·L)

Depois de embarcar os carros `1..k−1`, tem-se `A + B = 2L − (T1 + … + T(k−1))`. Logo, dado `k` e `A`, o valor de `B` fica **determinado**, e basta guardar a tabela `M[k][A]`. É a função `balsa_otimizada()` (opção `--otimizado`). Estados em que `B` ficaria fora de `[0, L]` são inalcançáveis e ignorados.

### 3.8 Exemplo de execução (slides)

Entrada: `L = 10`, `T = [4, 6, 3, 5, 2, 7, 1]`.

```
$ ./q1_balsa
Versao: completa, Theta(nL^2)
Instancia: L = 10 m (por pista), n = 7 carros

Carro   Tamanho   Pista
C1      4         1
C2      6         1
C3      3         2
C4      5         2
C5      2         2
C6      7         nao cabe (embarque encerrado)
C7      1         nao embarcou

M[1,10,10] = 5 carros embarcados
Pista 1: 10/10 m ocupados
Pista 2: 10/10 m ocupados
```

O carro `C6` (7 m) não cabe em nenhuma pista (restam 0 m na pista 1 e 0 m na pista 2). Como a ordem da fila deve ser respeitada, o embarque termina e `C7` não embarca, mesmo cabendo.

### 3.9 Segundo exemplo (`exemplos/q1_exemplo_medio.txt`, L = 20, 12 carros)

```
$ ./q1_balsa --otimizado exemplos/q1_exemplo_medio.txt
...
M[1,20,20] = 6 carros embarcados
Pista 1: 20/20 m ocupados
Pista 2: 16/20 m ocupados
```

---

## 4. Questão 2 – Tarefas com multa (Guloso)

### 4.1 Enunciado e modelagem

`n` tarefas, cada uma consome 1 dia; dias `1..n`; uma tarefa por dia. A tarefa `T` tem prazo `PT` e multa `MT ≥ 0`, paga se for executada após o prazo (independente do atraso). Objetivo: escalonar minimizando a multa total.

**Observação-chave:** só interessa escolher o conjunto `S` de tarefas feitas **no prazo**; as demais pagam multa de qualquer forma e ocupam os dias que sobrarem.

```
multa total = Σ_T MT − Σ_{T∈S} MT
```

Minimizar a multa equivale a **maximizar** `Σ_{T∈S} MT` com `S` viável.

### 4.2 Algoritmo guloso

1. Ordene as tarefas por multa **decrescente**.
2. Para cada tarefa (nessa ordem), coloque-a no **último dia livre** `≤ min(PT, n)`.
3. Se não existir dia livre, a tarefa será atrasada (multa paga).
4. Ao final, as atrasadas ocupam os dias livres restantes (todos posteriores aos seus prazos).

- **Por que o último dia livre?** Deixa livres os dias mais cedo, úteis para tarefas de prazo curto.
- **Por que as atrasadas vão para dias após o prazo?** Quando `T` foi rejeitada, todos os dias `≤ PT` estavam ocupados; dias nunca são liberados, logo os livres restantes são `> PT`.

### 4.3 Pseudocódigo (com Union-Find)

```
ESCALONA(P[1..n], Mu[1..n]):
  ordene índices i por Mu[i] decrescente
  para d = 0 até n: pai[d] = d            // Union-Find: último livre <= d
  para cada i na ordem:
    d = ACHAR(min(P[i], n))
    se d > 0: dia[i] = d; pai[d] = d − 1  // ocupa d
    senão: marque i como atrasada
  preencha os dias livres restantes com as atrasadas
  retorne dia[], soma das multas das atrasadas

ACHAR(d): enquanto pai[d] != d: d = pai[d]   (com compressão de caminho)
          retorne d
```

Implementado em `escalona()` (com `achar()` iterativo e compressão de caminho). O dia `0` funciona como sentinela de "nenhum dia livre".

### 4.4 Corretude (resumo da prova)

Seja `Nt(S) = |{T ∈ S : PT ≤ t}|`.

- **Lema 1 (caracterização).** `S` é viável (todas no prazo) ⇔ `Nt(S) ≤ t` para todo `t = 0..n`. (⇒) As `Nt(S)` tarefas com prazo `≤ t` precisam caber nos dias `1..t`. (⇐) Execute `S` em ordem crescente de prazo: a `j`-ésima tarefa, de prazo `P`, satisfaz `NP(S) ≥ j` e `NP(S) ≤ P`, logo `j ≤ P`.
- **Lema 2 (hereditariedade e troca).** (i) Subconjunto de viável é viável. (ii) Se `A` e `B` são viáveis e `|A| < |B|`, existe `x ∈ B \ A` com `A ∪ {x}` viável.
- **Otimalidade.** Sejam `G = g1..gr` o conjunto aceito pelo guloso (multas decrescentes) e `O = o1..os` um ótimo viável. Por (ii), `|G| = |O|`. Se existisse um primeiro índice `i` com `M(oi) > M(gi)`, aplicar (ii) a `{g1..g(i−1)}` e `{o1..oi}` daria um `oj` (`j ≤ i`) com `{g1..g(i−1), oj}` viável e `M(oj) > M(gi)`; o guloso teria examinado `oj` antes de `gi` e o aceitaria — contradição. Logo `ΣM(gi) ≥ ΣM(oi)` e `G` é ótimo.

### 4.5 Análise de tempo

| Etapa | Custo |
|---|---|
| Ordenação por multa | O(n log n) |
| Busca do último dia livre (Union-Find) | O(α(n)) amortizado por tarefa ⇒ O(n·α(n)) |
| Preencher dias livres com as atrasadas | O(n) |
| **Total** | **O(n log n)** |

Sem Union-Find (varrendo de `PT` até 1 atrás de um dia livre), o custo seria O(n²).

### 4.6 Exemplo de execução (slides)

Entrada: `P = [4,2,4,3,1,4,6]`, `M = [70,60,50,40,30,20,10]`.

```
$ ./q2_tarefas
Instancia: n = 7 tarefas

Passo a passo (tarefas por multa decrescente):
  T1 (multa 70, prazo 4): ultimo dia livre <= 4 e 4 -> dia 4
  T2 (multa 60, prazo 2): ultimo dia livre <= 2 e 2 -> dia 2
  T3 (multa 50, prazo 4): ultimo dia livre <= 4 e 3 -> dia 3
  T4 (multa 40, prazo 3): ultimo dia livre <= 3 e 1 -> dia 1
  T5 (multa 30, prazo 1): nenhum dia livre <= 1 -> ATRASADA
  T6 (multa 20, prazo 4): nenhum dia livre <= 4 -> ATRASADA
  T7 (multa 10, prazo 6): ultimo dia livre <= 6 e 6 -> dia 6
  T5 (atrasada) -> dia livre 5
  T6 (atrasada) -> dia livre 7

Dia   Tarefa  Prazo  Multa    Situacao
1     T4      3      40       no prazo
2     T2      2      60       no prazo
3     T3      4      50       no prazo
4     T1      4      70       no prazo
5     T5      1      30       atrasada (paga multa)
6     T7      6      10       no prazo
7     T6      4      20       atrasada (paga multa)

Multa total minima = 50
```

Multa mínima = 30 + 20 = **50**, igual ao resultado dos slides.

### 4.7 Segundo exemplo (`exemplos/q2_exemplo_medio.txt`, 10 tarefas)

```
$ ./q2_tarefas exemplos/q2_exemplo_medio.txt
...
Multa total minima = 76
```

(tarefas atrasadas: T4, T2, T5, T9 e T7, com multas 25 + 19 + 15 + 12 + 5 = 76).

Para `n > 30` o passo a passo não é impresso, apenas a tabela final.

---

## 5. Testes automáticos

Ambos os programas possuem a opção `--teste`, que gera **3000 instâncias aleatórias** pequenas (semente fixa, reproduzível) e compara com uma **força bruta**:

| Programa | O que é conferido |
|---|---|
| `q1_balsa --teste` | `balsa_pd` = `balsa_otimizada` = recursão exaustiva (n ≤ 14). O rastreio é válido: embarca um prefixo da fila e nenhuma pista estoura o comprimento `L`. |
| `q2_tarefas --teste` | multa do guloso = multa ótima obtida testando todos os subconjuntos viáveis (n ≤ 12, pelo Lema 1). Cada dia de `1..n` é usado exatamente uma vez, e a multa recalculada a partir da escala (`dia > prazo`) coincide com a reportada. |

Saída esperada:

```
$ make teste
./bin/q1_balsa --teste
Autoteste: 3000 casos aleatorios, 0 falha(s).
./bin/q2_tarefas --teste
Autoteste: 3000 casos aleatorios, 0 falha(s).
```

As instâncias aleatórias incluem casos de borda: carros maiores que `L`, prazos maiores que `n`, multas zero e empates de multa.

---
