#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP 100      /* max size of operand or operator */
#define NUMBER '0'     /* signal that a number was found */
#define FUNCTION '1'   /* signal that a custom op was found */
#define ASSIGNMENT '2' /* signal that a variable was found */
#define VARIABLE '3'   /* signal that a variable was found */
#define MAXVAL 100     /* max depth of val and var stack */
#define BUFSIZE 100
#define VARCOUNT 52                            /* supported variable count */
#define INDEX(x) (x <= 'Z' ? x - 'A' : x - 71) /* index of the variable */

int sp = 0;         /* next free stack position */
double val[MAXVAL]; /* value stack */

double lout = 0;
double varv[VARCOUNT]; /* variable value array */

int bufp = 0;      /* next free position in buf */
char buf[BUFSIZE]; /* buffer for ungetch */

/* get next operator or numeric operand */
int getop(char[]);

/* push f onto value stack */
void push(double);

/* pop and return top value from stack */
double pop();

/* returns a value from stack without removing it */
double peek();

/* clears the stack */
void clear();

/* add ungets(s) to program */
int main(int argc, char *argv[]) {
  int type, i;
  double op2, op1;
  char s[MAXOP];

  while ((type = getop(s)) != EOF) {
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      push(pop() + pop());
      break;
    case '*':
      push(pop() * pop());
      break;
    case '-':
      op2 = pop();
      push(pop() - op2);
      break;
    case '/':
      op2 = pop();
      if (op2 != 0.0)
        push(pop() / op2);
      else
        printf("error: division by zero\n");
      break;
    case '%':
      op2 = pop();
      if (op2 != 0.0)
        push((int)pop() % (int)op2);
      else
        printf("error: division by zero\n");
      break;
    case FUNCTION:
      if (strcmp("top", s) == 0) {
        lout = peek();
        printf("peek top: %.8g\n", lout);
      } else if (strcmp("dup", s) == 0)
        push(peek());
      else if (strcmp("swp", s) == 0) {
        op2 = pop();
        op1 = pop();
        push(op2);
        push(op1);
      } else if (strcmp("cls", s) == 0)
        clear();
      else if (strcmp("sin", s) == 0)
        push(sin(pop()));
      else if (strcmp("exp", s) == 0)
        push(exp(pop()));
      else if (strcmp("pow", s) == 0)
        push(pow(pop(), pop()));
      else
        printf("error: unknown function %s\n", s);
      break;
    case ASSIGNMENT:
      i = INDEX(s[0]);
      if (i < 0 || i > VARCOUNT - 1)
        printf("error: unknown symbol %s\n", s);
      else
        varv[i] = pop();
      break;
    case VARIABLE:
      if (strcmp("lout", s) == 0)
        push(lout);
      else {
        i = INDEX(s[0]);
        if (i < 0 || i > VARCOUNT - 1)
          printf("error: unknown symbol %s\n", s);
        else
          push(varv[i]);
      }
      break;
    case '\n':
      lout = pop();
      printf("\t%.8g\n", lout);
      break;
    default:
      printf("error: unknown command %s\n", s);
    }
  }

  return 0;
}

void push(double f) {
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("error: stack empty\n");
}

double pop() {
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

double peek() {
  if (sp > 0)
    return val[sp - 1];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

void clear() {
  for (; sp > 0; sp--)
    ;
}

int getch() { return (bufp > 0) ? buf[--bufp] : getchar(); }

void ungetch(int c) {
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}

void ungets(char s[]) {
  int len, i;
  len = strlen(s);
  if (bufp + len - 1 >= BUFSIZE)
    printf("ungets: too many characters\n");
  else {
    for (i = len; i >= 0; i--)
      buf[bufp++] = s[i];
  }
}

int getop(char s[]) {
  int i, c, nc, rv;

  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;
  s[1] = 0;

  // operators or line feed
  if (c == '\n' || c == '+' || c == '*' || c == '/' || c == '%')
    return c;

  i = 0;
  // numbers
  if (isdigit(c) || c == '-') {
    // minus operator
    if (c == '-' && !isdigit(s[++i] = c = getch())) {
      ungetch(c);
      s[i] = 0;
      return '-';
    }
    while (isdigit(s[++i] = c = getch()) || c == '.')
      ;
    rv = NUMBER;
  } else {
    // functions and variables
    int count;
    for (count = 0; isalnum(s[++i] = c = getch()); count++)
      ;
    int islout = strncmp("lout", s, 4 * sizeof(char)) == 0;
    if (count == 0 || islout) {
      if (islout)
        rv = VARIABLE;
      else if (c != '\n' && (nc = getch()) == '=')
        // syntax "5 A =" means "A = 5"
        rv = ASSIGNMENT;
      else {
        if (c != '\n') {
          ungetch(c);
          ungetch(nc);
        }  
        rv = VARIABLE;
      }
    } else {
      rv = FUNCTION;
    }
  }

  s[i] = 0;
  if (c != EOF)
    ungetch(c);

  return rv;
}
