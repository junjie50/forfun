#include <stdio.h>

int main(void) {
  int ch;
  int charCount = 0;
  while((ch = getchar()) != EOF){
    charCount++;
  }
  printf("%d\n", charCount);
  return 0;
}
