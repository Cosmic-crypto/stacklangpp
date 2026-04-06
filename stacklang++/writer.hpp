#include <fstream>
#include <string>
#include <vector>

void write(std::string filename, std::vector<std::string> program) {
  std::ofstream file(filename);

  for (std::string line : program) {
    file << line << "\n";
  }
}
