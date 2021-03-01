extern "C"
int add (int first, int second) {
  return first + second;
}

extern "C"
void memcpy(const char* in, long long size, char* out);

extern "C"
void unsafe_memcpy(const char* in, long long size, char* out) {
  memcpy(in, size, out);
}

#include <stdio.h>

int main() {
  printf("Hello world!");
}
