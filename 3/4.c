#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define MAXLEN 50

void itoa(int n, char s[]);
void reverse(char s[]);

int main(int argc, char *argv[]) {
  char s[MAXLEN];

  itoa(INT_MIN, s);
  printf("Largest negative number on machine: %d\n", INT_MIN);
  printf("Largest negative number after itoa: %s\n", s);

  return 0;
}

void itoa(int n, char s[]) {
  int i, sign;

  i = 0;
  
  if ((sign = n) < 0)
    n = -n;

  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0)
    s[i++] = '-';
  s[i] = 0;
  
  reverse(s);
}

void reverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}