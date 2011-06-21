#include <dlfcn.h>
#include <iostream>
int error(const char* err) { std::cerr << "Error: " << err << "\n"; return 1; }
typedef int (*RxRendererInfo)(const char* name, void *result,
                              int rl, int *rt, int *rc);
int main (int argc, const char* argv[]) {
  if(argc <= 1) return error("missing input path to rman lib");
  void* dlh = dlopen(argv[1], RTLD_LAZY|RTLD_GLOBAL);
  if (!dlh) return error(dlerror());
  void* symbol = dlsym (dlh, "RxRendererInfo");
  if (!symbol) return error(dlerror());
  int type, count;
  char* ver;
  RxRendererInfo func = (RxRendererInfo)symbol;
  func("versionstring", &ver, sizeof(char*), &type, &count);
  std::cout << ver;
  if(dlclose(dlh)) return error(dlerror());
  return 0;
}
