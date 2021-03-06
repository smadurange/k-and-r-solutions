#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000
#define MAXLINES 5000

/* Read lines into array without allocating storage */
int readlines(char **lines, int maxlines);

int main() {
  int i, count;
  char **lines;

  lines = malloc(sizeof(char *) * MAXLINES);
  for (i = 0; i < MAXLINES - 1; i++)
    lines[i] = malloc(sizeof(char) * MAXLEN);

  count = readlines(lines, MAXLINES);

  for (i = 0; i < count; i++) {
    printf("L%d: %s\n", i + 1, lines[i]);
    free(lines[i]);
  }
  
  free(lines);
  return 0;
}

int readlines(char **lines, int maxlines) {
  int i, len;
  char s[MAXLEN];

  for (i = 0; i < maxlines; i++) {
    if (!fgets(s, MAXLEN, stdin))
      return i;
    len = strlen(s);
    strncpy(*lines, s, len - 1);
    *lines[len] = 0;
    lines++;
  }

  return i;
}