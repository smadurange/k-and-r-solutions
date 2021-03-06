#include <stdio.h>

int ftoc(int fahr);

/* converts fahrenhite to celsius using a helper function */
int main(int argc, char *argv[]) {
  int fahr, celsius;
  int lower, upper, step;

  lower = 0;
  upper = 300;
  step = 20;
  fahr = lower;

  while (fahr <= upper) {
    printf("%d\t%d\n", fahr, ftoc(fahr));
    fahr += step;
  }

  return 0;
}

int ftoc(int fahr) { return 5 * (fahr - 32) / 9; }