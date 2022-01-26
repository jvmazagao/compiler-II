#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>
#include "hashtable.h"
#include <regex.h>

#define TOKEN_PROGRAM 1
#define TOKEN_IF 2
#define TOKEN_THEN 3
#define TOKEN_END 'E'
#define TOKEN_ELSE 5
#define TOKEN_REPEAT 6
#define TOKEN_UNTIL 7
#define TOKEN_WHILE 8
#define TOKEN_DO 9
#define TOKEN_READ 10
#define TOKEN_WRITE 11
#define TOKEN_FLOAT 13
#define TOKEN_INTEGER 14
#define TOKEN_CHARACTER 15
#define TOKEN_LITERAL 16
#define TOKEN_IDENTIFIER 17
#define TOKEN_IS 18
#define TOKEN_BEGIN 19
#define TOKEN_COMMENT 20
#define TOKEN_AND 'a'
#define TOKEN_OR 'o'
#define TOKEN_EQUALS 22
#define TOKEN_GREATER '>'
#define TOKEN_GREATER_THAN_EQUALS 'g'
#define TOKEN_LESSER '<'
#define TOKEN_LESSER_THAN_EQUALS 'l'
#define TOKEN_DIFFERENT '!'
#define TOKEN_SUM '+'
#define TOKEN_MINUS '-'
#define TOKEN_MULTIPLY '*'
#define TOKEN_QUOTIENT '/'
#define TOKEN_ASSIGN '='
#define TOKEN_SEMICOLON ';'
#define TOKEN_COMMA ','
#define TOKEN_LPAREN '('
#define TOKEN_RPAREN ')'
#define TOKEN_NEGATIVE '!'
#define TOKEN_EOF '.'
#define TOKEN_FLOAT_CONST 'F'
#define TOKEN_INTEGER_CONST 'I'
#define TOKEN_CHAR_CONST 'C'

static char *raw, *token;
static int type;
static size_t line = 1;

static void error(const char *fmt, ...)
{
    va_list ap;
    (void)fprintf(stderr, "lamp exception near line: %lu \nException: %s", line, fmt);
    va_start(ap, fmt);
    (void)fputc('\n', stderr);
    va_end(ap);
    (void)fputc('\n', stderr);
    exit(1);
}

static void debug(char *value)
{
    (void)fputs(value, stdout);
}

static void readin(char **file)
{
    int fd;
    struct stat st;

    if (strrchr(file, '.') == NULL)
        error("file must end in '.lamp");

    if (!!strcmp(strrchr(file, '.'), ".lamp"))
        error("file must end in '.lamp'");

    if ((fd = open(file, O_RDONLY)) == -1)
        error("couldn't open %s", file);

    if (fstat(fd, &st) == -1)
        error("couldn't get file size");

    if ((raw = malloc(st.st_size + 1)) == NULL)
        error("malloc failed");

    if (read(fd, raw, st.st_size) != st.st_size)
        error("couldn't read %s", file);

    raw[st.st_size] = '\0';

    (void)close(fd);
}

static int ident(void)
{
    char *p;
    size_t i, len;

    p = raw;
    while (isalnum(*raw) || *raw == '_')
        ++raw;

    len = raw - p;

    --raw;

    free(token);

    if ((token = malloc(len + 1)) == NULL)
        error("malloc failed");

    for (i = 0; i < len; i++)
        token[i] = *p++;
    token[i] = '\0';

    if (!strcmp(token, "float"))
        return TOKEN_FLOAT;
    else if (!strcmp(token, "int"))
        return TOKEN_INTEGER;
    else if (!strcmp(token, "char"))
        return TOKEN_CHARACTER;
    else if (!strcmp(token, "program"))
        return TOKEN_PROGRAM;
    else if (!strcmp(token, "begin"))
        return TOKEN_BEGIN;
    else if (!strcmp(token, "end"))
        return TOKEN_END;
    else if (!strcmp(token, "if"))
        return TOKEN_IF;
    else if (!strcmp(token, "then"))
        return TOKEN_THEN;
    else if (!strcmp(token, "while"))
        return TOKEN_WHILE;
    else if (!strcmp(token, "do"))
        return TOKEN_DO;
    else if (!strcmp(token, "else"))
        return TOKEN_ELSE;
    else if (!strcmp(token, "repeat"))
        return TOKEN_REPEAT;
    else if (!strcmp(token, "until"))
        return TOKEN_UNTIL;
    else if (!strcmp(token, "read"))
        return TOKEN_READ;
    else if (!strcmp(token, "write"))
        return TOKEN_WRITE;
    else if (!strcmp(token, "is"))
        return TOKEN_IS;
    return TOKEN_IDENTIFIER;
}

static int number(void)
{
    int type_token = TOKEN_INTEGER_CONST;
    char *p;
    size_t i, j = 0, len;
    p = raw;
    while (isdigit(*raw) || (*raw) == '.')
    {
        ++raw;
    }
    len = raw - p;

    --raw;

    free(token);

    if ((token = malloc(len + 1)) == NULL)
        error("malloc failed");

    for (i = 0; i < len; i++)
    {
        if (isdigit(*p) || (*p) == '.')
            token[j++] = *p;
        if ((*p) == '.')
        {
            type_token = TOKEN_FLOAT_CONST;
            ++p;
            if (isdigit(*p))
            {
                token[j++] = *p;
            }
            else
            {
                error("FLOAT CONST NOT FORMATTED");
            }
        }
        ++p;
    }
    token[j] = '\0';
    return type_token;
}

static int comment(void)
{
    int ch;
    if ((ch = *raw++) == '/')
    {
        if ((ch = *raw++) == '*')
        {
            while ((ch = *raw++) != '/')
            {
                if (ch == '\0')
                {
                    error("unterminated comment");
                }
                if (ch == '\n')
                {
                    ++line;
                }
            }
        }
        if (ch == '*' && (ch = *raw++) == '/')
        {
            return TOKEN_COMMENT;
        }
    }
}

static int literal(void)
{
    char *p;
    size_t i, len;

    p = ++raw;
    while (isalnum(*raw) || *raw != '}')
        ++raw;

    len = raw - p;

    --raw;

    free(token);

    if ((token = malloc(len + 1)) == NULL)
        error("malloc failed");

    for (i = 0; i < len; i++)
        token[i] = *p++;
    token[i] = '\0';

    while (*raw != '}')
        ++raw;
    return TOKEN_LITERAL;
}

static int lex(void)
{
again:
    while (*raw == ' ' || *raw == '\t' || *raw == '\n')
    {
        if (*raw++ == '\n')
        {
            ++line;
        }
    }

    if (isalpha(*raw))
    {
        return ident();
    }
    if (isdigit(*raw))
    {
        return number();
    }

    switch (*raw)
    {
    case '{':
        return literal();
    case '/':
        if (comment() == TOKEN_COMMENT)
            goto again;
        return TOKEN_QUOTIENT;
    case '=':
        if (*++raw == '=')
            return TOKEN_EQUALS;
        return TOKEN_ASSIGN;
    case ',':
        return TOKEN_COMMA;
    case ';':
        return TOKEN_SEMICOLON;
    case '>':
        if (*++raw == '=')
            return TOKEN_GREATER_THAN_EQUALS;
        return TOKEN_GREATER;
    case '<':
        if (*++raw == '=')
            return TOKEN_LESSER_THAN_EQUALS;
        return TOKEN_LESSER;
    case '+':
        return TOKEN_SUM;
    case '-':
        return TOKEN_MINUS;
    case '*':
        return TOKEN_MULTIPLY;
    case '(':
        return TOKEN_LPAREN;
    case ')':
        return TOKEN_RPAREN;
    case '!':
        if (*++raw == '=')
            return TOKEN_DIFFERENT;
        return TOKEN_NEGATIVE;
    case '.':
        return TOKEN_EOF;
    case '|':
        if (*++raw == '|')
            return TOKEN_OR;
        error("OR NOT FORMATED");
    case '&':
        if (*++raw == '&')
            return TOKEN_AND;
        error("AND NOT FORMATED");
    case '\'':
        if (isalnum(*++raw) && (*++raw == '\''))
            return TOKEN_CHAR_CONST;
        error("CHARACTER NOT FORMATTED");
    default:
        if (strlen(raw) == 0)
            return 0;
        // (void) fprintf(stdout, "DEFAULTS %i\n", strlen(raw));
        error("MALFORMATED RAW STRING");
    }
}

static void next(void)
{
    type = lex();
    raw++;
}

static void expect(int match)
{
    if (match != type)
    {
        (void)fprintf(stdout, "type: %c\n", type);
        (void)fprintf(stdout, "match: %c\n", match);
        (void)fprintf(stdout, "type %i not match with %i (%c)\n", type, match, type);
        (void)fprintf(stdout, "TOKEN: %s\n", token);
        (void)fprintf(stdout, "RAW: %s\n", raw);
        error("syntax error");
    }
    next();
}

static void block(HashTable *ht)
{
    bool write_state = false;
    bool if_state = false;
    bool while_state = false;
    bool then_state = false;
    bool do_state = false;
    bool repeat_state = true;

    int lparen_depth = 0;

    goto program;

decl_list:
    goto decl;

decl:
    if (type == TOKEN_IS)
    {
        expect(TOKEN_IS);
        goto type_assign;
    }
    goto ident_list;

ident_list:
    if (type == TOKEN_COMMA)
    {
        expect(TOKEN_COMMA);
        goto ident_list;
    }
    goto identifier;

identifier:
    expect(TOKEN_IDENTIFIER);
    if (ht_search(ht, token) == NULL)
    {
        ht_insert(ht, token, TOKEN_IDENTIFIER);
    }
    if (type == TOKEN_ASSIGN)
    {
        expect(TOKEN_ASSIGN);
        goto simple_expr;
    }
    goto decl;

type_assign:
    if (type == TOKEN_INTEGER)
    {
        expect(TOKEN_INTEGER);
    }
    if (type == TOKEN_FLOAT)
    {
        expect(TOKEN_FLOAT);
    }
    if (type == TOKEN_CHARACTER)
    {
        expect(TOKEN_CHARACTER);
    }

    if (type != TOKEN_IDENTIFIER)
    {
        goto stmt_list;
    }
    expect(TOKEN_SEMICOLON);
    goto decl_list;

stmt_list:
    switch (type)
    {
    case TOKEN_IDENTIFIER:
    case TOKEN_ASSIGN:
    case TOKEN_READ:
    case TOKEN_WRITE:
    case TOKEN_IF:
    case TOKEN_WHILE:
    case TOKEN_REPEAT:
        goto stmt;
    case TOKEN_DO:
        goto do_stmt;
    case TOKEN_THEN:
        goto then_stmt;
    case TOKEN_ELSE:
        expect(TOKEN_ELSE);
        if (then_state != true)
        {
            error("MISSING THEN STATE");
        }
        goto stmt;
    case TOKEN_UNTIL:
        if (repeat_state != true)
        {
            error("MISSING REPEAT STATE");
        }
        goto until_stmt;
    case TOKEN_END:
        if (then_state == true)
        {
            expect(TOKEN_END);
            then_state = false;
            goto stmt_list;
        }
        if (do_state == true)
        {
            expect(TOKEN_END);
            do_state = false;
            goto stmt_list;
        }
        expect(TOKEN_END);
        expect(TOKEN_EOF);
        (void)fputs("Compilation Succesfull. Exiting\n", stdout);
        exit(0);
    default:
        expect(TOKEN_SEMICOLON);
        goto stmt;
    }

stmt:
    switch (type)
    {
    case TOKEN_IDENTIFIER:
        goto assign_stmt;
    case TOKEN_ASSIGN:
        goto assign_stmt;
    case TOKEN_READ:
        goto read_stmt;
    case TOKEN_WRITE:
        goto write_stmt;
    case TOKEN_IF:
        goto if_stmt;
    case TOKEN_WHILE:
        goto while_stmt;
    case TOKEN_REPEAT:
        goto repeat_stmt;
    default:
        goto stmt_list;
    }

repeat_stmt:
    expect(TOKEN_REPEAT);
    repeat_state = true;
    goto stmt_list;

until_stmt:
    expect(TOKEN_UNTIL);
    repeat_state = false;
    goto simple_expr;

else_stmt:
    expect(TOKEN_ELSE);
    goto stmt_list;

do_stmt:
    expect(TOKEN_DO);
    do_state = true;
    goto simple_expr;

while_stmt:
    expect(TOKEN_WHILE);
    while_state = true;
    goto simple_expr;

if_stmt:
    expect(TOKEN_IF);
    if_state = true;
    goto simple_expr;

then_stmt:
    then_state = true;
    expect(TOKEN_THEN);
    goto stmt_list;

condition:
    goto simple_expr;

read_stmt:
    expect(TOKEN_READ);
    expect(TOKEN_LPAREN);
    expect(TOKEN_IDENTIFIER);
    expect(TOKEN_RPAREN);
    expect(TOKEN_SEMICOLON);
    goto stmt_list;

assign_stmt:
    goto identifier;

write_stmt:
    switch (type)
    {
    case TOKEN_WRITE:
        expect(TOKEN_WRITE);
        expect(TOKEN_LPAREN);
        goto writable;
    default:
        goto stmt_list;
    }

writable:
    switch (type)
    {
    case TOKEN_LITERAL:
        expect(TOKEN_LITERAL);
        expect(TOKEN_RPAREN);
        goto stmt;
    default:
        write_state = true;
        goto simple_expr;
    }

simple_expr:
    switch (type)
    {
    case TOKEN_LESSER_THAN_EQUALS:
    case TOKEN_LESSER:
    case TOKEN_EQUALS:
    case TOKEN_GREATER:
    case TOKEN_GREATER_THAN_EQUALS:
    case TOKEN_DIFFERENT:
        goto relop;
    case TOKEN_SUM:
    case TOKEN_MINUS:
    case TOKEN_OR:
        goto addop;
    case TOKEN_MULTIPLY:
    case TOKEN_QUOTIENT:
    case TOKEN_AND:
        goto mulop;
    case TOKEN_INTEGER_CONST:
    case TOKEN_FLOAT_CONST:
    case TOKEN_CHAR_CONST:
        goto constants;
    case TOKEN_IDENTIFIER:
        expect(TOKEN_IDENTIFIER);
        goto simple_expr;
    case TOKEN_LPAREN:
        expect(TOKEN_LPAREN);
        goto simple_expr;
    case TOKEN_RPAREN:
        if (write_state == true)
        {   
            write_state = false;
        }
        expect(TOKEN_RPAREN);
        goto simple_expr;
    default:
        if (write_state == true)
        {
            expect(TOKEN_RPAREN);
            write_state = false;
        }
        goto stmt_list;
    }

relop:
    switch (type)
    {
    case TOKEN_LESSER_THAN_EQUALS:
        expect(TOKEN_LESSER_THAN_EQUALS);
        goto simple_expr;
    case TOKEN_LESSER:
        expect(TOKEN_LESSER);
        goto simple_expr;
    case TOKEN_EQUALS:
        expect(TOKEN_EQUALS);
        goto simple_expr;
    case TOKEN_GREATER:
        expect(TOKEN_GREATER);
        goto simple_expr;
    case TOKEN_GREATER_THAN_EQUALS:
        expect(TOKEN_GREATER_THAN_EQUALS);
        goto simple_expr;
    case TOKEN_DIFFERENT:
        expect(TOKEN_DIFFERENT);
        goto simple_expr;
    }

mulop:
    switch (type)
    {
    case TOKEN_MULTIPLY:
        expect(TOKEN_MULTIPLY);
        goto simple_expr;
    case TOKEN_QUOTIENT:
        expect(TOKEN_MINUS);
        goto simple_expr;
    case TOKEN_AND:
        expect(TOKEN_AND);
        goto simple_expr;
    }

addop:
    switch (type)
    {
    case TOKEN_SUM:
        expect(TOKEN_SUM);
        goto simple_expr;
    case TOKEN_MINUS:
        expect(TOKEN_MINUS);
        goto simple_expr;
    case TOKEN_OR:
        expect(TOKEN_OR);
        goto simple_expr;
    }

constants:
    switch (type)
    {
    case TOKEN_INTEGER_CONST:
        expect(TOKEN_INTEGER_CONST);
        goto operator;
    case TOKEN_FLOAT_CONST:
        expect(TOKEN_FLOAT_CONST);
        goto operator;
    case TOKEN_CHAR_CONST:
        expect(TOKEN_CHAR_CONST);
        goto operator;
    }

    operator: switch(type)
    {
    case TOKEN_SUM:
    case TOKEN_MINUS:
    case TOKEN_OR:
        goto addop;
    case TOKEN_MULTIPLY:
    case TOKEN_QUOTIENT:
    case TOKEN_AND:
        goto mulop;
    case TOKEN_LESSER_THAN_EQUALS:
    case TOKEN_LESSER:
    case TOKEN_EQUALS:
    case TOKEN_GREATER:
    case TOKEN_GREATER_THAN_EQUALS:
    case TOKEN_DIFFERENT:
        goto relop;
    case TOKEN_LPAREN:
    case TOKEN_RPAREN:
        goto simple_expr;
    default:
        expect(TOKEN_SEMICOLON);
        goto stmt;
    }

program:
    expect(TOKEN_PROGRAM);
    expect(TOKEN_IDENTIFIER);
    ht_insert(ht, token, TOKEN_IDENTIFIER);
    expect(TOKEN_BEGIN);
    goto decl_list;
}

static void parser(HashTable *ht)
{
    next();
    block(ht);
}

static words(HashTable
                 *ht)
{
    ht_insert(ht,
              "program", TOKEN_PROGRAM);
    ht_insert(ht,
              "begin", TOKEN_BEGIN);
    ht_insert(ht,
              "end", TOKEN_END);
    ht_insert(ht,
              "is", TOKEN_IS);
    ht_insert(ht,
              "int", TOKEN_INTEGER);
    ht_insert(ht,
              "float", TOKEN_FLOAT);
    ht_insert(ht,
              "char", TOKEN_CHARACTER);
    ht_insert(ht,
              "if", TOKEN_IF);
    ht_insert(ht,
              "then", TOKEN_THEN);
    ht_insert(ht,
              "else", TOKEN_ELSE);
    ht_insert(ht,
              "repeat", TOKEN_REPEAT);
    ht_insert(ht,
              "until", TOKEN_UNTIL);
    ht_insert(ht,
              "while", TOKEN_WHILE);
    ht_insert(ht,
              "do", TOKEN_DO);
    ht_insert(ht,
              "read", TOKEN_READ);
    ht_insert(ht,
              "write", TOKEN_WRITE);
}

int main(int argc, char *argv[])
{
    char *startp;
    if (argc != 2)
    {
        fputs("execution: main *.lamp file\n", stderr);
        exit(1);
    }
    HashTable *ht = ht_create(3);
    words(ht);
    readin(argv[1]);

    startp = raw;
    parser(ht);
    free(startp);
    ht_print(ht);
    fprintf(stdout, "\n");
    ht_free(ht);
    return 0;
}
