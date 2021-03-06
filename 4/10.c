#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP 100
#define MAXLINE 1000
#define HEIGHT 1000

#define NUM '0'
#define FUN '1'
#define VAR '2'

#define INDEX(x) (x <= 'Z' ? x - 'A' : x - 71) /* index of the variable */

int var[52];
double lout;

void push(double);
double peek();
double pop();
void clear();
int mgetline();
int getop(char[]);

/* Reverse Polish notation calculator
      - single uppercase and lowercase characters are variables (syntax 2 A =);
      - special variable LOUT for the last printed value.
      - mathematical functions: sin, cos, tan, exp, pow.
      - custom functions: swp (swap top 2 values), dup (duplicate last value),
        top (peek at the stack top), cls (clear stack).
      - operators: +, -, *, /, % and =.
      - press enter to print the value at the top of the stack.
 */
int main(int argc, char *argv[]) {
  int type;
  double op1, op2;
  char s[MAXOP];

  while (mgetline()) {
    while ((type = getop(s)) != 0) {
      switch (type) {
      case NUM:
        push(atof(s));
        break;
      case '+':
        op1 = pop();
        op2 = pop();
        push(op1 + op2);
        break;
      case '-':
        op2 = pop();
        op1 = pop();
        push(op1 - op2);
        break;
      case '*':
        op2 = pop();
        op1 = pop();
        push(op1 * op2);
        break;
      case '/':
        op2 = pop();
        op1 = pop();
        if (op1 == 0.0)
          printf("error: division by zero\n");
        else
          push((double)op1 / op2);
        break;
      case '%':
        op2 = pop();
        op1 = pop();
        if (op2 == 0.0)
          printf("error: division by zero\n");
        else
          push((int)op1 % (int)op2);
        break;
      case '=':
        var[INDEX(s[0])] = pop();
        break;
      case VAR:
        if (strcmp("LOUT", s) == 0)
          push(lout);
        else
          push(var[INDEX(s[0])]);
        break;
      case FUN:
        if (strcmp("sin", s) == 0)
          push(sin(pop()));
        else if (strcmp("cos", s) == 0)
          push(cos(pop()));
        else if (strcmp("tan", s) == 0)
          push(tan(pop()));
        else if (strcmp("exp", s) == 0)
          push(exp(pop()));
        else if (strcmp("pow", s) == 0)
          push(pow(pop(), pop()));
        else if (strcmp("cls", s) == 0)
          clear();
        else if (strcmp("top", s) == 0) {
          lout = peek();
          printf("\t%.8g\n", lout);
        } else if (strcmp("swp", s) == 0) {
          op2 = pop();
          op1 = pop();
          push(op2);
          push(op1);
        } else if (strcmp("dup", s) == 0)
          push(peek());
        else
          printf("error: unknown function %s\n", s);
        break;
      case '\n':
        lout = pop();
        printf("\t%.8g\n", lout);
        break;
      default:
        printf("error: unknown command %s\n", s);
      }
    }
  }

  return 0;
}

int idx = 0;
int line[MAXLINE];

int mgetline() {
  int i, c;

  for (i = 0; i < MAXLINE - 1 && (c = getchar()) != '\n' && c != EOF; i++)
    line[i] = c;

  line[i++] = '\n';
  line[i] = 0;
  idx = 0; 
  return c == EOF ? 0 : 1;
}

int getop(char s[]) {
  int i, c, rc;

  while ((s[0] = c = line[idx++]) == ' ' || c == '\t')
    ;
  s[1] = 0;

  // special characters and operators
  if (c == 0 || c == '+' || c == '*' || c == '/' || c == '%' || c == '\n' ||
      (c == '-' && !isdigit(line[idx])) || c == EOF)
    return c;

  i = 0;

  if (isdigit(c) || c == '-' || c == '.') {
    while (isdigit((s[++i] = c = line[idx++])) || c == '.')
      ;
    idx--;
    rc = NUM;
  } else if (isalpha(c) && line[idx + 1] == '=') {
    s[++i] = c;
    idx += 2;
    rc = '=';
  } else {
    while (isalnum(s[++i] = c = line[idx++]))
      ;
    s[i] = 0;
    idx--;
    rc = strcmp("LOUT", s) == 0 || strlen(s) == 1 ? VAR : FUN;
  }

  s[i] = 0;
  return rc;
}

int sp = 0;
double stack[HEIGHT];

void push(double f) {
  if (sp < HEIGHT)
    stack[sp++] = f;
  else
    printf("error: stack empty\n");
}

double pop() {
  if (sp > 0)
    return stack[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

double peek() {
  if (sp > 0)
    return stack[sp - 1];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

void clear() {
  for (; sp > 0; sp--)
    ;
}