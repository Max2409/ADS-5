// Copyright 2025 NNTU-CS
#include <string>
#include <map>
#include "tstack.h"
#include <cctype>
#include <sstream>

int extractNumber(int* position, const std::string& input) {
    int value = 0;
    while (*position < static_cast<int>(input.size()) && std::isdigit(input[*position])) {
        value = value * 10 + (input[*position] - '0');
        ++(*position);
    }
    return value;
}

int getOperatorPriority(char op) {
    static std::string operators("-+/*^");
    return operators.find(op);
}

std::string infx2pstfx(const std::string& inf) {
  std::ostringstream buffer;          
  TStack<char, 100> st;              

  for (int idx = 0; idx < (int)inf.size(); ++idx) {
      char symbol = inf[idx];

      if (std::isdigit(symbol)) {
          buffer << ' ' << extractNumber(&idx, inf);
          continue;
      }

      if (symbol == '(') {
          st.push(symbol);
          continue;
      }

      if (symbol == ')') {
          while (st.get() != '(') {
              buffer << ' ' << st.pop();
          }
          st.pop();
          continue;
      }

      if (getOperatorPriority(symbol) == std::string::npos) {
          continue;
      }

      while (!st.isEmpty() && st.get() != '(' &&
               getOperatorPriority(st.get()) >= getOperatorPriority(symbol)) {
          buffer << ' ' << st.pop();
      }

      st.push(symbol);
    }

    while (!st.isEmpty()) {
        buffer << ' ' << st.pop();
    }

    std::string result = buffer.str();
    if (!result.empty() && result[0] == ' ') {
        result.erase(0, 1);
    }
    return result;
}

int eval(const std::string& pref) {
  TStack<int, 100> st;                 
  int length = static_cast<int>(pref.size());

  for (int pos = 0; pos < length; ++pos) {
      char current = pref[pos];

      if (std::isdigit(current)) {
          st.push(extractNumber(&pos, pref));
          continue;
      }

      int a = 0, b = 0;                 

      switch (current) {
          case '+':
              b = st.pop();
              a = st.pop();
              st.push(a + b);
              break;
          case '-':
              b = st.pop();
              a = st.pop();
              st.push(a - b);
              break;
          case '*':
              b = st.pop();
              a = st.pop();
              st.push(a * b);
              break;
          case '/':
              b = st.pop();
              a = st.pop();
              st.push(a / b);
              break;
          default:
              break;
        }
    }

    return st.pop();
}
