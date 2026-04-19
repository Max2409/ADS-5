// Copyright 2025 NNTU-CS
#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <cctype>
#include <cmath>

int extractNumber(size_t& pos, const std::string& str) {
    int value = 0;
    while (pos < str.size() && std::isdigit(str[pos])) {
        value = value * 10 + (str[pos] - '0');
        ++pos;
    }
    --pos;
    return value;
}

int getOperatorPriority(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^':           return 3;
        default:            return -1;
    }
}

bool isRightAssociative(char op) {
    return op == '^';
}

std::string infx2pstfx(const std::string& inf) {
    std::ostringstream buffer;
    std::stack<char> st;

    for (size_t idx = 0; idx < inf.size(); ++idx) {
        char symbol = inf[idx];

        if (std::isspace(symbol)) {
            continue;
        }

        if (std::isdigit(symbol)) {
            int number = extractNumber(idx, inf);
            buffer << ' ' << number;
            continue;
        }

        if (symbol == '(') {
            st.push(symbol);
            continue;
        }

        if (symbol == ')') {
            while (!st.empty() && st.top() != '(') {
                buffer << ' ' << st.top();
                st.pop();
            }
            if (!st.empty() && st.top() == '(') {
                st.pop();
            }
            continue;
        }

        int priority = getOperatorPriority(symbol);
        if (priority == -1) {
            continue;
        }

        while (!st.empty() && st.top() != '(') {
            char topOp = st.top();
            int topPriority = getOperatorPriority(topOp);

            if ( (!isRightAssociative(symbol) && topPriority >= priority) ||
                 ( isRightAssociative(symbol) && topPriority > priority) ) {
                buffer << ' ' << topOp;
                st.pop();
            } else {
                break;
            }
        }

        st.push(symbol);
    }

    while (!st.empty()) {
        buffer << ' ' << st.top();
        st.pop();
    }

    std::string result = buffer.str();
    if (!result.empty() && result[0] == ' ') {
        result.erase(0, 1);
    }
    return result;
}

int eval(const std::string& postfix) {
    std::stack<int> st;

    for (size_t pos = 0; pos < postfix.size(); ++pos) {
        char current = postfix[pos];

        if (std::isspace(current)) {
            continue;
        }

        if (std::isdigit(current)) {
            int number = extractNumber(pos, postfix);
            st.push(number);
            continue;
        }

        if (st.size() < 2) {
            throw std::runtime_error("Not enough operands");
        }

        int b = st.top(); st.pop();
        int a = st.top(); st.pop();
        int result = 0;

        switch (current) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': 
                if (b == 0) throw std::runtime_error("Division by zero");
                result = a / b; 
                break;
            case '^': 
                result = static_cast<int>(std::pow(a, b));
                break;
            default:
                throw std::runtime_error(std::string("Unknown operator: ") + current);
        }
        st.push(result);
    }

    if (st.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return st.top();
}
