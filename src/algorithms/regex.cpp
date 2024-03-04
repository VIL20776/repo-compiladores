#include "regex.hpp"

#include <algorithm>
#include <array>

namespace algorithms {

    int check_char_pairs(const std::string& str, std::array<char,2> pair) {
        int count_open = std::count(str.begin(), str.end(), pair[0]);
        int count_close = std::count(str.begin(), str.end(), pair[1]);

        if (count_open == count_close)
            return count_close;
        else
            return -1;
    }

    std::string extract_substring(std::string& str, int closing_index, std::array<char,2> bounds) {
        // Buscar el índice del paréntesis de apertura correspondiente al índice del paréntesis de cierre proporcionado
        int opening_index = -1;
        int count = 0;
        for (int i = closing_index; i >= 0; i--) {
            if (str[i] == bounds[1]) {
                count++;
            } else if (str[i] == bounds[0]) {
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

    // std::string replace_char_class (const std::string &char_class)
    // {
        
        
    // }

    std::string replace_extentions (std::string regex)
    {
        using std::string;

        string std_regex = {};
        string substring = "";
        for (int i = 0; i < regex.size(); i++)
        {
            switch (regex.at(i))
            {
            case '+':
                if (std_regex.back() == ')') {
                    substring = extract_substring(std_regex, std_regex.size() - 1, {'(',')'});
                    std_regex.append(string("(") + substring + substring + "*)");
                } else  {
                    std_regex.pop_back();
                    std_regex.append(string("(") + regex.at(i - 1) + regex.at(i - 1) + "*)");
                }
                break;
            case '?':
                if (std_regex.back() == ')') {
                    substring = extract_substring(std_regex, std_regex.size() - 1, {'(',')'});
                    std_regex.append(string("($|") + substring + ")");
                } else
                {
                    std_regex.pop_back();
                    std_regex.append(string("($|") + regex.at(i - 1) + ")");
                }
                break;
            // case ']':
            //     substring = extract_substring(std_regex, std_regex.size() - 1, {'[',']'});
            //     std_regex.append(string("(") + replace_char_class(substring) + ")");
            //     break;
            default:
                std_regex.push_back(regex.at(i));
                break;
            }
        }

        return std_regex;
    }

    std::string to_standard(std::string regex)
    {
        if (check_char_pairs(regex, {'(',')'}) > 0) {

        }
        
        if (check_char_pairs(regex, {'[',']'}) > 0) {

        }
    }

}