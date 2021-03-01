extern "C"
int add (int first, int second) {
  return first + second;
}

extern "C"
void unsafe_memcpy(const char* in, long long size, char* out) {
  for (int i = 0; i < size; ++i) {
    out[i] = in[i];
  }
}
