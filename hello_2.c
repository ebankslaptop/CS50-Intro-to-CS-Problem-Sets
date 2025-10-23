#include <stdio.h>
int main() {
  char name[50];
  printf("what is your name? ");
  scanf("%49s", name);
  printf("Hello, %s!\n", name);
  return 0;
}
