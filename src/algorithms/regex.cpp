#include "regex.hpp"

namespace algorithms {

    std::string to_standard (std::string regex)
    {
        using std::string;

        string std_regex = {};
        for (int i = 0; i < regex.size(); i++)
        {
            switch (regex.at(i))
            {
            case '+':
                std_regex.append(string() + regex.at(i - 1) + "*");
                break;
            case '?':
                std_regex.pop_back();
                std_regex.append(string() + "($|" + regex.at(i - 1) + ")");
                break;
            default:
                std_regex.push_back(regex.at(i));
                break;
            }
        }

        return (std_regex);
    }

}