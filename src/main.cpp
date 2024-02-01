#include <iostream>

#include "algorithms/postfix.hpp"

int main(int argc, char const *argv[])
{
    std::string regex = "(a|b)*abab";
    std::string postfix = algorithms::regex_to_postfix(regex);
    std::cout << postfix << "\n";
    return 0;
}
