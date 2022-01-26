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

## Teste de compilação
A execução de todos os testes serão executados e verificados se existe algum erro. 
> Todos os testes serão executados com base nos arquivos da entrega 1.

### Teste 1
O arquivo de teste inicial contém o seguinte conteudo no inicio da compilação:
```
/* Test */
program teste1
    a is int;
    b is float;
begin
    write(a+b);
end
```

```
./main tests/test1.lamp
type: 
match: 
type 17 not match with 19 ()
TOKEN: a
RAW:  is int;
    b is float;
begin
    write(a+b);
end
lamp exception near line: 3 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> O teste 1, que fora criado por nosso grupo, contém um erro na declaração da linguagem. Onde o token `begin` deve estar logo em seguinda no nome do arquivo de teste.

#### Correção

```
./main tests/test1.lamp
type: 
match: .
type 0 not match with 46 ()
TOKEN: end
RAW: 
lamp exception near line: 7 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Agora o erro encontrado, é por causa da falta do sinal terminal `.` que se encontra no fim do arquivo. 

#### Correção
```
./main tests/test1.lamp
Compilation Succesfull. Exiting
```
O arquivo corrigido fica:
```
/* Test */
program teste1
begin
    a is int;
    b is float;
    write(a+b);
end.
```
### Teste 2
O arquivo de teste contém o seguinte conteudo no inicio da compilação:
```
program teste1
 a, b is int;
 result is int;
 a,x is float;
begin
 a = 12a;
 x = 12.0;
 read (a);
 read (b);
 read (c)
 result = (a*b + 1) / (c+2);
 write {Resultado: };
 write (result);
end.
```
> O teste 2 contém um erro na declaração inicial. Onde o token `begin` deve estar logo em seguinda no nome do arquivo de teste.

```
./main tests/test2.lamp
type: 
match: 
type 17 not match with 19 ()
TOKEN: a
RAW: , b is int;
 result is int;
 a,x is float;
begin
 a = 12a;
 x = 12.0;
 read (a);
 read (b);
 read (c)
 result = (a*b + 1) / (c+2);
 write {Resultado: };
 write (result);
end.
lamp exception near line: 2 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
#### Correção
> Encontra-se um erro na atribuição `a = 12a`. Sendo que deve conter um valor de operação. 

```
./main tests/test2.lamp
type: 
match: ;
type 17 not match with 59 ()
TOKEN: result
RAW:  = (a*b + 1) / (c+2);
 write {Resultado: };
 write (result);
end.
lamp exception near line: 11 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Encontra-se com falta de `;` no fim da linha 10 - precisa de uma melhoria de leitura de tokens para mostrar a linha correta.
```
./main tests/test2.lamp
type: 
match: (
type 16 not match with 40 ()
TOKEN: Resultado: 
RAW: ;
 write (result);
end.
lamp exception near line: 12 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Encontra-se com falta do simbolo inicial de parentese para começar a definição do write. 
```
./main tests/test2.lamp
type: ;
match: )
type 59 not match with 41 (;)
TOKEN: Resultado: 
RAW: 
 write (result);
end.
lamp exception near line: 12 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Encontra-se com falta do simbolo final de parentese para começar a definição do write. 
#### Conclusão 
O codigo para ser compilado, ficará na seguinte forma e compilará corretamente.
```
program teste1
begin
 a, b is int;
 result is int;
 a,x is float;
 a = 12*a;
 x = 12.0;
 read (a);
 read (b);
 read (c);
 result = (a*b + 1) / (c+2);
 write ({Resultado: });
 write (result);
end.
```
Saída do compilador.
```
./main tests/test2.lamp
Compilation Succesfull. Exiting
```
### Teste 3
O arquivo de teste contém o seguinte conteudo no inicio da compilação:
```
program teste2
 a, b, c:int;
 d, _var: float;
 teste2 = 1;
 Read (a);
 b = a * a;
 c = b + a/2 * (35/b);
 write c;
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
```
### Correção
```
./main tests/test3.lamp
type: 
match: 
type 17 not match with 19 ()
TOKEN: a
RAW: , b, c:int;
 d, _var: float;
 teste2 = 1;
 Read (a);
 b = a * a;
 c = b + a/2 * (35/b);
 write c;
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
lamp exception near line: 2 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Ocorre o priemeiro erro de compilação, que é a falta de `begin` na compilação.
```
./main tests/test3.lamp
lamp exception near line: 3 
Exception: MALFORMATED RAW STRING

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Erro generico, que no caso mostra que a declaração da variável está errado. 
```
./main tests/test3.lamp
lamp exception near line: 4 
Exception: MALFORMATED RAW STRING

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Erro generico, que no caso mostra que a declaração da variável está errado. 
```
./main tests/test3.lamp
lamp exception near line: 4 
Exception: MALFORMATED RAW STRING

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Erro generico, que no caso mostra que a declaração da variável está errado. 
```
./main tests/test3.lamp
type: (
match: 
type 40 not match with 17 (()
TOKEN: Read
RAW: a);
 b = a * a;
 c = b + a/2 * (35/b);
 write c;
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
lamp exception near line: 6 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Acontece um erro no token `read` que se encontra com R maiusculo. 
```
./main tests/test3.lamp
type: 
match: (
type 17 not match with 40 ()
TOKEN: c
RAW: ;
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
lamp exception near line: 9 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> O identificador write se encontra mal formatado, faltando primeiro parentese.
```
./main tests/test3.lamp
type: ;
match: )
type 59 not match with 41 (;)
TOKEN: c
RAW: 
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
lamp exception near line: 9 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
``` 
> O identificador write se encontra mal formatado, Faltando o parentese de fechamento.
```
./main tests/test3.lamp
lamp exception near line: 10 
Exception: MALFORMATED RAW STRING

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Erro na atribuição da variável.
```
./main tests/test3.lamp
type: 
match: ;
type 17 not match with 59 ()
TOKEN: c
RAW:  = val + 2.2 + a;
 write (val)
end.
lamp exception near line: 11 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta de simbolo terminal `;`
```
./main tests/test3.lamp
type: 
match: ;
type 17 not match with 59 ()
TOKEN: c
RAW:  = val + 2.2 + a;
 write (val)
end.
lamp exception near line: 12
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta de `;`
#### Conclusão
O codigo a ser compilado será:
```
program teste2
begin
 a, b, c is int;
 d, var is float;
 teste2 = 1;
 read (a);
 b = a * a;
 c = b + a/2 * (35/b);
 write (c);
 val = 34.2;
 c = val + 2.2 + a;
 write (val);
end.
```
Após todas as correções e retornará 
```
test3.lamp... 
Compilation Succesfull. Exiting
```

### Teste 4
O arquivo de teste contém o seguinte conteudo no inicio da compilação:
```  ​
program
​a, aux is int;
​b is float
​begin
​b = 0;
​in (a);
​in(b);
​if (a>b) then
​aux = b;
​b = a;
​a = aux
​end;
​write(a;
​write(b)

```

### Correções
> Primeiro erro encontrado é a falta de nome no progama. 
```
./main tests/test4.lamp
type: ,
match: 
type 44 not match with 19 (,)
TOKEN: a
RAW:  aux is int;
 b is float
 begin
 b = 0;
 in (a);
 in(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
lamp exception near line: 2 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Ordem da compilação, a falta de begin após o nome do programa

```
./main tests/test4.lamp
type: 
match: 
type 17 not match with 19 ()
TOKEN: a
RAW: , aux is int;
 b is float
 begin
 b = 0;
 in (a);
 in(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
lamp exception near line: 2 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Falta de `;`
```
./main tests/test4.lamp
type: 
match: ;
type 17 not match with 59 ()
TOKEN: b
RAW:  = 0;
 in (a);
 in(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
lamp exception near line: 5 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Token inexistente `in` que será trocado por `read`
```
./main tests/test4.lamp
type: (
match: 
type 40 not match with 17 (()
TOKEN: in
RAW: a);
 in(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
lamp exception near line: 6 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Token inexistente `in` que será trocado por `read`
```
./main tests/test4.lamp
type: (
match: 
type 40 not match with 17 (()
TOKEN: in
RAW: b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
lamp exception near line: 7 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta de `;`
```
./main tests/test4.lamp
type: ;
match: )
type 59 not match with 41 (;)
TOKEN: a
RAW: 
 write(b)
lamp exception near line: 12 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)

```
> Falta de parentese no write
```
./main tests/test4.lamp
type: ;
match: )
type 59 not match with 41 (;)
TOKEN: a
RAW: 
 write(b)
lamp exception near line: 13 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)

```

> `;` sendo usado incorretamente;
```
./main tests/test4.lamp
type: 
match: write
type 0 not match with 59 ()
TOKEN: b
RAW: 
lamp exception near line: 13
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta de `;`
```
./main tests/test4.lamp
type: 
match: ;
type 0 not match with 59 ()
TOKEN: b
RAW: 
lamp exception near line: 14 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta de `end.`
```
./main tests/test4.lamp
type: 
match: ;
type 0 not match with 59 ()
TOKEN: b
RAW: 
lamp exception near line: 14 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
#### Conclusão
O teste finalisado se encontra:
```
program test4
 begin
 a, aux is int;
 b is float;
 b = 0;
 read(a);
 read(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux;
 end
 write(a);
 write(b);
 end.
```
Compilando corretamente.

### Teste 5
O arquivo de teste contém o seguinte conteudo no inicio da compilação:
```
programa teste4
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is inteiro;
 pontuacaoMinima is char;
begin
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while (pontuacao>0 && (pontuacao<=pontuacaoMaxima) do
 if ((pontuação > pontuacaoMinima) && (disponibilidade==1)) then
 write({Candidato aprovado.})
 else
 write({Candidato reprovado.})
 end
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);
 end
end
```
> Linha 1 contém o nome `programa` ao invés de `program`
```
./main tests/test5.lamp
type: 
match: 
type 17 not match with 1 ()
TOKEN: programa
RAW:  teste4
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is inteiro;
 pontuacaoMinima is char;
begin
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while (pontuacao>0 && (pontuacao<=pontuacaoMaxima) do
 if ((pontuação > pontuacaoMinima) && (disponibilidade==1)) then
 write({Candidato aprovado.})
 else
 write({Candidato reprovado.})
 end
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);
 end
end
lamp exception near line: 1 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Ordem da declaração está errada, `begin` deve vir logo após nome do programa
```
❯ make run ARGS=tests/test5.lamp
./main tests/test5.lamp
type: 
match: 
type 17 not match with 19 ()
TOKEN: pontuacao
RAW: , pontuacaoMaxina, disponibilidade is inteiro;
 pontuacaoMinima is char;
begin
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while (pontuacao>0 && (pontuacao<=pontuacaoMaxima) do
 if ((pontuação > pontuacaoMinima) && (disponibilidade==1)) then
 write({Candidato aprovado.})
 else
 write({Candidato reprovado.})
 end
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);
 end
end
lamp exception near line: 3 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Identificador `inteiro nao existe`
```
./main tests/test5.lamp
type: 
match: ;
type 17 not match with 59 ()
TOKEN: inteiro
RAW: ;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while (pontuacao>0 && (pontuacao<=pontuacaoMaxima) do
 if ((pontuação > pontuacaoMinima) && (disponibilidade==1)) then
 write({Candidato aprovado.})
 else
 write({Candidato reprovado.})
 end
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);
 end
end
lamp exception near line: 4 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Parentese não fechado. 
```
./main tests/test5.lamp
type: 
match: )
type 22 not match with 41 ()
TOKEN: &&
RAW: && (pontuacao<=pontuacaoMaxima) do
 if ((pontuação > pontuacaoMinima) && (disponibilidade==1)) then
 write({Candidato aprovado.})
 else
 write({Candidato reprovado.})
 end
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);
 end
end
lamp exception near line: 4 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
>  Pontuação não pode ser usado na gramatica. 

```
./main tests/test5.lamp
lamp exception near line: 14 
Exception: MALFORMATED RAW STRING

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta de `;`
```
```
./main tests/test5.lamp
type: end
match: ;
type 69 not match with 59 ()
TOKEN: &&
RAW:
 end
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);
 end
end
lamp exception near line: 4 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
>  Pontuação não pode ser usado na gramatica. 

```
./main tests/test5.lamp
lamp exception near line: 14 
Exception: MALFORMATED RAW STRING

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
```

> Falta de simbolo terminal no fim da execução. 
```
./main tests/test5.lamp
type: 
match: .
type 0 not match with 46 ()
TOKEN: end
RAW: 
lamp exception near line: 24 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
#### Correção
Codigo pós correção se encontra:
```
program teste4
/* Teste4 do meu compilador */
begin
 pontuacao, pontuacaoMaxina, disponibilidade is int;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while (pontuacao>0) && (pontuacao<=pontuacaoMaxima) do
 if ((pontuacao > pontuacaoMinima) && (disponibilidade==1)) then
 write({Candidato aprovado.});
 else
 write({Candidato reprovado.})
 end
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);
 end
end.
```
### Teste 6
O arquivo de teste contém o seguinte conteudo no inicio da compilação:
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
### Correção
> Econtra com a o `begin` fora de posição
```
./main tests/test6.lamp
type: 
match: 
type 17 not match with 19 ()
TOKEN: a
RAW: , b, c, maior is int;
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
lamp exception near line: 3 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (igno
```

> Uso de `end` ao invez de `then`
```
./main tests/test6.lamp
type: 
match: .
type 17 not match with 46 ()
TOKEN: maior
RAW:  = a;

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
lamp exception near line: 14 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Falta de `;`
```
./main tests/test6.lamp
type: ;
match: .
type 59 not match with 46 (;)
TOKEN: end
RAW: 
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N)});
 read(outro);
 until (outro == 'N' || outro == 'n')
end
lamp exception near line: 23 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta de `;`
```
./main tests/test6.lamp
type: ;
match: .
type 59 not match with 46 (;)
TOKEN: end
RAW: 
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N)});
 read(outro);
 until (outro == 'N' || outro == 'n')
end
lamp exception near line: 23 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)

```
> Soma de `;`

```./main tests/test6.lamp
type: ;
match: .
type 59 not match with 46 (;)
TOKEN: end
RAW: 
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N)});
 read(outro);
 until (outro == 'N' || outro == 'n')
end
lamp exception near line: 23 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)

```
> String literal mal formatada
```
./main tests/test6.lamp
lamp exception near line: 23 
Exception: MALFORMATED RAW STRING

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```

> Falta de simbolo terminal `.`
```
./main tests/test6.lamp
type: 
match: .
type 0 not match with 46 ()
TOKEN: end
RAW: 
lamp exception near line: 28 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
#### Conclusão
O codigo corrigido ficará
```
/* Teste do meu compilador */
program teste5
begin
 a, b, c, maior is int;
 outro is char;
 repeat
 write({A});
 read(a);
 write({B});
 read(b);
 write({C});
 read(c);
 if ( (a>b) && (a>c) ) then
 maior = a;
 end
 if (b>c) then
 maior = b;
 else
 maior = c;
 end
 write({Maior valor:});
 write (maior);
 write ({Outro? (S/N)});
 read(outro);
 until (outro == 'N' || outro == 'n')
end.
```
### Teste 7
O codigo no inicio da execução era
```
/* Test */
program teste1
    a is int;
    b is float;
begin
    write(a+b);
end
```
#### Correção
> Encontra-se na ordem incorreta o begin e o write
```
./main tests/test7.lamp
type: 
match: 
type 17 not match with 19 ()
TOKEN: a
RAW:  is int;
    b is float;
begin
    write(a+b);
end
lamp exception near line: 3 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
> Falta do simbolo terminal `.`
```
./main tests/test7.lamp
type: 
match: .
type 0 not match with 46 ()
TOKEN: end
RAW: 
lamp exception near line: 7 
Exception: syntax error

Makefile:19: recipe for target 'run' failed
make: [run] Error 1 (ignored)
```
#### Correção
O Codigo final ficaria: 
```
/* Test */
program teste1
begin
    a is int;
    b is float;
    write(a+b);
end.
```
### Considerações
O codigo está em um arquivo apenas, dado que queriamos experimentar uma implementação em C pois precisariamos da mesma para algumas materias no futuro do nosso curso e não tivemos muito contato com a mesma. Com isso o codigo pode conter duplicações e problemas de padrões de C dado o contexto que cada um de nós (Lara e João) estamos inseridos. 


## Autores João Victor Mazagão e Lara Loures