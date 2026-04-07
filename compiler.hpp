#pragma once
#include "reader.hpp"
#include "writer.hpp"
#include "vm.hpp"
#include "parser.hpp"
#include <cstddef>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

std::optional<Instr> getInstr(const std::string& cmd) {
  static const std::unordered_map<std::string, Instr> map = {
    {"EnabDefPop", EnabDefPop},
    {"Push", Push},
    {"Pop", Pop},
    {"PopIdx", PopIdx},
    {"Add", Add},
    {"Sub", Sub},
    {"Mul", Mul},
    {"Div", Div},
    {"Pow", Pow},
    {"Print", Print},
    {"PrintIdx", PrintIdx},
    {"PrintAll", PrintAll},
    {"Dup", Dup},
    {"Jmp", Jmp},
    {"JmpIfZ", JmpIfZ},
  };

  auto it = map.find(cmd);
  if (it == map.end()) return std::nullopt;
  return it->second;
}

Cmd parse(const std::string& line) {
  std::optional<ParsedLine> parsed = parseLine(line);
  if (!parsed) return Cmd{Push, 0};

  std::optional<Instr> instr = getInstr(std::string(parsed->cmd));
  if (!instr) return Cmd{Push, 0};

  int val = 0;
  if (parsed->val.has_value() && !parsed->val->empty()) {
    val = std::stoi(std::string(*parsed->val));
  }

  return Cmd{*instr, val};
}

void compile(std::string filename) {
  Ret reader_ret = read(filename);
  VM vm_instance = VM{};

  if (reader_ret.precomped) {
    std::vector<Cmd> translated;
    translated.reserve(reader_ret.lines.size());

    for (const std::string& s : reader_ret.lines) {
      translated.emplace_back(parse(s));
    }

    vm_instance.run(translated);
    return;
  }

  std::vector<std::string> compiled_program;
  std::vector<std::string> error_lines;

  std::size_t line_idx = 0;
  for (const std::string& line : reader_ret.lines) {
    std::optional<ParsedLine> parsed = parseLine(line);
    
    if (!parsed) {
      ++line_idx;
      continue;
    }

    std::optional<Instr> instr = getInstr(std::string(parsed->cmd));
    if (!instr) {
      error_lines.push_back("Unknown command: " + std::string(parsed->cmd));
      ++line_idx;
      continue;
    }

    int val = 0;
    if (parsed->val.has_value() && !parsed->val->empty()) {
      try {
        val = std::stoi(std::string(*parsed->val));
      } catch (...) {
        error_lines.push_back("Invalid value at line " + std::to_string(line_idx));
        ++line_idx;
        continue;
      }
    }

    std::string out = std::string(parsed->cmd);
    compiled_program.push_back(out + "," + std::to_string(val));

    ++line_idx;
  }

  if (!error_lines.empty()) {
    for (const auto& err : error_lines) {
      std::cerr << err << "\n";
    }
  }

  std::string compiled_filename = filename + "c";
  write(compiled_filename, compiled_program);

  std::vector<Cmd> translated;
  translated.reserve(compiled_program.size());
  for (const std::string& s : compiled_program) {
    translated.emplace_back(parse(s));
  }

  vm_instance.run(translated);
}
