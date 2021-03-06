#include <stdio.h>

/* prints a conversion table from fahrenheit to celsius in reverse order */
int main(int argc, char *argv[]) {
  int fahr;

  for (fahr = 300; fahr >= 0; fahr = fahr - 20)
    printf("%3d %6.1f\n", fahr, (5.0 / 9.0) * (fahr - 32));

  return 0;
}