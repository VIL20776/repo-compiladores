#include "regex.hpp"

#include <algorithm>

namespace algorithms {

    bool check_parenthesis(const std::string& str) {
        int count_open = std::count(str.begin(), str.end(), '(');
        int count_close = std::count(str.begin(), str.end(), ')');

        return count_open == count_close;
    }

    std::string extract_substring(std::string& str, int closing_index) {
        // Buscar el índice del paréntesis de apertura correspondiente al índice del paréntesis de cierre proporcionado
        int opening_index = -1;
        int count = 0;
        for (int i = closing_index; i >= 0; i--) {
            if (str[i] == ')') {
                count++;
            } else if (str[i] == '(') {
                count--;
                if (count == 0) {
                    opening_index = i;
                    break;
                }
            }
        }

        // Si no se encuentra ningún paréntesis de apertura correspondiente, devolvemos una cadena vacía
        if (opening_index == -1) {
            return "";
        }

        // Extraer la subcadena entre los índices de paréntesis de apertura y cierre
        std::string substring = str.substr(opening_index, closing_index - opening_index + 1);
        str.erase(opening_index, closing_index - opening_index + 1);
        return substring;
    }

    std::string to_standard (std::string regex)
    {
        using std::string;

        string std_regex = {};
        for (int i = 0; i < regex.size(); i++)
        {
            switch (regex.at(i))
            {
            case '+':
                if (std_regex.back() == ')') {
                    std::string substring = extract_substring(std_regex, std_regex.size() - 1);
                    std_regex.append(string("(") + substring + substring + "*)");
                } else  {
                    std_regex.pop_back();
                    std_regex.append(string("(") + regex.at(i - 1) + regex.at(i - 1) + "*)");
                }
                break;
            case '?':
                if (std_regex.back() == ')') {
                    std::string substring = extract_substring(std_regex, std_regex.size() - 1);
                    std_regex.append(string("($|") + substring + ")");
                } else
                {
                    std_regex.pop_back();
                    std_regex.append(string("($|") + regex.at(i - 1) + ")");
                }
                break;
            default:
                std_regex.push_back(regex.at(i));
                break;
            }
        }

        return (std_regex);
    }

}