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

int var[52];

int getop(char[]);
void push(double);
double peek();
double pop();
void clear();
double vartoval(char);

int main(int argc, char *argv[]) {
  int type, op1, op2;
  char s[MAXOP];

  printf("Press CTRL+C to exit\n");

  while (type == getop(s)) {
    switch (type) {
    case NUM:
      push(atof(s));
      break;
    case '+':
      op1 = pop();
      op2 = pop();
      if (isalpha(op1))
        op1 = vartoval(op1);
      if (isalpha(op2))
        op2 = vartoval(op2);
      push(op1 + op2);
      break;
    case '-':
      op1 = pop();
      op2 = pop();
      if (isalpha(op1))
        op1 = vartoval(op1);
      if (isalpha(op2))
        op2 = vartoval(op2);
      push(op1 - op2);
      break;
    case '*':
      op1 = pop();
      op2 = pop();
      if (isalpha(op1))
        op1 = vartoval(op1);
      if (isalpha(op2))
        op2 = vartoval(op2);
      push(op1 * op2);
      break;
    case '/':
      op1 = pop();
      op2 = pop();
      if (isalpha(op1))
        op1 = vartoval(op1);
      if (isalpha(op2))
        op2 = vartoval(op2);
      if (op1 == 0.0)
        printf("error: division by zero\n");
      else
        push((double)op2 / op1);
      break;
    case '%':
      op1 = pop();
      op2 = pop();
      if (isalpha(op1))
        op1 = vartoval(op1);
      if (isalpha(op2))
        op2 = vartoval(op2);
      if (op1 == 0.0)
        printf("error: division by zero\n");
      else
        push((int)op2 % (int)op1);
      break;
    case '=':
      break;
    case FUN:
      break;
    default:
      printf("error: unknown command %s\n", s);
    }
  }

  return 0;
}

int idx = 0;
int line[MAXLINE];

void mgetline() {
  int i, c;

  for (i = 0; i < MAXLINE - 1 && (c = getchar()) != '\n' && c != EOF; i++) {
    line[i] = c;
  }
  line[i] = 0;
  idx = 0;
}

int getop(char s[]) {
  int i, c;

  while ((s[0] = c = line[idx++]) == ' ' || c == '\t')
    ;
  s[1] = 0;

  // special characters and operators
  if (c == 0 || c == '+' || c == '*' || c == '/' || c == '%' || c == '=' ||
      (c == '-' && isdigit(line[idx + 1])) || c == EOF)
    return c;

  i = 0;
  
  if (isdigit(c) || c == '-' || (c == '.' && isdigit(line[idx + 1]))) {
    while (isdigit((s[++i] = c = line[idx++])) || (c == '.' && isdigit(line[idx + 1])))
      ;
    return NUM;
  }

  
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