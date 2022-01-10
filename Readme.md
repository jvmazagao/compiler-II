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

#### Test 1
O teste original tinha como codigo
```
/* Test */
program teste1
    a is int;
    b is float;
begin
    write(a+b);
end
```
Apresenta erros de sintaxe na linha três, que logo em seguida do nome do programa deve começar com `begin`.

Resposta do compilador
```
./main tests/test1.lamp
lamp: error: 3 - exception: syntax error
```
Correção do caso de teste:
```
/* Test */
program teste1
begin
    a is int;
    b is float;
    write(a+b);
end
```
Apresenta um erro de sintaxe no final do arquivo na linha 7, falta o simbolo terminal `.`.
```
./main tests/test1.lamp
lamp: error: 7 - exception: syntax error
```
Correção do caso de teste:
```
/* Test */
program teste1
begin
    a is int;
    b is float;
    write(a+b);
end.
```#### Test 2
O teste original tinha como codigo
```
program teste1
 a, b is int;
 result is int;
 a,x is float;
begin
 a = 12a;
 x = 12.;
 read (a);
 read (b);
 read (c)
 result = (a*b + 1) / (c+2);
 write {Resultado: };
 write (result);
end.
```
Apresenta erros de sintaxe na linha três, que logo em seguida do nome do programa deve começar com `begin`.

Resposta do compilador
```
./main tests/test1.lamp
lamp: error: 2 - exception: syntax error
```
Correção do caso de teste:

```
program teste1
begin
 a, b is int;
 result is int;
 a,x is float;
 a = 12a;
 x = 12.;
 read (a);
 read (b);
 read (c)
 result = (a*b + 1) / (c+2);
 write {Resultado: };
 write (result);
end.
```
Apresenta a falta de um `;` na linha 10, que é apresentado como linha 11 (existe uma possibilidade de melhorria para mostrar a linha correta). 

Resposta do compilador
```
./main tests/test1.lamp
lamp: error: 11 - exception: syntax error
```

Correção do caso de teste:

```
program teste1
begin
 a, b is int;
 result is int;
 a,x is float;
 a = 12a;
 x = 12.;
 read (a);

 read (b);
 read (c);
 result = (a*b + 1) / (c+2);
 write {Resultado: };
 write (result);
end.
```

Apresenta a falta de abertura e fechamento de parenteses na linha 12.

Resposta do compilador
```
./main tests/test1.lamp
lamp: error: 11 - exception: syntax error
```

Correção do codigo:
```
program teste1
begin
 a, b is int;
 result is int;
 a,x is float;
 a = 12a;
 x = 12.;
 read (a);
 read (b);
 read (c);
 result = (a*b + 1) / (c+2);
 write ({Resultado: });
 write (result);
end.
```
Compilação ocorre normalmente.

#### Test 3
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
Erro de compilação na linha 2, falta o `begin` após o nome do arquivo.

```
./main tests/test3.lamp
lamp: error: 2 - exception: syntax error
```
Correção e recompilação:
```
program teste2
begin
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
Erro de compilação na linha 2. Declaração de variável errada, `c:int`.

```
./main tests/test3.lamp
lamp: error: 3 - exception: syntax error
```
Correção e recompilação:
```
program teste2
begin
 a, b, c is int;
 d, var: float;
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
Erro de compilação na linha 4. Declaração de variável errada, `_var: float`.

```
./main tests/test3.lamp
lamp: error: 4 - exception: syntax error
```
Correção e recompilação:
```
program teste2
begin
 a, b, c is int;
 d, var is float;
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
Erro de compilação na linha 4. Declaração de variável errada, `var: float`.

```
./main tests/test3.lamp
lamp: error: 4 - exception: syntax error
```

Correção e recompilação:
```
program teste2
begin
 a, b, c is int;
 d, var is float;
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
Erro de compilação na linha 6. Sintaxe do `Read` está incorreta.

```
./main tests/test3.lamp
lamp: error: 6 - exception: syntax error
```

Correção e recompilação:
```
program teste2
begin
 a, b, c is int;
 d, var is float;
 teste2 = 1;
 read (a);
 b = a * a;
 c = b + a/2 * (35/b);
 write c;
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
```
Erro de compilação na linha 9. Sintaxe do `write` está incorreta faltando parenteses.

```
./main tests/test3.lamp
lamp: error: 9 - exception: syntax error
```
Correção e recompilação:
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
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
```
Erro de compilação na linha 10. Sintaxe do `assign (:=)` está errada.

```
./main tests/test3.lamp
lamp: error: 10 - exception: syntax error
```
Correção e recompilação:
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
 val := 34.2
 c = val + 2.2 + a;
 write (val)
end.
```
Erro de compilação na linha 10. Falta `;`.

```
./main tests/test3.lamp
lamp: error: 10 - exception: syntax error
```

Compilação ocorre normalmente.

#### Test 4

```
 program
 a, aux is int;
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
```

Error: Falta de identifier - nome do programa.
```
./main tests/test4.lamp
lamp: error: 2 - exception: syntax error
```

Correção e recompilação


```
 program test4
 a, aux is int;
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
```

Error: Falta de `begin` após `identifier`
```
./main tests/test4.lamp
lamp: error: 2 - exception: syntax error

```

Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
 b = 0
 in (a);
 in(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
```

> Vi uma oportunidade de fazer uma melhoria na forma como a exceção estava sendo mostrada, sendo assim, apartir desse memomento a exceção fica mais limpa e mais facil a leitura. Podendo assim melhorar a implementação.

Error: Falta de `;`
```
./main tests/test4.lamp
lamp exception near line: 5 
Exception: Syntax error near token: b
```


Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
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
```

Error: O uso de in nesse contexto é errado. Fora substituido por `read`.

```
./main tests/test4.lamp
lamp exception near line: 6 
Exception: Syntax error near token: in
```

Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
 b = 0;
 read(a);
 in(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
```

Error: O uso de in nesse contexto é errado. Fora substituido por `read`.

```
./main tests/test4.lamp
lamp exception near line: 7
Exception: Syntax error near token: in
```

Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
 b = 0;
 read(a);
 read(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
```

Error: O uso de in nesse contexto é errado. Fora substituido por `read`.

```
./main tests/test4.lamp
lamp exception near line: 7
Exception: Syntax error near token: in
```
Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
 b = 0;
 read(a);
 read(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux
 end;
 write(a;
 write(b)
```
Falta de `;`
```
./main tests/test4.lamp
lamp exception near line: 12
Exception: Syntax error near token: end
```
Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
 b = 0;
 read(a);
 read(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux;
 end;
 write(a;
 write(b)
```
Uso de `;`
```
./main tests/test4.lamp
lamp exception near line: 12
Exception: Syntax error near token: end
```
```
Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
 b = 0;
 read(a);
 read(b);
 if (a>b) then
 aux = b;
 b = a;
 a = aux;
 end
 write(a;
 write(b)
```
Falta de `)`
```
./main tests/test4.lamp
lamp exception near line: 13
Exception: Syntax error near token: write
```

Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
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
```
Falta de `;`
```
./main tests/test4.lamp
lamp exception near line: 14
Exception: Syntax error near token: write
```

Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
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
```
Loop infinito por causa da falta de end.
```
./main tests/test4.lamp
```

Correção e recompilação

```
 program test4
 a, aux is int;
 b is float;
 begin
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

#### Test 5

```
programa teste4
/* Teste4 do meu compilador
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
Erro
```
lamp exception near line: 1 
Exception: Syntax error near token: programa
```

Correção e recompilação
```
program teste4
/* Teste4 do meu compilador
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
Erro
```
./main tests/test5.lamp
lamp exception near line: 24 
Exception: unterminated comment
```
Correção e recompilação

```
program teste4 
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

```
./main tests/test5.lamp
lamp exception near line: 3 
Exception: Syntax error near token: pontuacao
```
Correção e recompilação
```
program teste4 begin
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is inteiro;
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
```
./main tests/test5.lamp
lamp exception near line: 3 
Exception: Syntax error near token: pontuacao
```

Correção e recompilação
```
program teste4 begin
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is int;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while (pontuacao>0) && (pontuacao<=pontuacaoMaxima) do
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
./main tests/test5.lamp
lamp exception near line: 13 
Exception: Syntax error near token: &&
```
Correção e recompilação
```
program teste4 begin
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is int;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while ((pontuacao>0) && (pontuacao<=pontuacaoMaxima)) do
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
./main tests/test5.lamp
lamp exception near line: 13 
Exception: Syntax error near token: pontuacao
```

Correção e recompilação
```
program teste4 begin
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is int;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while ((pontuacao>0) && (pontuacao<=pontuacaoMaxima)) do
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
./main tests/test5.lamp
lamp exception near line: 16 
Exception: Syntax error near token: else
```

Correção e recompilação
```
program teste4 begin
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is int;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while ((pontuacao>0) && (pontuacao<=pontuacaoMaxima)) do
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
./main tests/test5.lamp
lamp exception near line: 16 
Exception: Syntax error near token: else
```

Correção e recompilação
```
program teste4 begin
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is int;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while ((pontuacao>0) && (pontuacao<=pontuacaoMaxima)) do
 if ((pontuacao > pontuacaoMinima) && (disponibilidade==1)) then
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
./main tests/test5.lamp
lamp exception near line: 24
Exception: Syntax error near token: en
```

Correção e recompilação
```
program teste4 begin
/* Teste4 do meu compilador */
 pontuacao, pontuacaoMaxina, disponibilidade is int;
 pontuacaoMinima is char;
 pontuacaoMinima = 50;
 pontuacaoMaxima = 100;
 write({Pontuacao do candidato: });
 read(pontuacao);
 write({Disponibilidade do candidato: });
 read(disponibilidade);

 while ((pontuacao>0) && (pontuacao<=pontuacaoMaxima)) do
 if ((pontuacao > pontuacaoMinima) && (disponibilidade==1)) then
 write({Candidato aprovado.})
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
./main tests/test5.lamp
lamp exception near line: 24
Exception: Syntax error near token: en
```

#### Test 6

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
 until (outro == 'N' || outro == 'n)
end
```
```
lamp exception near line: 3 
Exception: Syntax error near token: a
```

Correção e recompilação
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
 until (outro == 'N' || outro == 'n)
end
```
```
lamp exception near line: 13 
Exception: Syntax error near token: end
```
Correção e recompilação
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
 until (outro == 'N' || outro == 'n)
end
```
```
lamp exception near line: 13 
Exception: Syntax error near token: end
```
Correção e recompilação
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
 until (outro == 'N' || outro == 'n)
end
```
```
lamp exception near line: 16
Exception: Syntax error near token: else
```
Correção e recompilação
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

 else
 if (b>c) then
 maior = b;

 else
 maior = c;
 end
 end;
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N)});
 read(outro);
 until (outro == 'N' || outro == 'n)
end
```
```
lamp exception near line: 23
Exception: Syntax error near token:end
```

Correção e recompilação
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

 else
 if (b>c) then
 maior = b;

 else
 maior = c;
 end
 end;
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N)});
 read(outro);
 until (outro == 'N' || outro == 'n)
end
```
```
lamp exception near line: 24
Exception: Syntax error near token:read;
```
Correção e recompilação
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

 else
 if (b>c) then
 maior = b;

 else
 maior = c;
 end
 end;
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N));
 read(outro);
 until (outro == 'N' || outro == 'n)
end
```
```
lamp exception near line: 25 
Exception: CHARACTER NOT FORMATTED
```
Correção e recompilação
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

 else
 if (b>c) then
 maior = b;

 else
 maior = c;
 end
 end;
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N));
 read(outro);
 until (outro == 'N' || outro == 'n')
end
```
```
lamp exception near line: 25 
Exception: CHARACTER NOT FORMATTED
```
Correção e recompilação
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

 else
 if (b>c) then
 maior = b;

 else
 maior = c;
 end
 end;
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N));
 read(outro);
 until (outro == 'N' || outro == 'n')
end
```
```
lamp exception near line: 26 
Exception: Syntax error near token: end
```
Correção e recompilação
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

 else
 if (b>c) then
 maior = b;

 else
 maior = c;
 end
 end;
 write({Maior valor:}});
 write (maior);
 write ({Outro? (S/N));
 read(outro);
 until (outro == 'N' || outro == 'n')
end.
```

#### Test 7
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
lamp exception near line: 3 
Exception: Syntax error near token: a
```
Corrigindo e recompilando
```
/* Test */
program teste1
begin
    a is int;
    b is float;
    write(a+b);
end
```

```
lamp exception near line: 7 
Exception: Syntax error near token: end
```
Corrigindo e recompilando
```
/* Test */
program teste1
begin
    a is int;
    b is float;
    write(a+b);
end.
```

## Considerações
O codigo está em um arquivo apenas, dado que queriamos experimentar uma implementação em C pois precisariamos da mesma para algumas materias no futuro do nosso curso e não tivemos muito contato com a mesma. Com isso o codigo pode conter duplicações e problemas de padrões de C dado o contexto que cada um de nós (Lara e João) estamos inseridos. 


## Autores João Victor Mazagão e Lara Loures
