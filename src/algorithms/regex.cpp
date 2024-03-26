#include "regex.hpp"

#include <algorithm>
#include <iostream>
#include <array>
#include <set>
#include <map>

namespace algorithms {

    const std::map<char,char> escape_chars = {
        {'t','\t'},
        {'n','\n'},
        {'s',' '}
    };

    int check_char_pairs(const std::string& str, std::array<char,2> pair) {
        int count_open = std::count(str.begin(), str.end(), pair[0]);
        int count_close = std::count(str.begin(), str.end(), pair[1]);

        return count_open - count_close;
    }

    std::string extract_substring(std::string& str, int closing_index, std::array<char,2> bounds, bool exclusive = false) {
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
        std::string substring = str.substr(opening_index + exclusive, closing_index - opening_index + 1 - 2*exclusive);
        str.erase(opening_index, closing_index - opening_index + 1);
        return substring;
    }

    std::set<char> char_set_diff(const std::set<char> &char_set1, const std::set<char> &char_set2) {
        std::set<char> diff_set = {};

        std::set_difference(
            char_set1.begin(), char_set1.end(),
            char_set2.begin(), char_set2.end(),
            std::inserter(diff_set, diff_set.begin())
        );

        return diff_set;
    }

    std::set<char> create_char_set(const std::string &char_class) {
        std::set<char> char_set = {};
        bool range = false;
        char start = 0; 
        // find chars in set
        bool inverse = char_class.starts_with('^');
        for (size_t i = inverse; i < char_class.size(); i++) {
            char c = char_class.at(i);
            switch (c)
            {
            case '\'':
            case '\"':
                break;
            case '\\':
                c = char_class.at(++i);
                if (escape_chars.contains(c)) {
                    char_set.insert(escape_chars.at(c));
                } else 
                    char_set.insert(c);
                break;
            case '-':
                range = true;
                break;
            default:
                if (range) {
                    char end = c;
                    for (char j = start + 1; j < end; j++)
                        char_set.insert(j);
                    
                    range = false;
                }

                char_set.insert(c);
                start = c;
                break;
            }
        }

        if (inverse) {
            std::set<char> all_chars = {};
            for (char i = 0; i >= 0; i++) {
                all_chars.insert(i);
            }
            char_set = char_set_diff(all_chars, char_set);
        }

        return char_set;
    }

    std::string replace_char_class (const std::set<char> &char_set)
    {
        using std::string;
        string expression = "";

        for (auto it = char_set.begin(); it != char_set.end(); it++) {
            expression.append(string("\'") + *it + "\'");
            if (*it != *char_set.rbegin()) expression.push_back('|');
        }
            
        return expression;
    }

    std::string replace_extentions (const std::string &regex)
    {
        using std::string;

        string std_regex = {};
        string substring = {};

        std::set<char> char_set = {};
        bool difference = false;
        bool literal = false;
        for (int i = 0; i < regex.size(); i++)
        {
            char ch = regex.at(i);

            if (literal && ch != '\"'){
                std_regex.push_back(ch);
                continue;
            }

            switch (ch)
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
            case '#':
                difference = true;
                extract_substring(std_regex, std_regex.size() - 1, {'(',')'});
                break;
            case ']':
                std_regex += ']';
                substring = extract_substring(std_regex, std_regex.size() - 1, {'[',']'}, true);
                if (difference) {
                    std::set<char> diff_set = char_set_diff(char_set, create_char_set(substring));
                    std_regex.append(string("(") + replace_char_class(diff_set) + ")");
                    difference = false;
                } else {
                    char_set = create_char_set(substring);
                    std_regex.append(string("(") + replace_char_class(char_set) + ")");
                }
                
                break;
            case '\"':
                literal = !literal;
                std_regex.push_back(ch);
                break;
            case '\'':
                ch = regex.at(++i);
                if (ch != '\\') {
                    std_regex.append(string("\'") + ch + "\'");
                    i++;
                    break;
                } 
            case '\\':
                ch = regex.at(++i);
                if (escape_chars.contains(ch)) {
                    std_regex.append(string("\'") + escape_chars.at(ch) + "\'");
                } else 
                    std_regex.append(string("\'") + ch + "\'");
                i++;
                break;
            case '_':
                std_regex.push_back('(');
                for (char i = 0; i >= 0; i++) {
                    std_regex.append(string("\'") + i + "\'");
                    if (i != 127) std_regex.push_back('|');
                }
                std_regex.push_back(')');
                break;
            default:
                std_regex.push_back(ch);
                break;
            }
        }

        return std_regex;
    }

    int check_regex (const std::string &regex)
    {
        int error = 0;
        error = 
            check_char_pairs(regex, {'(',')'}) + 
            check_char_pairs(regex, {'[',']'});
        
        for (char c: "#*+?|")
            error += (int) regex.starts_with(c);
        
        for (char c: "#|")
            error += (int) regex.ends_with(c);
        
        return error;
    }

    std::string to_standard(const std::string &regex)
    {
        if (check_regex(regex) > 0) {
            std::cerr << "La regex " << regex << " es invalida." << std::endl;
            return "";
        }
        
        return replace_extentions(regex);
    }
} // namespace algorithms