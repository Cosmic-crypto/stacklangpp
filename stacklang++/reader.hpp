#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <stdexcept>

struct Ret {
  std::vector<std::string> lines;
  bool precomped;
};

std::vector<std::string> readlines(std::string filename) {
  std::fstream file(filename);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(file, line)) {
    if (!line.empty()) lines.push_back(line);
  }

  return lines;
}

Ret read(std::string filename) {
  if (std::filesystem::path(filename).extension() != ".stl") throw std::runtime_error("Unknown file format");

  if (std::filesystem::exists(filename + "c")) {
    std::vector<std::string> lines = readlines(filename + "c");
    return Ret{
      lines,
      true,
    };
  }
  std::vector<std::string> lines = readlines(filename);
  return Ret{
    lines,
    false,
  };
}
