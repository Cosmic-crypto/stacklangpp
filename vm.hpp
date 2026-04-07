#pragma once
#include <cstddef>
#include <vector>
#include <iostream>
#include <utility>
#include <cmath>

enum Instr {
  EnabDefPop,
  Push,
  Pop,
  PopIdx,
  Add,
  Sub,
  Mul,
  Div,
  Pow,
  Print,
  PrintIdx,
  PrintAll,
  Dup,
  Jmp,
  JmpIfZ,
};

struct Cmd {
  Instr cmd;
  int val;
};

struct VM {
  bool enabdefpop = true;
  std::vector<int> stack;
  size_t sp;

  int ppopcheck() {
    int val;
    if (enabdefpop) {
      val = stack.back();
      stack.pop_back();
    } else {
      val = stack[sp];
    }

    return val;
  }

  std::pair<int, int> popcheck() {
    int b;
    int a;

    if (enabdefpop) {
      b = stack.back();
      stack.pop_back();

      a = stack.back();
      stack.pop_back();
    } else {
      b = stack[sp];
      a = stack[sp - 1];
    }

    return {a, b};
  }

  void run(const std::vector<Cmd>& program) {
    size_t ip = 0;
    while (ip < program.size()) {
      const Cmd cmd = program[ip];
      
      switch (cmd.cmd) {
        case EnabDefPop: {
          enabdefpop = !enabdefpop;
          break;
        }

        case Push: {
          stack.push_back(cmd.val);
          break;
        }
        
        case Pop: {
          stack.pop_back();
          break;
        }

        case PopIdx: {
          stack.erase(stack.begin() + cmd.val);
          break;
        }

        case Add: {
         auto [a, b] = popcheck();

          stack.push_back(a + b);
          break;
        }

        case Sub: {
          auto [a, b] = popcheck();

          stack.push_back(a - b);
          break;
        }

        case Mul: {
          auto [a, b] = popcheck();

          stack.push_back(a * b);
          break;
        }

        case Div: {
          auto [a, b] = popcheck();

          stack.push_back(a / b);
          break;
        }

        case Pow: {
          auto [a, b] = popcheck();

          stack.push_back((int)::std::pow(a, b));
          break;
        }

        case Print: {
          int val = ppopcheck();
          std::cout << val << "\n";
          
          break;
        }

        case PrintIdx: {
          int idx = stack[cmd.val];

          if (enabdefpop) {
            stack.erase(stack.begin() + cmd.val);
          }

          std::cout << idx << "\n";
          break;
        }

        case PrintAll: {
          for (int val : stack) {
            std::cout << val << "\n";
          }

          if (enabdefpop) stack.clear();
          break;
        }

        case Dup: {
          stack.push_back(stack.back());
        }

        case Jmp: {
          ip = cmd.val - 1;
        }

        case JmpIfZ: {
          if (stack.back() == 0) ip = cmd.val - 1;
        }
      }
      ip++;
    }
  }
};
