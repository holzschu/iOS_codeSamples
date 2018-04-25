// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int num;
  printf("Hello World\n");
  fflush(stdout);
  write(STDOUT_FILENO, "write\n", 6);
  printf("Enter a number:");
  scanf("%d", &num);
  printf("The number entered was: %d\n", num);
  return 0;
}
