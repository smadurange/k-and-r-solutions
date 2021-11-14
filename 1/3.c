#include <stdio.h>

/* Prints a conversion table from fahrenheit to celsius */

int main(int argc, char *argv[]) {
  float fahr, celsius;
  int lower, upper, step;

  lower = 0;
  upper = 300;
  step = 20;
  fahr = lower;

  printf("fahrenheit\tcelsius\n");

  while (fahr <= upper) {
    celsius = (5.0 / 9.0) * (fahr - 32.0);
    printf("%10.0f\t%7.1f\n", fahr, celsius);
    fahr = fahr + step;
  }

  return 0;
}