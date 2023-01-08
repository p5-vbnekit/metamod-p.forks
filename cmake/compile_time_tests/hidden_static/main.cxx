inline __attribute__((visibility("hidden"))) static int test_() { return 0; }

int main(int, char **) { return test_(); }
