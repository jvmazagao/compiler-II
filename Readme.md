# Compilador analisador léxico e Tabela de simbolos
    O compilador consiste em um unico arquivo que compila todos 
os valores necessarios para sua compilação. 
    O nosso compiladore é baseado funções estaticas que são chamadas
pela main. 
    Possui apenas dois cabeçalhos `tokens.h` e `hashtable.h` que são
duas interfaces para uma a extração das definições de tokens, e a segunda
para o uso e implementação da Tabela de simbolos. 

## Execução
    O programa faz uso de Makefile, logo para uma pre compilação usa-se 
`make all` para a compilação e para a execução dos testes usa-se `make test`.
Como nessa primeira entrega o make test vai printar toda a tabela de simbolos 
com todos os token pre definidos e os tokens escaneados. 

## Tabelas de simbolos
```
cd tests && ./test.sh
lamp compiler test suite
========================
test1.lamp... 


---- Hash Table ---
@0: float => 13 | char => 15 | if => 2 | while => 8 | write => 11 | 
@1: program => 1 | begin => 19 | is => 18 | int => 14 | until => 7 | do => 9 | read => 10 | teste1 => 17 | a => 17 | 
@2: end => 4 | then => 3 | else => 5 | repeat => 6 | b => 17 | 
-------------------
test2.lamp... 


---- Hash Table ---
@0: float => 13 | char => 15 | if => 2 | while => 8 | write => 11 | x => 17 | c => 17 | 
@1: program => 1 | begin => 19 | is => 18 | int => 14 | until => 7 | do => 9 | read => 10 | teste1 => 17 | a => 17 | 
@2: end => 4 | then => 3 | else => 5 | repeat => 6 | b => 17 | result => 17 | 
-------------------
test3.lamp... 


---- Hash Table ---
@0: float => 13 | char => 15 | if => 2 | while => 8 | write => 11 | c => 17 | 
@1: program => 1 | begin => 19 | is => 18 | int => 14 | until => 7 | do => 9 | read => 10 | a => 17 | 
@2: end => 4 | then => 3 | else => 5 | repeat => 6 | teste2 => 17 | b => 17 | 
-------------------
test4.lamp... 


---- Hash Table ---
@0: float => 13 | char => 15 | if => 2 | while => 8 | write => 11 | 
@1: program => 1 | begin => 19 | is => 18 | int => 14 | until => 7 | do => 9 | read => 10 | a => 17 | aux => 17 | 
@2: end => 4 | then => 3 | else => 5 | repeat => 6 | b => 17 | in => 17 | 
-------------------
test5.lamp... 
lamp: error: 24 - exception: unterminated comment

test6.lamp... 


---- Hash Table ---
@0: float => 13 | char => 15 | if => 2 | while => 8 | write => 11 | c => 17 | 
@1: program => 1 | begin => 19 | is => 18 | int => 14 | until => 7 | do => 9 | read => 10 | a => 17 | 
@2: end => 4 | then => 3 | else => 5 | repeat => 6 | teste5 => 17 | b => 17 | maior => 17 | outro => 17 | 
-------------------
test7.lamp... 


---- Hash Table ---
@0: float => 13 | char => 15 | if => 2 | while => 8 | write => 11 | 
@1: program => 1 | begin => 19 | is => 18 | int => 14 | until => 7 | do => 9 | read => 10 | teste1 => 17 | a => 17 | 
@2: end => 4 | then => 3 | else => 5 | repeat => 6 | b => 17 | 
-------------------
```

## Autores João Victor Mazagão e Lara Loures
