#include <iostream>
#include <Python.h>
int main (int argc, const char* argv[]) {
  std::cout << PY_MAJOR_VERSION << "." << PY_MINOR_VERSION;
  return 0;
}
