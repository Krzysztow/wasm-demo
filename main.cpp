extern "C"
int add (int first, int second) {
  return first + second;
}

#include <stdio.h>
#include <string.h>

extern "C"
void unsafe_memcpy(const char* in, long long size, char* out) {
  memcpy(out, in, size);
}

int main() {
  printf("Hello world!");
}
