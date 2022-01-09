# Compilador LAMP

## Analisador léxico e Tabela de simbolos

O compilador consiste em um unico arquivo que compila todos os valores necessarios para sua compilação. O nosso compiladore é baseado funções estaticas que são chamadas
pela main. Possui apenas um cabeçalho `hashtable.h` que é a interface que da forma a nossa tabela de simbolos.  

### Arquitetura do compilador
O compilador contem funções que são chamadas estaticamentes.
 
A primeira função é a função `error` que é responsavél por printar a mensagem de erro e parar a execução da compilação. 

A função `readin` tem como função abrir o arquivo e popular o nosse raw com aquela string que está no arquivo. Essa função pode lançar diversas exceções e finalizar a compilação. 

A função `ident` tem como obrigação buscar os valores dos tokens literais, ou seja, aquelas strings que não são simbolos logicos, como `&&`, etc. Essa função tem como finalidade mostrar tanto os tokens reservados, como as variáveis - denominadas identificadores.

A função `number` tem como responsabilidade a construção dos valores numéricos inteiros ou flutuantes. 

A função `comment` tem como responsabilidade a leitura de um comentário.

A função `literal` tem como responsabilidade a leitura de uma string constante.

A função `parser` é o parser do programa que usa o lexico como função - aqui ele não tem um função bem definida, ele apenas printa os tokens lidos. 

A função `words` consiste nas palavras reservadas, que usam a tabela de simbolos para bloquear essas palavras. 

## Execução
    O programa faz uso de Makefile, logo para uma pre compilação usa-se 
`make all` para a compilação e para a execução dos testes usa-se `make test`.
Como nessa primeira entrega o make test vai printar toda a tabela de simbolos 
com todos os token pre definidos e os tokens escaneados. 
### Execução de um arquivo
    Caso seja nescessário a executção de apenas um arquivo melo compilador,
deverá executar o comand `make ARGS=file`;
    - O arquivo retornará a string Compilation Succesfull. Exiting. se o arquivo compilar corretamente sem nenhum erro de sintaxe. 
    - Caso retorne algum erro retornará qual token e e onde aconteceu o erro. 

## Mudanças de codigo
Para uma melhor legibilidade ocorreram alterações na forma como o compilador 
interpreta os erros, e com isso melhorou a leitura dos mesmos. 

Encontramos um erro de interpretação na forma como o comentário era lido e não retornava erro quando o mesmo nao se fechava. Com isso resultou numa modificação de codigo. 

## Mudanças na gramatica
Não fora necessários nenhuma mudanças na gramática, porém os casos de testes foram modificados.
### Caso 1:
Varios casos de testes de encontravam com a declaração de suas variáveis antes do token `begin`, porem a instrução da gramática mostrava que, essas declarações deveriam vir após o `begin`.

### Caso 2:
Varios casos de testes não continham o token terminal `.` que significava o fim da execução do arquivo, com isso retornando o valor integral. 

### Caso 3:
Varios leques de `;` não fechavam os blocos e com isso incrementamos os tokens e o codigo compilou normalmente. 

## Considerações
O codigo está em um arquivo apenas, dado que queriamos experimentar uma implementação em C pois precisariamos da mesma para algumas materias no futuro do nosso curso e não tivemos muito contato com a mesma. Com isso o codigo pode conter duplicações e problemas de padrões de C dado o contexto que cada um de nós (Lara e João) estamos inseridos. 

### Teste 6

O caractere `'n` está mal formatado, a correção para `'n'` corrige a compilação.

> Correção do codigo
```
/* Teste do meu compilador */
program teste5
 a, b, c, maior is int;
 outro is char;
begin
 repeat
 write({A});
 read(a);
 write({B});
 read(b);
 write({C});
 read(c);
 if ( (a>b) && (a>c) ) end
 maior = a

 else
 if (b>c) then
 maior = b;

 else
 maior = c
 end
 end;
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N)});
 read(outro);
 until (outro == 'N' || outro == 'n')
end
```
Recompilação
```
test6.lamp... 
Token: program - Type: 1
Token: teste5 - Type: 17
Token: a - Type: 17
Token: a - Type: 44
Token: b - Type: 17
Token: b - Type: 44
Token: c - Type: 17
Token: c - Type: 44
Token: maior - Type: 17
Token: is - Type: 18
Token: int - Type: 14
Token: int - Type: 59
Token: outro - Type: 17
Token: is - Type: 18
Token: char - Type: 15
Token: char - Type: 59
Token: begin - Type: 19
Token: repeat - Type: 6
Token: write - Type: 11
Token: write - Type: 40
Token: A - Type: 16
Token: A - Type: 41
Token: A - Type: 59
Token: read - Type: 10
Token: read - Type: 40
Token: a - Type: 17
Token: a - Type: 41
Token: a - Type: 59
Token: write - Type: 11
Token: write - Type: 40
Token: B - Type: 16
Token: B - Type: 41
Token: B - Type: 59
Token: read - Type: 10
Token: read - Type: 40
Token: b - Type: 17
Token: b - Type: 41
Token: b - Type: 59
Token: write - Type: 11
Token: write - Type: 40
Token: C - Type: 16
Token: C - Type: 41
Token: C - Type: 59
Token: read - Type: 10
Token: read - Type: 40
Token: c - Type: 17
Token: c - Type: 41
Token: c - Type: 59
Token: if - Type: 2
Token: if - Type: 40
Token: if - Type: 40
Token: a - Type: 17
Token: a - Type: 62
Token: a - Type: 41
Token: a - Type: 97
Token: a - Type: 40
Token: a - Type: 17
Token: a - Type: 62
Token: a - Type: 41
Token: a - Type: 41
Token: end - Type: 69
Token: maior - Type: 17
Token: maior - Type: 61
Token: a - Type: 17
Token: else - Type: 5
Token: if - Type: 2
Token: if - Type: 40
Token: b - Type: 17
Token: b - Type: 62
Token: b - Type: 41
Token: then - Type: 3
Token: maior - Type: 17
Token: maior - Type: 61
Token: b - Type: 17
Token: b - Type: 59
Token: else - Type: 5
Token: maior - Type: 17
Token: maior - Type: 61
Token: c - Type: 17
Token: end - Type: 69
Token: end - Type: 69
Token: end - Type: 59
Token: write - Type: 11
Token: write - Type: 40
Token: Maior valor: - Type: 16
Token: Maior valor: - Type: 16
Token: Maior valor: - Type: 41
Token: Maior valor: - Type: 59
Token: write - Type: 11
Token: write - Type: 40
Token: maior - Type: 17
Token: maior - Type: 41
Token: maior - Type: 59
Token: write - Type: 11
Token: write - Type: 40
Token: Outro? (S/N) - Type: 16
Token: Outro? (S/N) - Type: 41
Token: Outro? (S/N) - Type: 59
Token: read - Type: 10
Token: read - Type: 40
Token: outro - Type: 17
Token: outro - Type: 41
Token: outro - Type: 59
Token: until - Type: 7
Token: until - Type: 40
Token: outro - Type: 17
Token: outro - Type: 22
Token: outro - Type: 67
Token: outro - Type: 111
Token: outro - Type: 17
Token: outro - Type: 22
Token: outro - Type: 67
Token: outro - Type: 41
Token: end - Type: 69


---- Hash Table ---
@0: float => 13 | char => 15 | if => 2 | while => 8 | write => 11 | c => 17 | 
@1: program => 1 | begin => 19 | is => 18 | int => 14 | until => 7 | do => 9 | read => 10 | a => 17 | 
@2: end => 69 | then => 3 | else => 5 | repeat => 6 | teste5 => 17 | b => 17 | maior => 17 | outro => 17 | 
-------------------

```

## Autores João Victor Mazagão e Lara Loures
