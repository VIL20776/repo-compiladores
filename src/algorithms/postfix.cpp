#include "./postfix.hpp"

#include <map>
#include <stack>
namespace algorithms {

    const std::map<char,int> precedence_map {
        {'|', 1},
        {'&', 2},
        {'*', 3}
    };

    
    namespace {
        using std::string, std::map;
        string explicit_concat(const string &regex)
        {
            string result;
            std::stack<char> symbols;
            bool literal = false;

            for (size_t i = 0; i < regex.length(); ++i) {
                char currentSymbol = regex[i];
                result += currentSymbol;

                if (currentSymbol == '\"') {
                    literal = !literal;
                    if (literal) continue;
                }

                if (currentSymbol == '\'') {
                    result += regex[++i]; 
                    result += regex[++i];
                    continue;
                }

                if (literal || currentSymbol == '(' || currentSymbol == '|')
                    continue;

                if (i < regex.length() - 1) {
                    char nextSymbol = regex[i + 1];
                    if (nextSymbol == '*' || nextSymbol == '|' || 
                        nextSymbol == ')' || nextSymbol == '$'
                    ) continue;
                    
                    result += '&';
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
        bool literal = false;
        std::stack<char> stack {};

        string exp_regex = explicit_concat(infix);

        for (size_t i = 0; i < exp_regex.size(); i++)
        {
            char ch = exp_regex[i];
            if (ch == '(' || ch == ')')
            {
                handle_parentesis(ch, stack, output);   
                continue;         
            }
            if (ch == '\"') {
                literal = !literal;
                if (literal) continue;
            }

            if (ch == '\'') {
                output.push_back(ch);
                output.push_back(exp_regex[++i]); 
                output.push_back(exp_regex[++i]);
                continue;
            }
            
            if (literal || !precedence_map.contains(ch))
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
