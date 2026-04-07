#include "compiler.hpp"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file.stl>\n";
    return 1;
  }

  compile(argv[1]);
  return 0;
}
