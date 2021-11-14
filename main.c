#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/** 
 * program ::= program identifier begin [decl-list] stmt-list end "."
 * decl-list ::= decl ";" { decl ";"}
 * decl ::= ident-list is type
 * ident-list ::= identifier {"," identifier}
 * type ::= int | float | char
 * stmt-list ::= stmt {";" stmt}
 * stmt ::= assign-stmt | if-stmt | while-stmt | repeat-stmt
 *  | read-stmt | write-stmt
 * assign-stmt ::= identifier "=" simple_expr
 * if-stmt ::= if condition then stmt-list end
 *  | if condition then stmt-list else stmt-list end
 * condition ::= expression
 * repeat-stmt ::= repeat stmt-list stmt-suffix
 * stmt-suffix ::= until condition
 * while-stmt ::= stmt-prefix stmt-list end
 * stmt-prefix ::= while condition do
 * read-stmt ::= read "(" identifier ")"
 * write-stmt ::= write "(" writable ")"
 * writable ::= simple-expr | literal
 * expression ::= simple-expr | simple-expr relop simple-expr
 * simple-expr ::= term | simple-expr addop term
 * term ::= factor-a | term mulop factor-a
 * fator-a ::= factor | ! factor | "-" factor
 * factor ::= identifier | constant | "(" expression ")"
 * relop ::= "==" | ">" | ">=" | "<" | "<=" | "!="
 * addop ::= "+" | "-" | ||
 * mulop ::= "*" | "/" | &&
 * constant ::= integer_const | float_const | char_const
 * **/

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

/**
 * Lexer
 */
// TODO: Remover comentarios
static void comment(void)
{
    int ch;
    if ((ch = *raw++) == '/')
    {
        fputs("Comment /\n", stdout);
        if ((ch = *raw++) == '*')
        {
            fputs("Comment *\n", stdout);
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
                    fputs("Comment *\n", stdout);
                    if (ch == '/')
                    {
                        fputs("Comment /\n", stdout);
                        break;
                    }
                }
            }
        }
    }
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
    switch (*raw)
    {
    case '/':
        comment();
        goto again;
    }
}

static void parser(void)
{
    while ((type = lex()) != 0)
    {
        ++raw;
        (void)fprintf(stdout, "%lu|%d\t", line, type);
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
