#include "./postfix.hpp"

#include <map>
#include <stack>
namespace algorithms {

    const std::map<char,int> precedence_map {
        {'|', 1},
        {'.', 2},
        {'*', 3}
    };

    
    namespace {
        using std::string, std::map;
        string explicit_concat(const string &regex)
        {
            string result;
            std::stack<char> symbols;

            for (size_t i = 0; i < regex.length(); ++i) {
                char currentSymbol = regex[i];
                result += currentSymbol;

                if (currentSymbol == '(' || currentSymbol == '|')
                    continue;

                if (i < regex.length() - 1) {
                    char nextSymbol = regex[i + 1];
                    if (nextSymbol == '*' || nextSymbol == '|' || 
                        nextSymbol == ')' || nextSymbol == '$'
                    ) continue;
                    

                    result += '.';
                }
            }

            return result;
        }

        void handle_parentesis(char ch, std::stack<char> &stack, string &output)
        {
            if (ch == '(')
            {
                stack.push(ch);
                return;
            }

            while (!stack.empty())
            {
                char last = stack.top();
                if (last != '(')
                {
                    output.push_back(last);
                    stack.pop();
                } else
                {
                    stack.pop();
                    break;
                }
            }
            
        }

        void handle_operator(char ch, std::stack<char> &stack, string &output)
        {
            while (
                !stack.empty() &&
                stack.top() != '(' &&
                precedence_map.at(stack.top()) >= precedence_map.at(ch)) 
            {
                output.push_back(stack.top());
                stack.pop();
            }
            stack.push(ch);
            
        }
    }

    using std::string, std::map;
    string regex_to_postfix(string &infix) {
        string output {};
        std::stack<char> stack {};

        string explicit_regex = explicit_concat(infix);

        for (char ch : explicit_regex)
        {
            if (ch == '(' || ch == ')')
            {
                handle_parentesis(ch, stack, output);   
                continue;         
            }
            
            if (precedence_map.find(ch) == precedence_map.end())
            {
                output.push_back(ch);
                continue;
            }

            handle_operator(ch, stack, output);
        }

        while (!stack.empty()) { 
            output.push_back(stack.top());
            stack.pop(); 
        }

        return output;
    }


}
