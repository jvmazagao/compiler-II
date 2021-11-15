#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

#define TOKEN_PROGRAM 1
#define TOKEN_NUMBER 3
#define TOKEN_IF 4
#define TOKEN_THEN 5
#define TOKEN_END 6
#define TOKEN_ELSE 7
#define TOKEN_REPEAT 8
#define TOKEN_UNTIL 9
#define TOKEN_WHILE 10
#define TOKEN_DO 11
#define TOKEN_READ 12
#define TOKEN_WRITE 13
#define TOKEN_AND 14
#define TOKEN_OR 15
#define TOKEN_EQUALS 'e'
#define TOKEN_GREATER '>'
#define TOKEN_GREATER_THAN_EQUALS 'g'
#define TOKEN_LESSER '<'
#define TOKEN_LESSER_THAN_EQUALS 'l'
#define TOKEN_DIFFERENT '!'
#define TOKEN_FLOAT 26
#define TOKEN_INTEGER 27
#define TOKEN_CHARACTER 28
#define TOKEN_LITERAL 29
#define TOKEN_IDENTIFIER 30
#define TOKEN_IS 31
#define TOKEN_BEGIN 32
#define TOKEN_COMMENT 33
#define TOKEN_SUM '+'
#define TOKEN_MINUS '-'
#define TOKEN_MULTIPLY '*'
#define TOKEN_QUOTIENT '/'
#define TOKEN_ASSIGN '='
#define TOKEN_SEMICOLON ';'
#define TOKEN_COMMA ','
#define TOKEN_LPAREN '('
#define TOKEN_RPAREN ')'

static char *raw, *token;
static int type;
static size_t line = 1;

/** Functions **/
static void error(const char *fmt, ...)
{
    va_list ap;
    (void)fprintf(stderr, "lamp: error: %lu", line);
    va_start(ap, fmt);
    (void)fputc('\n', stderr);
    va_end(ap);
    (void)fputc('\n', stderr);
    exit(1);
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

static int
ident(void)
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

    return TOKEN_IDENTIFIER;
}

//todo: Change to static typing
static int number(void)
{
    const char *errstr;
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
        if (isdigit(*p))
            token[j++] = *p;
        ++p;
    }
    token[j] = '\0';

    (void)strtonum(token, 0, LONG_MAX, &errstr);
    if (errstr != NULL)
        error("invalid number: %s", token);

    return TOKEN_NUMBER;
}

/**
 * Lexer
 */
static int comment(void)
{
    int ch;
    if ((ch = *raw++) == '/')
    {
        if ((ch = *raw++) == '*')
        {
            while ((ch = *raw++))
            {
                if (ch == '\0')
                {
                    error("unterminated comment");
                }

                if (ch == '\n')
                {
                    ++line;
                }
                if (ch == '*')
                {
                    ch = *raw++;
                    if (ch == '/')
                    {
                        return TOKEN_COMMENT;
                    }
                }
            }
        }
        return TOKEN_QUOTIENT;
    }
}

static int literal(void)
{
   char *p;
    size_t i, len;

    p = ++raw;
    while (isalnum(*raw) || *raw == '}')
        ++raw;

    len = raw - p;

    --raw;

    free(token);

    if ((token = malloc(len + 1)) == NULL)
        error("malloc failed");

    for (i = 0; i < len; i++)
        token[i] = *p++;
    token[i] = '\0';

    while(*raw != '}')
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
        if(comment() == TOKEN_COMMENT)
            goto again;
        return TOKEN_QUOTIENT;
    case '=':
        return TOKEN_ASSIGN;
    case ',':
        return TOKEN_COMMA;
    case ';':
        return TOKEN_SEMICOLON;
    case '>':
        return TOKEN_GREATER;
    case '<':
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
    }
    return 0;
}

static void parser(void)
{
    while ((type = lex()) != 0)
    {
        ++raw;
        (void)fprintf(stdout, "Table %lu|%d\t", line, type);
        switch (type)
        {
        case TOKEN_LITERAL:
        case TOKEN_INTEGER:
        case TOKEN_PROGRAM:
        case TOKEN_FLOAT:
        case TOKEN_READ:
        case TOKEN_WRITE:
        case TOKEN_END:
        case TOKEN_IF:
        case TOKEN_THEN:
        case TOKEN_ELSE:
        case TOKEN_IDENTIFIER:
        case TOKEN_BEGIN:
        case TOKEN_NUMBER:
            (void)fprintf(stdout, "%s\n", token);
            break;
        case TOKEN_QUOTIENT:
        case TOKEN_ASSIGN:
        case TOKEN_SEMICOLON:
        case TOKEN_COMMA:
        case TOKEN_MULTIPLY:
        case TOKEN_SUM:
        case TOKEN_MINUS:
        case TOKEN_LESSER:
        case TOKEN_LESSER_THAN_EQUALS:
        case TOKEN_GREATER:
        case TOKEN_GREATER_THAN_EQUALS:
        case TOKEN_DIFFERENT:
        case TOKEN_LPAREN:
        case TOKEN_RPAREN:
            (void)fprintf(stdout, "%c\n", type);
            break;
        }
    }
    (void)fputc('\n', stdout);
}

int main(int argc, char *argv[])
{
    char *startp;
    if (argc != 2)
    {
        fputs("execution: main *.lamp file\n", stderr);
        exit(1);
    }
    readin(argv[1]);

    startp = raw;
    parser();
    free(startp);
    return 0;
}
