# rpc — Reverse Polish Calculator

Calculadora de linha de comando que avalia expressões em notação polonesa reversa (RPN). Implementada em C puro, sem dependências externas, usando uma stack alocada estaticamente em um array global.

Há duas variantes:

| Arquivo | Tipo de dado | Suporte a decimais | Proteção contra divisão por zero |
|---|---|---|---|
| `main.c` | `int` | Não | Não |
| `mainf.c` | `double` | Sim (`,` ou `.`) | Sim |

## O que é RPN?

Na notação polonesa reversa os operandos são empilhados antes do operador. Ao encontrar um operador, os dois valores do topo da stack são consumidos e o resultado é empurrado de volta.

```
Expressão infixa:  (3 + 4) * 2
RPN equivalente:   3 4 + 2 *
```

## Como funciona

### Stack

A stack é um array global de tamanho fixo (`BUFSIZE = 1024`). Um ponteiro `ptr` percorre o array: `push` avança o ponteiro após escrever; `pop` recua o ponteiro e retorna o valor.

```c
void push(int num, int **ptr) {
    **ptr = num;
    (*ptr)++;
}

int pop(int **ptr) {
    (*ptr)--;
    return **ptr;
}
```

Ambas as operações têm proteção contra overflow e underflow da stack.

### Loop de entrada

O programa lê caractere a caractere, sem `scanf` ou parsing de linha completa. A lógica por token é:

- **Dígito** → acumula o número (`num = num * 10 + digito`)
- **Espaço ou `\n`** → finaliza o número acumulado e o empurra na stack
- **Operador** (`+ - * /`) → faz pop de dois operandos, calcula e empurra o resultado; imprime o topo da stack
- **`EOF`** → encerra o programa

### Versão float (`mainf.c`)

Além de operar com `double`, a versão float adiciona:

- **Decimais** — ao encontrar `.` ou `,`, os dígitos seguintes são interpretados como parte fracionária
- **Sinal negativo** — o `-` seguido imediatamente de um dígito é interpretado como número negativo (não como operador de subtração); a função auxiliar `see()` faz lookahead de um caractere sem consumir o stream
- **Divisão por zero** — verificada antes de executar a divisão

```c
char see(void) {
    char c = getchar();
    ungetc((unsigned char)c, stdin);
    return c;
}
```

## Compilação

```bash
# Versão inteira
gcc -o rpc main.c

# Versão float
gcc -o rpcf mainf.c -lm
```

A flag `-lm` é necessária na versão float por conta do uso de `pow()` da `<math.h>`.

## Uso

O programa opera em loop contínuo. Tokens são separados por espaço ou quebra de linha. Cada operador imprime o resultado parcial do topo da stack.

```
$ ./rpc
3
4
+
= 7
2
*
= 14
```

```
$ ./rpcf
3.5
1,5
+
= 5.000000
2
/
= 2.500000
```

Para encerrar: `Ctrl+D` (envia EOF).

## Exemplos de expressões

| Expressão infixa | Entrada RPN | Resultado |
|---|---|---|
| `2 + 3` | `2 3 +` | `5` |
| `10 - 4` | `10 4 -` | `6` |
| `(2 + 3) * 4` | `2 3 + 4 *` | `20` |
| `10 / (2 + 3)` | `10 2 3 + /` | `2` |
| `-1.5 + 3` | `-1.5 3 +` | `1.500000` |

## Estrutura do projeto

```
rpc/
├── main.c    # Calculadora RPN com inteiros
└── mainf.c   # Calculadora RPN com ponto flutuante
```
